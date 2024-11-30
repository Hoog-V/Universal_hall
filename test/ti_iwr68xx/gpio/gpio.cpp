#include <bit_manipulation.h>
#include <cstring>
#include "AWR6843.h"
#include "gmock/gmock.h"
#include "hal_gpio.h"

GIO_Type    Mock_GIO = {};
IOMUX_Type  Mock_IOMUX = {};
IOMUX_Type* IOMUX = &Mock_IOMUX;
GIO_Type*   GIO = &Mock_GIO;

TEST(ti_iwr68xx_gpio, toggle_pin) {
    /* Reset the whole GIO struct */
    memset((void*)GIO, 0x00, sizeof(Mock_GIO));
    uint32_t res = 0;
    res = GIO->GIOPORT[0].GIODCLR.reg;
    /* It is expected to be empty, just to be sure */
    EXPECT_EQ(res, 0);
    /* Toggle the pin, gpio dclr should now be set */
    gpio_toggle_pin_output(GPIO_PIN_5);
    /* Check that GIODSET is set */
    res = GIO->GIOPORT[0].GIODSET.reg;
    uint32_t expected = (1 << 5);
    EXPECT_EQ(res, expected);
    /* Check that GIODCLR is not set */
    res = GIO->GIOPORT[0].GIODCLR.reg;
    expected = 0;
    EXPECT_EQ(res, expected);
    GIO->GIOPORT[0].GIODSET.reg = 0;

    /* Set GPIO 5 so that toggle should now set GIODCLR */
    GIO->GIOPORT[0].GIODIN.bit.DIN = (1 << 5);
    gpio_toggle_pin_output(GPIO_PIN_5);
    /* As the pin was manually turned on, it should now set GIODCLR */
    res = GIO->GIOPORT[0].GIODCLR.reg;
    expected = (1 << 5);
    EXPECT_EQ(res, expected);

    /* Check that GIODSET is set */
    res = GIO->GIOPORT[0].GIODSET.reg;
    expected = 0;
    EXPECT_EQ(res, expected);
}

TEST(ti_iwr68xx_gpio, toggle_pin_multiple) {
    /* Reset the whole GIO struct */
    memset((void*)GIO, 0x00, sizeof(Mock_GIO));
    gpio_pin_t test_pins[] = {
        GPIO_PIN_0,  GPIO_PIN_1,  GPIO_PIN_2,  GPIO_PIN_3,  GPIO_PIN_4,  GPIO_PIN_5,  GPIO_PIN_6,  GPIO_PIN_7,  GPIO_PIN_8,  GPIO_PIN_9,
        GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_16, GPIO_PIN_17, GPIO_PIN_18, GPIO_PIN_19,
        GPIO_PIN_20, GPIO_PIN_21, GPIO_PIN_22, GPIO_PIN_23, GPIO_PIN_24, GPIO_PIN_25, GPIO_PIN_26, GPIO_PIN_27, GPIO_PIN_28, GPIO_PIN_30,
        GPIO_PIN_31, GPIO_PIN_32, GPIO_PIN_33, GPIO_PIN_34, GPIO_PIN_35, GPIO_PIN_36, GPIO_PIN_37, GPIO_PIN_38, GPIO_PIN_39, GPIO_PIN_40,
        GPIO_PIN_41, GPIO_PIN_42, GPIO_PIN_43, GPIO_PIN_44, GPIO_PIN_45, GPIO_PIN_46, GPIO_PIN_47,
    };

    for (gpio_pin_t pin : test_pins) {
        uint32_t pin_val = GET_UPPER_4_BITS_OF_BYTE(pin);
        uint32_t port_val = GET_LOWER_4_BITS_OF_BYTE(pin);

        /* It is expected to be not set, just to be sure */
        uint32_t res = GIO->GIOPORT[port_val].GIODCLR.bit.DCLR;
        EXPECT_EQ(0, BIT_IS_SET(res, pin_val));

        /* Toggle the pin, gpio dclr should now be set */
        gpio_toggle_pin_output(pin);

        /* Check that GIODSET is set */
        res = GIO->GIOPORT[port_val].GIODSET.bit.DSET;
        uint32_t expected = (1 << pin_val);
        EXPECT_EQ(res, expected);

        /* Check that GIODCLR is not set */
        expected = 0;
        res = GIO->GIOPORT[port_val].GIODCLR.bit.DCLR;
        EXPECT_EQ(expected, BIT_IS_SET(res, pin_val));

        /* Set GPIO 5 so that toggle should now set GIODCLR */
        GIO->GIOPORT[port_val].GIODIN.bit.DIN = (1 << pin_val);
        /* Clear the GIODSET register as it allows to test for unsudden changes*/
        GIO->GIOPORT[port_val].GIODSET.bit.DSET = 0;

        /* Toggle the pin, gpio dset should now be set */
        gpio_toggle_pin_output(pin);

        /* As the pin was manually turned on, it should now set GIODCLR */
        res = GIO->GIOPORT[port_val].GIODCLR.reg;
        expected = 1;
        EXPECT_EQ(expected, BIT_IS_SET(res, pin_val));

        /* Check that GIODSET is set */
        res = GIO->GIOPORT[port_val].GIODSET.reg;
        expected = 0;
        EXPECT_EQ(expected, BIT_IS_SET(res, pin_val));
    }
}

TEST(ti_iwr68xx_gpio, set_pin_mode) {
    /* Reset the whole GIO struct */
    memset((void*)GIO, 0x00, sizeof(Mock_GIO));
    uint32_t res = 0;
    res = GIO->GIOPORT[0].GIODIR.reg;
    /* It is expected to be empty, just to be sure */
    EXPECT_EQ(res, 0);
    /* Toggle the pin, giodir register should now be set */
    gpio_set_pin_mode(GPIO_PIN_5, GPIO_MODE_OUTPUT);
    /* Check that GIODSET is set */
    res = GIO->GIOPORT[0].GIODIR.reg;
    uint32_t expected = (1 << 5);
    EXPECT_EQ(res, expected);
    /* Toggle the pin, giodir register should now be cleared */
    gpio_set_pin_mode(GPIO_PIN_5, GPIO_MODE_INPUT);

    /* 
    * As the pin was previously set as output. 
    * It should now be set as input thus 0 
    */
    res = GIO->GIOPORT[0].GIODIR.reg;
    expected = 0;
    EXPECT_EQ(res, expected);
}

TEST(ti_iwr68xx_gpio, set_pin_mode_multiple) {
    /* Reset the whole GIO struct */
    memset((void*)GIO, 0x00, sizeof(Mock_GIO));
    gpio_pin_t test_pins[] = {
        GPIO_PIN_0,  GPIO_PIN_1,  GPIO_PIN_2,  GPIO_PIN_3,  GPIO_PIN_4,  GPIO_PIN_5,  GPIO_PIN_6,  GPIO_PIN_7,  GPIO_PIN_8,  GPIO_PIN_9,
        GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_16, GPIO_PIN_17, GPIO_PIN_18, GPIO_PIN_19,
        GPIO_PIN_20, GPIO_PIN_21, GPIO_PIN_22, GPIO_PIN_23, GPIO_PIN_24, GPIO_PIN_25, GPIO_PIN_26, GPIO_PIN_27, GPIO_PIN_28, GPIO_PIN_30,
        GPIO_PIN_31, GPIO_PIN_32, GPIO_PIN_33, GPIO_PIN_34, GPIO_PIN_35, GPIO_PIN_36, GPIO_PIN_37, GPIO_PIN_38, GPIO_PIN_39, GPIO_PIN_40,
        GPIO_PIN_41, GPIO_PIN_42, GPIO_PIN_43, GPIO_PIN_44, GPIO_PIN_45, GPIO_PIN_46, GPIO_PIN_47,
    };
    for (gpio_pin_t pin : test_pins) {
        uint32_t pin_val = GET_UPPER_4_BITS_OF_BYTE(pin);
        uint32_t port_val = GET_LOWER_4_BITS_OF_BYTE(pin);

        uint32_t res = 0;
        res = GIO->GIOPORT[port_val].GIODIR.reg;
        /* It is expected to be empty, just to be sure */
        EXPECT_EQ(res, 0);
        /* Toggle the pin, giodir register should now be set */
        gpio_set_pin_mode(pin, GPIO_MODE_OUTPUT);
        /* Check that GIODSET is set */
        res = GIO->GIOPORT[port_val].GIODIR.reg;
        uint32_t expected = (1 << pin_val);
        EXPECT_EQ(res, expected);
        /* Toggle the pin, giodir register should now be cleared */
        gpio_set_pin_mode(pin, GPIO_MODE_INPUT);

        /* 
        * As the pin was previously set as output. 
        * It should now be set as input thus 0 
        */
        res = GIO->GIOPORT[port_val].GIODIR.reg;
        expected = 0;
        EXPECT_EQ(res, expected);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);

    if (RUN_ALL_TESTS()) {}
    return 0;
}