/**
* \file            uart.c
* \brief           UART driver module source file
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
#ifndef DISABLE_UART_MODULE
#include "hal_uart.h"
#include "sam.h"

#define UART_EXTRA_OPT_BAUD_RANGE_BITMASK (0x700)
#define UART_EXTRA_OPT_BAUD_RANGE_BITPOS  (8)

#define SERCOM_SLOW_CLOCK_SOURCE(x)       (x >> 8)
Sercom* uart_peripheral_mapping[] = {SERCOM0, SERCOM1, SERCOM2, SERCOM3, SERCOM4, SERCOM5};

static inline void uart_wait_for_sync(const void* const hw, const uint32_t bits_to_read) {
    while (((Sercom*)hw)->SPI.SYNCBUSY.reg & bits_to_read) {
        ;
    }
}

static inline uint8_t get_fast_clk_gen_val(const uart_clock_sources_t clock_sources) {
    const uint16_t fast_clk_val = (clock_sources & 0xFF) - 1;
    return fast_clk_val;
}

static inline uint8_t get_slow_clk_gen_val(const uart_clock_sources_t clock_sources) {
    const uint16_t slow_clk_val = SERCOM_SLOW_CLOCK_SOURCE(clock_sources) - 1;
    return slow_clk_val;
}

static inline void set_baud_rate(Sercom* const sercom_inst, const uint32_t clock_source_freq, const uint32_t baudrate,
                                 const uart_extra_config_opt_t opt) {
    const uint32_t config_opt = (opt & UART_EXTRA_OPT_BAUD_RANGE_BITMASK) >> UART_EXTRA_OPT_BAUD_RANGE_BITPOS;
    //printf("Config opt: %d \n", config_opt);
    volatile uint64_t baud;
    switch (config_opt) {
        case 1: {
            baud = (clock_source_freq * 8) / (16 * baudrate);
            sercom_inst->USART.BAUD.FRAC.FP = (baud % 8);
            sercom_inst->USART.BAUD.FRAC.BAUD = (baud / 8);
            break;
        }
        case 2: {
            baud = 65536 - ((65536 * 8.0f * (baudrate / 100)) / (clock_source_freq / 100));
            sercom_inst->USART.BAUD.reg = baud;
            break;
        }
        case 3: {
            baud = (clock_source_freq * 8) / (8 * baudrate);
            sercom_inst->USART.BAUD.FRAC.FP = (baud % 8);
            sercom_inst->USART.BAUD.FRAC.BAUD = (baud / 8);
            break;
        }
        case 4: {
            baud = 65536 - ((65536 * 3.0f * (baudrate / 100)) / (clock_source_freq / 100));
            sercom_inst->USART.BAUD.reg = baud;
            break;
        }
        default: {
            baud = 65536 - ((65536 * 16.0f * (baudrate / 100)) / (clock_source_freq / 100));
            sercom_inst->USART.BAUD.reg = baud;
            break;
        }
    }
}

static inline void set_uart_clocks(const uart_peripheral_inst_t uart_peripheral, const uart_clock_sources_t clock_source) {
    // Set the clock system
#ifdef __SAMD51__

#else
    PM->APBCMASK.reg |= 1 << (PM_APBCMASK_SERCOM0_Pos + uart_peripheral);
    if (clock_source != UART_CLK_SOURCE_USE_DEFAULT) {
        const uint8_t clk_gen_slow = get_slow_clk_gen_val(clock_source);
        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(clk_gen_slow) | GCLK_CLKCTRL_ID_SERCOMX_SLOW | GCLK_CLKCTRL_CLKEN;
        while (GCLK->STATUS.bit.SYNCBUSY)
            ;
        const uint8_t clk_gen_fast = get_fast_clk_gen_val(clock_source);
        GCLK->CLKCTRL.reg =
            GCLK_CLKCTRL_GEN(clk_gen_fast) | ((GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + uart_peripheral) << GCLK_CLKCTRL_ID_Pos) | GCLK_CLKCTRL_CLKEN;
        GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(clk_gen_fast);
        while (GCLK->STATUS.bit.SYNCBUSY)
            ;
    } else {
        const uint8_t clk_gen_slow = 1;
        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_GEN(clk_gen_slow) | GCLK_CLKCTRL_ID_SERCOMX_SLOW | GCLK_CLKCTRL_CLKEN;
        while (GCLK->STATUS.bit.SYNCBUSY)
            ;
        const uint8_t clk_gen_fast = 0;
        GCLK->CLKCTRL.reg =
            GCLK_CLKCTRL_GEN(clk_gen_fast) | ((GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + uart_peripheral) << GCLK_CLKCTRL_ID_Pos) | GCLK_CLKCTRL_CLKEN;
        GCLK->GENDIV.reg = GCLK_GENDIV_DIV(0x01) | GCLK_GENDIV_ID(clk_gen_fast);
        while (GCLK->STATUS.bit.SYNCBUSY)
            ;
    }
#endif
}

uhal_status_t uhal_uart_init(const uart_peripheral_inst_t uart_peripheral, const uint32_t baudrate, const uart_clock_sources_t clock_source,
                        const uint32_t clock_source_freq, const uart_extra_config_opt_t uart_extra_opt) {

    set_uart_clocks(uart_peripheral, clock_source);
    Sercom* const sercom_inst = uart_peripheral_mapping[uart_peripheral];
    sercom_inst->USART.CTRLA.reg = SERCOM_USART_CTRLA_SWRST;
    uart_wait_for_sync(sercom_inst, (SERCOM_USART_SYNCBUSY_SWRST | SERCOM_USART_SYNCBUSY_ENABLE));

    const uint32_t not_auto_baud = (uart_extra_opt & UART_EXTRA_OPT_AUTO_BAUD) == 0;
    uint32_t       cmode = (uart_extra_opt & UART_EXTRA_OPT_SYNCHRONOUS_COMMUNICATION);
    uint32_t       sampr = (uart_extra_opt & UART_EXTRA_OPT_BAUD_RANGE_BITMASK) << 5;

    if (not_auto_baud) {
        set_baud_rate(sercom_inst, clock_source_freq, baudrate, uart_extra_opt);
    } else {
        /* 
            Auto baud only works with:
           - Async communication
           - Fractional 16x oversampling 
        */
        cmode = 0;
        set_baud_rate(sercom_inst, clock_source_freq, baudrate, UART_EXTRA_OPT_OVERSAMPL_16X_FRACT);
        sampr = SERCOM_USART_CTRLA_SAMPR(1);
    }

    const uint32_t cpol = (uart_extra_opt & UART_EXTRA_OPT_INVERSE_CLOCK_POLARITY);
    const uint32_t form = (uart_extra_opt & UART_EXTRA_OPT_AUTO_BAUD);
    const uint32_t rxpo = (uart_extra_opt & UART_EXTRA_OPT_RX_PAD_3);
    const uint32_t txpo = (uart_extra_opt & UART_EXTRA_OPT_TX_PAD_2);
    sercom_inst->USART.CTRLA.reg = sampr | txpo | rxpo | form | cmode | cpol;

    const uint32_t mode_def_opt = (uart_extra_opt & UART_EXTRA_OPT_EXTERNAL_CLOCK) == 0;
    if(mode_def_opt) {
        sercom_inst->USART.CTRLA.reg |= SERCOM_USART_CTRLA_MODE(1);
    }

    uint32_t dord_def_opt = (uart_extra_opt & UART_EXTRA_OPT_MSB_FIRST) == 0;
    if (dord_def_opt) {
        sercom_inst->USART.CTRLA.reg |= SERCOM_USART_CTRLA_DORD;
    }

    sercom_inst->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
    uart_wait_for_sync(sercom_inst, (SERCOM_USART_SYNCBUSY_SWRST | SERCOM_USART_SYNCBUSY_ENABLE));

    return UHAL_STATUS_OK;
}

uhal_status_t uhal_uart_deinit(const uart_peripheral_inst_t uart_peripheral) {
    Sercom* const sercom_inst = uart_peripheral_mapping[uart_peripheral];
    sercom_inst->USART.CTRLA.reg &= ~(SERCOM_USART_CTRLA_ENABLE);
    uart_wait_for_sync(sercom_inst, (SERCOM_USART_SYNCBUSY_SWRST | SERCOM_USART_SYNCBUSY_ENABLE));
    
    return UHAL_STATUS_OK;
}

uhal_status_t uhal_uart_transmit(const uart_peripheral_inst_t uart_peripheral, const uint8_t* transmit_buffer, const size_t size) {
    Sercom* sercom_inst = uart_peripheral_mapping[uart_peripheral];
    sercom_inst->USART.CTRLB.reg |= SERCOM_USART_CTRLB_TXEN;
    for (uint8_t i = 0; i < size; i++) {
        sercom_inst->USART.DATA.reg = transmit_buffer[i];
        while (!sercom_inst->USART.INTFLAG.bit.TXC)
            ;
    }
    sercom_inst->USART.CTRLB.reg &= ~SERCOM_USART_CTRLB_TXEN;
    return UHAL_STATUS_OK;
}

uhal_status_t uhal_uart_receive(const uart_peripheral_inst_t uart_peripheral, uint8_t* receive_buffer, const size_t size) {
    Sercom* sercom_inst = uart_peripheral_mapping[uart_peripheral];
    sercom_inst->USART.CTRLB.reg |= SERCOM_USART_CTRLB_RXEN;
    for (uint8_t i = 0; i < size; i++) {
        while (!sercom_inst->USART.INTFLAG.bit.RXC)
            ;
        receive_buffer[i] = sercom_inst->USART.DATA.reg;
    }
    return UHAL_STATUS_OK;
}
#endif /* DISABLE_UART_MODULE */