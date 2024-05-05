

# File hal\_i2c\_host.h



[**FileList**](files.md) **>** [**hal**](dir_52c912f73dac9c4c0e442232e1b2bd80.md) **>** [**hal\_i2c\_host.h**](hal__i2c__host_8h.md)

[Go to the source code of this file](hal__i2c__host_8h_source.md)



* `#include "assert.h"`
* `#include "error_handling.h"`
* `#include "i2c_common/i2c_platform_specific.h"`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**i2c\_stop\_bit\_t**](#enum-i2c_stop_bit_t)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**i2c\_host\_data\_recv\_irq**](#function-i2c_host_data_recv_irq) (const void \* hw, volatile bustransaction\_t \* transaction) <br>_IRQ handler for I2C host data receive interrupt. Gets run when a host read action is executed. By defining this function inside a source file outside the Universal HALL, the default IRQ handler will be overridden and the compiler will automatically link your own custom implementation._  |
|  void | [**i2c\_host\_data\_send\_irq**](#function-i2c_host_data_send_irq) (const void \* hw, volatile bustransaction\_t \* transaction) <br>_IRQ handler for I2C host data send interrupt. Gets run when a host write action is executed. By defining this function inside a -Obinarsource file outside the Universal HALL, the default IRQ handler will be overridden and the compiler will automatically link your own custom implementation._  |
|  uhal\_status\_t | [**i2c\_host\_deinit**](#function-i2c_host_deinit) (const i2c\_periph\_inst\_t i2c\_peripheral\_num) <br>_Function to de-initialize the specified HW peripheral (disables I2C on the HW peripheral)._  |
|  uhal\_status\_t | [**i2c\_host\_init**](#function-i2c_host_init) (const i2c\_periph\_inst\_t i2c\_peripheral\_num, const i2c\_clock\_sources\_t clock\_sources, const uint32\_t periph\_clk\_freq, const uint32\_t baud\_rate\_freq, const i2c\_extra\_opt\_t extra\_configuration\_options) <br>_Function to initialize the specified HW peripheral with I2C host functionality. To ensure platform compatibility use the default option as much as possible for each hw peripheral._  |
|  uhal\_status\_t | [**i2c\_host\_read\_blocking**](#function-i2c_host_read_blocking) (const i2c\_periph\_inst\_t i2c\_peripheral\_num, const uint16\_t addr, uint8\_t \* read\_buff, const size\_t amount\_of\_bytes) <br>_Function to execute a read blocking transaction (blocking means it will wait till the transaction is finished) This function does only work in host-mode._  |
|  uhal\_status\_t | [**i2c\_host\_read\_non\_blocking**](#function-i2c_host_read_non_blocking) (const i2c\_periph\_inst\_t i2c\_peripheral\_num, const uint16\_t addr, uint8\_t \* read\_buff, const size\_t amount\_of\_bytes) <br>_Function to execute a read non-blocking transaction (non-blocking means it will not wait till the transaction is finished and stack the transactions in to a buffer) This function does only work in host-mode._  |
|  uhal\_status\_t | [**i2c\_host\_write\_blocking**](#function-i2c_host_write_blocking) (const i2c\_periph\_inst\_t i2c\_peripheral\_num, const uint16\_t addr, const uint8\_t \* write\_buff, const size\_t size, const [**i2c\_stop\_bit\_t**](hal__i2c__host_8h.md#enum-i2c_stop_bit_t) stop\_bit) <br>_Function to execute a write blocking transaction (blocking means it will wait till the transaction is finished) This function does only work in host-mode._  |
|  uhal\_status\_t | [**i2c\_host\_write\_non\_blocking**](#function-i2c_host_write_non_blocking) (const i2c\_periph\_inst\_t i2c\_peripheral\_num, const uint16\_t addr, const uint8\_t \* write\_buff, const size\_t size, const [**i2c\_stop\_bit\_t**](hal__i2c__host_8h.md#enum-i2c_stop_bit_t) stop\_bit) <br>_Function to execute a write non-blocking transaction (non-blocking means it will not wait till the transaction is finished and stack them in a buffer or such) This function does only work in host-mode._  |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**I2C\_HOST\_DEINIT**](hal__i2c__host_8h.md#define-i2c_host_deinit) (i2c\_peripheral\_num) <br> |
| define  | [**I2C\_HOST\_INIT**](hal__i2c__host_8h.md#define-i2c_host_init) (i2c\_peripheral\_num, clock\_sources, periph\_clk\_freq, baud\_rate\_freq, extra\_configuration\_options) <br> |
| define  | [**I2C\_HOST\_READ\_BLOCKING**](hal__i2c__host_8h.md#define-i2c_host_read_blocking) (i2c\_peripheral\_num, addr, read\_buff, size) <br> |
| define  | [**I2C\_HOST\_READ\_NON\_BLOCKING**](hal__i2c__host_8h.md#define-i2c_host_read_non_blocking) (i2c\_peripheral\_num, addr, read\_buff, size) <br> |
| define  | [**I2C\_HOST\_WRITE\_BLOCKING**](hal__i2c__host_8h.md#define-i2c_host_write_blocking) (i2c\_peripheral\_num, addr, write\_buff, size, stop\_bit) <br> |
| define  | [**I2C\_HOST\_WRITE\_NON\_BLOCKING**](hal__i2c__host_8h.md#define-i2c_host_write_non_blocking) (i2c\_peripheral\_num, addr, write\_buff, size, stop\_bit) <br> |

## Public Types Documentation




### enum i2c\_stop\_bit\_t 

```C++
enum i2c_stop_bit_t {
    I2C_NO_STOP_BIT,
    I2C_STOP_BIT
};
```




<hr>
## Public Functions Documentation




### function i2c\_host\_data\_recv\_irq 

_IRQ handler for I2C host data receive interrupt. Gets run when a host read action is executed. By defining this function inside a source file outside the Universal HALL, the default IRQ handler will be overridden and the compiler will automatically link your own custom implementation._ 
```C++
void i2c_host_data_recv_irq (
    const void * hw,
    volatile bustransaction_t * transaction
) 
```





**Parameters:**


* `hw` Handle to the HW peripheral on which the I2C bus is ran 
* `transaction` I2C transaction info about the current initialized transaction on the HW peripheral. The info will be automatically supplied when using the i2c\_write and i2c\_read functions below



**Note:**

Using your own custom IRQ handler might break the use of the write and read functions listed above 





        

<hr>



### function i2c\_host\_data\_send\_irq 

_IRQ handler for I2C host data send interrupt. Gets run when a host write action is executed. By defining this function inside a -Obinarsource file outside the Universal HALL, the default IRQ handler will be overridden and the compiler will automatically link your own custom implementation._ 
```C++
void i2c_host_data_send_irq (
    const void * hw,
    volatile bustransaction_t * transaction
) 
```





**Parameters:**


* `hw` Handle to the HW peripheral on which the I2C bus is ran 
* `transaction` I2C transaction info about the current initialized transaction on the HW peripheral. The info will be automatically supplied when using the i2c\_write and i2c\_read functions below.



**Note:**

Using your own custom IRQ handler might break the use of the write and read functions listed above 





        

<hr>



### function i2c\_host\_deinit 

_Function to de-initialize the specified HW peripheral (disables I2C on the HW peripheral)._ 
```C++
uhal_status_t i2c_host_deinit (
    const i2c_periph_inst_t i2c_peripheral_num
) 
```





**Parameters:**


* `i2c_peripheral_num` I2C options used when configuring the HW peripheral. 




        

<hr>



### function i2c\_host\_init 

_Function to initialize the specified HW peripheral with I2C host functionality. To ensure platform compatibility use the default option as much as possible for each hw peripheral._ 
```C++
uhal_status_t i2c_host_init (
    const i2c_periph_inst_t i2c_peripheral_num,
    const i2c_clock_sources_t clock_sources,
    const uint32_t periph_clk_freq,
    const uint32_t baud_rate_freq,
    const i2c_extra_opt_t extra_configuration_options
) 
```





**Parameters:**


* `i2c_peripheral_num` The i2c peripheral to configure 
* `clock_sources` The clock source(s) to use.. 
* `periph_clk_freq` The clock frequency of the peripheral (if multiple clocks are used this will be the fastest frequency). 
* `baud_rate_freq` The I2C Clock frequency to be used in transactions (only used in host mode, when in slave mode every value will be discarded). 
* `extra_configuration_options` The extra configuration options like: - DMA use
  * 4-Wire mode
  * IRQ priority 






        

<hr>



### function i2c\_host\_read\_blocking 

_Function to execute a read blocking transaction (blocking means it will wait till the transaction is finished) This function does only work in host-mode._ 
```C++
uhal_status_t i2c_host_read_blocking (
    const i2c_periph_inst_t i2c_peripheral_num,
    const uint16_t addr,
    uint8_t * read_buff,
    const size_t amount_of_bytes
) 
```





**Parameters:**


* `i2c_peripheral_num` The i2c peripheral to use 
* `addr` The I2C address of the client device to write to 
* `read_buff` Pointer to the read buffer where all read bytes will be written 
* `amount_of_bytes` The amount of bytes which have to be read 




        

<hr>



### function i2c\_host\_read\_non\_blocking 

_Function to execute a read non-blocking transaction (non-blocking means it will not wait till the transaction is finished and stack the transactions in to a buffer) This function does only work in host-mode._ 
```C++
uhal_status_t i2c_host_read_non_blocking (
    const i2c_periph_inst_t i2c_peripheral_num,
    const uint16_t addr,
    uint8_t * read_buff,
    const size_t amount_of_bytes
) 
```





**Parameters:**


* `i2c_peripheral_num` The i2c peripheral to use 
* `addr` The I2C address of the client device to write to 
* `read_buff` Pointer to the read buffer where all read bytes will be written 
* `amount_of_bytes` The amount of bytes which have to be read 




        

<hr>



### function i2c\_host\_write\_blocking 

_Function to execute a write blocking transaction (blocking means it will wait till the transaction is finished) This function does only work in host-mode._ 
```C++
uhal_status_t i2c_host_write_blocking (
    const i2c_periph_inst_t i2c_peripheral_num,
    const uint16_t addr,
    const uint8_t * write_buff,
    const size_t size,
    const i2c_stop_bit_t stop_bit
) 
```





**Parameters:**


* `i2c_peripheral_num` The i2c peripheral to use 
* `addr` The I2C address of the client device to write to 
* `write_buff` Pointer to the write buffer with all the bytes that have to be written 
* `size` The amount of bytes which have to be written 
* `stop_bit` Does this transaction end with or without a stop-bit: Value 1 is with stop-bit Value 0 is without stop-bit 




        

<hr>



### function i2c\_host\_write\_non\_blocking 

_Function to execute a write non-blocking transaction (non-blocking means it will not wait till the transaction is finished and stack them in a buffer or such) This function does only work in host-mode._ 
```C++
uhal_status_t i2c_host_write_non_blocking (
    const i2c_periph_inst_t i2c_peripheral_num,
    const uint16_t addr,
    const uint8_t * write_buff,
    const size_t size,
    const i2c_stop_bit_t stop_bit
) 
```





**Parameters:**


* `i2c_peripheral_num` The i2c peripheral to use 
* `addr` The I2C address of the client device to write to 
* `write_buff` Pointer to the write buffer with all the bytes that have to be written 
* `size` The amount of bytes which have to be written 
* `stop_bit` Does this transaction end with or without a stop-bit: Value 1 is with stop-bit Value 0 is without stop-bit 




        

<hr>
## Macro Definition Documentation





### define I2C\_HOST\_DEINIT 

```C++
#define I2C_HOST_DEINIT (
    i2c_peripheral_num
) do {                                        \
I2C_HOST_DEINIT_FUNC_PARAMETER_CHECK(i2c_peripheral_num);    \ i2c_host_deinit (i2c_peripheral_num);\
}while(0);
```




<hr>



### define I2C\_HOST\_INIT 

```C++
#define I2C_HOST_INIT (
    i2c_peripheral_num,
    clock_sources,
    periph_clk_freq,
    baud_rate_freq,
    extra_configuration_options
) do {                                                                                                                    \
    I2C_HOST_INIT_FUNC_PARAMETER_CHECK(i2c_peripheral_num, clock_sources, periph_clk_freq, baud_rate_freq, extra_configuration_options); \ i2c_host_init (i2c_peripheral_num, clock_sources, periph_clk_freq, baud_rate_freq, extra_configuration_options);               \
}while(0);
```




<hr>



### define I2C\_HOST\_READ\_BLOCKING 

```C++
#define I2C_HOST_READ_BLOCKING (
    i2c_peripheral_num,
    addr,
    read_buff,
    size
) do {                                                                            \
I2C_HOST_READ_FUNC_PARAMETER_CHECK(i2c_peripheral_num, addr, read_buff, size); \ i2c_host_read_blocking (i2c_peripheral_num, addr, read_buff, size);             \
}while(0);
```




<hr>



### define I2C\_HOST\_READ\_NON\_BLOCKING 

```C++
#define I2C_HOST_READ_NON_BLOCKING (
    i2c_peripheral_num,
    addr,
    read_buff,
    size
) do {                                                                          \
I2C_HOST_READ_FUNC_PARAMETER_CHECK(i2c_peripheral_num, addr, read_buff, size); \ i2c_host_read_non_blocking (i2c_peripheral_num, addr, read_buff, size);             \
}while(0);
```




<hr>



### define I2C\_HOST\_WRITE\_BLOCKING 

```C++
#define I2C_HOST_WRITE_BLOCKING (
    i2c_peripheral_num,
    addr,
    write_buff,
    size,
    stop_bit
) do {                                                                            \
I2C_HOST_WRITE_FUNC_PARAMETER_CHECK(i2c_peripheral_num, addr, write_buff, size, stop_bit); \ i2c_host_write_blocking (i2c_peripheral_num, addr, write_buff, size, stop_bit);             \
}while(0);
```




<hr>



### define I2C\_HOST\_WRITE\_NON\_BLOCKING 

```C++
#define I2C_HOST_WRITE_NON_BLOCKING (
    i2c_peripheral_num,
    addr,
    write_buff,
    size,
    stop_bit
) do {                                                                            \
I2C_HOST_WRITE_FUNC_PARAMETER_CHECK(i2c_peripheral_num, addr, write_buff, size, stop_bit); \ i2c_host_write_non_blocking (i2c_peripheral_num, addr, write_buff, size, stop_bit);             \
}while(0);
```




<hr>

------------------------------
The documentation for this class was generated from the following file `hal/hal_i2c_host.h`

