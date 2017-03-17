#include "../../usb_otg/inc/usbd_core.h"
#include "../inc/usbd_desc.h"
#include "../../usb_otg/inc/usbd_req.h"
#include "../inc/usbd_conf.h"
#include "../../usb_otg/inc/usb_regs.h"

//vendor id
#define USBD_VID                        0x1781
//product id
#define USBD_PID                        0x0B96

/** @defgroup USB_String_Descriptors
  * @{
  */ 
#define USBD_LANGID_STRING              0x409
#define USBD_MANUFACTURER_STRING        (cyg_uint8*)"Keystone Electronic Solutions"
#define USBD_PRODUCT_STRING             (cyg_uint8*)"STM32F4 RFlock jig"

#define USBD_CONFIGURATION_STRING       (cyg_uint8*)"usb-cdc config"
#define USBD_INTERFACE_STRING           (cyg_uint8*)"usb-cdc Interface"

extern cyg_uint8 usbd_serial[16];

USBD_DEVICE USR_desc =
{
  USBD_USR_DeviceDescriptor,
  USBD_USR_LangIDStrDescriptor, 
  USBD_USR_ManufacturerStrDescriptor,
  USBD_USR_ProductStrDescriptor,
  USBD_USR_SerialStrDescriptor,
  USBD_USR_ConfigStrDescriptor,
  USBD_USR_InterfaceStrDescriptor,
  
};


/* USB Standard Device Descriptor */
__ALIGN_BEGIN cyg_uint8 USBD_DeviceDesc[USB_SIZ_DEVICE_DESC] __ALIGN_END =
{
        0x12,                       /*bLength */
        USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
        0x00,                       /*bcdUSB */
        0x02,
        0x00,                       /*bDeviceClass*/
        0x00,                       /*bDeviceSubClass*/
        0x00,                       /*bDeviceProtocol*/
        USB_OTG_MAX_EP0_SIZE,      /*bMaxPacketSize*/
        LOBYTE(USBD_VID),           /*idVendor*/
        HIBYTE(USBD_VID),           /*idVendor*/
        LOBYTE(USBD_PID),           /*idVendor*/
        HIBYTE(USBD_PID),           /*idVendor*/
        0x00,                       /*bcdDevice rel. 2.00*/
        0x02,
        USBD_IDX_MFC_STR,           /*Index of manufacturer  string*/
        USBD_IDX_PRODUCT_STR,       /*Index of product string*/
        USBD_IDX_SERIAL_STR,        /*Index of serial number string*/
        USBD_CFG_MAX_NUM            /*bNumConfigurations*/
} ; /* USB_DeviceDescriptor */

/* USB Standard Device Descriptor */
__ALIGN_BEGIN cyg_uint8 USBD_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x03,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

/* USB Standard Device Descriptor */
__ALIGN_BEGIN cyg_uint8 USBD_LangIDDesc[USB_SIZ_STRING_LANGID] __ALIGN_END =
{
     USB_SIZ_STRING_LANGID,         
     USB_DESC_TYPE_STRING,       
     LOBYTE(USBD_LANGID_STRING),
     HIBYTE(USBD_LANGID_STRING), 
};

cyg_uint8 *  USBD_USR_DeviceDescriptor( cyg_uint8 speed , cyg_uint16 *length)
{
  *length = sizeof(USBD_DeviceDesc);
  return USBD_DeviceDesc;
}

cyg_uint8 *  USBD_USR_LangIDStrDescriptor( cyg_uint8 speed , cyg_uint16 *length)
{
  *length =  sizeof(USBD_LangIDDesc);  
  return USBD_LangIDDesc;
}

cyg_uint8 *  USBD_USR_ProductStrDescriptor( cyg_uint8 speed , cyg_uint16 *length)
{
    USBD_GetString (USBD_PRODUCT_STRING, USBD_StrDesc, length);

    return USBD_StrDesc;
}

cyg_uint8 *  USBD_USR_ManufacturerStrDescriptor( cyg_uint8 speed , cyg_uint16 *length)
{
  USBD_GetString (USBD_MANUFACTURER_STRING, USBD_StrDesc, length);
  return USBD_StrDesc;
}

cyg_uint8 *  USBD_USR_SerialStrDescriptor( cyg_uint8 speed , cyg_uint16 *length)
{
    USBD_GetString (usbd_serial, USBD_StrDesc, length);

    return USBD_StrDesc;
}

cyg_uint8 *  USBD_USR_ConfigStrDescriptor( cyg_uint8 speed , cyg_uint16 *length)
{
    USBD_GetString (USBD_CONFIGURATION_STRING, USBD_StrDesc, length);

    return USBD_StrDesc;
}


cyg_uint8 *  USBD_USR_InterfaceStrDescriptor( cyg_uint8 speed , cyg_uint16 *length)
{
    USBD_GetString (USBD_INTERFACE_STRING, USBD_StrDesc, length);

    return USBD_StrDesc;
}




