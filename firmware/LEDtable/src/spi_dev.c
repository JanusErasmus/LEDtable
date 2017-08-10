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

CYG_DEVS_SPI_CORTEXM_STM32_DEVICE(BLE_spi_dev       , 1, 0, false, 0,0,10000000,1,1,1)

CYG_DEVS_SPI_CORTEXM_STM32_DEVICE(stm32_flash_dev   , 3, 0, false, 0,0,6000000,1,4,1)

CYG_DEVS_FLASH_SPI_AT25DFXXX_DRIVER(dataflash, 0, &stm32_flash_dev.spi_bus);

#define TRACE(_x, ...) diag_printf( _x,  ##__VA_ARGS__)

bool initFlash()
{
	cyg_flash_info_t info;
	      cyg_flash_init(0);
	      int ret = cyg_flash_get_info(0,&info);
	      TRACE("cyg_flash_info return: %d\n", ret);
	      if(!ret)
	      {
	         TRACE("SPI_FLASH: Device enabled\n");
	         TRACE("Spansion - number of block infos: 0x%8.0X\n",info.num_block_infos);
	         TRACE("         - block count: 0x%8.0X\n",info.block_info[0].blocks);
	         TRACE("         - block size 0x%8.0X\n",info.block_info[0].block_size);
	         TRACE("         - START address 0x%8.0X \n",info.start);
	         TRACE("         - end address 0x%8.0X \n",info.end);

	         return true;
	      }

	      return true;
}

