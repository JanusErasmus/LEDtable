/*
 * spi_dev.h
 *
 *  Created on: 05 Apr 2017
 *      Author: Janus
 */

#ifndef SRC_SPI_DEV_H_
#define SRC_SPI_DEV_H_
#include <cyg/io/spi.h>

extern cyg_spi_device BLE_spi_dev;
extern cyg_spi_device stm32_flash_dev;
extern cyg_spi_device fpga_spi_dev;

externC bool initFlash(void);

#endif /* SRC_SPI_DEV_H_ */
