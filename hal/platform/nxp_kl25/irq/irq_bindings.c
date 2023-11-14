/**
* \file            irq_bindings.c
* \brief           Source file which binds peripheral/system irq handlers 
*                  to the internal module irq handlers of this framework.
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

#include <stddef.h>
#include <fsl_device_registers.h>
#include <irq/communication_peripheral_typedefs.h>

#ifndef DISABLE_GPIO_MODULE

#include "hal_gpio.h"

#endif

#ifndef DISABLE_I2C_MODULE
#include "hal_i2c_host.h"
#endif


#include "bit_manipulation.h"



void enable_irq_handler(IRQn_Type irq_type, uint8_t priority) {
    NVIC_DisableIRQ(irq_type);
    NVIC_ClearPendingIRQ(irq_type);
    NVIC_SetPriority(irq_type, priority);
    NVIC_EnableIRQ(irq_type);
}


void PORTA_IRQHandler(void) {
    gpio_irq_handler(PORTA);
}
void PORTD_IRQHandler(void) {
    gpio_irq_handler(PORTD);
}

void I2C0_IRQHandler(void) {
    volatile bustransaction_t *bustransaction = &i2c_comm_peripheral_bus_trans[0];
if(bustransaction->transaction_type >= PERIPH_ACT_I2C_TRANSMIT_NO_STOP) {
    i2c_host_data_send_irq(I2C0, bustransaction);
} else {
    i2c_host_data_recv_irq(I2C0, bustransaction);
}
}