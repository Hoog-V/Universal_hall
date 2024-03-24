/**
* \file            spi.c
* \brief           Source file which implements the standard SPI API functions
*/
/*
*  Copyright 2023 (C) Victor Hogeweij <hogeweyv@gmail.com>
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
* This file is part of the Universal Hal Framework.
*
* Author:          Victor Hogeweij <hogeweyv@gmail.com>
*/

#include <hal_spi_host.h>
#include <stddef.h>

void spi_host_init(const spi_dev_t *spi_instance, unsigned long baud_rate) {

}

void spi_host_deinit(const spi_dev_t *spi_instance) {

}

void spi_write_blocking(const spi_dev_t *spi_instance, const unsigned char *write_buff, size_t size) {

}

void spi_write_non_blocking(const spi_dev_t *spi_instance, const unsigned char *write_buff, size_t size) {

}

void spi_read_blocking(const spi_dev_t *spi_instance, unsigned char *read_buff, size_t amount_of_bytes) {

}

void spi_read_non_blocking(const spi_dev_t *spi_instance, unsigned char *read_buff, size_t amount_of_bytes) {

}