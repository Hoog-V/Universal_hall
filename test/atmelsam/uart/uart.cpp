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

const uint32_t common_baud_rates[] = {1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200, 256000};

TEST(HAL_UART, UART_PM_SET_APBCMASK) {
    const uart_peripheral_inst_t uart_peripheral = UART_PERIPHERAL_4;
    uhal_uart_init(uart_peripheral, 115200, UART_CLK_SOURCE_USE_DEFAULT, (48e6), UART_EXTRA_OPT_USE_DEFAULT);
    EXPECT_EQ(PM->APBCMASK.reg, (1 << (PM_APBCMASK_SERCOM0_Pos + uart_peripheral)));
}

TEST(HAL_UART, UART_DEFAULT_CLOCK_SOURCE) {
    const uart_peripheral_inst_t uart_peripheral = UART_PERIPHERAL_0;
    const uint8_t                clk_gen_fast_default = 0;
    uhal_uart_init(uart_peripheral, 115200, UART_CLK_SOURCE_USE_DEFAULT, (48e6), UART_EXTRA_OPT_USE_DEFAULT);
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
    uint32_t baudrate, clock_source_div, clock_source_freq, expected_ctrla;
    uint64_t expected_baud;

    for (uint8_t peripheral = UART_PERIPHERAL_0; peripheral < UART_PERIPHERAL_5; peripheral++) {
        for (uint32_t i = 0; i < (sizeof(common_baud_rates)/sizeof(uint32_t)); i++) {
            baudrate = common_baud_rates[i];
            clock_source_freq = (48e6);
            clock_source_div = (clock_source_freq);
            expected_baud = 65536 - ((65535 * 16.0f * (baudrate)) / (clock_source_div));
            expected_ctrla = (SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_ENABLE | SERCOM_USART_CTRLA_MODE(1));

            uhal_uart_init((uart_peripheral_inst_t) peripheral, baudrate, UART_CLK_SOURCE_USE_DEFAULT, clock_source_freq, UART_EXTRA_OPT_USE_DEFAULT);

            EXPECT_EQ(MockSercom[peripheral].USART.BAUD.reg, expected_baud) << "Testing baudrate: " 
                                                                            << std::to_string(baudrate) 
                                                                            << " With peripheral num: " 
                                                                            << std::to_string(peripheral)
                                                                            << '\n';
            EXPECT_EQ(MockSercom[peripheral].USART.CTRLA.reg, expected_ctrla) << "Testing baudrate: " 
                                                                            << std::to_string(baudrate) 
                                                                            << " With peripheral num: " 
                                                                            << std::to_string(peripheral)
                                                                            << '\n';
       }
    }
}

TEST(HAL_UART, BAUD_CALCULATION_OVSMPL_16_FRACT) {
    memset(MockSercom, 0x00, sizeof(MockSercom));
    uint32_t baudrate, clock_source_div, clock_source_freq, expected_ctrla;
    uint64_t baudmult, expected_baud, expected_baud_fp;

    for (uint8_t peripheral = UART_PERIPHERAL_0; peripheral < UART_PERIPHERAL_5; peripheral++) {
        for (uint32_t i = 0; i < (sizeof(common_baud_rates)/sizeof(uint32_t)); i++) {
            baudrate = common_baud_rates[i];
            clock_source_freq = (48e6);
            clock_source_div = (clock_source_freq);
            baudmult = (clock_source_freq * 8) / (16 * baudrate);
            expected_baud = (baudmult / 8);
            expected_baud_fp = (baudmult % 8);
            expected_ctrla = (SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_SAMPR(1) | SERCOM_USART_CTRLA_ENABLE | SERCOM_USART_CTRLA_MODE(1));
            uhal_uart_init((uart_peripheral_inst_t) peripheral, baudrate, UART_CLK_SOURCE_USE_DEFAULT, clock_source_freq, UART_EXTRA_OPT_OVERSAMPL_16X_FRACT);

            EXPECT_EQ(MockSercom[peripheral].USART.BAUD.FRAC.BAUD, expected_baud) << "Testing baudrate: " 
                                                                            << std::to_string(baudrate) 
                                                                            << " With peripheral num: " 
                                                                            << std::to_string(peripheral)
                                                                            << '\n';
            EXPECT_EQ(MockSercom[peripheral].USART.BAUD.FRAC.FP, expected_baud_fp) << "Testing baudrate: " 
                                                                            << std::to_string(baudrate) 
                                                                            << " With peripheral num: " 
                                                                            << std::to_string(peripheral)
                                                                            << '\n';
            EXPECT_EQ(MockSercom[peripheral].USART.CTRLA.reg, expected_ctrla) << "Testing baudrate: " 
                                                                            << std::to_string(baudrate) 
                                                                            << " With peripheral num: " 
                                                                            << std::to_string(peripheral)
                                                                            << '\n';
       }
    }
}

TEST(HAL_UART, BAUD_CALCULATION_OVSMPL_8_ARITH) {
    memset(MockSercom, 0x00, sizeof(MockSercom));
    uint32_t baudrate, clock_source_div, clock_source_freq, expected_ctrla;
    uint64_t expected_baud;

    for (uint8_t peripheral = UART_PERIPHERAL_0; peripheral < UART_PERIPHERAL_5; peripheral++) {
        for (uint32_t i = 0; i < (sizeof(common_baud_rates)/sizeof(uint32_t)); i++) {
            baudrate = common_baud_rates[i];
            clock_source_freq = (48e6);
            clock_source_div = (clock_source_freq);
            expected_baud = 65536 - ((65535 * 8.0f * (baudrate)) / (clock_source_div));
            expected_ctrla = (SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_SAMPR(2) | SERCOM_USART_CTRLA_ENABLE | SERCOM_USART_CTRLA_MODE(1));

            uhal_uart_init((uart_peripheral_inst_t) peripheral, baudrate, UART_CLK_SOURCE_USE_DEFAULT, clock_source_freq, UART_EXTRA_OPT_OVERSAMPL_8X_ARITH);

            EXPECT_EQ(MockSercom[peripheral].USART.BAUD.reg, expected_baud) << "Testing baudrate: " 
                                                                            << std::to_string(baudrate) 
                                                                            << " With peripheral num: " 
                                                                            << std::to_string(peripheral)
                                                                            << '\n';
            EXPECT_EQ(MockSercom[peripheral].USART.CTRLA.reg, expected_ctrla) << "Testing baudrate: " 
                                                                            << std::to_string(baudrate) 
                                                                            << " With peripheral num: " 
                                                                            << std::to_string(peripheral)
                                                                            << '\n';
            EXPECT_EQ(MockSercom[peripheral].USART.CTRLA.reg, expected_ctrla) << "Testing baudrate: " 
                                                                            << std::to_string(baudrate) 
                                                                            << " With peripheral num: " 
                                                                            << std::to_string(peripheral)
                                                                            << '\n';
       }
    }
}

TEST(HAL_UART, BAUD_CALCULATION_OVSMPL_8_FRACT) {
    memset(MockSercom, 0x00, sizeof(MockSercom));
    uint32_t baudrate, clock_source_div, clock_source_freq, expected_ctrla;
    uint64_t baudmult, expected_baud, expected_baud_fp;

    for (uint8_t peripheral = UART_PERIPHERAL_0; peripheral < UART_PERIPHERAL_5; peripheral++) {
        for (uint32_t i = 0; i < (sizeof(common_baud_rates)/sizeof(uint32_t)); i++) {
            baudrate = common_baud_rates[i];
            clock_source_freq = (48e6);
            clock_source_div = (clock_source_freq);
            baudmult = (clock_source_freq * 8) / (8 * baudrate);
            expected_baud = (baudmult / 8);
            expected_baud_fp = (baudmult % 8);
            expected_ctrla = (SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_SAMPR(3) | SERCOM_USART_CTRLA_ENABLE | SERCOM_USART_CTRLA_MODE(1));

            uhal_uart_init((uart_peripheral_inst_t) peripheral, baudrate, UART_CLK_SOURCE_USE_DEFAULT, clock_source_freq, UART_EXTRA_OPT_OVERSAMPL_8X_FRACT);

            EXPECT_EQ(MockSercom[peripheral].USART.BAUD.FRAC.BAUD, expected_baud) << "Testing baudrate: " 
                                                                            << std::to_string(baudrate) 
                                                                            << " With peripheral num: " 
                                                                            << std::to_string(peripheral)
                                                                            << '\n';
            EXPECT_EQ(MockSercom[peripheral].USART.BAUD.FRAC.FP, expected_baud_fp) << "Testing baudrate: " 
                                                                            << std::to_string(baudrate) 
                                                                            << " With peripheral num: " 
                                                                            << std::to_string(peripheral)
                                                                            << '\n';
            EXPECT_EQ(MockSercom[peripheral].USART.CTRLA.reg, expected_ctrla) << "Testing baudrate: " 
                                                                            << std::to_string(baudrate) 
                                                                            << " With peripheral num: " 
                                                                            << std::to_string(peripheral)
                                                                            << '\n';
       }
    }
}

TEST(HAL_UART, BAUD_CALCULATION_OVSMPL_3_ARITH) {
    memset(MockSercom, 0x00, sizeof(MockSercom));
    uint32_t baudrate, clock_source_div, clock_source_freq, expected_ctrla;
    uint64_t expected_baud = 0;

    for (uint8_t peripheral = UART_PERIPHERAL_0; peripheral < UART_PERIPHERAL_5; peripheral++) {
        for (uint32_t i = 0; i < (sizeof(common_baud_rates)/sizeof(uint32_t)); i++) {
            baudrate = common_baud_rates[i];
            clock_source_freq = (48e6);
            clock_source_div = (clock_source_freq);
            expected_baud = 65536 - ((65535 * 3.0f * (baudrate)) / (clock_source_div));
            expected_ctrla = (SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_SAMPR(4) | SERCOM_USART_CTRLA_ENABLE | SERCOM_USART_CTRLA_MODE(1));

            uhal_uart_init((uart_peripheral_inst_t) peripheral, baudrate, UART_CLK_SOURCE_USE_DEFAULT, clock_source_freq, UART_EXTRA_OPT_OVERSAMPL_3X_ARITH);

            EXPECT_EQ(MockSercom[peripheral].USART.BAUD.reg, expected_baud) << "Testing baudrate: " 
                                                                            << std::to_string(baudrate) 
                                                                            << " With peripheral num: " 
                                                                            << std::to_string(peripheral)
                                                                            << '\n';
            
            EXPECT_EQ(MockSercom[peripheral].USART.CTRLA.reg, expected_ctrla) << "Testing baudrate: " 
                                                                            << std::to_string(baudrate) 
                                                                            << " With peripheral num: " 
                                                                            << std::to_string(peripheral)
                                                                            << '\n';
       }
    }
}

TEST(HAL_UART, EXTRA_OPT_MSB_FIRST) {
    memset(MockSercom, 0x00, sizeof(MockSercom));

    const uint32_t clock_source_freq = (48e6);
    const uint32_t baud_rate = 115200;

    uint32_t expected_ctrla = 0;

    for (uint8_t peripheral = UART_PERIPHERAL_0; peripheral < UART_PERIPHERAL_5; peripheral++) {
        uhal_uart_init((uart_peripheral_inst_t) peripheral, baud_rate, UART_CLK_SOURCE_USE_DEFAULT, clock_source_freq, UART_EXTRA_OPT_MSB_FIRST);
        expected_ctrla = (SERCOM_USART_CTRLA_ENABLE | SERCOM_USART_CTRLA_SAMPR(0) | SERCOM_USART_CTRLA_MODE(1));
        EXPECT_EQ(MockSercom[peripheral].USART.CTRLA.reg, expected_ctrla) << " With peripheral num: " 
                                                                        << std::to_string(peripheral)
                                                                        << '\n';
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);

    if (RUN_ALL_TESTS()) {}
    return 0;
}