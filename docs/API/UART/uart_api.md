# UART API

## API Functionality

The API for UART functionality has the following functions available:

```c
/* UART driver initialization function (without compile-time parameter checking) */
uhal_status_t uhal_uart_init(const uart_peripheral_inst_t uart_peripheral, 
                             const uint32_t baudrate, 
                             const uart_clock_sources_t clock_source, 
                             const uint32_t clock_source_freq, 
                             const uart_extra_config_opt_t uart_extra_opt);

/* UART driver transmit function (without compile-time parameter checking) */
uhal_status_t uhal_uart_transmit(const uart_peripheral_inst_t uart_peripheral, 
                                 const uint8_t* transmit_buffer, 
                                 const size_t size);

/* UART driver receive function (without compile-time parameter checking) */
uhal_status_t uart_receive(const uart_peripheral_inst_t uart_peripheral, 
                           uint8_t* receive_buffer, 
                           const size_t size);

/* UART driver deinit function */
uhal_status_t uhal_uart_deinit(const uart_peripheral_inst_t uart_peripheral);

```

## uart_init function

```c
uhal_status_t uhal_uart_init(const uart_peripheral_inst_t uart_peripheral, 
                             const uint32_t baudrate, 
                             const uart_clock_sources_t clock_source, 
                             const uint32_t clock_source_freq, 
                             const uart_extra_config_opt_t uart_extra_opt);
```

### Description:

The `uart_init` function is designed for initializing a UART peripheral. It configures the peripheral with specific settings, including the clock source and additional options that dictate the behavior of the UART bus.
    
### Parameters:

1. **uart_peripheral (const uart_peripheral_inst_t)**: 
    - Specifies the UART peripheral instance to initialize. 

2. **baudrate (const uint32_t)**:
    - Specifies the baudrate of the to be intiliazed UART connection.

3. **clock_source (const uint32_t)**: 
    - Determines the clock source for the UART peripheral. This setting is critical as it affects the timing and synchronization of UART communications.

4. **uart_extra_opt (const uart_extra_config_opt_t)**: 
    - Provides additional configuration options for the UART bus. These options can include settings for clock polarity, phase, data order, character size, and pad-specific configurations necessary for the correct operation of the UART slave peripheral.

### Return:

- **uhal_status_t**: This function returns a status code that indicates whether the initialization was successful. It provides feedback on the setup process, helping to identify any issues that might have occurred during the initialization.

### Working:

1. The initialization process begins with the assignment of the specified `uart_peripheral` to the desired UART peripheral. This step ensures that the correct hardware unit is being configured.

2. The `clock_source` is then configured, setting up the timing and synchronization aspect of the UART communication based on the microcontroller's clock system.

3. Additional configurations, as specified by `uart_extra_config_opt`, are applied. These settings enable the customization of the UART operation according to the specific requirements of the application, such as adjusting clock polarity, data order, and other bus-specific parameters.

4. The UART peripheral is then enabled, ready to participate in UART communications



## uart_transmit function

```c

uhal_status_t uhal_uart_transmit(const uart_peripheral_inst_t uart_peripheral, 
                                 const uint8_t* transmit_buffer, 
                                 const size_t size);

```

### Description:

The `uart_transmit` function is used to transmit bytes over the serial bus.


### Parameters:

1. **uart_peripheral (const UART_host_inst_t)**: Specifies the UART peripheral instance to use for transmitting the bytes.

2. **transmit_buffer (const uint8_t*)**: Specifies the buffer which contains the data to transmit

3. **size (const size_t)**: Specifies the amount of bytes to transmit

### Return:

- **uhal_status_t**: The function returns a status, giving an indication of whether the transmit was succesful or not.

## uart_receive function

```c

uhal_status_t uhal_uart_receive(const uart_peripheral_inst_t uart_peripheral, 
                                uint8_t* receive_buffer, 
                                const size_t size);

```

### Description:

The `uart_receive` function is used to receive bytes over the serial bus.


### Parameters:

1.  **uart_peripheral (const UART_host_inst_t)**: Specifies the UART peripheral instance to use for receiving the bytes.

2.  **receive_buffer (const uint8_t*)**: Specifies the buffer where the data has to be saved to

3.  **size (const size_t)**: Specifies the amount of bytes to receive

### Return:

- **uhal_status_t**: The function returns a status, giving an indication of whether the receive was succesful or not.
