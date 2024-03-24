# Atmel SAMD21/51 UART API Usage

The UART API for Atmel SAMD21/51 microcontrollers is designed with platform-specific nuances, particularly in clock system linkage and peripheral management. To address these intricacies, the API employs a generic approach, enabling the configuration of key parameters while accommodating specific platform requirements.

## Platform Specific Settings

For effective use of the UART peripheral, follow these essential steps:

- [ ] Configure the system clocks for the UART peripheral (skip if using ASF or Arduino).
- [ ] Set up the GPIO pins for the UART peripheral (details in the GPIO section).
- [ ] Use the `uhal_uart_init` function with the right configuration settings.

!!! note
    You can click on the checkmarks above to mark your progress.

### Clocks

The UART implementation does not set up clock generators; it connects the UART peripheral to existing clock generators. This setup is the responsibility of the integrating code or software framework.

Frameworks like Arduino and ASF typically handle most configurations. Both set clock generator 0 at 48 MHz (Arduino) or 8 MHz (ASF) and clock generator 1 at 32.768 KHz.

!!! Warning
    The UART peripheral needs two clocks: a slow one (< 100 KHz) for internal timing and synchronization, and a fast one (≥ (oversampling, default 16)x baud frequency)

### GPIO Pinmux Settings

Apart from configuring the `uhal_uart_init()` function, you must link the GPIO pins to the hardware peripheral using the SAMD's pinmux with `gpio_set_pin_mode`.

```c
void gpio_set_pin_mode(const gpio_pin_t pin, gpio_mode_t pin_mode);
```

??? question "Setting the right pin mode"
    Consult the SAMD21 or SAMD51 datasheet for the correct `pin_mode`. For example, to configure PA4 and PA5 of the SAMD21 for SERCOM 0's RX and TX:
    ```c
    const gpio_pin_t RX_PIN = GPIO_PIN_PA4;
    const gpio_pin_t TX_PIN = GPIO_PIN_PA5;

    gpio_set_pin_mode(RX_PIN, GPIO_MODE_D);
    gpio_set_pin_mode(TX_PIN, GPIO_MODE_D);
    ```

### uart_init Function

Use `uart_init` to initialize the UART hardware peripheral.

```c
uhal_status_t uhal_uart_init(const uart_peripheral_inst_t uart_peripheral, 
                             const uint32_t baudrate, 
                             const uart_clock_sources_t clock_source, 
                             const uint32_t clock_source_freq, 
                             const uart_extra_config_opt_t uart_extra_opt);
```

Set the following parameters:

??? info "uart_peripheral"
    Specifies the SERCOM number for the UART module.

    !!! Warning
        These peripherals are shared with UART, SPI, and I2C. Ensure exclusive use to avoid configuration conflicts.
    
    Configuration for SAMD21:
    
    ```c
    typedef enum {
        UART_PERIPHERAL_0,
        ...
        UART_PERIPHERAL_5
    } uart_peripheral_inst_t;
    ```

??? info "baudrate"
    The desired UART baudrate (600-1M baud supported).


??? info "clock_sources"
    Identifies the clock sources for the SERCOM peripheral.

    SAMD21 options:
    ```c
    typedef enum {
        UART_CLK_SOURCE_USE_DEFAULT = 0x00,
        ...
        UART_CLK_SOURCE_SLOW_CLKGEN7 = 0x800,
    } uart_clock_sources_t;
    ```
    Combine a fast and a slow clock using an OR operation.
    ```c
    #define uart_clocks (UART_CLK_SOURCE_FAST_CLKGEN0 | UART_CLK_SOURCE_SLOW_CLKGEN1)
    ```

??? info "clock_source_freq"
    The output frequency of the fast clock generator.

    Typical values:
    
    | Clock generator | Platform | Framework | Frequency |
    | --------------- | -------- | --------- | --------- |
    | Clock gen 0     | SAMD51   | Arduino   | 120 MHz   |
    | Clock gen 0     | SAMD21   | Arduino   | 48 MHz    |
    | Clock gen 0     | SAMD21   | ASF       | 8 MHz     |

    Use `F_CPU` with Arduino.

??? info "uart_extra_opt"
    Extra configuration options for the UART peripheral.

    Available options:

    - **UART_EXTRA_OPT_USE_DEFAULT**: Default UART settings.

    - **UART_EXTRA_OPT_MSB_FIRST**: Transfer bytes MSB first.

    - **UART_EXTRA_OPT_INVERSE_CLOCK_POLARITY**: Invert the clock polarity.

    - **UART_EXTRA_OPT_SYNCHRONOUS_COMMUNICATION**: Enable synchronous communication.

    - **UART_EXTRA_OPT_AUTO_BAUD**: Enable automatic baudrate negotiation.

    - **UART_EXTRA_OPT_PARITY**: Enable parity bit

    - **UART_EXTRA_OPT_RX_PAD_x**: Corresponds to SERCOM_PAD[x] in the mux table (possible values range from 1 to 3, default 0)

    - **UART_EXTRA_OPT_TX_PAD_x**: Corresponds to SERCOM_PAD[x] as well (possible values; 1 or 2, default 0)

    - **UART_EXTRA_OPT_OVERSAMPL_3X_ARITH**: UART baudrate calculation using 3x oversampling with arithmetic baud calculation

    - **UART_EXTRA_OPT_OVERSAMPL_8X_FRACT**: UART baudrate calculation using 8x oversampling with fractional baud calculation

    - **UART_EXTRA_OPT_OVERSAMPL_8X_ARITH**: UART baudrate calculation using 8x oversampling with arithmetic baud calculation

    - **UART_EXTRA_OPT_OVERSAMPL_16X_FRACT**: UART baudrate calculation using 16x oversampling with fractional baud calculation

    - **UART_EXTRA_OPT_EXTERNAL_CLOCK**: Force the usage of external clock source (mapped to pin) for the UART module

    - **UART_EXTRA_OPT_ENABLE_RX**: Enable the Receiver of the UART module preconditionally

    - **UART_EXTRA_OPT_IRDA_ENCODING**: Enable IRDA encoding for usage with IR remotes

    - **UART_EXTRA_OPT_COLLISION_DETECT**: Enable collision detection for the UART bus

    - **UART_EXTRA_OPT_TWO_STOP_BITS**: Enable the usage of two stop-bits

    - **UART_EXTRA_OPT_CHAR_SIZE_x_BITS**: Set custom character sizes (possible values range from 5 to 9, default 8)

    Combine options with OR:
    ```c
    uart_extra_config_opt_t config = (UART_EXTRA_OPT_AUTO_BAUD | UART_EXTRA_OPT_PARITY);
    ```

## Example Configuration

!!! example "Adafruit Feather M0 (SAMD21) UART setup"
    ```c
    #include <hal_gpio.h>
    #include <hal_uart.h>

    #ifndef F_CPU
    #define F_CPU 48000000
    #endif

    /* Define GPIO pins */
    #define RX_PIN GPIO_PIN_PA13
    #define TX_PIN GPIO_PIN_PA12

    /* UART settings */
    #define UART_BAUDRATE 115200
    #define UART_PERIPHERAL UART_PERIPHERAL_4
    #define UART_CLOCK_SOURCE UART_CLK_SOURCE_USE_DEFAULT
    #define UART_CONFIG_OPTIONS UART_OPT_RX_PAD_1

    void setup() {
       GPIO_SET_PIN_MODE(RX_PIN, GPIO_MODE_D);
       GPIO_SET_PIN_MODE(TX_PIN, GPIO_MODE_D);
       uhal_uart_init(UART_PERIPHERAL,UART_BAUDRATE, UART_CLOCK_SOURCE, F_CPU, UART_CONFIG_OPTIONS);
    }
    ...
    ```

