/**
* \file            gpio_iwr68xx.c
* \brief           Source file which implements the standard GPIO API functions
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
*/
#include <gpio/gpio_platform_specific.h>
#include <hal_gpio.h>
#include <AWR6843.h>
#include <AWR6843_GIO.h>
#include <bit_manipulation.h>

#define GPIO_PORT(pin) GET_LOWER_4_BITS_OF_BYTE(pin)
#define GPIO_PIN(pin) GET_UPPER_4_BITS_OF_BYTE(pin)

uhal_status_t gpio_toggle_pin_output(const gpio_pin_t pin) {
    #ifdef TI_RTOS
    uintptr_t key = HwiP_disable();
    #endif
    uint32_t res =  GIO->GIOPORT[GPIO_PORT(pin)].GIODIN.bit.DIN;
    if(BIT_IS_SET(res, GPIO_PIN(pin))) {
        GIO->GIOPORT[GPIO_PORT(pin)].GIODCLR.bit.DCLR = (1 << GPIO_PIN(pin));
    } else {
        GIO->GIOPORT[GPIO_PORT(pin)].GIODSET.bit.DSET = (1 << GPIO_PIN(pin));
    }
    #ifdef TI_RTOS
        HwiP_restore(key);
    #endif
    return UHAL_STATUS_OK;
}

uhal_status_t gpio_set_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode) {
    #ifdef TI_RTOS
    uintptr_t key = HwiP_disable();
    #endif
    GIO->GIOPORT[GPIO_PORT(pin)].GIODIR.bit.DIR = ((pin_mode & 1) << GPIO_PIN(pin));
    #ifdef TI_RTOS
        HwiP_restore(key);
    #endif
    return UHAL_STATUS_OK;
}

uhal_status_t gpio_set_pin_lvl(const gpio_pin_t pin, gpio_level_t level) {
    #ifdef TI_RTOS
    uintptr_t key = HwiP_disable();
    #endif
   if(level) {
    GIO->GIOPORT[GPIO_PORT(pin)].GIODOUT.bit.DOUT = (1 << GPIO_PIN(pin));
   } else {
    GIO->GIOPORT[GPIO_PORT(pin)].GIODCLR.bit.DCLR = (1 << GPIO_PIN(pin));
   }
    #ifdef TI_RTOS
        HwiP_restore(key);
    #endif
    return UHAL_STATUS_OK;
}

gpio_level_t gpio_get_pin_lvl(const gpio_pin_t pin) {
    uint32_t res;
    #ifdef TI_RTOS
    uintptr_t key = HwiP_disable();
    #endif
    res = GIO->GIOPORT[GPIO_PORT(pin)].GIODIN.bit.DIN;
    #ifdef TI_RTOS
        HwiP_restore(key);
    #endif    
    return (gpio_level_t)(BIT_IS_SET(res, GPIO_PIN(pin)));
}

gpio_mode_t gpio_get_pin_mode(const gpio_pin_t pin) {
    uint32_t res;
    #ifdef TI_RTOS
    uintptr_t key = HwiP_disable();
    #endif
    res = GIO->GIOPORT[GPIO_PORT(pin)].GIODIR.bit.DIR;
    #ifdef TI_RTOS
        HwiP_restore(key);
    #endif    
    return (gpio_mode_t)(BIT_IS_SET(res, GPIO_PIN(pin)));
}

uhal_status_t gpio_set_pin_options(const gpio_pin_t pin, const gpio_opt_t opt) {
    return UHAL_STATUS_OK;
}

gpio_opt_t gpio_get_pin_options(const gpio_pin_t pin) {
    return GPIO_OPT_DRIVE_STRENGTH_LOW;
}

uhal_status_t gpio_set_interrupt_on_pin(const gpio_pin_t pin, gpio_irq_opt_t irq_opt) {
    return UHAL_STATUS_OK;
}
