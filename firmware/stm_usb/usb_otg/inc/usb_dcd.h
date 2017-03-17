#ifndef __DCD_H__
#define __DCD_H__

#include "usb_core.h"

#define USB_OTG_EP_CONTROL                       0
#define USB_OTG_EP_ISOC                          1
#define USB_OTG_EP_BULK                          2
#define USB_OTG_EP_INT                           3
#define USB_OTG_EP_MASK                          3

/*  Device Status */
#define USB_OTG_DEFAULT                          1
#define USB_OTG_ADDRESSED                        2
#define USB_OTG_CONFIGURED                       3
#define USB_OTG_SUSPENDED                        4

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  cyg_uint8  bLength;
  cyg_uint8  bDescriptorType;
  cyg_uint8  bEndpointAddress;
  cyg_uint8  bmAttributes;
  cyg_uint16 wMaxPacketSize;
  cyg_uint8  bInterval;
}
EP_DESCRIPTOR , *PEP_DESCRIPTOR;

void       DCD_Init(USB_OTG_CORE_HANDLE *pdev ,
                    USB_OTG_CORE_ID_TypeDef coreID);

void        DCD_DevConnect (USB_OTG_CORE_HANDLE *pdev);
void        DCD_DevDisconnect (USB_OTG_CORE_HANDLE *pdev);
void        DCD_EP_SetAddress (USB_OTG_CORE_HANDLE *pdev,
                               cyg_uint8 address);
cyg_uint32    DCD_EP_Open(USB_OTG_CORE_HANDLE *pdev ,
                     cyg_uint8 ep_addr,
                     cyg_uint16 ep_mps,
                     cyg_uint8 ep_type);

cyg_uint32    DCD_EP_Close  (USB_OTG_CORE_HANDLE *pdev,
                                cyg_uint8  ep_addr);


cyg_uint32   DCD_EP_PrepareRx ( USB_OTG_CORE_HANDLE *pdev,
                        cyg_uint8   ep_addr,
                        cyg_uint8 *pbuf,
                        cyg_uint16  buf_len);
  
cyg_uint32    DCD_EP_Tx (USB_OTG_CORE_HANDLE *pdev,
                               cyg_uint8  ep_addr,
                               cyg_uint8  *pbuf,
                               cyg_uint32   buf_len);
cyg_uint32    DCD_EP_Stall (USB_OTG_CORE_HANDLE *pdev,
                              cyg_uint8   epnum);
cyg_uint32    DCD_EP_ClrStall (USB_OTG_CORE_HANDLE *pdev,
                                  cyg_uint8 epnum);
cyg_uint32    DCD_EP_Flush (USB_OTG_CORE_HANDLE *pdev,
                               cyg_uint8 epnum);
cyg_uint32    DCD_Handle_ISR(USB_OTG_CORE_HANDLE *pdev);

cyg_uint32 DCD_GetEPStatus(USB_OTG_CORE_HANDLE *pdev ,
                         cyg_uint8 epnum);

void DCD_SetEPStatus (USB_OTG_CORE_HANDLE *pdev , 
                      cyg_uint8 epnum ,
                      cyg_uint32 Status);

#ifdef __cplusplus
}
#endif


#endif //__DCD_H__
