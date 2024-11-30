/**
* \file            pinmux_iwr68xx.c
* \brief           Source file which implements the standard GPIO API functions
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
#include <hal_pinmux.h>
#include <AWR6843.h>
#include <AWR6843_IOMUX.h>

/* 
* Magic numbers; 
* No register description available for CFGKICK0 & CFGKICK1 
* Values were retrieved from original MMWave-SDK
*/
#define PINMUX_UNLOCK_CFGKICK0 0x83E70B13U
#define PINMUX_UNLOCK_CFGKICK1 0x95A4F1E0U

#define PINMUX_LOCK_CFGKICK0 0x0U
#define PINMUX_LOCK_CFGKICK1 0x0U

uhal_status_t pinmux_set_pin_function(const mux_pin_t pin, const pinmux_func_t function) {
    #ifdef TI_RTOS
    uintptr_t key = HwiP_disable();
#endif
    /* Enable Pin Muxing */
    IOMUX->IOCFGKICK0 = PINMUX_UNLOCK_CFGKICK0;
    IOMUX->IOCFGKICK1 = PINMUX_UNLOCK_CFGKICK1;


    IOMUX->PADxx_CFG_REG[pin].bit.FUNC_SEL = function & 0xFF;
    IOMUX->PADxx_CFG_REG[pin].bit.IE_OVERRIDE_BIT = 0;
    IOMUX->PADxx_CFG_REG[pin].bit.IE_OVERRIDE_CTRL = 0;
    IOMUX->PADxx_CFG_REG[pin].bit.OE_OVERRIDE_BIT = 0;
    IOMUX->PADxx_CFG_REG[pin].bit.OE_OVERRIDE_CTRL = 0;

    /* Disable Pin Muxing */
    IOMUX->IOCFGKICK1 = PINMUX_LOCK_CFGKICK0;
    IOMUX->IOCFGKICK0 = PINMUX_LOCK_CFGKICK1;
#ifdef TI_RTOS
    HwiP_restore(key);
#endif
return UHAL_STATUS_OK;
}
