/*
 * spi_dev.c
 *
 *  Created on: 05 Apr 2017
 *      Author: Janus
 */

#include <cyg/io/at25dfxxx.h>
#include <cyg/io/spi_stm32.h>

#include <cyg/kernel/diag.h>

#include "spi_dev.h"

CYG_DEVS_SPI_CORTEXM_STM32_DEVICE(BLE_spi_dev       , 1, 0, false, 0,0,5000000,4,4,4)

CYG_DEVS_SPI_CORTEXM_STM32_DEVICE(stm32_flash_dev   , 3, 0, false, 0,0,6000000,4,4,4)

CYG_DEVS_FLASH_SPI_AT25DFXXX_DRIVER(dataflash, 0, &stm32_flash_dev.spi_bus);



