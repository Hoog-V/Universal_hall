/**
* \file            uart_platform_specific.h
* \brief           Source file which implements the standard SPI API functions
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
*
*/

#ifndef UART_PLATFORM_SPECIFIC
#define UART_PLATFORM_SPECIFIC

#include "error_handling.h"
#include <stdint.h>
#include <stddef.h>

typedef enum {
    UART_PERIPHERAL_0,
    UART_PERIPHERAL_1
} uart_peripheral_inst_t;

typedef enum {
    UART_EXTRA_OPT_USE_DEFAULT,
    UART_EXTRA_OPT_PARITY_EVEN = 16777216,
    UART_EXTRA_OPT_PARITY_ODD = 33554432,
    UART_EXTRA_OPT_TWO_STOP_BITS = 8,
    UART_EXTRA_OPT_CHAR_SIZE_5_BITS = 5,
    UART_EXTRA_OPT_CHAR_SIZE_6_BITS = 6,
    UART_EXTRA_OPT_CHAR_SIZE_7_BITS = 7,
} uart_extra_config_opt_t;

typedef enum {
    UART_CLK_SOURCE_USE_DEFAULT = 0x00
} uart_clock_sources_t;

#endif /* UART_PLATFORM_SPECIFIC */