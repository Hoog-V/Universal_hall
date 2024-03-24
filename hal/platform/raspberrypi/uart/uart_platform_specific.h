#ifndef UART_PLATFORM_SPECIFIC
#define UART_PLATFORM_SPECIFIC

#include "error_handling.h"
#include <stdint.h>
#include <stddef.h>

typedef enum {
    UART_PERIPHERAL_0,
    UART_PERIPHERAL_1
} uart_peripheral_inst_t;

typedef enum {
    UART_EXTRA_OPT_USE_DEFAULT,
    UART_EXTRA_OPT_PARITY_EVEN = 16777216,
    UART_EXTRA_OPT_PARITY_ODD = 33554432,
    UART_EXTRA_OPT_TWO_STOP_BITS = 8,
    UART_EXTRA_OPT_CHAR_SIZE_5_BITS = 5,
    UART_EXTRA_OPT_CHAR_SIZE_6_BITS = 6,
    UART_EXTRA_OPT_CHAR_SIZE_7_BITS = 7,
} uart_extra_config_opt_t;

typedef enum {
    UART_CLK_SOURCE_USE_DEFAULT = 0x00
} uart_clock_sources_t;

#endif /* UART_PLATFORM_SPECIFIC */