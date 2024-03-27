#include <MCXA153.h>
#include "bit_manipulation.h"
#include "hal_gpio.h"
#include "irq/irq_bindings.h"

#define GPIO_PIN_GROUP(pin) ((pin >> 8) - 1)
#define GPIO_PIN(pin)       (pin & 0xFF)

#define GPIO_PULL_MASK (GPIO_OPT_PULL_DOWN | GPIO_OPT_PULL_UP)
#define GPIO_PULL_EN(opt) ((opt & GPIO_PULL_MASK) > 0)

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
    if (level) {
        gpio->PSOR |= (1 << pin_index);
    } else {
        gpio->PCOR |= (1 << pin_index);
    }
    return UHAL_STATUS_OK;
}

gpio_level_t gpio_get_pin_lvl(const gpio_pin_t pin) {
    const uint8_t      port_num = GPIO_PIN_GROUP(pin);
    const gpio_level_t res = BIT_IS_SET((((GPIO_Type*)GPIOS[port_num])->PDIR), GPIO_PIN(pin));
    return res;
}

static inline gpio_mode_t prv_get_dir(const gpio_pin_t pin) {
    const uint8_t  port_num = GPIO_PIN_GROUP(pin);
    const uint32_t pin_is_set_as_output_pin = BIT_IS_SET(((GPIO_Type*)GPIOS[port_num])->PDDR, GPIO_PIN(pin));
    if (pin_is_set_as_output_pin) {
        return GPIO_MODE_OUTPUT;
    } else {
        return GPIO_MODE_INPUT;
    }
}

static inline gpio_mode_t prv_get_mux_mode(const gpio_pin_t pin) {
    const uint8_t port_num = GPIO_PIN_GROUP(pin);
    const uint8_t pin_num = GPIO_PIN(pin);
    PORT_Type*    port = (PORT_Type*)PORTS[port_num];
    const uint32_t pcr_val = (port->PCR[pin_num] & PORT_PCR_MUX_MASK) >> PORT_PCR_MUX_SHIFT;
    return pcr_val;
}

gpio_mode_t gpio_get_pin_mode(const gpio_pin_t pin) {
    const uint8_t mux_val = prv_get_mux_mode(pin);
    if (mux_val == 0) {
        return prv_get_dir(pin);
    } else {
        return mux_val;
    }
}

static inline uint32_t get_non_settable_pcr_options(const gpio_pin_t pin) {
    const uint8_t port_num = GPIO_PIN_GROUP(pin);
    const uint32_t prev_pcr_val = ((PORT_Type*)PORTS[port_num])->PCR[GPIO_PIN(pin)];

    const uint32_t non_settable_opt = (BITMASK_COMPARE(prev_pcr_val, PORT_PCR_MUX_MASK));
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

    const uint8_t pull_en = GPIO_PULL_EN(opt) << 1;
    const uint8_t pull_up_en = BITMASK_COMPARE(opt, GPIO_OPT_PULL_UP) >> 2;
    const uint32_t drive_str_en = ((port_num == 1) && (GPIO_PIN(pin) == 30)) ? BITMASK_COMPARE(opt, GPIO_OPT_DRIVE_STRENGTH_HIGH)  << 1 : BITMASK_COMPARE(opt, GPIO_OPT_DRIVE_STRENGTH_HIGH);
    const uint32_t reg_val = non_settable_pincfg_options 
                            | BITMASK_COMPARE(opt, GPIO_OPT_SLOW_SLEW_RATE)
                            | BITMASK_COMPARE(opt, GPIO_OPT_OPEN_DRAIN_OUTPUT)
                            | drive_str_en
                            | pull_up_en
                            | (BITMASK_COMPARE(opt, GPIO_OPT_INPUT_BUFFER_ENABLE) << 5)
                            | (BITMASK_COMPARE(opt, GPIO_OPT_INVERT_INPUT) << 5)
                            | (BITMASK_COMPARE(opt, GPIO_OPT_LOCK) << PORT_PCR_LK_SHIFT)
                            | pull_en;
    ((PORT_Type*)PORTS[port_num])->PCR[GPIO_PIN(pin)] = reg_val;
    return UHAL_STATUS_OK;
}

gpio_opt_t gpio_get_pin_options(const gpio_pin_t pin) {
    const uint8_t port_num = GPIO_PIN_GROUP(pin);
    const uint32_t reg_val = ((PORT_Type*)PORTS[port_num])->PCR[GPIO_PIN(pin)];
    const uint32_t pull_en = ((reg_val & PORT_PCR_PE_MASK));
    const uint32_t drv_str = (BITMASK_COMPARE(reg_val, (PORT_PCR_DSE_MASK | PORT_PCR_DSE1_MASK))) ? GPIO_OPT_DRIVE_STRENGTH_HIGH : 0;
    uint32_t res = (BITMASK_COMPARE(reg_val, GPIO_OPT_SLOW_SLEW_RATE)) 
                       | (BITMASK_COMPARE(reg_val, GPIO_OPT_OPEN_DRAIN_OUTPUT))
                       | ((BIT_IS_SET(reg_val, PORT_PCR_IBE_SHIFT)) << 7)
                       | ((BIT_IS_SET(reg_val, PORT_PCR_INV_SHIFT)) << 8)
                       | (BIT_IS_SET(reg_val, PORT_PCR_LK_SHIFT))
                       | (BITMASK_COMPARE(reg_val, GPIO_OPT_DRIVE_STRENGTH_HIGH))
                       | drv_str;
    if(pull_en) {
       res |= (reg_val & PORT_PCR_PS_MASK) ? GPIO_OPT_PULL_UP : GPIO_OPT_PULL_DOWN; 
    }
    return res;
}

uhal_status_t gpio_set_interrupt_on_pin(const gpio_pin_t pin, gpio_irq_opt_t irq_opt) {
       /*
     * Check whether pin given is set as output or input. If set as output, make it an input.
     */
    const uint8_t port_num = GPIO_PIN_GROUP(pin);

    const uint32_t pin_is_set_as_output = prv_get_dir(pin);
    if (pin_is_set_as_output == GPIO_MODE_OUTPUT) {
        gpio_set_pin_mode(pin, GPIO_MODE_INPUT);
    }

    GPIO_Type*    gpio = (GPIO_Type*)GPIOS[port_num];
    uint32_t irq_bits = irq_opt.irq_condition << 16;
    gpio->ICR[GPIO_PIN(pin)] = irq_bits;

    enable_irq_handler((GPIO0_IRQn + port_num), 2);

    return UHAL_STATUS_OK;
}