include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.x
)

# CMake 3.14+
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    fff
    GIT_REPOSITORY https://github.com/meekrosoft/fff.git
    GIT_TAG master
)

FetchContent_MakeAvailable(fff)

add_library(board_sdk_ti_iwr68xx INTERFACE "mocks/AWR6843_GIO.h"
                                     "mocks/AWR6843_IOMUX.h"
                                     "mocks/AWR6843_types.h"
                                     "mocks/AWR6843.h")

target_include_directories(board_sdk_ti_iwr68xx INTERFACE mocks/)


add_library(universal_hal_gpio_ti_iwr68xx ${UHAL_FILEPATH}/hal/platform/ti/iwr68xx/gpio/gpio_iwr68xx.c
                                          ${UHAL_FILEPATH}/hal/platform/ti/iwr68xx/pinmux/pinmux_iwr68xx.c)

target_compile_definitions(universal_hal_gpio_ti_iwr68xx PUBLIC "DISABLE_I2C_HOST_MODULE"
                                         "DISABLE_I2C_SLAVE_MODULE"
                                         "DISABLE_SPI_HOST_MODULE"
                                         "DISABLE_SPI_SLAVE_MODULE"
                                         "DISABLE_DMA_MODULE"
                                         "DISABLE_DMA_HANDLER"
                                         "DISABLE_UART_MODULE")

target_include_directories(universal_hal_gpio_ti_iwr68xx PUBLIC ${UHAL_FILEPATH}/hal/platform/ti/iwr68xx/
                                                                ${UHAL_FILEPATH}/hal/)

target_link_libraries(universal_hal_gpio_ti_iwr68xx board_sdk_ti_iwr68xx)



add_executable(TI_IWR68XX_GPIO_TEST gpio/gpio.cpp)
target_link_libraries(TI_IWR68XX_GPIO_TEST universal_hal_gpio_ti_iwr68xx board_sdk_ti_iwr68xx gtest_main gmock_main)
# target_compile_definitions(TI_IWR68XX_GPIO_TEST PUBLIC "FFF_GENERATE")

add_executable(TI_IWR68XX_PINMUX_TEST pinmux/pinmux.cpp)
target_link_libraries(TI_IWR68XX_PINMUX_TEST universal_hal_gpio_ti_iwr68xx board_sdk_ti_iwr68xx gtest_main gmock_main)

add_test(
        NAME TI_IWR68XX_GPIO_TEST
        COMMAND TI_IWR68XX_GPIO_TEST
)

add_test(
        NAME TI_IWR68XX_PINMUX_TEST
        COMMAND TI_IWR68XX_PINMUX_TEST
)
