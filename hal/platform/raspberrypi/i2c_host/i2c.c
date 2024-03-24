/**
* \file            i2c.c
* \brief           Source file which implements the standard I2C API functions
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

#include "hardware/i2c.h"
#include "hal_i2c_host.h"

static inline i2c_inst_t* get_i2c_inst(const i2c_periph_inst_t i2c_peripheral_num) {
    return (i2c_peripheral_num == 0) ? i2c0 : i2c1;
}

uhal_status_t i2c_host_init(const i2c_periph_inst_t i2c_peripheral_num, const i2c_clock_sources_t clock_sources, const uint32_t periph_clk_freq,
                            const uint32_t baud_rate_freq, const i2c_extra_opt_t extra_configuration_options) {

    i2c_inst_t* inst = get_i2c_inst(i2c_peripheral_num);
    uint32_t baudrate_ret = i2c_init(inst, baud_rate_freq);
    return (baudrate_ret > 0) ? UHAL_STATUS_OK : UHAL_STATUS_ERROR;
}

uhal_status_t i2c_host_deinit(const i2c_periph_inst_t i2c_peripheral_num) {
    i2c_inst_t* inst = get_i2c_inst(i2c_peripheral_num);
    i2c_deinit(inst);
    return UHAL_STATUS_OK;
}

uhal_status_t i2c_host_write_blocking(const i2c_periph_inst_t i2c_peripheral_num, const uint16_t addr, const uint8_t* write_buff, const size_t size,
                                      const i2c_stop_bit_t stop_bit) {

    i2c_inst_t* inst = get_i2c_inst(i2c_peripheral_num);
    size_t n = i2c_write_blocking(inst, addr, write_buff, size, stop_bit);
    return (n == size) ? UHAL_STATUS_OK : UHAL_STATUS_ERROR;
}

uhal_status_t i2c_host_write_non_blocking(const i2c_periph_inst_t i2c_peripheral_num, const uint16_t addr, const uint8_t* write_buff,
                                          const size_t size, const i2c_stop_bit_t stop_bit) {

    i2c_inst_t* inst = get_i2c_inst(i2c_peripheral_num);
    size_t n = i2c_write_blocking(inst, addr, write_buff, size, stop_bit);
    return (n == size) ? UHAL_STATUS_OK : UHAL_STATUS_ERROR;
}

uhal_status_t i2c_host_read_blocking(const i2c_periph_inst_t i2c_peripheral_num, const uint16_t addr, uint8_t* read_buff,
                                     const size_t amount_of_bytes) {

    i2c_inst_t* inst = get_i2c_inst(i2c_peripheral_num);
    size_t n = i2c_read_blocking(inst, addr, read_buff, amount_of_bytes, I2C_STOP_BIT);
    return (n == amount_of_bytes) ? UHAL_STATUS_OK : UHAL_STATUS_ERROR;
}

uhal_status_t i2c_host_read_non_blocking(const i2c_periph_inst_t i2c_peripheral_num, const uint16_t addr, uint8_t* read_buff,
                                         const size_t amount_of_bytes) {

    i2c_inst_t* inst = get_i2c_inst(i2c_peripheral_num);
    size_t n = i2c_read_blocking(inst, addr, read_buff, amount_of_bytes, I2C_STOP_BIT);
    return (n == amount_of_bytes) ? UHAL_STATUS_OK : UHAL_STATUS_ERROR;
}