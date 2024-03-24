/**
* \file            spi_platform_specific.h
* \brief           Include file with platform specific options for the SPI module
*/
/*
*  Copyright 2024 (C) Victor Hogeweij <hogeweyv@gmail.com>
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

#ifndef SPI_PLATFORM_SPECIFIC_H
#define SPI_PLATFORM_SPECIFIC_H

#include <stdint.h>
#include <stdio.h>
#include <irq/irq_typedefs.h>

typedef enum {
    SPI_PERIPHERAL_0,
    SPI_PERIPHERAL_1
} spi_host_inst_t;

typedef spi_host_inst_t spi_slave_inst_t;

typedef enum {
    SPI_CLK_SOURCE_USE_DEFAULT = 0x00
} spi_clock_sources_t;

typedef enum {
    SPI_BUS_OPT_USE_DEFAULT = 0,
    SPI_BUS_OPT_DATA_VALID_ON_TRAILING = 0x01,
    SPI_BUS_OPT_CLOCK_POLARITY_SCK_HIGH = 0x02,
    SPI_BUS_OPT_DATA_ORDER_LSB_FIRST = 0x04,
    SPI_BUS_OPT_CHAR_SIZE_4_BIT = 0x08,
    SPI_BUS_OPT_CHAR_SIZE_5_BIT = 0x10,
    SPI_BUS_OPT_CHAR_SIZE_6_BIT = 0x18,
    SPI_BUS_OPT_CHAR_SIZE_7_BIT = 0x20,
    SPI_BUS_OPT_CHAR_SIZE_8_BIT = 0x28,
    SPI_BUS_OPT_CHAR_SIZE_9_BIT = 0x30,
    SPI_BUS_OPT_CHAR_SIZE_10_BIT = 0x38,
    SPI_BUS_OPT_CHAR_SIZE_11_BIT = 0x40,
    SPI_BUS_OPT_CHAR_SIZE_12_BIT = 0x48,
    SPI_BUS_OPT_CHAR_SIZE_13_BIT = 0x50,
    SPI_BUS_OPT_CHAR_SIZE_14_BIT = 0x58,
    SPI_BUS_OPT_CHAR_SIZE_15_BIT = 0x60,
    SPI_BUS_OPT_CHAR_SIZE_16_BIT = 0x68
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

#endif /* SPI_PLATFORM_SPECIFIC_H */
