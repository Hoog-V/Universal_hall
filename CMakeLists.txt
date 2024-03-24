#[[
  Copyright 2023 (C) Victor Hogeweij <hogeweyv@gmail.com>

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
]]
cmake_minimum_required(VERSION 3.27)
project(Universal_hal)

option(UHAL_BUILD_TESTS "Enable building tests" NO)

set(UHAL_FILEPATH ${CMAKE_CURRENT_SOURCE_DIR})

get_directory_property(PLATFORM_DEFINED COMPILE_DEFINITIONS)
if (PLATFORM_DEFINED MATCHES "^__SAMD21")
    option(UHAL_DISABLE_GPIO_MODULE "Disable the GPIO module" NO)
    option(UHAL_DISABLE_I2C_HOST_MODULE "Disable the I2C Host module" NO)
    option(UHAL_DISABLE_I2C_SLAVE_MODULE "Disable the I2C Slave module" NO)
    option(UHAL_DISABLE_SPI_HOST_MODULE "Disable the SPI Host module" NO)
    option(UHAL_DISABLE_SPI_SLAVE_MODULE "Disable the SPI Slave module" NO)
    option(UHAL_ENABLE_USB_SERIAL_MODULE "Enable the USB serial module" NO)

    add_library(Universal_hal 
            "hal/platform/atmelsam/irq/irq_bindings.c"
            "hal/platform/atmelsam/gpio/gpio_samd.c"
            "hal/platform/atmelsam/i2c_host/i2c.c"
            "hal/platform/atmelsam/i2c_slave/i2c_slave.c"
            "hal/platform/atmelsam/spi_host/spi_host.c"
            "hal/platform/atmelsam/spi_slave/spi_slave.c"
            "hal/platform/atmelsam/dma/dma.c"
            "hal/platform/atmelsam/uart/uart.c"
            )
    target_include_directories(Universal_hal PUBLIC "hal/" "utils/" "hal/platform/atmelsam/")

    if(UHAL_DISABLE_GPIO_MODULE)
    add_compile_definitions("DISABLE_GPIO_MODULE")
    endif()

    if(UHAL_DISABLE_I2C_HOST_MODULE)
    add_compile_definitions("DISABLE_I2C_HOST_MODULE")
    endif()

    if(UHAL_DISABLE_I2C_SLAVE_MODULE)
    add_compile_definitions("DISABLE_I2C_SLAVE_MODULE")
    endif()

    if(UHAL_DISABLE_SPI_HOST_MODULE)
    add_compile_definitions("DISABLE_SPI_HOST_MODULE")
    endif()

    if(UHAL_DISABLE_SPI_SLAVE_MODULE)
    add_compile_definitions("DISABLE_SPI_SLAVE_MODULE")
    endif()

    if(UHAL_ENABLE_USB_SERIAL_MODULE)
    add_library(Universal_hal_usb STATIC "${CMAKE_CURRENT_LIST_DIR}/middleware/usb/platform/atmelsam/usb_serial_samd.c"
                                         "${CMAKE_CURRENT_LIST_DIR}/middleware/usb/lib/src/portable/microchip/samd/dcd_samd.c")
    target_include_directories(Universal_hal_usb PUBLIC "${CMAKE_CURRENT_LIST_DIR}/middleware/usb/"
                                                        "${CMAKE_CURRENT_LIST_DIR}/middleware/usb/platform/atmelsam/")

    target_link_libraries(Universal_hal_usb PRIVATE Universal_hal)

    add_library(tinyusb_config INTERFACE)

    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/middleware/usb/lib/src)    

    add_library(Universal_hal_usb_conf INTERFACE )

    target_link_libraries(tinyusb_config INTERFACE Universal_hal_usb_conf)
    target_link_libraries(Universal_hal_usb PUBLIC Universal_hal_usb_conf)
    target_link_libraries(Universal_hal_usb_conf INTERFACE tinyusb)
    target_compile_definitions(Universal_hal_usb_conf INTERFACE "-DCFG_TUSB_MCU=OPT_MCU_SAMD21")
    target_include_directories(Universal_hal_usb_conf INTERFACE "middleware/usb/lib/hw/bsp/")
  endif()
elseif(PLATFORM_DEFINED MATCHES "^PICO")
  set(UHAL_BUILD_TESTS NO)
  MESSAGE(STATUS "PICO Platform detected!")
  add_library(Universal_hal  "hal/platform/raspberrypi/gpio/gpio_raspberrypi.c"
                             "hal/platform/raspberrypi/spi_host/spi.c"
                             "hal/platform/raspberrypi/i2c_host/i2c.c")
  target_include_directories(Universal_hal PUBLIC "hal/" "hal/platform/raspberrypi")
  target_link_libraries(Universal_hal pico_stdlib hardware_spi)
else ()
    # You can define your OS here if desired
    MESSAGE(STATUS "No MCU platform selected!")
endif ()

if(UHAL_BUILD_TESTS)
MESSAGE(STATUS "Building tests!")
enable_testing()
add_subdirectory(test/atmelsam)
endif()
