/**
* \file            spi_host_irq_handler.h
* \brief           Header file which implements the default SPI Host IRQ Handler
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

#ifndef SPI_HOST_IRQ_HANDLER_H
#define SPI_HOST_IRQ_HANDLER_H

#include <stddef.h>
#include <sam.h>
#include "irq/communication_peripheral_typedefs.h"

void spi_host_data_send_irq(const void *hw, volatile bustransaction_t *transaction) {
    Sercom *sercom_instance = ((Sercom *) hw);
    if (transaction->transaction_type == SERCOMACT_SPI_DATA_TRANSMIT) {
        const bool write_buffer_exists = (transaction->write_buffer != NULL);
        const bool has_bytes_left_to_write = (transaction->buf_cnt < transaction->buf_size);
        if (write_buffer_exists && has_bytes_left_to_write) {
            sercom_instance->SPI.DATA.reg = transaction->write_buffer[transaction->buf_cnt++];
        } else {
            sercom_instance->SPI.INTFLAG.reg = SERCOM_SPI_INTFLAG_TXC;
            sercom_instance->SPI.INTENCLR.reg = SERCOM_SPI_INTFLAG_DRE;
            transaction->transaction_type = SERCOMACT_IDLE_SPI_HOST;
        }
    } else {
        sercom_instance->SPI.INTFLAG.reg = 0xFF;
    }
}

void spi_host_data_recv_irq(const void *hw, volatile bustransaction_t *transaction) {
    Sercom *sercom_instance = ((Sercom *) hw);
    if (transaction->transaction_type == SERCOMACT_SPI_DATA_RECEIVE) {
        if (transaction->read_buffer != NULL && transaction->buf_cnt < transaction->buf_size + 1) {
            if (transaction->buf_cnt == 0) {
                transaction->read_buffer[transaction->buf_cnt] = sercom_instance->SPI.DATA.reg;
            } else {
                transaction->read_buffer[transaction->buf_cnt - 1] = sercom_instance->SPI.DATA.reg;
            }
            transaction->buf_cnt++;
            const bool last_byte_read = transaction->buf_cnt >= transaction->buf_size;
            if (!last_byte_read) {
                sercom_instance->SPI.DATA.reg = 0;
            }
        } else {
            uint32_t reg = sercom_instance->SPI.CTRLB.reg;
            reg &= ~(SERCOM_SPI_CTRLB_RXEN);
            sercom_instance->SPI.CTRLB.reg = reg;
            sercom_instance->SPI.INTENCLR.reg = SERCOM_SPI_INTFLAG_DRE;
            transaction->transaction_type = SERCOMACT_IDLE_SPI_HOST;
        }
    } else {
        sercom_instance->SPI.INTFLAG.reg = 0xFF;
    }
}

#endif