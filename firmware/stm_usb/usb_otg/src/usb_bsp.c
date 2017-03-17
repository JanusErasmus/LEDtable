#include "../inc/usb_core.h"
#include "../inc/usb_bsp.h"
#include "../../usb_dev/inc/usbd_conf.h"
#include "../inc/usb_dcd_int.h"

void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
  cyg_uint32 reg32;

#if defined(CYGHWR_HAL_CORTEXM_STM32_FAMILY_HIPERFORMANCE)
  // Enable OTG FS clock

   HAL_READ_UINT32(CYGHWR_HAL_STM32_RCC+CYGHWR_HAL_STM32_RCC_AHB2ENR, reg32);
   reg32 |= BIT_(CYGHWR_HAL_STM32_RCC_AHB2ENR_OTGFS);
   HAL_WRITE_UINT32(CYGHWR_HAL_STM32_RCC+CYGHWR_HAL_STM32_RCC_AHB2ENR, reg32);

   //configure the USB pins
   CYGHWR_HAL_STM32_GPIO_SET(CYGHWR_HAL_STM32_PIN_ALTFN_OUT( A, 11, 10, OPENDRAIN, NONE, HIGH ));	 	//DM
   CYGHWR_HAL_STM32_GPIO_SET(CYGHWR_HAL_STM32_PIN_ALTFN_OUT( A, 12, 10, OPENDRAIN, NONE, HIGH ));	 	//DP
   //CYGHWR_HAL_STM32_PIN_ALTFN_OUT( B, 11, 7, OPENDRAIN, NONE, 50MHZ );		//VBUS
#endif
#if defined (CYGHWR_HAL_CORTEXM_STM32_FAMILY_F1)
  // Because the PLL frequency is set up to be 72MHz, the following bit must be cleared:
  HAL_READ_UINT32(CYGHWR_HAL_STM32_RCC+CYGHWR_HAL_STM32_RCC_CFGR, reg32);
  reg32 &=~ CYGHWR_HAL_STM32_RCC_CFGR_USBPRE;
  HAL_WRITE_UINT32(CYGHWR_HAL_STM32_RCC+CYGHWR_HAL_STM32_RCC_CFGR, reg32);

  // Enable OTG clock
  HAL_READ_UINT32(CYGHWR_HAL_STM32_RCC+CYGHWR_HAL_STM32_RCC_AHBENR, reg32);
  reg32 |= CYGHWR_HAL_STM32_RCC_AHBENR_OTGFS;
  HAL_WRITE_UINT32(CYGHWR_HAL_STM32_RCC+CYGHWR_HAL_STM32_RCC_AHBENR, reg32);

  //configure the USB pins
  CYGHWR_HAL_STM32_GPIO_SET(CYGHWR_HAL_STM32_GPIO( A, 11,  OUT_50MHZ, ALT_OPENDRAIN)); 	//DM
  CYGHWR_HAL_STM32_GPIO_SET(CYGHWR_HAL_STM32_GPIO( A, 12,  OUT_50MHZ, ALT_OPENDRAIN)); 	//DP
  CYGHWR_HAL_STM32_GPIO_SET(CYGHWR_HAL_STM32_GPIO( A,  9, IN, ALT_OPENDRAIN)); 			//VBUS
#endif
}


/**
* @brief  USB_OTG_BSP_EnableInterrupt
*         Enabele USB Global interrupt
* @param  None
* @retval None
*/
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
	cyg_interrupt_mask(CYGNUM_HAL_INTERRUPT_OTG_FS);
	cyg_interrupt_create(CYGNUM_HAL_INTERRUPT_OTG_FS,
			1,
			(cyg_addrword_t) pdev,
			ISR_OTGFS,
			DSR_OTGFS,
			&OTGFS_IntHandle,
			&OTGFS_Interrupt);

	cyg_interrupt_attach(OTGFS_IntHandle);

	cyg_interrupt_unmask(CYGNUM_HAL_INTERRUPT_OTG_FS);
}

cyg_uint32 ISR_OTGFS(cyg_vector_t vector, cyg_addrword_t data)
{
    cyg_interrupt_mask(vector);
    cyg_interrupt_acknowledge(vector);
    return(CYG_ISR_CALL_DSR);
}

void DSR_OTGFS(cyg_vector_t vector,cyg_uint32 count,cyg_addrword_t data)
{
    USBD_OTG_ISR_Handler ((USB_OTG_CORE_HANDLE*) data);

    cyg_interrupt_unmask(vector);
}


/**
* @brief  USB_OTG_BSP_uDelay
*         This function provides delay time in micro sec
* @param  usec : Value of delay required in micro sec
* @retval None
*/
void USB_OTG_BSP_uDelay (const cyg_uint32 usec)
{
	HAL_DELAY_US(usec);
}



void USB_OTG_BSP_mDelay (const cyg_uint32 msec)
{
  USB_OTG_BSP_uDelay(msec * 1000);   
}



