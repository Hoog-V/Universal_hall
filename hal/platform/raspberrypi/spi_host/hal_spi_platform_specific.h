/**
* \file            spi_platform_specific.h
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

#ifndef HAL_SPI_PLATFORM_SPECIFIC
#define HAL_SPI_PLATFORM_SPECIFIC
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stddef.h>
#include <gpio_platform_specific.h>

typedef enum { SPI_OPERATING_MODE_MASTER, SPI_OPERATING_MODE_SLAVE } spi_operating_mode_t;

typedef struct {
    spi_operating_mode_t operating_mode;
} spi_periph_inst_t;

typedef struct {
    spi_periph_inst_t* spi_peripheral;
    gpio_pin_t cs_pin;
} spi_dev_t;

typedef struct {
    uint8_t transaction_type;
    uint8_t instance_num;
    const uint8_t* write_buffer;
    uint8_t* read_buffer;
    uint8_t buf_size;
    uint8_t buf_cnt;
} bustransaction_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
