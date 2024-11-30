#ifndef PINMUX_PLATFORM_SPECIFIC
#define PINMUX_PLATFORM_SPECIFIC
#include <stdint.h>

typedef uint16_t pinmux_func_t;

typedef enum {
    MUX_PIN_M2 = 1,
    MUX_PIN_L3 = 2,
    MUX_PIN_F2 = 3,
    MUX_PIN_D1 = 4,
    MUX_PIN_D2 = 5,
    MUX_PIN_C2 = 6,
    MUX_PIN_G1 = 7,
    MUX_PIN_G3 = 8,
    MUX_PIN_E2 = 9,
    MUX_PIN_D3 = 10,
    MUX_PIN_H3 = 11,
    MUX_PIN_G2 = 12,
    MUX_PIN_J3 = 13,
    MUX_PIN_K2 = 14,
    MUX_PIN_H2 = 15,
    MUX_PIN_J2 = 16,
    MUX_PIN_U14 = 17,
    MUX_PIN_U15 = 19,
    MUX_PIN_T3 = 20,
    MUX_PIN_U8 = 21,
    MUX_PIN_U9 = 22,
    MUX_PIN_U10 = 23,
    MUX_PIN_V13 = 24,
    MUX_PIN_K3 = 25,
    MUX_PIN_V10 = 26,
    MUX_PIN_U12 = 27,
    MUX_PIN_M3 = 28,
    MUX_PIN_V16 = 29,
    MUX_PIN_U16 = 30,
    MUX_PIN_U7 = 31,
    MUX_PIN_U6 = 32,
    MUX_PIN_V5 = 33,
    MUX_PIN_U5 = 34,
    MUX_PIN_V3 = 35,
    MUX_PIN_M1 = 36,
    MUX_PIN_L2 = 37,
    MUX_PIN_L1 = 38,
    MUX_PIN_C3 = 39,
    MUX_PIN_B3 = 40,
    MUX_PIN_C4 = 41,
    MUX_PIN_A3 = 42,
    MUX_PIN_B4 = 43,
    MUX_PIN_A4 = 44,
    MUX_PIN_C5 = 45,
    MUX_PIN_B5 = 46,
    MUX_PIN_U3 = 47,
    MUX_PIN_U4 = 48,
} mux_pin_t;

#define PIN_PINMUX_VAL(val)           (val << 4)

#define MUX_FUNC_PIN_M2_GPIO13        (pinmux_func_t)(0)
#define MUX_FUNC_PIN_M2_GPIO0         (pinmux_func_t)(1)
#define MUX_FUNC_PIN_M2_PMIC_CLKOUT   (pinmux_func_t)(2)
#define MUX_FUNC_PIN_M2_EPWM1B        (pinmux_func_t)(10)
#define MUX_FUNC_PIN_M2_EPWM2A        (pinmux_func_t)(11)

#define MUX_FUNC_PIN_L3_GPIO16        (pinmux_func_t)(0)
#define MUX_FUNC_PIN_L3_GPIO1         (pinmux_func_t)(1)
#define MUX_FUNC_PIN_L3_SYNC_OUT      (pinmux_func_t)(2)
#define MUX_FUNC_PIN_L3_DMM_MUX_IN    (pinmux_func_t)(12)
#define MUX_FUNC_PIN_L3_SPIB_CSN_1    (pinmux_func_t)(13)
#define MUX_FUNC_PIN_L3_SPIB_CSN_2    (pinmux_func_t)(14)
#define MUX_FUNC_PIN_L3_1SYNCI        (pinmux_func_t)(15)

#define MUX_FUNC_PIN_K3_GPIO26        (pinmux_func_t)(0)
#define MUX_FUNC_PIN_K3_GPIO2         (pinmux_func_t)(1)
#define MUX_FUNC_PIN_K3_OSC_CLKOUT    (pinmux_func_t)(2)
#define MUX_FUNC_PIN_K3_MSS_UART_TX   (pinmux_func_t)(7)
#define MUX_FUNC_PIN_K3_BSS_UART_TX   (pinmux_func_t)(8)
#define MUX_FUNC_PIN_K3_SYNC_OUT      (pinmux_func_t)(9)
#define MUX_FUNC_PIN_K3_PMIC_CLKOUT   (pinmux_func_t)(10)
#define MUX_FUNC_PIN_K3_CHIRP_START   (pinmux_func_t)(11)
#define MUX_FUNC_PIN_K3_CHIRP_END     (pinmux_func_t)(12)
#define MUX_FUNC_PIN_K3_FRAME_START   (pinmux_func_t)(13)

#define MUX_FUNC_PIN_U7_TRACE_DATA_0  (pinmux_func_t)(0)
#define MUX_FUNC_PIN_U7_GPIO31        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_U7_DMM0          (pinmux_func_t)(2)
#define MUX_FUNC_PIN_U7_MSS_UART_TX   (pinmux_func_t)(4)

#define MUX_FUNC_PIN_U6_TRACE_DATA_1  (pinmux_func_t)(0)
#define MUX_FUNC_PIN_U6_GPIO32        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_U6_DMM1          (pinmux_func_t)(2)

#define MUX_FUNC_PIN_V5_TRACE_DATA_2  (pinmux_func_t)(0)
#define MUX_FUNC_PIN_V5_GPIO33        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_V5_DMM2          (pinmux_func_t)(2)

#define MUX_FUNC_PIN_U5_TRACE_DATA_3  (pinmux_func_t)(0)
#define MUX_FUNC_PIN_U5_GPIO34        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_U5_DMM3          (pinmux_func_t)(2)
#define MUX_FUNC_PIN_U5_EPWM3SYNC0    (pinmux_func_t)(4)

#define MUX_FUNC_PIN_V3_TRACE_DATA_4  (pinmux_func_t)(0)
#define MUX_FUNC_PIN_V3_GPIO35        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_V3_DMM4          (pinmux_func_t)(2)
#define MUX_FUNC_PIN_V3_EPWM2SYNC0    (pinmux_func_t)(4)

#define MUX_FUNC_PIN_M1_TRACE_DATA_5  (pinmux_func_t)(0)
#define MUX_FUNC_PIN_M1_GPIO36        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_M1_DMM5          (pinmux_func_t)(2)
#define MUX_FUNC_PIN_M1_MSS_UART_TX   (pinmux_func_t)(5)

#define MUX_FUNC_PIN_L2_TRACE_DATA_6  (pinmux_func_t)(0)
#define MUX_FUNC_PIN_L2_GPIO37        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_L2_DMM6          (pinmux_func_t)(2)
#define MUX_FUNC_PIN_L2_BSS_UART_TX   (pinmux_func_t)(5)

#define MUX_FUNC_PIN_L1_TRACE_DATA_7  (pinmux_func_t)(0)
#define MUX_FUNC_PIN_L1_GPIO38        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_L1_DMM7          (pinmux_func_t)(2)
#define MUX_FUNC_PIN_L1_DSS_UART_TX   (pinmux_func_t)(5)

#define MUX_FUNC_PIN_C3_TRACE_DATA_8  (pinmux_func_t)(0)
#define MUX_FUNC_PIN_C3_GPIO39        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_C3_DMM8          (pinmux_func_t)(2)
#define MUX_FUNC_PIN_C3_CAN1_FD_TX    (pinmux_func_t)(4)
#define MUX_FUNC_PIN_C3_EPWM15SYNC1   (pinmux_func_t)(5)

#define MUX_FUNC_PIN_B3_TRACE_DATA_9  (pinmux_func_t)(0)
#define MUX_FUNC_PIN_B3_GPIO40        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_B3_DMM9          (pinmux_func_t)(2)
#define MUX_FUNC_PIN_B3_CAN1_FD_RX    (pinmux_func_t)(4)
#define MUX_FUNC_PIN_B3_EPWM15SYNC0   (pinmux_func_t)(5)

#define MUX_FUNC_PIN_C4_TRACE_DATA_10 (pinmux_func_t)(0)
#define MUX_FUNC_PIN_C4_GPIO41        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_C4_DMM10         (pinmux_func_t)(2)
#define MUX_FUNC_PIN_C4_EPWM3A        (pinmux_func_t)(4)

#define MUX_FUNC_PIN_A3_TRACE_DATA_11 (pinmux_func_t)(0)
#define MUX_FUNC_PIN_A3_GPIO42        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_A3_DMM11         (pinmux_func_t)(2)
#define MUX_FUNC_PIN_A3_EPWM3B        (pinmux_func_t)(4)

#define MUX_FUNC_PIN_B4_TRACE_DATA_12 (pinmux_func_t)(0)
#define MUX_FUNC_PIN_B4_GPIO43        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_B4_DMM12         (pinmux_func_t)(2)
#define MUX_FUNC_PIN_B4_EPWM1A        (pinmux_func_t)(4)
#define MUX_FUNC_PIN_B4_CAN2_FD_TX    (pinmux_func_t)(5)

#define MUX_FUNC_PIN_A4_TRACE_DATA_13 (pinmux_func_t)(0)
#define MUX_FUNC_PIN_A4_GPIO44        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_A4_DMM13         (pinmux_func_t)(2)
#define MUX_FUNC_PIN_A4_EPWM1B        (pinmux_func_t)(4)
#define MUX_FUNC_PIN_A4_CAN1_FD_RX    (pinmux_func_t)(5)

#define MUX_FUNC_PIN_C5_TRACE_DATA_14 (pinmux_func_t)(0)
#define MUX_FUNC_PIN_C5_GPIO45        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_C5_DMM14         (pinmux_func_t)(2)
#define MUX_FUNC_PIN_C5_EPWM2A        (pinmux_func_t)(4)

#define MUX_FUNC_PIN_B5_TRACE_DATA_15 (pinmux_func_t)(0)
#define MUX_FUNC_PIN_B5_GPIO46        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_B5_DMM15         (pinmux_func_t)(2)
#define MUX_FUNC_PIN_B5_EPWM2B        (pinmux_func_t)(4)

#define MUX_FUNC_PIN_U3_TRACE_CLK     (pinmux_func_t)(0)
#define MUX_FUNC_PIN_U3_GPIO47        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_U3_DMM_CLK       (pinmux_func_t)(2)

#define MUX_FUNC_PIN_U4_TRACE_CTL     (pinmux_func_t)(0)
#define MUX_FUNC_PIN_U4_DMM_SYNC      (pinmux_func_t)(2)

#define MUX_FUNC_PIN_V13_GPIO25       (pinmux_func_t)(0)
#define MUX_FUNC_PIN_V13_MCU_CLKOUT   (pinmux_func_t)(1)
#define MUX_FUNC_PIN_V13_CHIRP_START  (pinmux_func_t)(2)
#define MUX_FUNC_PIN_V13_CHIRP_END    (pinmux_func_t)(6)
#define MUX_FUNC_PIN_V13_FRAME_START  (pinmux_func_t)(7)
#define MUX_FUNC_PIN_V13_EPWM1A       (pinmux_func_t)(12)

#define MUX_FUNC_PIN_U14_NERROR_IN    (pinmux_func_t)(0)

#define MUX_FUNC_PIN_U15_NERROR_OUT   (pinmux_func_t)(0)

#define MUX_FUNC_PIN_V10_GPIO27       (pinmux_func_t)(0)
#define MUX_FUNC_PIN_V10_PMIC_CLKOUT  (pinmux_func_t)(1)
#define MUX_FUNC_PIN_V10_CHIRP_START  (pinmux_func_t)(6)
#define MUX_FUNC_PIN_V10_CHIRP_END    (pinmux_func_t)(7)
#define MUX_FUNC_PIN_V10_FRAME_START  (pinmux_func_t)(8)
#define MUX_FUNC_PIN_V10_EPWM1B       (pinmux_func_t)(11)
#define MUX_FUNC_PIN_V10_EPWM2A       (pinmux_func_t)(12)

#define MUX_FUNC_PIN_H3_GPIO8         (pinmux_func_t)(0)
#define MUX_FUNC_PIN_H3_QSPI0         (pinmux_func_t)(1)
#define MUX_FUNC_PIN_H3_SPIB_MISO     (pinmux_func_t)(2)

#define MUX_FUNC_PIN_G2_GPIO9         (pinmux_func_t)(0)
#define MUX_FUNC_PIN_G2_QSPI1         (pinmux_func_t)(1)
#define MUX_FUNC_PIN_G2_SPIB_MOSI     (pinmux_func_t)(2)
#define MUX_FUNC_PIN_G2_SPIB_CS_N_2   (pinmux_func_t)(8)

#define MUX_FUNC_PIN_J3_GPIO10        (pinmux_func_t)(0)
#define MUX_FUNC_PIN_J3_QSPI2         (pinmux_func_t)(1)
#define MUX_FUNC_PIN_J3_CAN1_FD_TX    (pinmux_func_t)(8)

#define MUX_FUNC_PIN_K2_GPIO11        (pinmux_func_t)(0)
#define MUX_FUNC_PIN_K2_QSPI3         (pinmux_func_t)(1)
#define MUX_FUNC_PIN_K2_CAN1_FD_RX    (pinmux_func_t)(8)

#define MUX_FUNC_PIN_H2_GPIO7         (pinmux_func_t)(0)
#define MUX_FUNC_PIN_H2_QSPI_CLK      (pinmux_func_t)(1)
#define MUX_FUNC_PIN_H2_SPIB_CLK      (pinmux_func_t)(2)
#define MUX_FUNC_PIN_H2_DSS_UART_TX   (pinmux_func_t)(6)

#define MUX_FUNC_PIN_J2_GPIO6         (pinmux_func_t)(0)
#define MUX_FUNC_PIN_J2_QSPI_CS_N     (pinmux_func_t)(1)
#define MUX_FUNC_PIN_J2_SPIB_CS_N     (pinmux_func_t)(2)

#define MUX_FUNC_PIN_V16_GPIO15       (pinmux_func_t)(0)
#define MUX_FUNC_PIN_V16_RS232_RX     (pinmux_func_t)(1)
#define MUX_FUNC_PIN_V16_MSS_UART_RX  (pinmux_func_t)(2)
#define MUX_FUNC_PIN_V16_BSS_UART_TX  (pinmux_func_t)(6)
#define MUX_FUNC_PIN_V16_MSS_UARTB_RX (pinmux_func_t)(7)
#define MUX_FUNC_PIN_V16_CAN1_FD_RX   (pinmux_func_t)(8)
#define MUX_FUNC_PIN_V16_I2C_SCL      (pinmux_func_t)(9)
#define MUX_FUNC_PIN_V16_EPWM2A       (pinmux_func_t)(10)
#define MUX_FUNC_PIN_V16_EPWM2B       (pinmux_func_t)(11)
#define MUX_FUNC_PIN_V16_EPWM3A       (pinmux_func_t)(12)

#define MUX_FUNC_PIN_U16_GPIO14       (pinmux_func_t)(0)
#define MUX_FUNC_PIN_U16_RS232_TX     (pinmux_func_t)(1)
#define MUX_FUNC_PIN_U16_MSS_UART_TX  (pinmux_func_t)(2)
#define MUX_FUNC_PIN_U16_BSS_UART_TX  (pinmux_func_t)(6)
#define MUX_FUNC_PIN_U16_CAN1_FD_TX   (pinmux_func_t)(10)
#define MUX_FUNC_PIN_U16_I2C_SDA      (pinmux_func_t)(11)
#define MUX_FUNC_PIN_U16_EPWM1A       (pinmux_func_t)(12)
#define MUX_FUNC_PIN_U16_EPWM1B       (pinmux_func_t)(13)
#define MUX_FUNC_PIN_U16_NDMM_EN      (pinmux_func_t)(14)
#define MUX_FUNC_PIN_U16_EPWM2A       (pinmux_func_t)(15)

#define MUX_FUNC_PIN_D2_GPIO3         (pinmux_func_t)(0)
#define MUX_FUNC_PIN_D2_SPIA_CLK      (pinmux_func_t)(1)
#define MUX_FUNC_PIN_D2_CAN2_FD_RX    (pinmux_func_t)(6)
#define MUX_FUNC_PIN_D2_DSS_UART_TX   (pinmux_func_t)(7)

#define MUX_FUNC_PIN_C2_GPIO30        (pinmux_func_t)(0)
#define MUX_FUNC_PIN_C2_SPIA_CS_N     (pinmux_func_t)(1)
#define MUX_FUNC_PIN_C2_CAN1_FD_TX    (pinmux_func_t)(6)

#define MUX_FUNC_PIN_D1_GPIO20        (pinmux_func_t)(0)
#define MUX_FUNC_PIN_D1_SPIA_MISO     (pinmux_func_t)(1)
#define MUX_FUNC_PIN_D1_CAN1_FD_TX    (pinmux_func_t)(6)

#define MUX_FUNC_PIN_F2_GPIO19        (pinmux_func_t)(0)
#define MUX_FUNC_PIN_F2_SPIA_MOSI     (pinmux_func_t)(1)
#define MUX_FUNC_PIN_F2_CAN1_FD_RX    (pinmux_func_t)(2)
#define MUX_FUNC_PIN_F2_DSS_UART_TX   (pinmux_func_t)(8)

#define MUX_FUNC_PIN_E2_GPIO5         (pinmux_func_t)(0)
#define MUX_FUNC_PIN_E2_SPIB_CLK      (pinmux_func_t)(1)
#define MUX_FUNC_PIN_E2_MSS_UART_RX   (pinmux_func_t)(2)
#define MUX_FUNC_PIN_E2_MSS_UART_TX   (pinmux_func_t)(6)
#define MUX_FUNC_PIN_E2_BSS_UART_TX   (pinmux_func_t)(7)
#define MUX_FUNC_PIN_E2_CAN1_FD_RX    (pinmux_func_t)(8)

#define MUX_FUNC_PIN_D3_GPIO4         (pinmux_func_t)(0)
#define MUX_FUNC_PIN_D3_SPIB_CS_N     (pinmux_func_t)(1)
#define MUX_FUNC_PIN_D3_MSS_UART_TX   (pinmux_func_t)(2)
#define MUX_FUNC_PIN_D3_BSS_UART_TX   (pinmux_func_t)(7)
#define MUX_FUNC_PIN_D3_QSPI_CLK_EXT  (pinmux_func_t)(8)
#define MUX_FUNC_PIN_D3_CAN1_FD_TX    (pinmux_func_t)(9)

#define MUX_FUNC_PIN_G3_GPIO22        (pinmux_func_t)(0)
#define MUX_FUNC_PIN_G3_SPIB_MISO     (pinmux_func_t)(1)
#define MUX_FUNC_PIN_G3_I2C_SCL       (pinmux_func_t)(2)
#define MUX_FUNC_PIN_G3_DSS_UART_TX   (pinmux_func_t)(6)

#define MUX_FUNC_PIN_G1_GPIO21        (pinmux_func_t)(0)
#define MUX_FUNC_PIN_G1_SPIB_MOSI     (pinmux_func_t)(1)
#define MUX_FUNC_PIN_G1_I2C_SDA       (pinmux_func_t)(2)

#define MUX_FUNC_PIN_B2_GPIO12        (pinmux_func_t)(0)
#define MUX_FUNC_PIN_B2_SPI_HOST_INTR (pinmux_func_t)(1)
#define MUX_FUNC_PIN_B2_SPIB_CS_N_1   (pinmux_func_t)(6)

#define MUX_FUNC_PIN_U12_GPIO28       (pinmux_func_t)(0)
#define MUX_FUNC_PIN_U12_SYNC_IN      (pinmux_func_t)(1)
#define MUX_FUNC_PIN_U12_MSS_UARTB_RX (pinmux_func_t)(6)
#define MUX_FUNC_PIN_U12_DMM_MUX_IN   (pinmux_func_t)(7)
#define MUX_FUNC_PIN_U12_SYNC_OUT     (pinmux_func_t)(9)

#define MUX_FUNC_PIN_M3_GPIO29        (pinmux_func_t)(1)
#define MUX_FUNC_PIN_M3_SOP1          (pinmux_func_t)(0)
#define MUX_FUNC_PIN_M3_SYNC_OUT      (pinmux_func_t)(9)
#define MUX_FUNC_PIN_M3_DMM_MUX_IN    (pinmux_func_t)(10)
#define MUX_FUNC_PIN_M3_SPIB_CS_N_1   (pinmux_func_t)(11)
#define MUX_FUNC_PIN_M3_SPIB_CS_N_2   (pinmux_func_t)(11)

#define MUX_FUNC_PIN_T3_GPIO17        (pinmux_func_t)(0)
#define MUX_FUNC_PIN_T3_TCK           (pinmux_func_t)(1)
#define MUX_FUNC_PIN_T3_MSS_UARTB_TX  (pinmux_func_t)(2)
#define MUX_FUNC_PIN_T3_CAN1_FD_TX    (pinmux_func_t)(8)

#define MUX_FUNC_PIN_U9_GPIO23        (pinmux_func_t)(0)
#define MUX_FUNC_PIN_U9_TDI           (pinmux_func_t)(1)
#define MUX_FUNC_PIN_U9_MSS_UARTA_RX  (pinmux_func_t)(2)

#define MUX_FUNC_PIN_U10_GPIO24       (pinmux_func_t)(1)
#define MUX_FUNC_PIN_U10_SOP0         (pinmux_func_t)(0)
#define MUX_FUNC_PIN_U10_TDO          (pinmux_func_t)(2)
#define MUX_FUNC_PIN_U10_MSS_UARTA_TX (pinmux_func_t)(3)
#define MUX_FUNC_PIN_U10_MSS_UARTB_TX (pinmux_func_t)(4)
#define MUX_FUNC_PIN_U10_BSS_UART_TX  (pinmux_func_t)(6)
#define MUX_FUNC_PIN_U10_NDMM_EN      (pinmux_func_t)(7)

#define MUX_FUNC_PIN_U8_GPIO18        (pinmux_func_t)(0)
#define MUX_FUNC_PIN_U8_TMS           (pinmux_func_t)(1)
#define MUX_FUNC_PIN_U8_BSS_UART_TX   (pinmux_func_t)(2)
#define MUX_FUNC_PIN_U8_CAN1_FD_RX    (pinmux_func_t)(6)

#endif /* PINMUX_PLATFORM_SPECIFIC_H */
