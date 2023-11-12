/**
* \file            spi_host.c
* \brief           Source file which implements the standard SPI Host API functions
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

#ifndef DISABLE_SPI_HOST_MODULE

#include "bit_manipulation.h"
#include "hal_gpio.h"
#include "hal_spi_host.h"
#include "hal_i2c_host.h"
#include "spi_common/spi_platform_specific.h"
#include "irq/irq_bindings.h"

#define SERCOM_SLOW_CLOCK_SOURCE(x)               (x >> 8)

Sercom *spi_peripheral_mapping_table[6] = {SERCOM0, SERCOM1, SERCOM2, SERCOM3, SERCOM4, SERCOM5};


static inline Sercom *get_sercom_inst(const spi_host_inst_t peripheral_inst_num) {
    return spi_peripheral_mapping_table[peripheral_inst_num];
}

/**
 * @brief Helper function which waits for the sercom peripheral to get in sync and finish requested operations.
 *        By continually reading its SPI syncbusy register.
 * @param hw Pointer to the sercom peripheral to be manipulated or read
 * @param bits_to_read The bits to read within the syncbusy register (bitmask)
 * @note Possible bits that can be read are: SERCOM_SPI_SYNCBUSY_SWRST
 *                                           SERCOM_SPI_SYNCBUSY_ENABLE
 *                                           SERCOM_SPI_SYNCBUSY_CTRLB
 */
static inline void spi_wait_for_sync(const void *const hw, const uint32_t bits_to_read) {
    while (((Sercom *) hw)->SPI.SYNCBUSY.reg & bits_to_read) { ;
    }
}

static inline void spi_wait_for_transaction_finish(volatile bustransaction_t *bustransaction, busactions_t flag) {
    while (bustransaction->transaction_type != flag) { ;
    }
}

static inline uint8_t get_fast_clk_gen_val(const spi_clock_sources_t clock_sources) {
    const uint16_t fast_clk_val = (clock_sources & 0xFF) - 1;
    return fast_clk_val;
}

static inline uint8_t get_slow_clk_gen_val(const spi_clock_sources_t clock_sources) {
    const uint16_t slow_clk_val = SERCOM_SLOW_CLOCK_SOURCE(clock_sources) - 1;
    return slow_clk_val;
}

static inline uint8_t get_character_size_from_bus_opt(const spi_bus_opt_t bus_opt) {
    return BITMASK_COMPARE(bus_opt, SPI_BUS_OPT_CHAR_SIZE_10_BIT) >> 2;
}

static inline uint8_t get_clock_polarity_from_bus_opt(const spi_bus_opt_t bus_opt) {
    return BITMASK_COMPARE(bus_opt, SPI_BUS_OPT_CLOCK_POLARITY_SCK_HIGH);
}

static inline uint8_t get_data_order_from_bus_opt(const spi_bus_opt_t bus_opt) {
    return BITMASK_COMPARE(bus_opt, SPI_BUS_OPT_DATA_ORDER_LSB_FIRST) >> 1;
}

static inline uint8_t get_dopo_pad_from_bus_opt(const spi_bus_opt_t bus_opt) {
    return (BITMASK_COMPARE(bus_opt, 0x38) >> 3) - 1;
}

static inline uint8_t get_dipo_pad_from_bus_opt(const spi_bus_opt_t bus_opt) {
    return (BITMASK_COMPARE(bus_opt, 0x1C0) >> 6) - 1;
}

uhal_status_t spi_host_init(const spi_host_inst_t spi_peripheral_num, const uint32_t spi_clock_source,
                            const uint32_t spi_clock_source_freq,
                            const unsigned long spi_bus_frequency, const spi_bus_opt_t spi_extra_configuration_opt) {

    // Set the clock system
#ifdef __SAMD51__

#else
    PM->APBCMASK.reg |= 1 << (PM_APBCMASK_SERCOM0_Pos + spi_peripheral_num);
    if (spi_clock_source != SPI_CLK_SOURCE_USE_DEFAULT) {
        const uint8_t clk_gen_slow = get_slow_clk_gen_val(spi_clock_source);
        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(clk_gen_slow) | GCLK_CLKCTRL_ID_SERCOMX_SLOW | GCLK_CLKCTRL_CLKEN;
        while (GCLK->STATUS.bit.SYNCBUSY);
        const uint8_t clk_gen_fast = get_fast_clk_gen_val(spi_clock_source);
        GCLK->CLKCTRL.reg =
                GCLK_CLKCTRL_GEN(clk_gen_fast) |
                ((GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + spi_peripheral_num) << GCLK_CLKCTRL_ID_Pos) | GCLK_CLKCTRL_CLKEN;
        GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(clk_gen_fast);
        while (GCLK->STATUS.bit.SYNCBUSY);
    } else {
        const uint8_t clk_gen_slow = 3;
        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(clk_gen_slow) | GCLK_CLKCTRL_ID_SERCOMX_SLOW | GCLK_CLKCTRL_CLKEN;
        while (GCLK->STATUS.bit.SYNCBUSY);
        const uint8_t clk_gen_fast = 0;
        GCLK->CLKCTRL.reg =
                GCLK_CLKCTRL_GEN(clk_gen_fast) |
                ((GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + spi_peripheral_num) << GCLK_CLKCTRL_ID_Pos) | GCLK_CLKCTRL_CLKEN;
        GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(clk_gen_fast);
        while (GCLK->STATUS.bit.SYNCBUSY);
    }
#endif
    Sercom *sercom_instance = get_sercom_inst(spi_peripheral_num);
    const uint8_t dopo_pad = get_dopo_pad_from_bus_opt(spi_extra_configuration_opt);
    const uint8_t dipo_pad = get_dipo_pad_from_bus_opt(spi_extra_configuration_opt);
    const uint8_t clock_polarity = get_clock_polarity_from_bus_opt(spi_extra_configuration_opt);
    const uint8_t character_size = get_character_size_from_bus_opt(spi_extra_configuration_opt);
    const uint8_t data_order = get_data_order_from_bus_opt(spi_extra_configuration_opt);
    sercom_instance->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_SWRST;
    spi_wait_for_sync(sercom_instance, SERCOM_SPI_SYNCBUSY_SWRST | SERCOM_SPI_SYNCBUSY_ENABLE);
    sercom_instance->SPI.CTRLA.reg =
            SERCOM_SPI_CTRLA_MODE_SPI_MASTER | (0 << SERCOM_SPI_CTRLA_CPHA_Pos)
            | (0 << SERCOM_SPI_CTRLA_CPOL_Pos) | (SERCOM_SPI_CTRLA_DIPO(dipo_pad))
            | (SERCOM_SPI_CTRLA_DOPO(dopo_pad));
    sercom_instance->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_PLOADEN | SERCOM_SPI_CTRLB_CHSIZE(character_size);
    sercom_instance->SPI.BAUD.reg = spi_clock_source_freq / spi_bus_frequency / 2;
//    sercom_instance->SPI.INTENSET.reg = SERCOM_SPI_INTENSET_RXC | SERCOM_SPI_INTENSET_TXC | SERCOM_SPI_INTENSET_SSL;
    sercom_instance->SPI.CTRLA.reg |= SERCOM_SPI_CTRLA_ENABLE;
    sercom_instance->SPI.CTRLB.reg |= SERCOM_SPI_CTRLB_RXEN;
    spi_wait_for_sync(sercom_instance, SERCOM_SPI_SYNCBUSY_ENABLE);
    const enum IRQn irq_type = (SERCOM0_IRQn + spi_peripheral_num);
    enable_irq_handler(irq_type, 2);
    sercom_bustrans_buffer[spi_peripheral_num].transaction_type = SERCOMACT_IDLE_SPI_HOST;
    return UHAL_STATUS_OK;
}

uhal_status_t spi_host_deinit(const spi_host_inst_t spi_peripheral_num) {
    return UHAL_STATUS_OK;
}

uhal_status_t spi_host_start_transaction(const spi_host_inst_t spi_peripheral_num, const gpio_pin_t chip_select_pin,
                                         const spi_extra_dev_opt_t device_specific_config_opt) {
    Sercom *sercom_instance = get_sercom_inst(spi_peripheral_num);
    gpio_set_pin_lvl(chip_select_pin, GPIO_LOW);
    sercom_instance->SPI.CTRLA.reg |= (SERCOM_SPI_CTRLA_ENABLE);
    spi_wait_for_sync(sercom_instance, SERCOM_SPI_SYNCBUSY_ENABLE);
    return UHAL_STATUS_OK;
}

uhal_status_t spi_host_end_transaction(const spi_host_inst_t spi_peripheral_num, const gpio_pin_t chip_select_pin) {
    Sercom *sercom_instance = get_sercom_inst(spi_peripheral_num);
    sercom_instance->SPI.CTRLA.reg &= ~(SERCOM_SPI_CTRLA_ENABLE);
    spi_wait_for_sync(sercom_instance, SERCOM_SPI_SYNCBUSY_ENABLE);
    gpio_set_pin_lvl(chip_select_pin, GPIO_HIGH);
    return UHAL_STATUS_OK;
}

uhal_status_t
spi_host_write_blocking(const spi_host_inst_t spi_peripheral_num, const unsigned char *write_buff, const size_t size) {
    spi_host_write_non_blocking(spi_peripheral_num, write_buff, size);
//    spi_wait_for_transaction_finish(&sercom_bustrans_buffer[spi_peripheral_num], SERCOMACT_IDLE_SPI_HOST);
    sercom_bustrans_buffer[spi_peripheral_num].transaction_type = SERCOMACT_IDLE_SPI_HOST;
    return UHAL_STATUS_OK;
}

uint8_t transferdata(Sercom *sercom, uint8_t data) {
    sercom->SPI.DATA.bit.DATA = data; // Writing data into Data register

    while (sercom->SPI.INTFLAG.bit.RXC == 0) {
        // Waiting Complete Reception
    }

    return sercom->SPI.DATA.bit.DATA;  // Reading data
}

uhal_status_t spi_host_write_non_blocking(const spi_host_inst_t spi_peripheral_num, const unsigned char *write_buff,
                                          const size_t size) {
    Sercom *sercom_instance = get_sercom_inst(spi_peripheral_num);
    for (uint8_t i = 0; i < size; i++) {
        transferdata(sercom_instance, write_buff[i]);
    }
    return UHAL_STATUS_OK;
}

uhal_status_t
spi_host_read_blocking(const spi_host_inst_t spi_peripheral_num, unsigned char *read_buff, size_t amount_of_bytes) {
    spi_host_read_non_blocking(spi_peripheral_num, read_buff, amount_of_bytes);
//    spi_wait_for_transaction_finish(&sercom_bustrans_buffer[sercom_inst_num], SERCOMACT_IDLE_SPI_HOST);
    return UHAL_STATUS_OK;
}

uhal_status_t
spi_host_read_non_blocking(const spi_host_inst_t spi_peripheral_num, unsigned char *read_buff, size_t amount_of_bytes) {
    Sercom *sercom_instance = get_sercom_inst(spi_peripheral_num);
    for (uint8_t i = 0; i < amount_of_bytes; i++) {
        read_buff[i] = transferdata(sercom_instance, 0x00);
    }
    return UHAL_STATUS_OK;
}

#endif /* DISABLE_SPI_HOST_MODULE */