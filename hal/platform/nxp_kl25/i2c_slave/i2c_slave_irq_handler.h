/**
* \file            i2c_slave_irq_handler.h
* \brief           Header file which implements the default I2C Slave IRQ Handler
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

#ifndef I2C_SLAVE_IRQ_HANDLER_H
#define I2C_SLAVE_IRQ_HANDLER_H

#include <stdbool.h>
#include <sam.h>


void i2c_slave_data_recv_irq(const void *const hw, volatile bustransaction_t *Transaction) {
    ((Sercom *) hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_DRDY;
    Transaction->transaction_type = SERCOMACT_IDLE_I2CS;
}

void i2c_slave_data_send_irq(const void *const hw, volatile bustransaction_t *Transaction) {
    ((Sercom *) hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_DRDY;
    Transaction->transaction_type = SERCOMACT_IDLE_I2CS;
}

void i2c_slave_stop_irq(const void *const hw, volatile bustransaction_t *Transaction) {
    ((Sercom *) hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_PREC;
    Transaction->transaction_type = SERCOMACT_IDLE_I2CS;
}

void i2c_slave_address_match_irq(const void *const hw, volatile bustransaction_t *transaction) {
    ((Sercom *) hw)->I2CS.INTFLAG.reg = SERCOM_I2CS_INTFLAG_AMATCH;
    transaction->transaction_type = SERCOMACT_IDLE_I2CS;
}

void i2c_slave_handler(const void *const hw, volatile bustransaction_t *Transaction) {
    Sercom *sercom_instance = ((Sercom *) hw);
    const bool addressMatchInt = sercom_instance->I2CS.INTFLAG.reg & SERCOM_I2CS_INTFLAG_AMATCH;
    const bool stopInt = sercom_instance->I2CS.INTFLAG.reg & SERCOM_I2CS_INTFLAG_PREC;
    const bool dataReadyInt = sercom_instance->I2CS.INTFLAG.reg & SERCOM_I2CS_INTFLAG_DRDY;
    const bool isReadTransaction = sercom_instance->I2CS.STATUS.bit.DIR;
    if (addressMatchInt) {
        i2c_slave_address_match_irq(hw, Transaction);
    }
    if (stopInt) {
        i2c_slave_stop_irq(hw, Transaction);
    }
    if (dataReadyInt && isReadTransaction) {
        i2c_slave_data_send_irq(hw, Transaction);
    }
    if (dataReadyInt && !isReadTransaction) {
        i2c_slave_data_recv_irq(hw, Transaction);
    }
}

#endif