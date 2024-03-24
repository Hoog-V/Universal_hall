#include <hal_uart.h>
#include <hardware/uart.h>
#include "bit_manipulation.h"

static inline uart_inst_t* get_uart_inst(const uart_peripheral_inst_t uart_peripheral_num) {
    return (uart_peripheral_num == 0) ? uart0 : uart1;
}

uhal_status_t uhal_uart_init(const uart_peripheral_inst_t uart_peripheral, const uint32_t baudrate, const uart_clock_sources_t clock_source,
                             const uint32_t clock_source_freq, const uart_extra_config_opt_t uart_extra_opt) {
    uart_inst_t* inst = get_uart_inst(uart_peripheral);
    uint32_t     baudrate_ret = uart_init(inst, baudrate);
    if (uart_extra_opt != UART_EXTRA_OPT_USE_DEFAULT) {
        const uint8_t data_bits_mask = BITMASK_COMPARE(uart_extra_opt, UART_EXTRA_OPT_CHAR_SIZE_7_BITS);
        const uint8_t data_bits = (data_bits_mask != 0) ? data_bits_mask : 8;
        const uint8_t stop_bits = BIT_IS_SET(uart_extra_opt, 3) ? 2 : 1;
        const uint8_t parity_bits = BITMASK_COMPARE(uart_extra_opt, (UART_EXTRA_OPT_PARITY_EVEN | UART_EXTRA_OPT_PARITY_ODD)) >> 24;
        uart_set_format(inst, data_bits, stop_bits, parity_bits);
    }
    return (baudrate_ret > 0) ? UHAL_STATUS_OK : UHAL_STATUS_ERROR;
}

uhal_status_t uhal_uart_deinit(const uart_peripheral_inst_t uart_peripheral) {
    uart_inst_t* inst = get_uart_inst(uart_peripheral);
    uart_deinit(inst);
    return UHAL_STATUS_OK;
}

uhal_status_t uhal_uart_transmit(const uart_peripheral_inst_t uart_peripheral, const uint8_t* transmit_buffer, const size_t size) {
    uart_inst_t* inst = get_uart_inst(uart_peripheral);
    uart_write_blocking(inst, transmit_buffer, size);
    return UHAL_STATUS_OK;
}

uhal_status_t uhal_uart_receive(const uart_peripheral_inst_t uart_peripheral, uint8_t* receive_buffer, const size_t size) {
    uart_inst_t* inst = get_uart_inst(uart_peripheral);
    uart_read_blocking(inst, receive_buffer, size);
    return UHAL_STATUS_OK;
}