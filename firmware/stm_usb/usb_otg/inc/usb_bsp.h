#ifndef __USB_BSP__H__
#define __USB_BSP__H__
#include <cyg/kernel/diag.h>
#include <cyg/hal/hal_diag.h>
#include <cyg/kernel/kapi.h>

#include "usb_core.h"

#ifdef __cplusplus
extern "C" {
#endif

void BSP_Init(void);

void USB_OTG_BSP_Init (USB_OTG_CORE_HANDLE *pdev);
void USB_OTG_BSP_uDelay (const cyg_uint32 usec);
void USB_OTG_BSP_mDelay (const cyg_uint32 msec);
void USB_OTG_BSP_EnableInterrupt (USB_OTG_CORE_HANDLE *pdev);

cyg_interrupt OTGFS_Interrupt;
cyg_handle_t OTGFS_IntHandle;
void DSR_OTGFS(cyg_vector_t vector,cyg_uint32 count,cyg_addrword_t data);
cyg_uint32 ISR_OTGFS(cyg_vector_t vector, cyg_addrword_t data);

#ifdef __cplusplus
}
#endif

#endif //__USB_BSP__H__
