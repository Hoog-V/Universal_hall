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
#include <irq/sercom_stuff.h>

#ifndef DISABLE_GPIO_MODULE

#include "gpio/gpio_irq_handler.h"

#endif


#include "bit_manipulation.h"

void enable_irq_handler(IRQn_Type irq_type, uint8_t priority) {
    NVIC_EnableIRQ(irq_type);
    NVIC_SetPriority(irq_type, priority);
}


void PORTA_IRQHandler() {
   
}