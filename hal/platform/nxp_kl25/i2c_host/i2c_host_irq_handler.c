/**
* \file            i2c_host_irq_handler.h
* \brief           Header file which implements the default I2C HOST IRQ Handler
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
* Authors:          Victor Hogeweij <hogeweyv@gmail.com>
*/

#ifndef I2C_HOST_IRQ_HANDLER_H
#define I2C_HOST_IRQ_HANDLER_H

#include <stdbool.h>
#include <stddef.h>
#include <fsl_device_registers.h>
#include "irq/communication_peripheral_typedefs.h"
#include "error_handling.h"

volatile bustransaction_t i2c_comm_peripheral_bus_trans[2];

static inline void issue_stop_cond(const void *hw){
I2C_Type *i2c_periph = (I2C_Type*)hw;
i2c_periph->C1 &= ~(I2C_C1_MST_MASK);
}

/**
 * @brief Default IRQ Handler for the I2C master data send interrupt
 * @param hw Pointer to the HW peripheral to be manipulated
 * @param transaction The current transaction information
 */
void i2c_host_data_send_irq(const void *hw, volatile bustransaction_t *transaction) {
    I2C_Type *i2c_periph = (I2C_Type *)hw;
    const bool write_buffer_exists = (transaction->write_buffer != NULL);
    const bool has_bytes_left_to_write = (transaction->buf_cnt < transaction->buf_size);
    const bool device_has_not_acknowledged = (i2c_periph->C1 & I2C_S_RXAK_MASK);
    if(device_has_not_acknowledged) {
        transaction->status = UHAL_STATUS_I2C_NACK;
        issue_stop_cond(i2c_periph);
    }
    if (write_buffer_exists && has_bytes_left_to_write) {
        i2c_periph->D = transaction->write_buffer[transaction->buf_cnt++];
    } else {
        const bool send_stop_bit = (transaction->transaction_type != PERIPH_ACT_I2C_TRANSMIT_NO_STOP);
        if (send_stop_bit) {
            i2c_periph->C1 &= ~(I2C_C1_MST_MASK);
        } else {
            i2c_periph->S |= I2C_S_IICIF_MASK;
        }
        transaction->transaction_type = PERIPH_ACT_NONE;
        transaction->buf_cnt = 0;
    }
    i2c_periph->S |= I2C_S_IICIF_MASK;
}


void i2c_host_data_recv_irq(const void *hw, volatile bustransaction_t *transaction) {
    I2C_Type *i2c_periph = (I2C_Type *)hw;
    if (transaction->read_buffer != NULL && transaction->buf_cnt < transaction->buf_size) {
        transaction->read_buffer[transaction->buf_cnt++] = i2c_periph->D;
        const bool last_byte_read = transaction->buf_cnt >= transaction->buf_size;
        if (last_byte_read) {
            i2c_periph->C1 |= I2C_C1_TXAK_MASK;
        }
    } else {
        i2c_periph->C1 |= I2C_C1_TXAK_MASK;
        transaction->buf_cnt = 0;
        transaction->transaction_type = PERIPH_ACT_NONE;
    }
    i2c_periph->S |= I2C_S_IICIF_MASK;
}

#endif