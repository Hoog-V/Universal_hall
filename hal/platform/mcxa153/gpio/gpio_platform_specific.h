#ifndef GPIO_PLATFORM_SPECIFIC_H
#define GPIO_PLATFORM_SPECIFIC_H

#include <stdint.h>
#include "pins.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief The MCXA153 supports two GPIO levels,
 *        LOW AND HIGH... Use this when using the
 *        set_gpio_pin_level() or get_gpio_pin_level() functions.
 */
typedef enum {
    GPIO_LOW,
    GPIO_HIGH,
} gpio_level_t;

/**
 * @brief The supported GPIO modes of the MCXA153, GPIO_MODE A...N are PIN-MUX options,
 *        The other two options are just for pin direction (INPUT or OUTPUT).
 *        The default startup-option of the MCXA153 is input w/o pull (-up or -down) options
 */
typedef enum {
    GPIO_MODE_ALT0 = 0,
    GPIO_MODE_ALT1,
    GPIO_MODE_ALT2,
    GPIO_MODE_ALT3,
    GPIO_MODE_ALT4,
    GPIO_MODE_ALT5,
    GPIO_MODE_ALT6,
    GPIO_MODE_ALT7,
    GPIO_MODE_ALT8,
    GPIO_MODE_ALT9,
    GPIO_MODE_ALT10,
    GPIO_MODE_ALT11,
    GPIO_MODE_ALT12,
    GPIO_MODE_ALT13,
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_NULL = 0x1F
} gpio_mode_t;

/**
 * @brief A pin on the MCXA153 series consists of a PORT number with a pin number, to imitate this as closely as possible a port_num and pin_num is used:
 *        (port_number | pin_number)
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
    gpio_irq_channel_t   irq_channel;
    gpio_irq_condition_t irq_condition;
    gpio_irq_extra_opt_t irq_extra_opt;
} gpio_irq_opt_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* GPIO_PLATFORM_SPECIFIC_H */