#ifndef __USB_CDC_CORE_H_
#define __USB_CDC_CORE_H_
#include <cyg/kernel/kapi.h>

#include  "usbd_ioreq.h"

#define USB_CDC_CONFIG_DESC_SIZ                (67)
#define USB_CDC_DESC_SIZ                       (67 + - 9)

#define CDC_DESCRIPTOR_TYPE                     0x21

#define DEVICE_CLASS_CDC                        0x02
#define DEVICE_SUBCLASS_CDC                     0x00


#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

#define CDC_DATA_IN_PACKET_SIZE                 *(cyg_uint16 *)(((USB_OTG_CORE_HANDLE *)pdev)->dev.pConfig_descriptor + 57)
        
#define CDC_DATA_OUT_PACKET_SIZE                *(cyg_uint16 *)(((USB_OTG_CORE_HANDLE *)pdev)->dev.pConfig_descriptor + 64)




#define SEND_ENCAPSULATED_COMMAND               0x00
#define GET_ENCAPSULATED_RESPONSE               0x01
#define SET_COMM_FEATURE                        0x02
#define GET_COMM_FEATURE                        0x03
#define CLEAR_COMM_FEATURE                      0x04
#define SET_LINE_CODING                         0x20
#define GET_LINE_CODING                         0x21
#define SET_CONTROL_LINE_STATE                  0x22
#define SEND_BREAK                              0x23
#define NO_CMD                                  0xFF

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _CDC_IF_PROP
{
  cyg_uint16 (*pIf_Init)     (void);
  cyg_uint16 (*pIf_DeInit)   (void);
  cyg_uint16 (*pIf_Ctrl)     (cyg_uint32 Cmd, cyg_uint8* Buf, cyg_uint32 Len);
  cyg_uint16 (*pIf_DataTx)   (cyg_uint8* Buf, cyg_uint32 Len);
  cyg_uint16 (*pIf_DataRx)   (cyg_uint8* Buf, cyg_uint32 Len);
}
CDC_IF_Prop_TypeDef;


extern USBD_Class_cb_TypeDef  USBD_CDC_cb;

#ifdef __cplusplus
}
#endif

#endif  // __USB_CDC_CORE_H_
