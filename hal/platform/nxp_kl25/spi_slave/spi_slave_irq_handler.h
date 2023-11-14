/**
* \file            spi_slave_irq_handler.h
* \brief           Header file which implements the default SPI Slave IRQ Handler
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

#ifndef SPI_SLAVE_IRQ_HANDLER_H
#define SPI_SLAVE_IRQ_HANDLER_H

#include <stddef.h>
#include <sam.h>
#include "irq/communication_peripheral_typedefs.h"

void spi_slave_chip_select_irq(const void *hw, volatile bustransaction_t *transaction) {
    ((Sercom *) hw)->SPI.INTFLAG.reg = SERCOM_SPI_INTFLAG_SSL;
    transaction->transaction_type = SERCOMACT_IDLE_SPI_SLAVE;
}

void spi_slave_data_recv_irq(const void *hw, volatile bustransaction_t *transaction) {
    ((Sercom *) hw)->SPI.INTFLAG.reg = SERCOM_SPI_INTFLAG_RXC;
    transaction->transaction_type = SERCOMACT_IDLE_SPI_SLAVE;
}

void spi_slave_data_send_irq(const void *hw, volatile bustransaction_t *transaction) {
    ((Sercom *) hw)->SPI.INTFLAG.reg = SERCOM_SPI_INTFLAG_TXC;
    transaction->transaction_type = SERCOMACT_IDLE_SPI_SLAVE;
}

#endif