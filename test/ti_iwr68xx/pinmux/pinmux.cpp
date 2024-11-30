#include <bit_manipulation.h>
#include <cstring>
#include "AWR6843.h"
#include "gmock/gmock.h"
#include "hal_pinmux.h"

GIO_Type    Mock_GIO = {};
IOMUX_Type  Mock_IOMUX = {};
IOMUX_Type* IOMUX = &Mock_IOMUX;
GIO_Type*   GIO = &Mock_GIO;

TEST(ti_iwr68xx_pinmux, set_pin_func) {
    /* Reset the whole IOMUX struct */
    memset((void*)IOMUX, 0x00, sizeof(Mock_IOMUX));
    /* Initialize and reset both expected and res variable */
    uint32_t res = 0;
    uint32_t expected = 0;

    /* Test 1: Pin mux func 1 with pin L1 */
    pinmux_set_pin_function(MUX_PIN_L1, MUX_FUNC_PIN_L1_GPIO38);
    res = IOMUX->PADxx_CFG_REG[MUX_PIN_L1].bit.FUNC_SEL;
    expected = MUX_FUNC_PIN_L1_GPIO38;
    EXPECT_EQ(expected, res);
    
    /* Test 2: Pin mux func 0 with pin L1 */
    pinmux_set_pin_function(MUX_PIN_L1, MUX_FUNC_PIN_L1_TRACE_DATA_7);
    res = IOMUX->PADxx_CFG_REG[MUX_PIN_L1].bit.FUNC_SEL;
    expected = MUX_FUNC_PIN_L1_TRACE_DATA_7;
    EXPECT_EQ(expected, res);
    
    /* Test 3: Pin mux func 5 with pin L1 */
    pinmux_set_pin_function(MUX_PIN_L1, MUX_FUNC_PIN_L1_DSS_UART_TX);
    res = IOMUX->PADxx_CFG_REG[MUX_PIN_L1].bit.FUNC_SEL;
    expected = MUX_FUNC_PIN_L1_DSS_UART_TX;
    EXPECT_EQ(expected, res);
    
    /* Test 4: Pin mux func 2 with pin L1 */
    pinmux_set_pin_function(MUX_PIN_L1, MUX_FUNC_PIN_L1_DMM7);
    res = IOMUX->PADxx_CFG_REG[MUX_PIN_L1].bit.FUNC_SEL;
    expected = MUX_FUNC_PIN_L1_DMM7;
    EXPECT_EQ(expected, res);

    /* Test 5: Test another pin with a big bunch of functions */
    pinmux_func_t functions_to_test[] = {MUX_FUNC_PIN_U16_GPIO14, MUX_FUNC_PIN_U16_RS232_TX, 
                                        MUX_FUNC_PIN_U16_MSS_UART_TX, MUX_FUNC_PIN_U16_BSS_UART_TX,
                                        MUX_FUNC_PIN_U16_CAN1_FD_TX, MUX_FUNC_PIN_U16_I2C_SDA,
                                        MUX_FUNC_PIN_U16_EPWM1A, MUX_FUNC_PIN_U16_EPWM1B,
                                        MUX_FUNC_PIN_U16_NDMM_EN, MUX_FUNC_PIN_U16_EPWM2A};

    for (pinmux_func_t function : functions_to_test) {
        pinmux_set_pin_function(MUX_PIN_U16, function);
        res = IOMUX->PADxx_CFG_REG[MUX_PIN_U16].bit.FUNC_SEL;
        expected = function;
        EXPECT_EQ(expected, res);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);

    if (RUN_ALL_TESTS()) {}
    return 0;
}