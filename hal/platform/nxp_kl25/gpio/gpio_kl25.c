/**
* \file            gpio_samd.c
* \brief           Source file which implements the standard GPIO API functions
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

#ifndef DISABLE_GPIO_MODULE

#include <fsl_device_registers.h>
#include "bit_manipulation.h"
#include "gpio_platform_specific.h"
#include "hal_gpio.h"
#include "irq/irq_bindings.h"

#define GPIO_PIN_GROUP(pin) ((pin >> 8) - 1)
#define GPIO_PIN(pin) (pin & 0xFF)

unsigned int PORTS[] = PORT_BASE_ADDRS;
unsigned int GPIOS[] = FGPIO_BASE_ADDRS;

uhal_status_t gpio_set_pin_lvl(const gpio_pin_t pin, gpio_level_t level) {
    const uint8_t port_num = GPIO_PIN_GROUP(pin);
    if (level) {
        /*
         * GPIO LEVEL HIGH: Set the PSOR register to (1 << pin_num).
         * This will set a high output state if a pin is set to the output direction
         * or manually enable pull-ups if the pin is set to output and PULLEN bit is set.
         */
         ((GPIO_Type*)GPIOS[port_num])->PSOR |= (1 << GPIO_PIN(pin));
    } else {
        /* GPIO LEVEL LOW: Set the PCOR register to (1 << pin_num).
         * This will set a low output state if pin is set to the output direction.
         */
        ((GPIO_Type*)GPIOS[port_num])->PCOR |= (1 << GPIO_PIN(pin));
    }
    return UHAL_STATUS_OK;
}

uhal_status_t gpio_toggle_pin_output(const gpio_pin_t pin) {
     const uint8_t port_num = GPIO_PIN_GROUP(pin);
     ((FGPIO_Type*)GPIOS[port_num])->PTOR |= (1 << GPIO_PIN(pin));
    return UHAL_STATUS_OK;
}

gpio_level_t gpio_get_pin_lvl(const gpio_pin_t pin) {
    const uint8_t port_num = GPIO_PIN_GROUP(pin);
    const gpio_level_t res = BIT_IS_SET((((FGPIO_Type*)GPIOS[port_num])->PDIR), GPIO_PIN(pin));
    return res;
}

static inline void prv_set_function(const gpio_pin_t pin, const uint8_t function) {
    const uint8_t port_num = GPIO_PIN_GROUP(pin);
    ((PORT_Type*)PORTS[port_num])->PCR[GPIO_PIN(pin)] |= PORT_PCR_MUX(function);
}

static inline void prv_set_dir(const gpio_pin_t pin, const uint8_t direction) {
    const uint8_t port_num = GPIO_PIN_GROUP(pin);
    ((PORT_Type*)PORTS[port_num])->PCR[GPIO_PIN(pin)] |= PORT_PCR_MUX(1);
    if (direction == GPIO_MODE_OUTPUT) {
        ((FGPIO_Type*)GPIOS[port_num])->PDDR |= (1 << GPIO_PIN(pin));
    } else {
        ((FGPIO_Type*)GPIOS[port_num])->PDDR &= ~(1 << GPIO_PIN(pin));
    }
}

uhal_status_t gpio_set_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode) {
    /*
     * This microcontroller has a clock-gate on the PORT peripheral.
     * We need to enable the clock to this peripheral first
     */
    const uint8_t port_num = GPIO_PIN_GROUP(pin);
    SIM->SCGC5 |= (1 << (SIM_SCGC5_PORTA_SHIFT+port_num));

    /*
     * Detect using the offset of GPIO_MODE_INPUT in the enum whether the mode is an input or output
     */
    const uint8_t pin_mode_is_direction = (pin_mode == GPIO_MODE_INPUT) || (pin_mode == GPIO_MODE_OUTPUT);
    if (pin_mode_is_direction) {
        prv_set_dir(pin, pin_mode);
    } else {
        prv_set_function(pin, pin_mode);
    }
    return UHAL_STATUS_OK;
}

static inline gpio_mode_t prv_get_function(const gpio_pin_t pin) {
    uint8_t port_num = GPIO_PIN_GROUP(pin);
    return BITMASK_COMPARE(((PORT_Type*)PORTS[port_num])->PCR[GPIO_PIN(pin)], PORT_PCR_MUX_MASK);
}

static inline gpio_mode_t prv_get_dir(const gpio_pin_t pin) {
     const uint8_t port_num = GPIO_PIN_GROUP(pin);
     const uint32_t pin_is_set_as_output_pin = BIT_IS_SET(((FGPIO_Type*)GPIOS[port_num])->PDDR, GPIO_PIN(pin));
     if(pin_is_set_as_output_pin) {
         return GPIO_MODE_OUTPUT;
     } else {
         return GPIO_MODE_INPUT;
     }
}

gpio_mode_t gpio_get_pin_mode(const gpio_pin_t pin) {
    uint8_t function = prv_get_function(pin);
    if(function == GPIO_MODE_OUTPUT) {
        return prv_get_dir(pin);
    } else {
        return function;
    }
}

static inline uint32_t get_non_settable_pcr_options(const gpio_pin_t pin) {
    const uint8_t port_num = GPIO_PIN_GROUP(pin);
    const uint32_t prev_pcr_val = ((PORT_Type*)PORTS[port_num])->PCR[GPIO_PIN(pin)];

    const uint32_t non_settable_opt = (BITMASK_COMPARE(prev_pcr_val, PORT_PCR_MUX_MASK)) 
                                    | BITMASK_COMPARE(prev_pcr_val, PORT_PCR_IRQC_MASK) 
                                    | BITMASK_COMPARE(prev_pcr_val, PORT_PCR_ISF_MASK);
    return non_settable_opt;
}

uhal_status_t gpio_set_pin_options(const gpio_pin_t pin, const gpio_opt_t opt) {
    /*
     * Some bits in the pcr register are not set by this function and should not be changed.
     * These bits are retrieved with this function to be included in the final reg_val later.
     */
    const uint32_t non_settable_pincfg_options = get_non_settable_pcr_options(pin);
    /*
     * Do some trickery. The enum with gpio options has PULL_DOWN and PULL_UP defined.
     * The pull-up register, drive_strength and slow_slew_rate bit positions are aligned with the position
     * in the pcr register. Thus, minimal operations are needed to translate flags to setting bits.
     *
     * The PULL_DOWN option differs one-bit position from the PULL_UP option, and PULL_UP is aligned with the PULL_EN flag.
     * To enable the PULL_EN flag, we need to just shift one bit to enable the PULL_EN flag :)
     */
    const uint8_t port_num = GPIO_PIN_GROUP(pin);

    const uint8_t pull_up_en = BITMASK_COMPARE(opt, GPIO_OPT_PULL_UP);
    const uint32_t reg_val = non_settable_pincfg_options 
                            | BITMASK_COMPARE(opt, GPIO_OPT_DRIVE_STRENGTH_HIGH) 
                            | BITMASK_COMPARE(opt, GPIO_OPT_PASSIVE_FILTER)
                            | BITMASK_COMPARE(opt, GPIO_OPT_SLOW_SLEW_RATE)
                            | pull_up_en;
    ((PORT_Type*)PORTS[port_num])->PCR[GPIO_PIN(pin)] = reg_val;
    return UHAL_STATUS_OK;
}

gpio_opt_t gpio_get_pin_options(const gpio_pin_t pin) {
    /*
     * Get the pcr register to extract the PE, DSE, SRE, PS bits from.
     */
    const uint8_t port_num = GPIO_PIN_GROUP(pin);
    const uint32_t pcr = ((PORT_Type*)PORTS[port_num])->PCR[GPIO_PIN(pin)];

    uint32_t res = BITMASK_COMPARE(pcr, GPIO_OPT_DRIVE_STRENGTH_HIGH) |
                   BITMASK_COMPARE(pcr, GPIO_OPT_PASSIVE_FILTER) |
                   BITMASK_COMPARE(pcr, GPIO_OPT_SLOW_SLEW_RATE) |
                   BITMASK_COMPARE(pcr, GPIO_OPT_PULL_UP);


    return res;
}



uhal_status_t gpio_set_interrupt_on_pin(const gpio_pin_t pin, gpio_irq_opt_t irq_opt) {
    /*
     * Check whether pin given is set as output or input. If set as output, make it an input.
     */
    const uint8_t port_num = GPIO_PIN_GROUP(pin);
    if(port_num != 0 && port_num != 3) {
        return UHAL_STATUS_INVALID_PARAMETERS;
    }

    const uint32_t pin_is_set_as_output = prv_get_dir(pin);
    if (pin_is_set_as_output == GPIO_MODE_OUTPUT) {
        prv_set_dir(pin, GPIO_MODE_INPUT);
    }


    ((PORT_Type*)PORTS[port_num])->PCR[GPIO_PIN(pin)] &= ~(PORT_PCR_IRQC_MASK);

    ((PORT_Type*)PORTS[port_num])->PCR[GPIO_PIN(pin)] |= PORT_PCR_IRQC(irq_opt.irq_condition);

    if(port_num == 0) {
        enable_irq_handler(PORTA_IRQn, 2);
    } else {
        enable_irq_handler(PORTD_IRQn, 2);
    }

    return UHAL_STATUS_OK;
}

#endif /* IFNDEF DISABLE_GPIO_MODULE */