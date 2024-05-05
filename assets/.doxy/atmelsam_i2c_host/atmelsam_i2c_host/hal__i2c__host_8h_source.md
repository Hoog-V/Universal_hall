

# File hal\_i2c\_host.h

[**File List**](files.md) **>** [**hal**](dir_52c912f73dac9c4c0e442232e1b2bd80.md) **>** [**hal\_i2c\_host.h**](hal__i2c__host_8h.md)

[Go to the documentation of this file](hal__i2c__host_8h.md)


```C++

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

#ifndef HAL_I2C_H
#define HAL_I2C_H
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef DISABLE_I2C_HOST_MODULE

#include "assert.h"
#include "error_handling.h"
#include "i2c_common/i2c_platform_specific.h"

typedef enum {
    I2C_NO_STOP_BIT,
    I2C_STOP_BIT
} i2c_stop_bit_t;

uhal_status_t i2c_host_init(const i2c_periph_inst_t i2c_peripheral_num, const i2c_clock_sources_t clock_sources,
                            const uint32_t periph_clk_freq, const uint32_t baud_rate_freq,
                            const i2c_extra_opt_t extra_configuration_options);

#define I2C_HOST_INIT(i2c_peripheral_num, clock_sources, periph_clk_freq, baud_rate_freq, extra_configuration_options) \
do {                                                                                                                    \
    I2C_HOST_INIT_FUNC_PARAMETER_CHECK(i2c_peripheral_num, clock_sources, periph_clk_freq, baud_rate_freq, extra_configuration_options); \
    i2c_host_init(i2c_peripheral_num, clock_sources, periph_clk_freq, baud_rate_freq, extra_configuration_options);               \
}while(0);

uhal_status_t i2c_host_deinit(const i2c_periph_inst_t i2c_peripheral_num);


#define I2C_HOST_DEINIT(i2c_peripheral_num) \
do {                                        \
I2C_HOST_DEINIT_FUNC_PARAMETER_CHECK(i2c_peripheral_num);    \
i2c_host_deinit(i2c_peripheral_num);\
}while(0);

uhal_status_t i2c_host_write_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                                      const uint16_t addr,
                                      const uint8_t *write_buff,
                                      const size_t size,
                                      const i2c_stop_bit_t stop_bit);

#define I2C_HOST_WRITE_BLOCKING(i2c_peripheral_num, addr, write_buff, size, stop_bit) \
do {                                                                            \
I2C_HOST_WRITE_FUNC_PARAMETER_CHECK(i2c_peripheral_num, addr, write_buff, size, stop_bit); \
i2c_host_write_blocking(i2c_peripheral_num, addr, write_buff, size, stop_bit);             \
}while(0);

uhal_status_t i2c_host_write_non_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                                          const uint16_t addr,
                                          const uint8_t *write_buff,
                                          const size_t size,
                                          const i2c_stop_bit_t stop_bit);

#define I2C_HOST_WRITE_NON_BLOCKING(i2c_peripheral_num, addr, write_buff, size, stop_bit) \
do {                                                                            \
I2C_HOST_WRITE_FUNC_PARAMETER_CHECK(i2c_peripheral_num, addr, write_buff, size, stop_bit); \
i2c_host_write_non_blocking(i2c_peripheral_num, addr, write_buff, size, stop_bit);             \
}while(0);

uhal_status_t i2c_host_read_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                                     const uint16_t addr,
                                     uint8_t *read_buff,
                                     const size_t amount_of_bytes);

#define I2C_HOST_READ_BLOCKING(i2c_peripheral_num, addr, read_buff, size) \
do {                                                                            \
I2C_HOST_READ_FUNC_PARAMETER_CHECK(i2c_peripheral_num, addr, read_buff, size); \
i2c_host_read_blocking(i2c_peripheral_num, addr, read_buff, size);             \
}while(0);

uhal_status_t i2c_host_read_non_blocking(const i2c_periph_inst_t i2c_peripheral_num,
                                         const uint16_t addr,
                                         uint8_t *read_buff,
                                         const size_t amount_of_bytes);

#define I2C_HOST_READ_NON_BLOCKING(i2c_peripheral_num, addr, read_buff, size) \
do {                                                                          \
I2C_HOST_READ_FUNC_PARAMETER_CHECK(i2c_peripheral_num, addr, read_buff, size); \
i2c_host_read_non_blocking(i2c_peripheral_num, addr, read_buff, size);             \
}while(0);

void i2c_host_data_recv_irq(const void *hw, volatile bustransaction_t *transaction) __attribute__((weak));

void i2c_host_data_send_irq(const void *hw, volatile bustransaction_t *transaction) __attribute__((weak));

#endif /* IFNDEF DISABLE_I2C_HOST_MODULE*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif


```


