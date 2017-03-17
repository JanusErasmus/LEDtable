#ifndef __USBD_CORE_H
#define __USBD_CORE_H

#include "usb_dcd.h"
#include "usbd_def.h"
#include "../../usb_dev/inc/usbd_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  USBD_OK   = 0,
  USBD_BUSY,
  USBD_FAIL,
}USBD_Status;

void USBD_Init(cyg_uint32 serial,
		       USB_OTG_CORE_HANDLE *pdev,
               USB_OTG_CORE_ID_TypeDef coreID, 
               USBD_DEVICE *pDevice,                  
               USBD_Class_cb_TypeDef *class_cb, 
               USBD_Usr_cb_TypeDef *usr_cb);

USBD_Status USBD_DeInit(USB_OTG_CORE_HANDLE *pdev);

USBD_Status USBD_ClrCfg(USB_OTG_CORE_HANDLE  *pdev, cyg_uint8 cfgidx);

USBD_Status USBD_SetCfg(USB_OTG_CORE_HANDLE  *pdev, cyg_uint8 cfgidx);


#ifdef __cplusplus
 }
#endif

#endif /* __USBD_CORE_H */




