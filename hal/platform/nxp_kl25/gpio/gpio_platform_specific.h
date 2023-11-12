/**
* \file            gpio_platform_specific.h
* \brief           Include file with peripheral specific settings for the GPIO module
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

#ifndef GPIO_PLATFORM_SPECIFIC
#define GPIO_PLATFORM_SPECIFIC

#include <stdint.h>
#include <assert.h>
#include "gpio/pins.h"
#include "bit_manipulation.h"
#include "MKL25Z4_features.h"

/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/**
 * @brief The KL25 series support two GPIO levels,
 *        LOW AND HIGH... Use this when using the
 *        set_gpio_pin_level() or gpio_get_pin_lvl() functions.
 */
typedef enum {
    GPIO_LOW,
    GPIO_HIGH,
} gpio_level_t;

/**
 * @brief The KL25 series have a minimum of two GPIO ports.
 *        MACRO's for the specific device can be placed to expand the list.
 */
typedef enum {
    GPIO_PORT_A = 0x100,
    GPIO_PORT_B = 0x200,
    GPIO_PORT_C = 0x300,
    GPIO_PORT_D = 0x400,
    GPIO_PORT_E = 0x500
} gpio_port_t;

/**
 * @brief The supported GPIO modes of the KL25, GPIO_MODE A...N are PIN-MUX options,
 *        The other two options are just for pin direction (INPUT or OUTPUT).
 *        The default startup-option of the SAMD is input w/o pull (-up or -down) options
 */
typedef enum {
    GPIO_MODE_OFF =0,
    GPIO_MODE_2 = 2,
    GPIO_MODE_3 = 3,
    GPIO_MODE_4 = 4,
    GPIO_MODE_5 = 5,
    GPIO_MODE_6 = 6,
    GPIO_MODE_7 = 7,
    GPIO_MODE_INPUT = 8,
    GPIO_MODE_OUTPUT = 1,
} gpio_mode_t;

/**
 * @brief A pin on the SAMD series consists of a PORT letter with a number, to imitate this as closely as possible a port_num and pin_num is used:
 *        PA11 -> GPIO_PORT_A, 11
 */
typedef uint16_t gpio_pin_t;

/**
 * @brief The SAMD series have some special options:
 *        GPIO_OPT_PULL_UP: enables a internal pull-up on an input pin.
 *        GPIO_OPT_PULL_DOWN: enables a internal pull-down on an input pin.
 *        GPIO_OPT_SAMPLE_CONTINUOUSLY: enables continuously monitoring on input pins instead of on-demand (sips more energy)
 *        GPIO_OPT_DRIVE_STRENGTH_STRONG: enables stronger drive current on the selected (output) pin (20mA instead of 2mA) (is enabled on startup)
 * @note Not selecting a option in the set_gpio_pin_option() function disables the option prematurely.
 * @note If you want multiple options OR them together like this: options = GPIO_OPT_PULL_UP | GPIO_OPT_SAMPLE_CONTINUOUSLY;
 */
typedef enum {
    GPIO_OPT_PULL_UP = 2,
    GPIO_OPT_SLOW_SLEW_RATE = 4,
    GPIO_OPT_PASSIVE_FILTER = 16,
    GPIO_OPT_DRIVE_STRENGTH_HIGH = 64,
} gpio_opt_t;

/**
 * @brief The SAMD series use a specific IRQ peripheral for mapping IO pins to IRQ channels.
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
 * @brief The SAMD series support the following signal conditions on which a GPIO irq can be triggered.
 */
typedef enum {
    GPIO_IRQ_NONE = 0,
    GPIO_IRQ_COND_RISING_EDGE = 9,
    GPIO_IRQ_COND_FALLING_EDGE = 10,
    GPIO_IRQ_COND_BOTH_EDGES = 11,
    GPIO_IRQ_COND_HIGH_LVL = 12,
    GPIO_IRQ_COND_LOW_LVL = 8,
    GPIO_IRQ_COND_DMA_ON_RISING_EDGE = 1,
    GPIO_IRQ_COND_DMA_ON_FALLING_EDGE = 2,
    GPIO_IRQ_COND_DMA_ON_BOTH_EDGES = 3
} gpio_irq_condition_t;

/**
 * @brief These are extra options the SAMD series support besides the normal options:
 *        GPIO_IRQ_EXTRA_NONE: Will disable all other special functions
 *        GPIO_IRQ_EXTRA_FILTERING: Will enable extra filtering on the input pin
 *        GPIO_IRQ_USE_EVENTS: Will use the event system instead of the EIC of the SAMD
 *        GPIO_IRQ_WAKE_FROM_SLEEP: When set interrupts will wake the MCU up from sleep modes
 */
typedef enum {
    GPIO_IRQ_EXTRA_NONE,
    GPIO_IRQ_EXTRA_FILTERING,
    GPIO_IRQ_USE_EVENTS,
    GPIO_IRQ_WAKE_FROM_SLEEP
} gpio_irq_extra_opt_t;

/**
 * @brief The options which need to be set to get gpio interrupts working on the SAMD series:
 *        irq_channel: The channel which belongs to the input pin
 *        irq_condition: The condition to trigger the IRQ on
 *        irq_extra_opt: Extra options which the SAMD controller supports.
 * @note For minimal functionality at least set the irq_channel and irq_condition options.
 */
typedef struct {
    gpio_irq_condition_t irq_condition;
} gpio_irq_opt_t;


#define GPIO_PIN_PARAMETER_CHECK(pin) \
do {                                  \
static_assert(pin <= (((FSL_FEATURE_SOC_PORT_COUNT + 1) << 8) | 0x1F), "Selected pin not available on this mcu!"); \
}while(0);

#define GPIO_PIN_MODE_PARAMETER_CHECK(pin_mode) \
do {                                                          \
static_assert(pin_mode <= GPIO_MODE_7, "Selected pin mode not supported!");   \
}while(0);

#define GPIO_PIN_LEVEL_PARAMETER_CHECK(level) \
do {                                                          \
static_assert(pin_mode <= GPIO_HIGH, "Selected pin level not supported!");   \
}while(0);

#define GPIO_PIN_OPTIONS_PARAMETER_CHECK(options) \
do {                                              \
const uint8_t pull_up_defined = (BITMASK_COMPARE(options, GPIO_OPT_PULL_UP)); \
const uint8_t pull_down_defined = (BITMASK_COMPARE(options, GPIO_OPT_PULL_DOWN));\
static_assert(((options >= GPIO_OPT_PULL_UP) &&  (options <= GPIO_OPT_DRIVE_STRENGTH_HIGH)), "Invalid pin options set!"); \
static_assert((pull_up_defined && pull_down_defined) < 1, "Pull-up and Pull-down functionality on the same pin is not allowed!" );    \
}while(0);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
