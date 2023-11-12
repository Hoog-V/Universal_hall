/**
* \file            i2c_platform_specific.h
* \brief           Include file with platform specific options for the I2C module
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

#ifndef I2C_MASTER_PLATFORM_SPECIFIC
#define I2C_MASTER_PLATFORM_SPECIFIC
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <sam.h>
#include <stddef.h>
#include "clock_system/peripheral_clocking.h"
#include "error_handling.h"
#include "irq/sercom_stuff.h"

typedef enum {
    I2C_PERIPHERAL_0,
    I2C_PERIPHERAL_1,
    I2C_PERIPHERAL_2,
    I2C_PERIPHERAL_3,
    I2C_PERIPHERAL_4,
    I2C_PERIPHERAL_5
} i2c_periph_inst_t;

typedef enum {
    I2C_CLK_SOURCE_USE_DEFAULT = 0x00,
    I2C_CLK_SOURCE_FAST_CLKGEN0 = 0x01,
    I2C_CLK_SOURCE_FAST_CLKGEN1 = 0x02,
    I2C_CLK_SOURCE_FAST_CLKGEN2 = 0x03,
    I2C_CLK_SOURCE_FAST_CLKGEN3 = 0x04,
    I2C_CLK_SOURCE_FAST_CLKGEN4 = 0x05,
    I2C_CLK_SOURCE_FAST_CLKGEN5 = 0x06,
    I2C_CLK_SOURCE_FAST_CLKGEN6 = 0x07,
    I2C_CLK_SOURCE_FAST_CLKGEN7 = 0x08,
    I2C_CLK_SOURCE_SLOW_CLKGEN0 = 0x100,
    I2C_CLK_SOURCE_SLOW_CLKGEN1 = 0x200,
    I2C_CLK_SOURCE_SLOW_CLKGEN2 = 0x300,
    I2C_CLK_SOURCE_SLOW_CLKGEN3 = 0x400,
    I2C_CLK_SOURCE_SLOW_CLKGEN4 = 0x500,
    I2C_CLK_SOURCE_SLOW_CLKGEN5 = 0x600,
    I2C_CLK_SOURCE_SLOW_CLKGEN6 = 0x700,
    I2C_CLK_SOURCE_SLOW_CLKGEN7 = 0x800,
} i2c_clock_sources_t;

typedef enum {
    I2C_EXTRA_OPT_NONE = 0,
    I2C_EXTRA_OPT_4_WIRE_MODE = 1,
    I2C_EXTRA_OPT_IRQ_PRIO_0 = 0x100,
    I2C_EXTRA_OPT_IRQ_PRIO_1 = 0x200,
    I2C_EXTRA_OPT_IRQ_PRIO_2 = 0x300,
    I2C_EXTRA_OPT_IRQ_PRIO_3 = 0x400
} i2c_extra_opt_t;

#define I2C_HOST_INIT_FUNC_PARAMETER_CHECK(i2c_peripheral_num, clock_sources, periph_clk_freq, baud_rate_freq, extra_configuration_options)          \
    do {                                                                                                                                             \
        const uint32_t max_freq = 48000000;                                                                                                          \
        const uint32_t min_freq = 200000;                                                                                                            \
        const uint32_t max_supported_baud_rate = 1000000;                                                                                            \
        const uint32_t min_supported_baud_rate = 100000;                                                                                             \
        static_assert(i2c_peripheral_num <= I2C_PERIPHERAL_5 && i2c_peripheral_num >= I2C_PERIPHERAL_0,                                              \
                      "Invalid i2c peripheral instance number given to host driver!");                                                               \
        static_assert(clock_sources <= I2C_CLK_SOURCE_SLOW_CLKGEN7 && clock_sources >= I2C_CLK_SOURCE_USE_DEFAULT,                                   \
                      "Invalid clock-source used for the i2c host driver!");                                                                         \
        static_assert(periph_clk_freq <= max_freq, "I2C peripheral clock frequency higher than maximum allowed frequency");                          \
        static_assert(periph_clk_freq >= min_freq,                                                                                                   \
                      "I2C peripheral clock frequency has to be atleast higher than 2x the standard slow i2c baud_rate of 100KHz");                  \
        static_assert(baud_rate_freq <= max_supported_baud_rate && baud_rate_freq >= min_supported_baud_rate,                                        \
                      "Unsupported baud rate option set on I2C host driver!");                                                                       \
        static_assert((extra_configuration_options <= I2C_EXTRA_OPT_IRQ_PRIO_3 && extra_configuration_options >= I2C_EXTRA_OPT_IRQ_PRIO_0)           \
                          || extra_configuration_options == I2C_EXTRA_OPT_NONE,                                                                      \
                      "Invalid IRQ priority set on I2C host driver!");                                                                               \
        static_assert((extra_configuration_options & 0xFF) <= I2C_EXTRA_OPT_4_WIRE_MODE,                                                             \
                      "Unsupported extra configurations options set on I2C host driver!");                                                           \
    } while (0);

#define I2C_HOST_DEINIT_FUNC_PARAMETER_CHECK(i2c_peripheral_num)                                                                                     \
    do {                                                                                                                                             \
        static_assert(i2c_peripheral_num <= I2C_PERIPHERAL_5 && i2c_peripheral_num >= I2C_PERIPHERAL_0,                                              \
                      "Invalid i2c peripheral instance number given to host driver!");                                                               \
    } while (0);

#define I2C_HOST_WRITE_FUNC_PARAMETER_CHECK(i2c_peripheral_num, addr, write_buff, size, stop_bit)                                                    \
    do {                                                                                                                                             \
        static_assert(i2c_peripheral_num <= I2C_PERIPHERAL_5 && i2c_peripheral_num >= I2C_PERIPHERAL_0,                                              \
                      "Invalid i2c peripheral instance number given to host driver!");                                                               \
        static_assert(addr <= 1023 && addr > 0, "Invalid I2C address given!");                                                                       \
        static_assert(write_buff != NULL && sizeof(write_buff) >= size, "writebuffer is equal to NULL or buffer overflow!");                         \
        static_assert(stop_bit <= 1, "Stop-bit can't have a higher value than 1!");                                                                  \
    } while (0);

#define I2C_HOST_READ_FUNC_PARAMETER_CHECK(i2c_peripheral_num, addr, read_buff, size)                                                                \
    do {                                                                                                                                             \
        static_assert(i2c_peripheral_num <= I2C_PERIPHERAL_5 && i2c_peripheral_num >= I2C_PERIPHERAL_0,                                              \
                      "Invalid i2c peripheral instance number given to host driver!");                                                               \
        static_assert(addr <= 1023 && addr > 0, "Invalid I2C address given!");                                                                       \
        static_assert(read_buff != NULL && sizeof(read_buff) >= size, "readbuffer is equal to NULL or buffer overflow!");                            \
    } while (0);

#define I2C_SLAVE_INIT_PARAMETER_CHECK(i2c_peripheral_num, slave_addr, clock_sources, extra_configuration_options)                              \
    do {                                                                                                                                             \
    } while (0);

#define I2C_SLAVE_DEINIT_PARAMETER_CHECK(i2c_peripheral_num)                                                                                         \
    do {                                                                                                                                             \
    } while (0);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
