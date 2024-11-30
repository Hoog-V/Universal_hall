/**
* \file            gpio_platform_specific.h
* \brief           Include file with peripheral specific settings for the GPIO module
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

#ifndef GPIO_PLATFORM_SPECIFIC
#define GPIO_PLATFORM_SPECIFIC
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>

/**
 * @brief The TI IWR68xx supports two GPIO levels,
 *        LOW AND HIGH... Use this when using the
 *        set_gpio_pin_level() or get_gpio_pin_level() functions.
 */
typedef enum {
    GPIO_LOW,
    GPIO_HIGH,
} gpio_level_t;

/**
 * @brief A pin on the TI AWR6843 is represented as a number.
 */
typedef enum {
    GPIO_PIN_0 = 0 << 4,
    GPIO_PIN_1 = 1 << 4,
    GPIO_PIN_2 = 2 << 4,
    GPIO_PIN_3 = 3 << 4,
    GPIO_PIN_4 = 4 << 4,
    GPIO_PIN_5 = 5 << 4,
    GPIO_PIN_6 = 6 << 4,
    GPIO_PIN_7 = 7 << 4,
    GPIO_PIN_8 = (1 | 0 << 4),
    GPIO_PIN_9 = (1 | 1 << 4),
    GPIO_PIN_10 = (1 | 2 << 4),
    GPIO_PIN_11 = (1 | 3 << 4),
    GPIO_PIN_12 = (1 | 4 << 4),
    GPIO_PIN_13 = (1 | 5 << 4),
    GPIO_PIN_14 = (1 | 6 << 4),
    GPIO_PIN_15 = (1 | 7 << 4),
    GPIO_PIN_16 = (2 | 0 << 4),
    GPIO_PIN_17 = (2 | 1 << 4),
    GPIO_PIN_18 = (2 | 2 << 4),
    GPIO_PIN_19 = (2 | 3 << 4),
    GPIO_PIN_20 = (2 | 4 << 4),
    GPIO_PIN_21 = (2 | 5 << 4),
    GPIO_PIN_22 = (2 | 6 << 4),
    GPIO_PIN_23 = (2 | 7 << 4),
    GPIO_PIN_24 = (3 | 0 << 4),
    GPIO_PIN_25 = (3 | 1 << 4),
    GPIO_PIN_26 = (3 | 2 << 4),
    GPIO_PIN_27 = (3 | 3 << 4),
    GPIO_PIN_28 = (3 | 4 << 4),
    GPIO_PIN_30 = (3 | 6 << 4),
    GPIO_PIN_31 = (3 | 7 << 4),
    GPIO_PIN_32 = (4 | 0 << 4),
    GPIO_PIN_33 = (4 | 1 << 4),
    GPIO_PIN_34 = (4 | 2 << 4),
    GPIO_PIN_35 = (4 | 3 << 4),
    GPIO_PIN_36 = (4 | 4 << 4),
    GPIO_PIN_37 = (4 | 5 << 4),
    GPIO_PIN_38 = (4 | 6 << 4),
    GPIO_PIN_39 = (4 | 7 << 4),
    GPIO_PIN_40 = (5 | 0 << 4),
    GPIO_PIN_41 = (5 | 1 << 4),
    GPIO_PIN_42 = (5 | 2 << 4),
    GPIO_PIN_43 = (5 | 3 << 4),
    GPIO_PIN_44 = (5 | 4 << 4),
    GPIO_PIN_45 = (5 | 5 << 4),
    GPIO_PIN_46 = (5 | 6 << 4),
    GPIO_PIN_47 = (5 | 7 << 4)
} gpio_pin_t;

/**
 * @brief The supported GPIO modes of the IWR68xx
 */
typedef enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT
} gpio_mode_t;

/**
 * @brief The IWR68xx series have some special options:
 *        GPIO_OPT_PULL_UP: enables a internal pull-up on an input pin.
 *        GPIO_OPT_PULL_DOWN: enables a internal pull-down on an input pin.
 *        GPIO_OPT_SAMPLE_CONTINUOUSLY: enables continuously monitoring on input pins instead of on-demand (sips more energy)
 *        GPIO_OPT_DRIVE_STRENGTH_STRONG: enables stronger drive current on the selected (output) pin (20mA instead of 2mA) (is enabled on startup)
 * @note Not selecting a option in the set_gpio_pin_option() function disables the option prematurely.
 * @note If you want multiple options OR them together like this: options = GPIO_OPT_PULL_UP | GPIO_OPT_SAMPLE_CONTINUOUSLY;
 */
typedef enum {
    GPIO_OPT_PULL_UP = 1,
    GPIO_OPT_PULL_DOWN = 2,
    GPIO_OPT_SLEW_RATE_SLOW = 4,
    GPIO_OPT_SLEW_RATE_HIGH = 8,
    GPIO_OPT_DRIVE_STRENGTH_VLOW = 16,
    GPIO_OPT_DRIVE_STRENGTH_LOW = 32,
    GPIO_OPT_DRIVE_STRENGTH_MEDIUM = 64,
    GPIO_OPT_DRIVE_STRENGTH_HIGH = 128,
} gpio_opt_t;

/**
 * @brief The IWR68xx series use a specific IRQ peripheral for mapping IO pins to IRQ channels.
 *        Look in the datasheet which pin in group A of the pin-mux belongs to which input channel.
 *        Most of the times if the pin number is under 15 it maps directly to the IRQ channel number, except 8 which is probably NMI.
 */
typedef enum {
    GPIO_IRQ_CHANNEL_0,
    GPIO_IRQ_CHANNEL_1,
    GPIO_IRQ_CHANNEL_2,
    GPIO_IRQ_CHANNEL_3,
    GPIO_IRQ_CHANNEL_4,
    GPIO_IRQ_CHANNEL_5,
    GPIO_IRQ_CHANNEL_6,
    GPIO_IRQ_CHANNEL_7,
    GPIO_IRQ_CHANNEL_8,
    GPIO_IRQ_CHANNEL_9,
    GPIO_IRQ_CHANNEL_10,
    GPIO_IRQ_CHANNEL_11,
    GPIO_IRQ_CHANNEL_12,
    GPIO_IRQ_CHANNEL_13,
    GPIO_IRQ_CHANNEL_14,
    GPIO_IRQ_CHANNEL_15,
    GPIO_IRQ_NMI
} gpio_irq_channel_t;

/**
 * @brief The RPI pico support the following signal conditions on which a GPIO irq can be triggered.
 */
typedef enum {
    GPIO_IRQ_NONE,
    GPIO_IRQ_COND_RISING_EDGE,
    GPIO_IRQ_COND_FALLING_EDGE,
    GPIO_IRQ_COND_HIGH_LVL,
    GPIO_IRQ_COND_LOW_LVL
} gpio_irq_condition_t;

/**
 * @brief These are extra options the IWR68xx series support besides the normal options:
 *        GPIO_IRQ_EXTRA_NONE: Will disable all other special functions
 *        GPIO_IRQ_EXTRA_FILTERING: Will enable extra filtering on the input pin
 *        GPIO_IRQ_USE_EVENTS: Will use the event system instead of the EIC of the IWR68xx
 *        GPIO_IRQ_WAKE_FROM_SLEEP: When set interrupts will wake the MCU up from sleep modes
 */
typedef enum {
    GPIO_IRQ_EXTRA_NONE,
    GPIO_IRQ_EXTRA_FILTERING,
    GPIO_IRQ_USE_EVENTS,
    GPIO_IRQ_WAKE_FROM_SLEEP
} gpio_irq_extra_opt_t;

/**
 * @brief The options which need to be set to get gpio interrupts working on the IWR68xx series:
 *        irq_channel: The channel which belongs to the input pin
 *        irq_condition: The condition to trigger the IRQ on
 *        irq_extra_opt: Extra options which the IWR68xx controller supports.
 * @note For minimal functionality at least set the irq_channel and irq_condition options.
 */
typedef struct {
    gpio_irq_channel_t   irq_channel;
    gpio_irq_condition_t irq_condition;
    gpio_irq_extra_opt_t irq_extra_opt;
} gpio_irq_opt_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
