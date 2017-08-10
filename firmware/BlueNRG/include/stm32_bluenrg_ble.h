/**
  ******************************************************************************
  * @file    stm32_bluenrg_ble.h
  * @author  CL
  * @version V1.0.0
  * @date    04-July-2014
  * @brief   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_BLUENRG_BLE_H
#define __STM32_BLUENRG_BLE_H

#include <stdint.h>
#include <cyg/io/spi.h>

#define IDB04A1 0
#define IDB05A1 1

#define PRINT_REG(_base, _x)    diag_printf(#_x": 0x%08X\n", *((volatile CYG_WORD32 *)(_base + _x)));
 
void BNRG_SPI_Init(cyg_spi_device *dev, cyg_uint32 extiPin, cyg_uint32 resetPin, void (*interruptCB)(void));

void BlueNRG_RST(void);
uint8_t BlueNRG_DataPresent(void);
void    BlueNRG_HW_Bootloader(void);
int32_t BlueNRG_SPI_Read_All(uint8_t *buffer,
                             uint8_t buff_size);
int32_t BlueNRG_SPI_Write(uint8_t* data1,
                          uint8_t* data2,
                          uint8_t Nb_bytes1,
                          uint8_t Nb_bytes2);

void Clear_SPI_EXTI_Flag(void);
void Hal_Write_Serial(const void* data1, const void* data2, int32_t n_bytes1, int32_t n_bytes2);


#endif /* __STM32_BLUENRG_BLE_H */
    
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

