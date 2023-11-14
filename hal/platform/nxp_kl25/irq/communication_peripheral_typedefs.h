/**
* \file            sercom_stuff.h
* \brief           Include file with configuration options and structures used for the sercom
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

#ifndef COMMUNICATION_PERIPHERAL_TYPEDEFS_H
#define COMMUNICATION_PERIPHERAL_TYPEDEFS_H

#include <stdint.h>


typedef struct {
    uint8_t transaction_type;
    uint8_t instance_num;
    const uint8_t *write_buffer;
    uint8_t *read_buffer;
    uint8_t buf_size;
    uint8_t buf_cnt;
    uint8_t status;
} bustransaction_t;

extern volatile bustransaction_t i2c_comm_peripheral_bus_trans[2];

extern volatile bustransaction_t spi_comm_peripheral_bus_trans[2];

extern volatile bustransaction_t uart_comm_peripheral_bus_trans[3];


typedef enum {
    PERIPH_ACT_NONE,
    PERIPH_ACT_I2C_TRANSMIT_NO_STOP,
    PERIPH_ACT_I2C_TRANSMIT_STOP,
    PERIPH_ACT_I2C_RECEIVE_NO_STOP,
    PERIPH_ACT_I2C_RECEIVE_STOP
} busactions_t;


#endif // COMMUNICATION_PERIPHERAL_TYPEDEFS_H
