#include <hal_uart.h>
#include <hardware/uart.h>


static inline uart_inst_t* get_spi_inst(const uart_peripheral_inst_t uart_peripheral_num) {
    return (uart_peripheral_num == 0) ? uart0 : uart1;
}


uhal_status_t uhal_uart_init(const uart_peripheral_inst_t uart_peripheral, const uint32_t baudrate, const uart_clock_sources_t clock_source, const uint32_t clock_source_freq, const uart_extra_config_opt_t uart_extra_opt) {

}

uhal_status_t uhal_uart_transmit(const uart_peripheral_inst_t uart_peripheral, const uint8_t* transmit_buffer, const size_t size) {

}

uhal_status_t uhal_uart_receive(const uart_peripheral_inst_t uart_peripheral, uint8_t* receive_buffer, const size_t size) {

}