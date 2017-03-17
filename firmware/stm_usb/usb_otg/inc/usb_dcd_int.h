#ifndef USB_DCD_INT_H__
#define USB_DCD_INT_H__

#include <cyg/kernel/diag.h>
#include "usb_dcd.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _USBD_DCD_INT
{
  cyg_uint8 (* DataOutStage) (USB_OTG_CORE_HANDLE *pdev , cyg_uint8 epnum);
  cyg_uint8 (* DataInStage)  (USB_OTG_CORE_HANDLE *pdev , cyg_uint8 epnum);
  cyg_uint8 (* SetupStage) (USB_OTG_CORE_HANDLE *pdev);
  cyg_uint8 (* SOF) (USB_OTG_CORE_HANDLE *pdev);
  cyg_uint8 (* Reset) (USB_OTG_CORE_HANDLE *pdev);
  cyg_uint8 (* Suspend) (USB_OTG_CORE_HANDLE *pdev);
  cyg_uint8 (* Resume) (USB_OTG_CORE_HANDLE *pdev);
  cyg_uint8 (* IsoINIncomplete) (USB_OTG_CORE_HANDLE *pdev);
  cyg_uint8 (* IsoOUTIncomplete) (USB_OTG_CORE_HANDLE *pdev);
  
  cyg_uint8 (* DevConnected) (USB_OTG_CORE_HANDLE *pdev);
  cyg_uint8 (* DevDisconnected) (USB_OTG_CORE_HANDLE *pdev);
  
}USBD_DCD_INT_cb_TypeDef;

extern USBD_DCD_INT_cb_TypeDef *USBD_DCD_INT_fops;


#define CLEAR_IN_EP_INTR(epnum,intr) \
  diepint.d32=0; \
  diepint.b.intr = 1; \
  USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[epnum]->DIEPINT,diepint.d32);

#define CLEAR_OUT_EP_INTR(epnum,intr) \
  doepint.d32=0; \
  doepint.b.intr = 1; \
  USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[epnum]->DOEPINT,doepint.d32);


cyg_uint32 USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);


#ifdef __cplusplus
}
#endif

#endif // USB_DCD_INT_H__

