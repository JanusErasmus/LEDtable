#ifndef __USBD_IOREQ_H_
#define __USBD_IOREQ_H_

#include  "usbd_def.h"
#include  "usbd_core.h"

#ifdef __cplusplus
extern "C" {
#endif

USBD_Status  USBD_CtlSendData (USB_OTG_CORE_HANDLE  *pdev, 
                               cyg_uint8 *buf,
                               cyg_uint16 len);

USBD_Status  USBD_CtlContinueSendData (USB_OTG_CORE_HANDLE  *pdev, 
                               cyg_uint8 *pbuf,
                               cyg_uint16 len);

USBD_Status USBD_CtlPrepareRx (USB_OTG_CORE_HANDLE  *pdev, 
                               cyg_uint8 *pbuf,
                               cyg_uint16 len);

USBD_Status  USBD_CtlContinueRx (USB_OTG_CORE_HANDLE  *pdev, 
                              cyg_uint8 *pbuf,
                              cyg_uint16 len);

USBD_Status  USBD_CtlSendStatus (USB_OTG_CORE_HANDLE  *pdev);

USBD_Status  USBD_CtlReceiveStatus (USB_OTG_CORE_HANDLE  *pdev);

cyg_uint16  USBD_GetRxCount (USB_OTG_CORE_HANDLE  *pdev ,
                           cyg_uint8 epnum);

#ifdef __cplusplus
}
#endif


#endif /* __USBD_IOREQ_H_ */
