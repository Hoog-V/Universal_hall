/**
* \file            hal_pinmux.h
* \brief           Pinmux module include file
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
#ifndef UHAL_PINMUX_H
#define UHAL_PINMUX_H


#include "error_handling.h"
#include "hal_gpio.h"
#include <pinmux/pinmux_platform_specific.h>

/* Extern c for compiling with c++*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Sets pin to function specified by table in manufacturers schematic
 * @param pin The pin to map specific function to
 * @param function The pin function which maps to pin (see pinmux_platform_specific.h)
 * @return Whether pin got succesfully set; UHAL_STATUS_OK or UHAL_STATUS_ERR
 */
uhal_status_t pinmux_set_pin_function(const mux_pin_t pin, const pinmux_func_t function);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* UHAL_PINMUX_H */
