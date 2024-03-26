#include <MCXA153.h>
#include "bit_manipulation.h"
#include "hal_gpio.h"

#define GPIO_PIN_GROUP(pin) ((pin >> 8) - 1)
#define GPIO_PIN(pin)       (pin & 0xFF)

unsigned int PORTS[] = PORT_BASE_ADDRS;
unsigned int GPIOS[] = GPIO_BASE_ADDRS;

uhal_status_t gpio_toggle_pin_output(const gpio_pin_t pin) {
    const uint8_t pin_group = GPIO_PIN_GROUP(pin);
    const uint8_t pin_index = GPIO_PIN(pin);
    GPIO_Type*    gpio = (GPIO_Type*)GPIOS[pin_group];
    gpio->PTOR |= (1 << pin_index);
    return UHAL_STATUS_OK;
}

uhal_status_t gpio_set_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode) {
    const uint8_t pin_group = GPIO_PIN_GROUP(pin);
    const uint8_t pin_index = GPIO_PIN(pin);
    PORT_Type*    port = (PORT_Type*)PORTS[pin_group];
    GPIO_Type*    gpio = (GPIO_Type*)GPIOS[pin_group];
    uint32_t      pcr_mask = PORT_PCR_MUX(pin_mode);

    uint32_t shift_pos_port = MRCC_MRCC_GLB_CC0_PORT0_SHIFT + pin_group;
    uint32_t shift_pos_gpio = MRCC_MRCC_GLB_ACC1_GPIO0_SHIFT + pin_group;
    uint32_t port_mask = (1 << shift_pos_port);
    uint32_t gpio_mask = (1 << shift_pos_gpio);
    uint8_t  invalid_params = ((pin_mode > GPIO_MODE_ALT13 && pin_group >= 3) || (pin_mode > GPIO_MODE_ALT7))
                             && (pin_mode != GPIO_MODE_INPUT && pin_mode != GPIO_MODE_OUTPUT);
    if (invalid_params) {
        return UHAL_STATUS_INVALID_PARAMETERS;
    }

    if (pin_group >= 3) {
        // Special handling for pin groups >= 3
        const uint32_t port3_en = MRCC_MRCC_GLB_ACC1_PORT3_MASK;
        MRCC0->MRCC_GLB_CC1 |= gpio_mask | port3_en;
        MRCC0->MRCC_GLB_RST1 |= gpio_mask | port3_en;

    } else {
        MRCC0->MRCC_GLB_CC0 |= port_mask;
        MRCC0->MRCC_GLB_RST0 |= port_mask;
        MRCC0->MRCC_GLB_CC1 |= gpio_mask;
        MRCC0->MRCC_GLB_RST1 |= gpio_mask;
    }

    if (pin_mode == GPIO_MODE_INPUT || pin_mode == GPIO_MODE_OUTPUT) {
        pcr_mask = PORT_PCR_MUX(0x00); // Select default function for GPIO
    }

    port->PCR[pin_index] |= pcr_mask; // Apply pin control register configuration

    // Configure pin direction
    if (pin_mode == GPIO_MODE_OUTPUT) {
        gpio->PDDR |= (1 << pin_index);
    } else if (pin_mode == GPIO_MODE_INPUT) {
        gpio->PDDR &= ~(1 << pin_index);
    }

    return UHAL_STATUS_OK;
}

uhal_status_t gpio_set_pin_lvl(const gpio_pin_t pin, gpio_level_t level) {
    const uint8_t pin_group = GPIO_PIN_GROUP(pin);
    const uint8_t pin_index = GPIO_PIN(pin);
    GPIO_Type*    gpio = (GPIO_Type*)GPIOS[pin_group];
    if(level) {
        gpio->PSOR |= (1 << pin_index);
    } else {
        gpio->PCOR |= (1 << pin_index);
    }
    return UHAL_STATUS_OK;
}

gpio_level_t gpio_get_pin_lvl(const gpio_pin_t pin) {
    return UHAL_STATUS_OK;
}

gpio_mode_t gpio_get_pin_mode(const gpio_pin_t pin) {
    return UHAL_STATUS_OK;
}

uhal_status_t gpio_set_pin_options(const gpio_pin_t pin, const gpio_opt_t opt) {
    return UHAL_STATUS_OK;
}

gpio_opt_t gpio_get_pin_options(const gpio_pin_t pin) {
    return UHAL_STATUS_OK;
}

uhal_status_t gpio_set_interrupt_on_pin(const gpio_pin_t pin, gpio_irq_opt_t irq_opt) {
    return UHAL_STATUS_OK;
}