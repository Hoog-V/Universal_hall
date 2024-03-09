/**
* \file            usb_serial_samd.c
* \brief           USB serial module include file
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
#include <hal_usb_serial.h>
#include <lib/hw/bsp/board_api.h>
#include <sam.h>
#if CFG_TUSB_OS == OPT_OS_FREERTOS
#include <FreeRTOS.h>
#endif

static inline uint8_t get_fast_clk_gen_val(const usb_clock_sources_t clock_sources) {
    const uint16_t fast_clk_val = (clock_sources & 0xFF) - 1;
    return fast_clk_val;
}

uhal_status_t usb_serial_init(const usb_serial_inst_t serial_instance, const usb_clock_sources_t clock_source, const uint32_t clock_frequency) {
    uint8_t clk_gen_fast = 0;

    if (clock_source != USB_CLK_SOURCE_USE_DEFAULT) {
        clk_gen_fast = get_fast_clk_gen_val(clock_source);
    }

    PM->AHBMASK.reg |= PM_AHBMASK_USB;
    PM->APBBMASK.reg |= PM_APBBMASK_USB;
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(clk_gen_fast) | GCLK_CLKCTRL_ID_USB | GCLK_CLKCTRL_CLKEN;
    GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(clk_gen_fast);
    while (GCLK->STATUS.bit.SYNCBUSY)
        ;
    NVIC_ClearPendingIRQ(USB_IRQn);
    NVIC_EnableIRQ(USB_IRQn);
#if CFG_TUSB_OS == OPT_OS_FREERTOS
    NVIC_SetPriority(USB_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
#endif

    SystemCoreClock = clock_frequency;
    SysTick_Config(clock_frequency / 1000);

    volatile uint8_t res = tud_init(serial_instance);

    if (board_init_after_tusb) {
        board_init_after_tusb();
    }
    return res;
}

const uint8_t usb_serial_available(const usb_serial_inst_t serial_instance) {

    return tud_cdc_n_connected(serial_instance);
}

uhal_status_t usb_serial_write_bytes(const usb_serial_inst_t serial_instance, const uint8_t* buffer, const size_t size) {

    for (uint8_t index = 0; index < size; index++) {
        tud_cdc_n_write_char(serial_instance, buffer[index]);
    }
    tud_cdc_n_write_flush(serial_instance);

    return UHAL_STATUS_OK;
}

uhal_status_t usb_serial_read_bytes(const usb_serial_inst_t serial_instance, uint8_t* buffer, const size_t size) {
    tud_cdc_n_read(serial_instance, buffer, size);
    return UHAL_STATUS_OK;
}

uhal_status_t usb_serial_write_string(const usb_serial_inst_t serial_instance, const char* str) {
    tud_cdc_n_write_str(serial_instance, str);
    tud_cdc_n_write_flush(serial_instance);
    return UHAL_STATUS_OK;
}

uhal_status_t usb_serial_read_string(const usb_serial_inst_t serial_instance, char* buffer, const size_t size, const uint8_t read_until_newline) {
    uint8_t i = 0;
    int8_t  ch;
    do {
        ch = tud_cdc_n_read_char(serial_instance);
        if (ch != -1) {
            buffer[i++] = ch;
        }

        if (ch == '\r' && read_until_newline) {
            break;
        }
        #if CFG_TUSB_OS == OPT_OS_NONE
        tud_task();
        #endif
    } while (i < size);

    buffer[i] = '\0';
    
    return UHAL_STATUS_OK;
}


const int8_t usb_serial_read_char(const usb_serial_inst_t serial_instance) {
  char ch = tud_cdc_n_read_char(serial_instance);
  tud_cdc_n_read_flush(serial_instance);
  return ch;
}

uhal_status_t usb_serial_deinit(const usb_serial_inst_t serial_instance) {

    return UHAL_STATUS_OK;
}

void USB_Handler(void) {
    tud_int_handler(0);
}

void usb_serial_poll_task() {
    tud_task();
}

#if CFG_TUSB_OS == OPT_OS_NONE
volatile uint32_t system_ticks = 0;

void SysTick_Handler(void) {
    system_ticks++;
}

uint32_t board_millis(void) {
    return system_ticks;
}

#endif
