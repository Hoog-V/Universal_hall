/**
* \file            i2c_slave.c
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

#ifndef DISABLE_I2C_SLAVE_MODULE

#include <stdbool.h>
#include <hal_i2c_slave.h>
#include "irq/irq_bindings.h"

static Sercom *i2c_slave_peripheral_mapping_table[6] = {SERCOM0, SERCOM1, SERCOM2, SERCOM3, SERCOM4, SERCOM5};
#define SERCOM_SLOW_CLOCK_SOURCE(x)               (x >> 8)

/**
 * @brief Helper function which waits for the SERCOM peripheral to get in sync and finish requested operations.
 *        By continually reading its I2CS syncbusy register.
 * @param hw Pointer to the SERCOM peripheral to be manipulated or read
 * @param bits_to_read The bits to read within the syncbusy register (bitmask)
 * @note Possible bits that can be read are: SERCOM_I2CS_SYNCBUSY_SWRST
 *                                           SERCOM_I2CS_SYNCBUSY_ENABLE
 *                                           SERCOM_I2CS_SYNCBUSY_SYSOP
 * @note This function can only be used for use with I2C client/slave configuration
 */
static inline void i2c_slave_wait_for_sync(const void *const hw, const uint32_t bits_to_read) {
    while (((Sercom *) hw)->I2CS.SYNCBUSY.reg & bits_to_read) {
    };
}

void disable_i2c_interface(const void *const hw) {
    ((Sercom *) hw)->I2CM.CTRLA.reg &= ~SERCOM_I2CM_CTRLA_ENABLE;
    const uint32_t waitflags = (SERCOM_I2CS_SYNCBUSY_SWRST | SERCOM_I2CS_SYNCBUSY_ENABLE);
    i2c_slave_wait_for_sync(hw, waitflags);
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
    return i2c_slave_peripheral_mapping_table[peripheral_inst_num];
}

uhal_status_t i2c_slave_init(const i2c_periph_inst_t i2c_instance,
                             const uint16_t slave_addr,
                             const i2c_clock_sources_t clock_sources,
                             const i2c_extra_opt_t extra_configuration_options) {
    const bool InvalidSercomInstNum = (i2c_instance < 0 || i2c_instance);
    //const bool InvalidClockGen = (i2c_instance->clk_gen_slow < 0 || i2c_instance->clk_gen_slow > 6 || i2c_instance->clk_gen_fast < 0 || i2c_instance->clk_gen_fast > 6);
    if (InvalidSercomInstNum) {
        return UHAL_STATUS_INVALID_PARAMETERS;
    }
// Set the clock system
#ifdef __SAMD51__

#else
    if (clock_sources != I2C_CLK_SOURCE_USE_DEFAULT) {
        const uint8_t clk_gen_slow = get_slow_clk_gen_val(clock_sources);
        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(clk_gen_slow) | GCLK_CLKCTRL_ID_SERCOMX_SLOW | GCLK_CLKCTRL_CLKEN;
        while (GCLK->STATUS.bit.SYNCBUSY);
        const uint8_t clk_gen_fast = get_fast_clk_gen_val(clock_sources);
        GCLK->CLKCTRL.reg =
                GCLK_CLKCTRL_GEN(clk_gen_fast) |
                ((GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + i2c_instance) << GCLK_CLKCTRL_ID_Pos) | GCLK_CLKCTRL_CLKEN;
        GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(clk_gen_fast);
        while (GCLK->STATUS.bit.SYNCBUSY);
    } else {
        const uint8_t clk_gen_slow = 3;
        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(clk_gen_slow) | GCLK_CLKCTRL_ID_SERCOMX_SLOW | GCLK_CLKCTRL_CLKEN;
        while (GCLK->STATUS.bit.SYNCBUSY);
        const uint8_t clk_gen_fast = 0;
        GCLK->CLKCTRL.reg =
                GCLK_CLKCTRL_GEN(clk_gen_fast) |
                ((GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + i2c_instance) << GCLK_CLKCTRL_ID_Pos) | GCLK_CLKCTRL_CLKEN;
        GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(clk_gen_fast);
        while (GCLK->STATUS.bit.SYNCBUSY);
    }
#endif
    Sercom *SercomInst = get_sercom_inst(i2c_instance);
    const bool SercomEnabled = SercomInst->I2CM.CTRLA.bit.ENABLE;
    if (SercomEnabled) {
        disable_i2c_interface(SercomInst);
    }
    SercomInst->I2CS.CTRLA.reg = (SERCOM_I2CS_CTRLA_SWRST | SERCOM_I2CS_CTRLA_MODE(4));
    i2c_slave_wait_for_sync(SercomInst, SERCOM_I2CS_SYNCBUSY_SWRST);
    SercomInst->I2CS.CTRLA.reg = (1 << SERCOM_I2CS_CTRLA_LOWTOUTEN_Pos      /* SCL Low Time-Out: disabled */
                                  | 0 << SERCOM_I2CS_CTRLA_SCLSM_Pos    /* SCL Clock Stretch Mode: disabled */
                                  | 0 << SERCOM_I2CS_CTRLA_SPEED_Pos    /* Transfer Speed: 0 */
                                  | 1 << SERCOM_I2CS_CTRLA_SEXTTOEN_Pos /* Slave SCL Low Extend Time-Out: disabled */
                                  | 0b10 << SERCOM_I2CS_CTRLA_SDAHOLD_Pos  /* SDA Hold Time: 0 */
                                  | 0 << SERCOM_I2CS_CTRLA_PINOUT_Pos   /* Pin Usage: disabled */
                                  | 0 << SERCOM_I2CS_CTRLA_RUNSTDBY_Pos /* Run In Standby: disabled */
                                  | 4 << SERCOM_I2CS_CTRLA_MODE_Pos);
    SercomInst->I2CS.CTRLB.reg |= SERCOM_I2CS_CTRLB_SMEN;
    i2c_slave_wait_for_sync(SercomInst, SERCOM_I2CS_SYNCBUSY_MASK);
    SercomInst->I2CS.ADDR.reg = (0 << SERCOM_I2CS_ADDR_ADDRMASK_Pos       /* Address Mask: 0 */
                                 | 0 << SERCOM_I2CS_ADDR_TENBITEN_Pos /* Ten Bit Addressing Enable: disabled */
                                 | 0 << SERCOM_I2CS_ADDR_GENCEN_Pos   /* General Call Address Enable: disabled */
                                 | (slave_addr) << SERCOM_I2CS_ADDR_ADDR_Pos);
    SercomInst->I2CS.CTRLA.reg |= SERCOM_I2CS_CTRLA_ENABLE;
    SercomInst->I2CS.INTENSET.reg = SERCOM_I2CS_INTENSET_AMATCH | SERCOM_I2CS_INTENSET_PREC | SERCOM_I2CS_INTENSET_DRDY;
    const enum IRQn irq_type = (SERCOM0_IRQn + i2c_instance);
    NVIC_EnableIRQ(irq_type);
    const uint16_t irq_options = extra_configuration_options >> 8;
    if (irq_options) {
        enable_irq_handler(irq_type, irq_options - 1);
    } else {
        enable_irq_handler(irq_type, 2);
    }
    return UHAL_STATUS_OK;
}

uhal_status_t i2c_slave_deinit(const i2c_periph_inst_t i2c_instance) {
    Sercom *sercom_inst = get_sercom_inst(i2c_instance);
    disable_i2c_interface(sercom_inst);
    return UHAL_STATUS_OK;
}

#endif /* DISABLE_I2C_SLAVE_MODULE*/