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

option(UHAL_DISABLE_GPIO_MODULE "Disable the GPIO module" NO)
option(UHAL_DISABLE_I2C_HOST_MODULE "Disable the I2C Host module" NO)
option(UHAL_DISABLE_I2C_SLAVE_MODULE "Disable the I2C Slave module" NO)
option(UHAL_DISABLE_SPI_HOST_MODULE "Disable the SPI Host module" NO)
option(UHAL_DISABLE_SPI_SLAVE_MODULE "Disable the SPI Slave module" NO)

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

get_directory_property(PLATFORM_DEFINED COMPILE_DEFINITIONS)
if (PLATFORM_DEFINED MATCHES "^__SAMD21")
    add_library(Universal_hal 
            "hal/platform/atmelsam/irq/irq_bindings.c"
            "hal/platform/atmelsam/gpio/gpio_samd.c"
            "hal/platform/atmelsam/i2c_host/i2c.c"
            "hal/platform/atmelsam/i2c_slave/i2c_slave.c"
            "hal/platform/atmelsam/spi_host/spi_host.c"
            "hal/platform/atmelsam/spi_slave/spi_slave.c" 
            )
    target_include_directories(Universal_hal PUBLIC "hal/" "utils/" "hal/platform/atmelsam/")
    target_link_libraries(Universal_hal INTERFACE board_sdk)


elseif (PLATFORM_DEFINED MATCHES "^CPU_MKL25")
  MESSAGE(STATUS "PLATFORM: NXP MKL25")
  add_library(Universal_hal "hal/platform/nxp_kl25/gpio/gpio_kl25.c"
                            "hal/platform/nxp_kl25/gpio/gpio_irq_handler.c"
                            "hal/platform/nxp_kl25/irq/irq_bindings.c")
  target_include_directories(Universal_hal PUBLIC "hal/" "utils/" "hal/platform/nxp_kl25/")
  target_link_libraries(Universal_hal KL25Z_SDK::CMSIS)

else ()
    # You can define your OS here if desired
    MESSAGE(STATUS "PLATFORM NOT DETECTED")
endif ()