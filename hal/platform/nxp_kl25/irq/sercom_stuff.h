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

#ifndef ATMELSAMD21_SERCOM_STUFF_H
#define ATMELSAMD21_SERCOM_STUFF_H

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

extern volatile bustransaction_t sercom_bustrans_buffer[6];


typedef enum {
    SERCOM_NUM_0, SERCOM_NUM_1, SERCOM_NUM_2, SERCOM_NUM_3, SERCOM_NUM_4, SERCOM_NUM_5
} sercom_num_t;

typedef enum {
    SERCOMACT_NONE,
    SERCOMACT_IDLE_I2CS,
    SERCOMACT_IDLE_I2CM,
    SERCOMACT_IDLE_SPI_HOST,
    SERCOMACT_IDLE_SPI_SLAVE,
    SERCOMACT_I2C_DATA_TRANSMIT_NO_STOP,
    SERCOMACT_I2C_DATA_TRANSMIT_STOP,
    SERCOMACT_I2C_DATA_RECEIVE_STOP,
    SERCOMACT_SPI_DATA_TRANSMIT,
    SERCOMACT_SPI_DATA_RECEIVE
} busactions_t;


#endif //ATMELSAMD21_SERCOM_STUFF_H
