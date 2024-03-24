/**
* \file            gpio_raspberrypi.c
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
#include "hal_gpio.h"
#include <hardware/gpio.h>
#include "bit_manipulation.h"
#include "gpio_platform_specific.h"
#include "hardware/sync.h"

#include "hardware/irq.h"
#include "hardware/structs/iobank0.h"
#include "hardware/structs/sio.h"

#if LIB_PICO_BINARY_INFO
#include "pico/binary_info.h"
#endif

#define GPIO_OPT_SLEW_RATE_SLOW_POS 2
#define GPIO_OPT_DRIVE_STRENGTH_POS 4
#define GPIO_OPT_PULL_UP_POS 1
#define ADD_GPIO_DRIVE_STENGTH_OPT_OFFSET(x) (x + GPIO_OPT_DRIVE_STRENGTH_POS)

#define REMOVE_IO_OFFSET(x)             (x - GPIO_MODE_INPUT)

uhal_status_t gpio_set_pin_lvl(const gpio_pin_t pin, gpio_level_t level) {
    if (level) {
        sio_hw->gpio_set = (1 << pin);
    } else {
        sio_hw->gpio_clr = (1 << pin);
    }
    return UHAL_STATUS_OK;
}

uhal_status_t gpio_toggle_pin_output(const gpio_pin_t pin) {
    const uint32_t mask = 1ul << pin;
    sio_hw->gpio_togl = mask;
    return UHAL_STATUS_OK;
}

gpio_level_t gpio_get_pin_level(const gpio_pin_t pin) {
      return !!((1ul << pin) & sio_hw->gpio_in);
}

uhal_status_t gpio_set_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode) {
    if(BITMASK_COMPARE(pin_mode, GPIO_MODE_INPUT)) {
        sio_hw->gpio_oe_clr = (1 << pin);
    } else if (BITMASK_COMPARE(pin_mode, GPIO_MODE_OUTPUT)) {
        sio_hw->gpio_oe_set = (1 << pin);
    } else{
        padsbank0_hw->io[pin] |= (PADS_BANK0_GPIO0_IE_BITS | PADS_BANK0_GPIO0_OD_BITS);
        hw_write_masked(&padsbank0_hw->io[pin], PADS_BANK0_GPIO0_IE_BITS, PADS_BANK0_GPIO0_IE_BITS | PADS_BANK0_GPIO0_OD_BITS);
        // Zero all fields apart from fsel; we want this IO to do what the peripheral tells it.
        // This doesn't affect e.g. pullup/pulldown, as these are in pad controls.
        iobank0_hw->io[pin].ctrl = pin_mode << IO_BANK0_GPIO0_CTRL_FUNCSEL_LSB;
    }
    return UHAL_STATUS_OK;
}

gpio_mode_t gpio_get_pin_mode(const gpio_pin_t pin) {
    uint8_t function;

    function = gpio_get_function(pin);
    if (function == GPIO_MODE_F5) {
        function = gpio_get_dir(pin) + GPIO_MODE_INPUT;
        return function;
    } else {
        return function;
    }
}

uhal_status_t gpio_set_pin_options(const gpio_pin_t pin, const gpio_opt_t opt) {
    const uint8_t driver_mask_cmp_val = GPIO_OPT_DRIVE_STRENGTH_HIGH | GPIO_OPT_DRIVE_STRENGTH_MEDIUM 
                                        | GPIO_OPT_DRIVE_STRENGTH_LOW | GPIO_OPT_DRIVE_STRENGTH_VLOW;

    switch(BITMASK_COMPARE(opt, driver_mask_cmp_val)) {
        case GPIO_OPT_DRIVE_STRENGTH_HIGH:
        {
            hw_write_masked(&padsbank0_hw->io[pin],
                    (uint)GPIO_DRIVE_STRENGTH_12MA << PADS_BANK0_GPIO0_DRIVE_LSB,
                    PADS_BANK0_GPIO0_DRIVE_BITS);
            break;
        }
        case GPIO_OPT_DRIVE_STRENGTH_MEDIUM:
        {
            hw_write_masked(&padsbank0_hw->io[pin],
                    (uint)GPIO_DRIVE_STRENGTH_8MA << PADS_BANK0_GPIO0_DRIVE_LSB,
                    PADS_BANK0_GPIO0_DRIVE_BITS);
            break;
        }
        case GPIO_OPT_DRIVE_STRENGTH_LOW:
        {
             hw_write_masked(&padsbank0_hw->io[pin],
                    (uint)GPIO_DRIVE_STRENGTH_4MA << PADS_BANK0_GPIO0_DRIVE_LSB,
                    PADS_BANK0_GPIO0_DRIVE_BITS);
            break;
        }
        case GPIO_OPT_DRIVE_STRENGTH_VLOW:
        {
            hw_write_masked(&padsbank0_hw->io[pin],
                    (uint)GPIO_DRIVE_STRENGTH_2MA << PADS_BANK0_GPIO0_DRIVE_LSB,
                    PADS_BANK0_GPIO0_DRIVE_BITS);
            break;
        }
        default:
        {
            break;
        }
    }

    const uint8_t pullup_en = BITMASK_COMPARE(pin, GPIO_OPT_PULL_UP);
    const uint8_t pulldown_en = BITMASK_COMPARE(pin, GPIO_OPT_PULL_DOWN); 
    hw_write_masked(
            &padsbank0_hw->io[pin],
            (bool_to_bit(pullup_en) << PADS_BANK0_GPIO0_PUE_LSB) | (bool_to_bit(pulldown_en) << PADS_BANK0_GPIO0_PDE_LSB),
            PADS_BANK0_GPIO0_PUE_BITS | PADS_BANK0_GPIO0_PDE_BITS);

    const uint8_t slew_rate_val = BITMASK_COMPARE(opt, GPIO_OPT_SLEW_RATE_HIGH) ? GPIO_SLEW_RATE_FAST : GPIO_SLEW_RATE_SLOW;
    hw_write_masked(&padsbank0_hw->io[pin],
                    (uint)slew_rate_val << PADS_BANK0_GPIO0_SLEWFAST_LSB,
                    PADS_BANK0_GPIO0_SLEWFAST_BITS );
    return UHAL_STATUS_OK;
}

gpio_opt_t gpio_get_pin_options(const gpio_pin_t pin) {
    const uint8_t driver_strength_unp = (enum gpio_drive_strength)((padsbank0_hw->io[pin] & PADS_BANK0_GPIO0_DRIVE_BITS)
                                                                    >> PADS_BANK0_GPIO0_DRIVE_LSB);
    const uint8_t driver_strength = ADD_GPIO_DRIVE_STENGTH_OPT_OFFSET(driver_strength_unp);
    const uint8_t slew_rate_unp = (enum gpio_slew_rate)((padsbank0_hw->io[pin] & PADS_BANK0_GPIO0_SLEWFAST_BITS)
                                                        >> PADS_BANK0_GPIO0_SLEWFAST_LSB);
    const uint8_t slew_rate = (SHIFT_ONE_LEFT_BY_N( (GPIO_OPT_SLEW_RATE_SLOW_POS + slew_rate_unp)));
    const uint8_t pull_down_en = (padsbank0_hw->io[pin] & PADS_BANK0_GPIO0_PDE_BITS) != 0;
    const uint8_t pull_up_en = (padsbank0_hw->io[pin] & PADS_BANK0_GPIO0_PUE_BITS) != 0;
    const uint8_t options = (SHIFT_ONE_LEFT_BY_N(driver_strength)) | slew_rate | pull_down_en | (pull_up_en << GPIO_OPT_PULL_UP_POS) ;
    return options;
}

uhal_status_t gpio_set_interrupt_on_pin(const gpio_pin_t pin, gpio_irq_opt_t irq_opt) {
    return UHAL_STATUS_OK;
}
