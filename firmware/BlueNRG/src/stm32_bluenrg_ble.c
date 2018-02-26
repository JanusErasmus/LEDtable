/**
 ******************************************************************************
 * @file    stm32_bluenrg_ble.c
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

/* Includes ------------------------------------------------------------------*/

#include <stm32_ext_interrupt.h>

#include "stm32_bluenrg_ble.h"
#include "gp_timer.h"

cyg_uint32 mResetPin;
cyg_uint32 mIRQPin;
static cyg_spi_device *mSpiDevice;
cyg_uint32 mExtInterrupt;
cyg_interrupt mInterrupt;
cyg_handle_t mIntHandle;
static cyg_uint32 handleISR(cyg_vector_t vector,cyg_addrword_t data);
static void handleDSR(cyg_vector_t vector,cyg_uint32 count,cyg_addrword_t data);
void (*mInterruptCB)(void);

extern volatile uint32_t ms_counter;

#define HEADER_SIZE 5
#define MAX_BUFFER_SIZE 255
#define TIMEOUT_DURATION 15

/**
 * @brief  This function is a utility to print the log time
 *          in the format HH:MM:SS:MSS (DK GUI time format)
 * @param  None
 * @retval None
 */
void print_csv_time(void){
	uint32_t ms = ms_counter;
	printf("%02d:%02d:%02d.%03d", ms/(60*60*1000)%24, ms/(60*1000)%60, (ms/1000)%60, ms%1000);
}

/**
 * @brief  Writes data to a serial interface.
 * @param  data1   :  1st buffer
 * @param  data2   :  2nd buffer
 * @param  n_bytes1: number of bytes in 1st buffer
 * @param  n_bytes2: number of bytes in 2nd buffer
 * @retval None
 */
void Hal_Write_Serial(const void* data1, const void* data2, int32_t n_bytes1, int32_t n_bytes2)
{
	struct timer t;
	Timer_Set(&t, 5);

	while(1){
		if(BlueNRG_SPI_Write((uint8_t *)data1,(uint8_t *)data2, n_bytes1, n_bytes2) == 0) break;
		if(Timer_Expired(&t)){
			break;
		}
	}
}

void BNRG_SPI_Init(cyg_spi_device *dev, cyg_uint32 extiPin, cyg_uint32 resetPin, void (*interruptCB)(void))
{
	mInterruptCB = interruptCB;
	mSpiDevice = dev;

	mIRQPin = extiPin;
	mResetPin = resetPin;

	CYGHWR_HAL_STM32_GPIO_SET (mResetPin);
	CYGHWR_HAL_STM32_GPIO_OUT(mResetPin, 0);

	CYGHWR_HAL_STM32_GPIO_SET (mIRQPin);
	mExtInterrupt = setupInterrupt(mIRQPin, true, false);

    cyg_interrupt_mask(mExtInterrupt);
    cyg_interrupt_create(mExtInterrupt,
            7,
            (cyg_addrword_t)0,
            handleISR,
            handleDSR,
            &mIntHandle,
            &mInterrupt);
    cyg_interrupt_attach(mIntHandle);

}



/**
 * @brief  Resets the BlueNRG.
 * @param  None
 * @retval None
 */
void BlueNRG_RST(void)
{
	cyg_interrupt_mask(mExtInterrupt);

	CYGHWR_HAL_STM32_GPIO_OUT(mResetPin, 0);
	cyg_thread_delay(100);
	CYGHWR_HAL_STM32_GPIO_OUT(mResetPin, 1);
	cyg_thread_delay(1);

	if(mInterruptCB)
		mInterruptCB();

	cyg_interrupt_unmask(mExtInterrupt);
}

/**
 * @brief  Reports if the BlueNRG has data for the host micro.
 * @param  None
 * @retval 1 if data are present, 0 otherwise
 */
uint8_t BlueNRG_DataPresent(void)
{
	int bit;
	CYGHWR_HAL_STM32_GPIO_IN(mIRQPin, &bit);

	if (bit)
		return 1;
	else
		return 0;
} /* end BlueNRG_DataPresent() */

/**
// * @brief  Activate internal bootloader using pin.
// * @param  None
// * @retval None
// */
//void BlueNRG_HW_Bootloader(void)
//{
////  Disable_SPI_IRQ();
////  set_irq_as_output();
////  BlueNRG_RST();
////  set_irq_as_input();
////  Enable_SPI_IRQ();
//}

/**
 * @brief  Reads from BlueNRG SPI buffer and store data into local buffer.
 * @param  buffer   : Buffer where data from SPI are stored
 * @param  buff_size: Buffer size
 * @retval int32_t  : Number of read bytes
 */
int32_t BlueNRG_SPI_Read_All(uint8_t *buffer,
		uint8_t buff_size)
{
	uint16_t byte_count;
	uint8_t len = 0;
	uint8_t char_ff = 0xff;
	volatile uint8_t read_char;

	uint8_t header_master[HEADER_SIZE] = {0x0b, 0x00, 0x00, 0x00, 0x00};
	uint8_t header_slave[HEADER_SIZE];

	cyg_spi_transaction_begin(mSpiDevice);
	/* Read the header */
	cyg_spi_transaction_transfer(mSpiDevice, FALSE, HEADER_SIZE, header_master, header_slave, 0);

	if (header_slave[0] == 0x02) {
		/* device is ready */
		byte_count = (header_slave[4]<<8)|header_slave[3];

		//diag_printf("There is %d bytes\n", byte_count);
		if (byte_count > 0) {

			/* avoid to read more data that size of the buffer */
			if (byte_count > buff_size){
				byte_count = buff_size;
			}

			for (len = 0; len < byte_count; len++){
				cyg_spi_transaction_transfer(mSpiDevice, FALSE, 1, &char_ff, (uint8_t*)&read_char, 0);
				buffer[len] = read_char;

				//HAL_DELAY_US(1);
			}

		}
	}
	cyg_spi_transaction_end(mSpiDevice);

	// Add a small delay to give time to the BlueNRG to set the IRQ pin low
	// to avoid a useless SPI read at the end of the transaction
	//HAL_DELAY_US(1);
//	cyg_thread_delay(1);

	return len;
}

/**
 * @brief  Enable SPI IRQ.
 * @param  None
 * @retval None
 */
void Enable_SPI_IRQ(void)
{
	cyg_interrupt_unmask(mExtInterrupt);
}

/**
 * @brief  Disable SPI IRQ.
 * @param  None
 * @retval None
 */
void Disable_SPI_IRQ(void)
{
	cyg_interrupt_mask(mExtInterrupt);
}

/**
 * @brief  Writes data from local buffer to SPI.
 * @param  data1    : First data buffer to be written
 * @param  data2    : Second data buffer to be written
 * @param  Nb_bytes1: Size of first data buffer to be written
 * @param  Nb_bytes2: Size of second data buffer to be written
 * @retval Number of read bytes
 */
int32_t BlueNRG_SPI_Write(uint8_t* data1, uint8_t* data2, uint8_t Nb_bytes1, uint8_t Nb_bytes2)
{
	int32_t result = 0;

	unsigned char header_master[HEADER_SIZE] = {0x0a, 0x00, 0x00, 0x00, 0x00};
	unsigned char header_slave[HEADER_SIZE]  = {0xaa, 0x00, 0x00, 0x00, 0x00};

	unsigned char read_char_buf[MAX_BUFFER_SIZE];

	Disable_SPI_IRQ();


	cyg_spi_transaction_begin(mSpiDevice);
	/* Exchange header */
	cyg_spi_transaction_transfer(mSpiDevice, FALSE, HEADER_SIZE, header_master, header_slave, 0);

	if (header_slave[0] == 0x02) {
		/* SPI is ready */
		if (header_slave[1] >= (Nb_bytes1+Nb_bytes2)) {

			/*  Buffer is big enough */
			if (Nb_bytes1 > 0) {
				cyg_spi_transaction_transfer(mSpiDevice, FALSE,  Nb_bytes1, data1, read_char_buf, 0);
			}
			if (Nb_bytes2 > 0) {
				cyg_spi_transaction_transfer(mSpiDevice, FALSE,  Nb_bytes2, data2, read_char_buf, 0);
			}

		} else {
			/* Buffer is too small */
			result = -2;
		}
	} else {
		/* SPI is not ready */
		//diag_dump_buf(header_slave, HEADER_SIZE);
		result = -1;
	}

	cyg_spi_transaction_end(mSpiDevice);

	Enable_SPI_IRQ();

	//printf("BlueNRG_SPI_Write: %d\n", result);
	return result;

}

/**
 * @brief  Clear Pending SPI IRQ.
 * @param  None
 * @retval None
 */
void Clear_SPI_IRQ(void)
{
}

/**
 * @brief  Clear EXTI (External Interrupt) line for SPI IRQ.
 * @param  None
 * @retval None
 */
void Clear_SPI_EXTI_Flag(void)
{  
}

cyg_uint32 handleISR(cyg_vector_t vector,cyg_addrword_t data)
{
	cyg_interrupt_mask(vector);
	cyg_interrupt_acknowledge(vector);

	return(CYG_ISR_CALL_DSR | CYG_ISR_HANDLED);
}

void handleDSR(cyg_vector_t vector,cyg_uint32 count,cyg_addrword_t data)
{
	if(mInterruptCB)
		mInterruptCB();

	cyg_interrupt_unmask(vector);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
