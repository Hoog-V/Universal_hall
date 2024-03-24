/**
* \file            hal_uart.h
* \brief           UART driver module include file
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
#ifndef HAL_UART_H
#define HAL_UART_H
/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef DISABLE_UART_MODULE
#include "uart/uart_platform_specific.h"

/**
 * @brief Function to initialize the specified HW peripheral with uart functionality.
 *        To ensure platform compatibility use the default option as much as possible for each hw peripheral.
 *
 * @param uart_peripheral The uart peripheral to configure
 * @param baud_rate The UART baud frequency to be used
 * @param clock_source The clock source(s) to use..
 * @param clock_source_freq The clock frequency of the peripheral (if multiple clocks are used this will be the fastest frequency).
 * @param uart_extra_opt The extra configuration options like: - DMA use
 *                                                             - IRQ priority
 */
uhal_status_t uhal_uart_init(const uart_peripheral_inst_t uart_peripheral, const uint32_t baudrate, const uart_clock_sources_t clock_source, const uint32_t clock_source_freq, const uart_extra_config_opt_t uart_extra_opt);

/**
 * @brief Function to deinitialize the specified UART HW peripheral
 * 
 * @param uart_peripheral The uart peripheral to deinitialize
*/
uhal_status_t uhal_uart_deinit(const uart_peripheral_inst_t uart_peripheral);

/**
 * @brief Function to transmit data over UART connection
 * 
 * @param uart_peripheral The uart peripheral to use
 * @param transmit_buffer The buffer to transmit
 * @param size The amount of bytes to transmit
*/
uhal_status_t uhal_uart_transmit(const uart_peripheral_inst_t uart_peripheral, const uint8_t* transmit_buffer, const size_t size);

/**
 * @brief Function to receive data from UART connection
 * 
 * @param uart_peripheral The uart peripheral to use
 * @param receive_buffer The read buffer
 * @param size The amount of bytes to read
*/
uhal_status_t uhal_uart_receive(const uart_peripheral_inst_t uart_peripheral, uint8_t* receive_buffer, const size_t size);

#endif /* DISABLE_UART_MODULE */
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif