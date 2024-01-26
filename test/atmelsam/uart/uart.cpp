#include <hal_uart.h>
#include <sam.h>
#include "fff.h"
#include "gmock/gmock.h"

static Port MockPort = {};

static Eic MockEic = {};

static Gclk MockGclk = {};

static Pm MockPm = {};

Gclk* GCLK = &MockGclk;

Port* PORT = &MockPort;

Eic* EIC = &MockEic;

Pm* PM = &MockPm;

Sercom MockSercom[6] = {};

const uint32_t common_baud_rates[] = {600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200, 256000};

TEST(HAL_UART, UART_PM_SET_APBCMASK) {
    const uart_peripheral_inst_t uart_peripheral = UART_PERIPHERAL_4;
    uart_init(uart_peripheral, 115200, UART_CLK_SOURCE_USE_DEFAULT, (48e6), UART_EXTRA_OPT_USE_DEFAULT);
    EXPECT_EQ(PM->APBCMASK.reg, (1 << (PM_APBCMASK_SERCOM0_Pos + uart_peripheral)));
}

TEST(HAL_UART, UART_DEFAULT_CLOCK_SOURCE) {
    const uart_peripheral_inst_t uart_peripheral = UART_PERIPHERAL_0;
    const uint8_t                clk_gen_fast_default = 0;
    uart_init(uart_peripheral, 115200, UART_CLK_SOURCE_USE_DEFAULT, (48e6), UART_EXTRA_OPT_USE_DEFAULT);
    EXPECT_EQ(GCLK->GENDIV.bit.ID, clk_gen_fast_default);
    EXPECT_EQ(GCLK->CLKCTRL.bit.GEN, clk_gen_fast_default);
    EXPECT_EQ(GCLK->CLKCTRL.bit.ID, (GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + uart_peripheral));
}

TEST(HAL_UART, UART_CUSTOM_CLOCK_SOURCE) {
    const uart_peripheral_inst_t uart_peripheral = UART_PERIPHERAL_1;
    uart_init(uart_peripheral, 115200, UART_CLK_SOURCE_FAST_CLKGEN4, (48 * 10 ^ 6), UART_EXTRA_OPT_USE_DEFAULT);
    EXPECT_EQ(GCLK->GENDIV.bit.ID, 4);
    EXPECT_EQ(GCLK->CLKCTRL.bit.GEN, 4);
    EXPECT_EQ(GCLK->CLKCTRL.bit.ID, (GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + uart_peripheral));
}

TEST(HAL_UART, BAUD_CALCULATION_OVSMPL_16_ARITH) {
    memset(MockSercom, 0x00, sizeof(MockSercom));
    uint32_t baudrate, clock_source_div, clock_source_freq;
    uint64_t expected_baud;

    for (uint8_t peripheral = UART_PERIPHERAL_0; peripheral < UART_PERIPHERAL_5; peripheral++) {
        for (uint32_t i = 0; i < (sizeof(common_baud_rates)/sizeof(uint32_t)); i++) {
            baudrate = common_baud_rates[i];
            clock_source_freq = (48e6);
            clock_source_div = (clock_source_freq);
            expected_baud = 65536 - ((65536 * 16.0f * (baudrate)) / (clock_source_div));

            uart_init((uart_peripheral_inst_t) peripheral, baudrate, UART_CLK_SOURCE_USE_DEFAULT, clock_source_freq, UART_EXTRA_OPT_USE_DEFAULT);

            EXPECT_EQ(MockSercom[peripheral].USART.BAUD.reg, expected_baud) << "Testing baudrate: " << std::to_string(baudrate) << '\n';
       }
    }
}

TEST(HAL_UART, BAUD_CALCULATION_OVSMPL_16_FRACT) {
    memset(MockSercom, 0x00, sizeof(MockSercom));
}


int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);

    if (RUN_ALL_TESTS()) {}
    return 0;
}