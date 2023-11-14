/**
* \file            spi_common_platform_specific.h
* \brief           Include file with platform specific options for the SPI module
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

#ifndef ATMELSAMD21_SPI_PLATFORM_SPECIFIC_H
#define ATMELSAMD21_SPI_PLATFORM_SPECIFIC_H

#include <sam.h>
#include <stdint.h>
#include <stdio.h>
#include "clock_system/peripheral_clocking.h"
#include "irq/communication_peripheral_typedefs.h"

typedef enum {
    SPI_PERIPHERAL_0,
    SPI_PERIPHERAL_1,
    SPI_PERIPHERAL_2,
    SPI_PERIPHERAL_3,
    SPI_PERIPHERAL_4,
    SPI_PERIPHERAL_5
} spi_host_inst_t;

typedef spi_host_inst_t spi_slave_inst_t;

typedef enum {
    SPI_CLK_SOURCE_USE_DEFAULT = 0x00,
    SPI_CLK_SOURCE_FAST_CLKGEN0 = 0x01,
    SPI_CLK_SOURCE_FAST_CLKGEN1 = 0x02,
    SPI_CLK_SOURCE_FAST_CLKGEN2 = 0x03,
    SPI_CLK_SOURCE_FAST_CLKGEN3 = 0x04,
    SPI_CLK_SOURCE_FAST_CLKGEN4 = 0x05,
    SPI_CLK_SOURCE_FAST_CLKGEN5 = 0x06,
    SPI_CLK_SOURCE_FAST_CLKGEN6 = 0x07,
    SPI_CLK_SOURCE_FAST_CLKGEN7 = 0x08,
    SPI_CLK_SOURCE_SLOW_CLKGEN0 = 0x100,
    SPI_CLK_SOURCE_SLOW_CLKGEN1 = 0x200,
    SPI_CLK_SOURCE_SLOW_CLKGEN2 = 0x300,
    SPI_CLK_SOURCE_SLOW_CLKGEN3 = 0x400,
    SPI_CLK_SOURCE_SLOW_CLKGEN4 = 0x500,
    SPI_CLK_SOURCE_SLOW_CLKGEN5 = 0x600,
    SPI_CLK_SOURCE_SLOW_CLKGEN6 = 0x700,
    SPI_CLK_SOURCE_SLOW_CLKGEN7 = 0x800,
} spi_clock_sources_t;

typedef enum {
    SPI_BUS_OPT_USE_DEFAULT = 0,
    SPI_BUS_OPT_CLOCK_POLARITY_SCK_HIGH = 0x01,
    SPI_BUS_OPT_DATA_ORDER_LSB_FIRST = 0x02,
    SPI_BUS_OPT_CHAR_SIZE_10_BIT = 0x04,
    SPI_BUS_OPT_DOPO_PAD_0 = 0x08,
    SPI_BUS_OPT_DOPO_PAD_1 = 0x10,
    SPI_BUS_OPT_DOPO_PAD_2 = 0x18,
    SPI_BUS_OPT_DOPO_PAD_3 = 0x20,
    SPI_BUS_OPT_DIPO_PAD_0 = 0x40,
    SPI_BUS_OPT_DIPO_PAD_1 = 0x80,
    SPI_BUS_OPT_DIPO_PAD_2 = 0xC0,
    SPI_BUS_OPT_DIPO_PAD_3 = 0x100
} spi_bus_opt_t;

typedef enum {
    SPI_EXTRA_OPT_USE_DEFAULT = 0,
    SPI_EXTRA_OPT_CLOCK_POLARITY_SCK_HIGH = 0x01,
    SPI_EXTRA_OPT_DATA_ORDER_LSB_FIRST = 0x02,
} spi_extra_dev_opt_t;

#define SPI_HOST_INIT_PARAMETER_CHECK(spi_peripheral_num, peripheral_clock_source, peripheral_clock_freq, spi_bus_frequency, \
                                      spi_extra_configuration_opt)                                                                                   \
    {                                                                                                                                                \
        const uint32_t max_freq = 48000000;                                                                                                          \
        const uint32_t min_freq = 200000;                                                                                                            \
        const uint32_t max_supported_baud_rate = 24000000;                                                                                           \
        const uint32_t min_supported_baud_rate = 100000;                                                                                             \
        static_assert((spi_peripheral_num <= SERCOM_INST_NUM - 1 && spi_peripheral_num >= 0), "SPI_HOST_INIT: Invalid peripheral!");                 \
        static_assert(peripheral_clock_source <= SPI_CLK_SOURCE_SLOW_CLKGEN7 && peripheral_clock_source >= SPI_CLK_SOURCE_USE_DEFAULT,               \
                      "SPI_HOST_INIT: Invalid clock-source!");                                                                                       \
        static_assert(peripheral_clock_freq <= max_freq, "SPI_HOST_INIT: Peripheral clock frequency too high!");                                     \
        static_assert(peripheral_clock_freq >= min_freq,                                                                                             \
                      "SPI_HOST_INIT: Peripheral clock frequency has to be atleast higher than 2x the minimum spi baud_rate of 100KHz");             \
        static_assert(spi_bus_frequency <= max_supported_baud_rate && spi_bus_frequency >= min_supported_baud_rate,                                  \
                      "SPI_HOST_INIT: Unsupported bus frequency option set!");                                                                       \
        static_assert(spi_extra_configuration_opt <= SPI_BUS_OPT_DIPO_PAD_3, "SPI_HOST_INIT: Unsupported extra configuration options set!");         \
    }                                                                                                                                                \
    while (0)                                                                                                                                        \
        ;

#define SPI_HOST_DEINIT_PARAMETER_CHECK(spi_peripheral_num)                                                                                          \
    do {                                                                                                                                             \
    } while (0);

#define SPI_HOST_DEINIT_PARAMETER_CHECK(spi_peripheral_num)                                                                                          \
    do {                                                                                                                                             \
    } while (0);

#define SPI_HOST_START_TRANSACTION_PARAMETER_CHECK(spi_peripheral_num, chip_select_pin, device_specific_config_opt)                                  \
    do {                                                                                                                                             \
    } while (0);

#define SPI_HOST_END_TRANSACTION_PARAMETER_CHECK(spi_peripheral_num, chip_select_pin)                                                                \
    do {                                                                                                                                             \
    } while (0);

#define SPI_HOST_WRITE_PARAMETER_CHECK(spi_peripheral_num, write_buffer, buffer_size)                                                                \
    do {                                                                                                                                             \
    } while (0);

#define SPI_HOST_READ_PARAMETER_CHECK(spi_peripheral_num, read_buffer, size)                                                                         \
    do {                                                                                                                                             \
    } while (0);

#define SPI_SLAVE_INIT_PARAMETER_CHECK(spi_peripheral_num, peripheral_clock_source, spi_extra_configuration_opt)                                     \
    do {                                                                                                                                             \
    } while (0);

#define SPI_SLAVE_DEINIT_PARAMETER_CHECK(spi_peripheral_num)                                                                                         \
    do {                                                                                                                                             \
    } while (0);

#endif //ATMELSAMD21_SPI_PLATFORM_SPECIFIC_H
