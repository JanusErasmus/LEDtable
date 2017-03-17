#ifndef __USB_DESC_H
#define __USB_DESC_H

#include "../../usb_otg/inc/usbd_def.h"

#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05
#define USB_SIZ_DEVICE_DESC                     18
#define USB_SIZ_STRING_LANGID                   4

#ifdef __cplusplus
extern "C" {
#endif

struct stm32_usb_srings
{
    cyg_uint8 * manufacturer;
    cyg_uint8 * product;
    cyg_uint8 * serial;
};

extern  cyg_uint8 USBD_DeviceDesc  [USB_SIZ_DEVICE_DESC];
extern  cyg_uint8 USBD_StrDesc[USB_MAX_STR_DESC_SIZ];
extern  cyg_uint8 USBD_OtherSpeedCfgDesc[USB_LEN_CFG_DESC];
extern  cyg_uint8 USBD_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC];
extern  cyg_uint8 USBD_LangIDDesc[USB_SIZ_STRING_LANGID];


cyg_uint8 *     USBD_USR_DeviceDescriptor( cyg_uint8 speed , cyg_uint16 *length);
cyg_uint8 *     USBD_USR_LangIDStrDescriptor( cyg_uint8 speed , cyg_uint16 *length);
cyg_uint8 *     USBD_USR_ManufacturerStrDescriptor ( cyg_uint8 speed , cyg_uint16 *length);
cyg_uint8 *     USBD_USR_ProductStrDescriptor ( cyg_uint8 speed , cyg_uint16 *length);
cyg_uint8 *     USBD_USR_SerialStrDescriptor( cyg_uint8 speed , cyg_uint16 *length);
cyg_uint8 *     USBD_USR_ConfigStrDescriptor( cyg_uint8 speed , cyg_uint16 *length);
cyg_uint8 *     USBD_USR_InterfaceStrDescriptor( cyg_uint8 speed , cyg_uint16 *length);

#ifdef USB_SUPPORT_USER_STRING_DESC
cyg_uint8 *     USBD_USR_USRStringDesc (cyg_uint8 speed, cyg_uint8 idx , cyg_uint16 *length);
#endif /* USB_SUPPORT_USER_STRING_DESC */  

#ifdef __cplusplus
}
#endif

#endif /* __USBD_DESC_H */
