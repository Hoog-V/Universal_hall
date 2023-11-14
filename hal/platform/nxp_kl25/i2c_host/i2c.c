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
#include "bit_manipulation.h"


static I2C_Type *i2c_host_peripheral_mapping_table[2] = {I2C0, I2C1};



/**
 * @brief This formula is used to calculate the baud rate steps.
 * The formula is a rewritten form of the formula found on page 483 (section 28.10.3) of the SAMD21 Family datasheet.
 * The original formula is: fscl = 1/(10 + 2*BAUD_STEPS + fgclk + Trise)
 * If no external pullup resistors are added the Trise will be the equivalent of fgclk/10
 * fgclk will be the fast gclk frequency (probably same as the CPU frequency)
 * fscl will be the output clock frequency (100/400/1000 KHz)
 * Baud_steps are the amount of steps needed to generate the right output frequency
 */

#define ENABLE_BIT_SHIFT(i2c_peripheral)  ((uint8_t)((1<< (I2C_C1_IICEN_SHIFT+((uint8_t )i2c_peripheral)))) & I2C_C1_IICEN_MASK)

#define SIM_CLK_MASK_SHIFT(i2c_peripheral) ((uint8_t)((1<< (SIM_SCGC4_I2C0_SHIFT+((uint8_t )i2c_peripheral)))) & SIM_SCGC4_I2C0_MASK)

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
    I2C_Type * i2c_periph = (I2C_Type*)hw;
    return BIT_IS_SET(i2c_periph->S, I2C_S_BUSY_SHIFT);
}

/**
 * @brief Helper function to wait for a transaction to finish.
 *        It uses a combination of flag polling as well as cycles delay to achieve this.
 *
 * @param hw Pointer to the SERCOM peripheral to be manipulated or read.
 *
 * @note This function can only be used for use with I2C host/master configuration
 */
void wait_for_idle_busstate(I2C_Type *I2CInst) {
    int timeout = 65535;
    int timeout_attempt = 4;
    while (get_i2c_master_busstate(I2CInst) != 0x00) {
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

static inline I2C_Type *get_i2c_peripheral_inst(const i2c_periph_inst_t peripheral_inst_num) {
    return i2c_host_peripheral_mapping_table[peripheral_inst_num];
}

/**
 * @brief Internal function used for disabling the SERCOM i2c Host driver
 * @param hw Pointer to the SERCOM peripheral to be manipulated or read
 */
static inline void disable_host_i2c_driver(i2c_periph_inst_t i2c_peripheral_num) {
    I2C_Type *i2c_periph = get_i2c_peripheral_inst(i2c_peripheral_num);
    i2c_periph->C1 &= ~ENABLE_BIT_SHIFT(i2c_peripheral_num);
    SIM->SCGC4 &= ~(SIM_CLK_MASK_SHIFT(i2c_peripheral_num));
}


uhal_status_t i2c_host_init(const i2c_periph_inst_t i2c_peripheral_num,
                            const i2c_clock_sources_t clock_sources,
                            const uint32_t periph_clk_freq,
                            const uint32_t baud_rate_freq,
                            const i2c_extra_opt_t extra_configuration_options) {

    I2C_Type* i2c_periph = get_i2c_peripheral_inst(i2c_peripheral_num);
    const bool i2c_peripheral_enabled = BITMASK_COMPARE(i2c_periph->C1, I2C_C1_IICEN_MASK);
    if (i2c_peripheral_enabled) {
        disable_host_i2c_driver(i2c_peripheral_num);
    }
    SIM->SCGC4 |= SIM_CLK_MASK_SHIFT(i2c_peripheral_num);
    i2c_periph->C1 = I2C_C1_MST(1);
    if(extra_configuration_options == I2C_EXTRA_OPT_DMA){
        i2c_periph->C1 |= I2C_C1_DMAEN(1);
    } else if(extra_configuration_options != I2C_EXTRA_OPT_POLLING) {
        i2c_periph->C1 |= I2C_C1_IICIE(1);
    }

    //i2c_host_set_baudrate(i2c_periph, baud_rate_freq, periph_clk_freq);
    i2c_periph->C1 |= I2C_C1_IICEN(1);

    const enum IRQn irq_type = (I2C0_IRQn + i2c_peripheral_num);
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
    disable_host_i2c_driver(i2c_peripheral_num);
    return UHAL_STATUS_OK;
}

uhal_status_t i2c_host_write_non_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                                          const uint16_t addr,
                                          const uint8_t *write_buff,
                                          const size_t size,
                                          const i2c_stop_bit_t stop_bit) {
    I2C_Type *i2c_periph = get_i2c_peripheral_inst(i2c_peripheral_num);
    wait_for_idle_busstate(i2c_periph);
    volatile bustransaction_t *TransactionData = &i2c_comm_peripheral_bus_trans[i2c_peripheral_num];
    TransactionData->write_buffer = write_buff;
    TransactionData->buf_size = size+1;
    TransactionData->transaction_type = stop_bit ? PERIPH_ACT_I2C_TRANSMIT_STOP
                                                 : PERIPH_ACT_I2C_TRANSMIT_NO_STOP;
    TransactionData->buf_cnt = 0;
    i2c_periph->D = ((addr & 0xFF) << 1) | 1;
    i2c_periph->C1 |= I2C_C1_TX_MASK;
    while(TransactionData->transaction_type != PERIPH_ACT_NONE);
    return 0;
}

uhal_status_t i2c_host_write_blocking(const i2c_periph_inst_t i2c_peripheral_num, const uint16_t addr,
                                      const uint8_t *write_buff, const size_t size,
                                      const i2c_stop_bit_t stop_bit) {
    uhal_status_t status = i2c_host_write_non_blocking(i2c_peripheral_num, addr, write_buff, size, stop_bit);
    return 0;
}

uhal_status_t i2c_host_read_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                                     const uint16_t addr, uint8_t *read_buff,
                                     const size_t amount_of_bytes) {
   uhal_status_t status = i2c_host_read_non_blocking(i2c_peripheral_num, addr, read_buff, amount_of_bytes);
   return status;
}

uhal_status_t i2c_host_read_non_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                                         const uint16_t addr, uint8_t *read_buff,
                                         const size_t amount_of_bytes) {

//    Sercom *sercom_inst = get_sercom_inst(i2c_peripheral_num);
//    wait_for_idle_busstate(sercom_inst);
//    volatile bustransaction_t *TransactionData = &sercom_bustrans_buffer[i2c_peripheral_num];
//    i2c_master_wait_for_sync((sercom_inst), SERCOM_I2CM_SYNCBUSY_SYSOP);
//    TransactionData->read_buffer = read_buff;
//    TransactionData->buf_size = amount_of_bytes;
//    TransactionData->transaction_type = SERCOMACT_I2C_DATA_RECEIVE_STOP;
//    TransactionData->buf_cnt = 0;
//    sercom_inst->I2CM.ADDR.reg = (addr << 1) | 1;
//    i2c_master_wait_for_sync((sercom_inst), SERCOM_I2CM_SYNCBUSY_SYSOP);
    return 0;
}

#endif /* DISABLE_I2C_HOST_MODULE */