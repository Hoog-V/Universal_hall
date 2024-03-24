/**
* \file            spi.c
* \brief           Source file which implements the standard SPI API functions
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
*
*/
#include <hal_spi_host.h>
#include <hardware/spi.h>
#include <stddef.h>
#include "bit_manipulation.h"

static inline spi_inst_t* get_spi_inst(const spi_host_inst_t spi_peripheral_num) {
    return (spi_peripheral_num == 0) ? spi0 : spi1;
}

uhal_status_t spi_host_init(const spi_host_inst_t spi_peripheral_num, const uint32_t spi_clock_source, const uint32_t spi_clock_source_freq,
                            const unsigned long spi_bus_frequency, const spi_bus_opt_t spi_extra_configuration_opt) {

    spi_inst_t* inst = get_spi_inst(spi_peripheral_num);
    uint32_t    baudrate_ret = spi_init(inst, spi_bus_frequency);
    if (spi_extra_configuration_opt != SPI_BUS_OPT_USE_DEFAULT) {
        const uint8_t cpol = (BITMASK_COMPARE(spi_extra_configuration_opt, SPI_BUS_OPT_CLOCK_POLARITY_SCK_HIGH) >> 1);
        const uint8_t cpha = (BITMASK_COMPARE(spi_extra_configuration_opt, SPI_BUS_OPT_DATA_VALID_ON_TRAILING));
        const uint8_t databits = (BITMASK_COMPARE(spi_extra_configuration_opt, SPI_BUS_OPT_CHAR_SIZE_10_BIT) >> 3) + 3;
        const uint8_t msbfirst = ~((BITMASK_COMPARE(spi_extra_configuration_opt, SPI_BUS_OPT_DATA_ORDER_LSB_FIRST) >> 2));
        spi_set_format(inst, databits, cpol, cpha, msbfirst);
    }
    return (baudrate_ret > 0) ? UHAL_STATUS_OK : UHAL_STATUS_ERROR;
}

uhal_status_t spi_host_deinit(const spi_host_inst_t spi_peripheral_num) {
    spi_inst_t* inst = get_spi_inst(spi_peripheral_num);
    spi_deinit(inst);
    return UHAL_STATUS_OK;
}

uhal_status_t spi_host_start_transaction(const spi_host_inst_t spi_peripheral_num, const gpio_pin_t chip_select_pin,
                                         const spi_extra_dev_opt_t device_specific_config_opt) {
    if (chip_select_pin != -1) {
        return (gpio_set_pin_lvl(chip_select_pin, GPIO_LOW));
    } else {
        return UHAL_STATUS_OK;
    }
}

uhal_status_t spi_host_end_transaction(const spi_host_inst_t spi_peripheral_num, const gpio_pin_t chip_select_pin) {
    if (chip_select_pin != -1) {
        return (gpio_set_pin_lvl(chip_select_pin, GPIO_HIGH));
    } else {
        return UHAL_STATUS_OK;
    }
}

uhal_status_t spi_host_write_blocking(const spi_host_inst_t spi_peripheral_num, const unsigned char* write_buff, const size_t size) {
    spi_inst_t* inst = get_spi_inst(spi_peripheral_num);
    size_t      n = spi_write_blocking(inst, write_buff, size);
    return (n == size) ? UHAL_STATUS_OK : UHAL_STATUS_ERROR;
}

uhal_status_t spi_host_write_non_blocking(const spi_host_inst_t spi_peripheral_num, const unsigned char* write_buff, const size_t size) {
    spi_inst_t* inst = get_spi_inst(spi_peripheral_num);
    size_t      n = spi_write_blocking(inst, write_buff, size);
    return (n == size) ? UHAL_STATUS_OK : UHAL_STATUS_ERROR;
}

uhal_status_t spi_host_read_blocking(const spi_host_inst_t spi_peripheral_num, unsigned char* read_buff, size_t amount_of_bytes) {
    spi_inst_t* inst = get_spi_inst(spi_peripheral_num);
    size_t      n = spi_read_blocking(inst, 0, read_buff, amount_of_bytes);
    return (n == amount_of_bytes) ? UHAL_STATUS_OK : UHAL_STATUS_ERROR;
}

uhal_status_t spi_host_read_non_blocking(const spi_host_inst_t spi_peripheral_num, unsigned char* read_buff, size_t amount_of_bytes) {
    spi_inst_t* inst = get_spi_inst(spi_peripheral_num);
    size_t      n = spi_read_blocking(inst, 0, read_buff, amount_of_bytes);
    return (n == amount_of_bytes) ? UHAL_STATUS_OK : UHAL_STATUS_ERROR;
}