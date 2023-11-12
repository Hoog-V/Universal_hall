/**
* \file            i2c.c
* \brief           Source file which implements the standard I2C API functions
*/
/*
*  Copyright 2023 (C) Victor Hogeweij <hogeweyv@gmail.com>
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
* This file is part of the Universal Hal Framework.
*
* Author:          Victor Hogeweij <hogeweyv@gmail.com>
*/

#ifndef DISABLE_I2C_HOST_MODULE

#include <hal_i2c_host.h>
#include <stdbool.h>
#include "error_handling.h"
#include "irq/irq_bindings.h"

static Sercom *i2c_host_peripheral_mapping_table[6] = {SERCOM0, SERCOM1, SERCOM2, SERCOM3, SERCOM4, SERCOM5};
/**
 * @brief This formula is used to calculate the baud rate steps.
 * The formula is a rewritten form of the formula found on page 483 (section 28.10.3) of the SAMD21 Family datasheet.
 * The original formula is: fscl = 1/(10 + 2*BAUD_STEPS + fgclk + Trise)
 * If no external pullup resistors are added the Trise will be the equivalent of fgclk/10
 * fgclk will be the fast gclk frequency (probably same as the CPU frequency)
 * fscl will be the output clock frequency (100/400/1000 KHz)
 * Baud_steps are the amount of steps needed to generate the right output frequency
 */
#define calculate_baudrate(clock_freq, baud_freq) (clock_freq / (2 * baud_freq)) - (10 * clock_freq / (2 * clock_freq)) - 5

#define SERCOM_SLOW_CLOCK_SOURCE(x)               (x >> 8)


/**
 * @brief Helper function which waits for the sercom peripheral to get in sync and finish requested operations.
 *        By continually reading its I2CM syncbusy register.
 * @param hw Pointer to the sercom peripheral to be manipulated or read
 * @param bits_to_read The bits to read within the syncbusy register (bitmask)
 * @note Possible bits that can be read are: SERCOM_I2CM_SYNCBUSY_SWRST
 *                                           SERCOM_I2CM_SYNCBUSY_ENABLE
 *                                           SERCOM_I2CM_SYNCBUSY_SYSOP
 * @note This function can only be used for use with I2C host/master configuration
 */
static inline void i2c_master_wait_for_sync(const void *const hw, const uint32_t bits_to_read) {
    while (((Sercom *) hw)->I2CM.SYNCBUSY.reg & bits_to_read) {};
}

/**
 * @brief This function gets the current bus-state of the I2C bus.
 *        By reading the SERCOM I2CM Status register.
 * @param  hw Pointer of the SERCOM peripheral to be manipulated or read
 * @return busstate of I2C bus: 0x00: UNKNOWN
 *                              0x01: IDLE
 *                              0x02: OWNER
 *                              0x03: BUSY
 *                              Other: N/A
 * @note This function can only be used for use with I2C host/master configuration
 */
uint8_t get_i2c_master_busstate(const void *const hw) {
    i2c_master_wait_for_sync(hw, SERCOM_I2CM_SYNCBUSY_SYSOP);
    return (((Sercom *) hw)->I2CM.STATUS.reg & SERCOM_I2CM_STATUS_BUSSTATE_Msk) >> SERCOM_I2CM_STATUS_BUSSTATE_Pos;
}

/**
 * @brief Helper function to wait for a transaction to finish.
 *        It uses a combination of flag polling as well as cycles delay to achieve this.
 *
 * @param hw Pointer to the SERCOM peripheral to be manipulated or read.
 *
 * @note This function can only be used for use with I2C host/master configuration
 */
void wait_for_idle_busstate(Sercom *SercomInst) {
    int timeout = 65535;
    int timeout_attempt = 4;
    while (get_i2c_master_busstate(SercomInst) != 0x1) {
        timeout--;

        if (timeout <= 0) {
            if (--timeout_attempt) {
                timeout = 65535;
            } else {
                return;
            }
        }
    }
    /**
     * Wait an additional 20 cycles
     * There seems to be internal buffering inside the sercom peripheral, which seems to be stacking transactions.
     * To overcome this an extra delay has been added. Otherwise, read or write transactions will be collected and stacked
     * after each other.
     * @todo Change this delay to flag/checking other means of overcoming this problem
    */
    timeout = 20;
    do {
        timeout--;
    } while (timeout >= 1);
}

static inline uint8_t get_fast_clk_gen_val(const i2c_clock_sources_t clock_sources) {
    const uint16_t fast_clk_val = (clock_sources & 0xFF) - 1;
    return fast_clk_val;
}

static inline uint8_t get_slow_clk_gen_val(const i2c_clock_sources_t clock_sources) {
    const uint16_t slow_clk_val = SERCOM_SLOW_CLOCK_SOURCE(clock_sources) - 1;
    return slow_clk_val;
}

static inline Sercom *get_sercom_inst(const i2c_periph_inst_t peripheral_inst_num) {
    return i2c_host_peripheral_mapping_table[peripheral_inst_num];
}

/**
 * @brief Internal function used for disabling the SERCOM i2c Host driver
 * @param hw Pointer to the SERCOM peripheral to be manipulated or read
 */
static inline void disable_host_i2c_driver(const void *const hw) {
    ((Sercom *) hw)->I2CM.CTRLA.reg &= ~SERCOM_I2CM_CTRLA_ENABLE;
    const uint32_t waitflags = (SERCOM_I2CM_SYNCBUSY_SWRST | SERCOM_I2CM_SYNCBUSY_ENABLE);
    i2c_master_wait_for_sync(hw, waitflags);
}

uhal_status_t i2c_host_init(const i2c_periph_inst_t i2c_peripheral_num,
                            const i2c_clock_sources_t clock_sources,
                            const uint32_t periph_clk_freq,
                            const uint32_t baud_rate_freq,
                            const i2c_extra_opt_t extra_configuration_options) {
#ifdef __SAMD51__

#else
    PM->APBCMASK.reg |= 1 << (PM_APBCMASK_SERCOM0_Pos + i2c_peripheral_num);
    if (clock_sources != I2C_CLK_SOURCE_USE_DEFAULT) {
        const uint8_t clk_gen_slow = get_slow_clk_gen_val(clock_sources);
        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(clk_gen_slow) | GCLK_CLKCTRL_ID_SERCOMX_SLOW | GCLK_CLKCTRL_CLKEN;
        while (GCLK->STATUS.bit.SYNCBUSY);
        const uint8_t clk_gen_fast = get_fast_clk_gen_val(clock_sources);
        GCLK->CLKCTRL.reg =
                GCLK_CLKCTRL_GEN(clk_gen_fast) |
                ((GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + i2c_peripheral_num) << GCLK_CLKCTRL_ID_Pos) | GCLK_CLKCTRL_CLKEN;
        GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(clk_gen_fast);
        while (GCLK->STATUS.bit.SYNCBUSY);
    } else {
        const uint8_t clk_gen_slow = 3;
        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(clk_gen_slow) | GCLK_CLKCTRL_ID_SERCOMX_SLOW | GCLK_CLKCTRL_CLKEN;
        while (GCLK->STATUS.bit.SYNCBUSY);
        const uint8_t clk_gen_fast = 0;
        GCLK->CLKCTRL.reg =
                GCLK_CLKCTRL_GEN(clk_gen_fast) |
                ((GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + i2c_peripheral_num) << GCLK_CLKCTRL_ID_Pos) | GCLK_CLKCTRL_CLKEN;
        GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(clk_gen_fast);
        while (GCLK->STATUS.bit.SYNCBUSY);
    }
#endif

    Sercom *SercomInst = get_sercom_inst(i2c_peripheral_num);
    const bool SercomEnabled = SercomInst->I2CM.CTRLA.bit.ENABLE;
    if (SercomEnabled) {
        disable_host_i2c_driver(SercomInst);
    }
    SercomInst->I2CM.CTRLA.reg = (SERCOM_I2CM_CTRLA_SWRST | SERCOM_I2CM_CTRLA_MODE(5));
    const uint32_t waitflags = (SERCOM_I2CM_SYNCBUSY_SWRST | SERCOM_I2CM_SYNCBUSY_ENABLE);
    i2c_master_wait_for_sync(SercomInst, waitflags);
    SercomInst->I2CM.CTRLA.reg = (0 << SERCOM_I2CM_CTRLA_LOWTOUTEN_Pos    /* SCL Low Time-Out: disabled */
                                  | 0 << SERCOM_I2CM_CTRLA_INACTOUT_Pos   /* Inactive Time-Out: 0 */
                                  | 0 << SERCOM_I2CM_CTRLA_SCLSM_Pos      /* SCL Clock Stretch Mode: disabled */
                                  | 0 << SERCOM_I2CM_CTRLA_SPEED_Pos      /* Transfer Speed: 0 */
                                  | 0 << SERCOM_I2CM_CTRLA_SEXTTOEN_Pos   /* Slave SCL Low Extend Time-Out: disabled */
                                  | 0 << SERCOM_I2CM_CTRLA_MEXTTOEN_Pos   /* Master SCL Low Extend Time-Out: 0 */
                                  | 0b10 << SERCOM_I2CM_CTRLA_SDAHOLD_Pos /* SDA Hold Time: 0 */
                                  | 0 << SERCOM_I2CM_CTRLA_PINOUT_Pos     /* Pin Usage: disabled */
                                  | 0 << SERCOM_I2CM_CTRLA_RUNSTDBY_Pos   /* Run In Standby: disabled */
                                  | 5 << SERCOM_I2CM_CTRLA_MODE_Pos);

    i2c_master_wait_for_sync(SercomInst, SERCOM_I2CM_SYNCBUSY_MASK);
    SercomInst->I2CM.BAUD.reg = calculate_baudrate(periph_clk_freq, baud_rate_freq);
    int timeout = 65535;
    int timeout_attempt = 4;
    SercomInst->I2CM.CTRLA.reg |= SERCOM_I2CM_CTRLA_ENABLE;
    SercomInst->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_SMEN;
    i2c_master_wait_for_sync(SercomInst, SERCOM_I2CM_SYNCBUSY_SWRST | SERCOM_I2CM_SYNCBUSY_ENABLE);
    while (get_i2c_master_busstate(SercomInst) != 0x1) {
        timeout--;

        if (timeout <= 0) {
            if (--timeout_attempt) {
                timeout = 65535;
            } else {
                return UHAL_STATUS_PERIPHERAL_CLOCK_ERROR;
            }
            SercomInst->I2CM.STATUS.reg = SERCOM_I2CM_STATUS_BUSSTATE(0x1);
            i2c_master_wait_for_sync(SercomInst, SERCOM_I2CM_SYNCBUSY_SYSOP);
        }
    }
    SercomInst->I2CM.INTENSET.reg = SERCOM_I2CM_INTENSET_MB | SERCOM_I2CM_INTENSET_SB;

    const enum IRQn irq_type = (SERCOM0_IRQn + i2c_peripheral_num);
    NVIC_EnableIRQ(irq_type);
    const uint16_t irq_options = extra_configuration_options >> 8;
    if (irq_options) {
        enable_irq_handler(irq_type, irq_options - 1);
    } else {
        enable_irq_handler(irq_type, 2);
    }
    return UHAL_STATUS_OK;
}

uhal_status_t i2c_host_deinit(const i2c_periph_inst_t i2c_peripheral_num) {
    Sercom *sercom_inst = get_sercom_inst(i2c_peripheral_num);
    disable_host_i2c_driver(sercom_inst);
    return UHAL_STATUS_OK;
}

uhal_status_t i2c_host_write_non_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                                          const uint16_t addr,
                                          const uint8_t *write_buff,
                                          const size_t size,
                                          const i2c_stop_bit_t stop_bit) {
    Sercom *sercom_inst = get_sercom_inst(i2c_peripheral_num);
    wait_for_idle_busstate(sercom_inst);
    volatile bustransaction_t *TransactionData = &sercom_bustrans_buffer[i2c_peripheral_num];
    while ((sercom_inst->I2CM.STATUS.bit.BUSSTATE != 0x1) &&
           sercom_bustrans_buffer[i2c_peripheral_num].transaction_type != SERCOMACT_NONE
           && sercom_inst->I2CM.INTFLAG.reg == 0);
    i2c_master_wait_for_sync((sercom_inst), SERCOM_I2CM_SYNCBUSY_SYSOP);
    TransactionData->write_buffer = write_buff;
    TransactionData->buf_size = size;
    TransactionData->transaction_type = stop_bit ? SERCOMACT_I2C_DATA_TRANSMIT_STOP
                                                 : SERCOMACT_I2C_DATA_TRANSMIT_NO_STOP;
    TransactionData->buf_cnt = 0;
    sercom_inst->I2CM.ADDR.reg = (addr << 1);
    i2c_master_wait_for_sync((sercom_inst), SERCOM_I2CM_SYNCBUSY_SYSOP);
    return TransactionData->status;
}

uhal_status_t i2c_host_write_blocking(const i2c_periph_inst_t i2c_peripheral_num, const uint16_t addr,
                                      const uint8_t *write_buff, const size_t size,
                                      const i2c_stop_bit_t stop_bit) {
    Sercom *sercom_inst = get_sercom_inst(i2c_peripheral_num);
    uhal_status_t status = i2c_host_write_non_blocking(i2c_peripheral_num, addr, write_buff, size, stop_bit);
    wait_for_idle_busstate(sercom_inst);
    return status;
}

uhal_status_t i2c_host_read_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                                     const uint16_t addr, uint8_t *read_buff,
                                     const size_t amount_of_bytes) {
    volatile bustransaction_t *TransactionData = &sercom_bustrans_buffer[i2c_peripheral_num];
    i2c_host_read_non_blocking(i2c_peripheral_num, addr, read_buff, amount_of_bytes);
    Sercom *sercom_inst = get_sercom_inst(i2c_peripheral_num);
    wait_for_idle_busstate(sercom_inst);
    return TransactionData->status;
}

uhal_status_t i2c_host_read_non_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                                         const uint16_t addr, uint8_t *read_buff,
                                         const size_t amount_of_bytes) {

    Sercom *sercom_inst = get_sercom_inst(i2c_peripheral_num);
    wait_for_idle_busstate(sercom_inst);
    volatile bustransaction_t *TransactionData = &sercom_bustrans_buffer[i2c_peripheral_num];
    i2c_master_wait_for_sync((sercom_inst), SERCOM_I2CM_SYNCBUSY_SYSOP);
    TransactionData->read_buffer = read_buff;
    TransactionData->buf_size = amount_of_bytes;
    TransactionData->transaction_type = SERCOMACT_I2C_DATA_RECEIVE_STOP;
    TransactionData->buf_cnt = 0;
    sercom_inst->I2CM.ADDR.reg = (addr << 1) | 1;
    i2c_master_wait_for_sync((sercom_inst), SERCOM_I2CM_SYNCBUSY_SYSOP);
    return TransactionData->status;
}

#endif /* DISABLE_I2C_HOST_MODULE */