#ifndef __USB_CORE_H__
#define __USB_CORE_H__
#include "../../usb_dev/inc/usb_conf.h"
#include "usb_regs.h"
#include "usb_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

#define USB_OTG_EP0_IDLE                          0
#define USB_OTG_EP0_SETUP                         1
#define USB_OTG_EP0_DATA_IN                       2
#define USB_OTG_EP0_DATA_OUT                      3
#define USB_OTG_EP0_STATUS_IN                     4
#define USB_OTG_EP0_STATUS_OUT                    5
#define USB_OTG_EP0_STALL                         6

#define USB_OTG_EP_TX_DIS       0x0000
#define USB_OTG_EP_TX_STALL     0x0010
#define USB_OTG_EP_TX_NAK       0x0020
#define USB_OTG_EP_TX_VALID     0x0030
 
#define USB_OTG_EP_RX_DIS       0x0000
#define USB_OTG_EP_RX_STALL     0x1000
#define USB_OTG_EP_RX_NAK       0x2000
#define USB_OTG_EP_RX_VALID     0x3000

#define   MAX_DATA_LENGTH                        0xFF


typedef enum {
  USB_OTG_OK = 0,
  USB_OTG_FAIL
}USB_OTG_STS;

typedef enum {
  HC_IDLE = 0,
  HC_XFRC,
  HC_HALTED,
  HC_NAK,
  HC_NYET,
  HC_STALL,
  HC_XACTERR,  
  HC_BBLERR,   
  HC_DATATGLERR,  
}HC_STATUS;

typedef enum {
  URB_IDLE = 0,
  URB_DONE,
  URB_NOTREADY,
  URB_ERROR,
  URB_STALL
}URB_STATE;

typedef enum {
  CTRL_START = 0,
  CTRL_XFRC,
  CTRL_HALTED,
  CTRL_NAK,
  CTRL_STALL,
  CTRL_XACTERR,  
  CTRL_BBLERR,   
  CTRL_DATATGLERR,  
  CTRL_FAIL
}CTRL_STATUS;


typedef struct USB_OTG_hc
{
  cyg_uint8       dev_addr ;
  cyg_uint8       ep_num;
  cyg_uint8       ep_is_in;
  cyg_uint8       speed;
  cyg_uint8       do_ping;
  cyg_uint8       ep_type;
  cyg_uint16      max_packet;
  cyg_uint8       data_pid;
  cyg_uint8       *xfer_buff;
  cyg_uint32      xfer_len;
  cyg_uint32      xfer_count;
  cyg_uint8       toggle_in;
  cyg_uint8       toggle_out;
  cyg_uint32       dma_addr;
}
USB_OTG_HC , *PUSB_OTG_HC;

typedef struct USB_OTG_ep
{
  cyg_uint8        num;
  cyg_uint8        is_in;
  cyg_uint8        is_stall;
  cyg_uint8        type;
  cyg_uint8        data_pid_start;
  cyg_uint8        even_odd_frame;
  cyg_uint16       tx_fifo_num;
  cyg_uint32       maxpacket;
  /* transaction level variables*/
  cyg_uint8        *xfer_buff;
  cyg_uint32       dma_addr;
  cyg_uint32       xfer_len;
  cyg_uint32       xfer_count;
  /* Transfer level variables*/  
  cyg_uint32       rem_data_len;
  cyg_uint32       total_data_len;
  cyg_uint32       ctl_data_len;

}

USB_OTG_EP , *PUSB_OTG_EP;



typedef struct USB_OTG_core_cfg
{
  cyg_uint8       host_channels;
  cyg_uint8       dev_endpoints;
  cyg_uint8       speed;
  cyg_uint8       dma_enable;
  cyg_uint16      mps;
  cyg_uint16      TotalFifoSize;
  cyg_uint8       phy_itface;
  cyg_uint8       Sof_output;
  cyg_uint8       low_power;
  cyg_uint8       coreID;
 
}
USB_OTG_CORE_CFGS, *PUSB_OTG_CORE_CFGS;



typedef  struct  usb_setup_req {
    
    cyg_uint8   bmRequest;
    cyg_uint8   bRequest;
    cyg_uint16  wValue;
    cyg_uint16  wIndex;
    cyg_uint16  wLength;
} USB_SETUP_REQ;

typedef struct _Device_TypeDef
{
  cyg_uint8  *(*GetDeviceDescriptor)( cyg_uint8 speed , cyg_uint16 *length);
  cyg_uint8  *(*GetLangIDStrDescriptor)( cyg_uint8 speed , cyg_uint16 *length);
  cyg_uint8  *(*GetManufacturerStrDescriptor)( cyg_uint8 speed , cyg_uint16 *length);
  cyg_uint8  *(*GetProductStrDescriptor)( cyg_uint8 speed , cyg_uint16 *length);
  cyg_uint8  *(*GetSerialStrDescriptor)( cyg_uint8 speed , cyg_uint16 *length);
  cyg_uint8  *(*GetConfigurationStrDescriptor)( cyg_uint8 speed , cyg_uint16 *length);
  cyg_uint8  *(*GetInterfaceStrDescriptor)( cyg_uint8 speed , cyg_uint16 *length);
} USBD_DEVICE, *pUSBD_DEVICE;

typedef struct USB_OTG_hPort
{
  void (*Disconnect) (void *phost);
  void (*Connect) (void *phost); 
  cyg_uint8 ConnStatus;
  cyg_uint8 DisconnStatus;
  cyg_uint8 ConnHandled;
  cyg_uint8 DisconnHandled;
} USB_OTG_hPort_TypeDef;

typedef struct _Device_cb
{
  cyg_uint8  (*Init)         (void *pdev , cyg_uint8 cfgidx);
  cyg_uint8  (*DeInit)       (void *pdev , cyg_uint8 cfgidx);
 /* Control Endpoints*/
  cyg_uint8  (*Setup)        (void *pdev , USB_SETUP_REQ  *req);
  cyg_uint8  (*EP0_TxSent)   (void *pdev );
  cyg_uint8  (*EP0_RxReady)  (void *pdev );
  /* Class Specific Endpoints*/
  cyg_uint8  (*DataIn)       (void *pdev , cyg_uint8 epnum);
  cyg_uint8  (*DataOut)      (void *pdev , cyg_uint8 epnum);
  cyg_uint8  (*SOF)          (void *pdev);
  cyg_uint8  (*IsoINIncomplete)  (void *pdev);
  cyg_uint8  (*IsoOUTIncomplete)  (void *pdev);

  cyg_uint8  *(*GetConfigDescriptor)( cyg_uint8 speed , cyg_uint16 *length);
#ifdef USB_OTG_HS_CORE 
  cyg_uint8  *(*GetOtherConfigDescriptor)( cyg_uint8 speed , cyg_uint16 *length);
#endif

#ifdef USB_SUPPORT_USER_STRING_DESC 
  cyg_uint8  *(*GetUsrStrDescriptor)( cyg_uint8 speed ,cyg_uint8 index,  cyg_uint16 *length);
#endif  
  
} USBD_Class_cb_TypeDef;



typedef struct _USBD_USR_PROP
{
  void (*Init)(void);   
  void (*DeviceReset)(cyg_uint8 speed);
  void (*DeviceConfigured)(void);
  void (*DeviceSuspended)(void);
  void (*DeviceResumed)(void);  
  
  void (*DeviceConnected)(void);  
  void (*DeviceDisconnected)(void);    
  
}
USBD_Usr_cb_TypeDef;

typedef struct _DCD
{
  cyg_uint8        device_config;
  cyg_uint8        device_state;
  cyg_uint8        device_status;
  cyg_uint8        device_address;
  cyg_uint32       DevRemoteWakeup;
  USB_OTG_EP     in_ep   [USB_OTG_MAX_TX_FIFOS];
  USB_OTG_EP     out_ep  [USB_OTG_MAX_TX_FIFOS];
  cyg_uint8        setup_packet [8*3];
  USBD_Class_cb_TypeDef         *class_cb;
  USBD_Usr_cb_TypeDef           *usr_cb;
  USBD_DEVICE                   *usr_device;  
  cyg_uint8        *pConfig_descriptor;
 }
DCD_DEV , *DCD_PDEV;


typedef struct _HCD
{
  cyg_uint8                  Rx_Buffer [MAX_DATA_LENGTH];
  volatile cyg_uint32            ConnSts;
  volatile cyg_uint32            ErrCnt[USB_OTG_MAX_TX_FIFOS];
  volatile cyg_uint32            XferCnt[USB_OTG_MAX_TX_FIFOS];
  volatile HC_STATUS           HC_Status[USB_OTG_MAX_TX_FIFOS];
  volatile URB_STATE           URB_State[USB_OTG_MAX_TX_FIFOS];
  USB_OTG_HC               hc [USB_OTG_MAX_TX_FIFOS];
  cyg_uint16                 channel [USB_OTG_MAX_TX_FIFOS];
  USB_OTG_hPort_TypeDef    *port_cb;  
}
HCD_DEV , *USB_OTG_USBH_PDEV;


typedef struct _OTG
{
  cyg_uint8    OTG_State;
  cyg_uint8    OTG_PrevState;
  cyg_uint8    OTG_Mode;
}
OTG_DEV , *USB_OTG_USBO_PDEV;

typedef struct USB_OTG_handle
{
  USB_OTG_CORE_CFGS    cfg;
  USB_OTG_CORE_REGS    regs;
#ifdef USE_DEVICE_MODE
  DCD_DEV     dev;
#endif
#ifdef USE_HOST_MODE
  HCD_DEV     host;
#endif
#ifdef USE_OTG_MODE
  OTG_DEV     otg;
#endif
}
USB_OTG_CORE_HANDLE , *PUSB_OTG_CORE_HANDLE;

/**
  * @}
  */ 


/** @defgroup USB_CORE_Exported_Macros
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_FunctionsPrototype
  * @{
  */ 


USB_OTG_STS  USB_OTG_CoreInit        (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_SelectCore      (USB_OTG_CORE_HANDLE *pdev, 
                                      USB_OTG_CORE_ID_TypeDef coreID);
USB_OTG_STS  USB_OTG_EnableGlobalInt (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_DisableGlobalInt(USB_OTG_CORE_HANDLE *pdev);
void*           USB_OTG_ReadPacket   (USB_OTG_CORE_HANDLE *pdev ,
    cyg_uint8 *dest,
    cyg_uint16 len);
USB_OTG_STS  USB_OTG_WritePacket     (USB_OTG_CORE_HANDLE *pdev ,
    cyg_uint8 *src,
    cyg_uint8 ch_ep_num,
    cyg_uint16 len);
USB_OTG_STS  USB_OTG_FlushTxFifo     (USB_OTG_CORE_HANDLE *pdev , cyg_uint32 num);
USB_OTG_STS  USB_OTG_FlushRxFifo     (USB_OTG_CORE_HANDLE *pdev);

cyg_uint32     USB_OTG_ReadCoreItr     (USB_OTG_CORE_HANDLE *pdev);
cyg_uint32     USB_OTG_ReadOtgItr      (USB_OTG_CORE_HANDLE *pdev);
cyg_uint8      USB_OTG_IsHostMode      (USB_OTG_CORE_HANDLE *pdev);
cyg_uint8      USB_OTG_IsDeviceMode    (USB_OTG_CORE_HANDLE *pdev);
cyg_uint32     USB_OTG_GetMode         (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_PhyInit         (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_SetCurrentMode  (USB_OTG_CORE_HANDLE *pdev,
    cyg_uint8 mode);

/*********************** HOST APIs ********************************************/
#ifdef USE_HOST_MODE
USB_OTG_STS  USB_OTG_CoreInitHost    (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_EnableHostInt   (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_HC_Init         (USB_OTG_CORE_HANDLE *pdev, cyg_uint8 hc_num);
USB_OTG_STS  USB_OTG_HC_Halt         (USB_OTG_CORE_HANDLE *pdev, cyg_uint8 hc_num);
USB_OTG_STS  USB_OTG_HC_StartXfer    (USB_OTG_CORE_HANDLE *pdev, cyg_uint8 hc_num);
USB_OTG_STS  USB_OTG_HC_DoPing       (USB_OTG_CORE_HANDLE *pdev , cyg_uint8 hc_num);
cyg_uint32     USB_OTG_ReadHostAllChannels_intr    (USB_OTG_CORE_HANDLE *pdev);
cyg_uint32     USB_OTG_ResetPort       (USB_OTG_CORE_HANDLE *pdev);
cyg_uint32     USB_OTG_ReadHPRT0       (USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_DriveVbus       (USB_OTG_CORE_HANDLE *pdev, cyg_uint8 state);
void         USB_OTG_InitFSLSPClkSel (USB_OTG_CORE_HANDLE *pdev ,cyg_uint8 freq);
cyg_uint8      USB_OTG_IsEvenFrame     (USB_OTG_CORE_HANDLE *pdev) ;
void         USB_OTG_StopHost        (USB_OTG_CORE_HANDLE *pdev);
#endif
/********************* DEVICE APIs ********************************************/
#ifdef USE_DEVICE_MODE
USB_OTG_STS  USB_OTG_CoreInitDev         (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_EnableDevInt        (USB_OTG_CORE_HANDLE *pdev);
cyg_uint32     USB_OTG_ReadDevAllInEPItr           (USB_OTG_CORE_HANDLE *pdev);
enum USB_OTG_SPEED USB_OTG_GetDeviceSpeed (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_EP0Activate (USB_OTG_CORE_HANDLE *pdev);
USB_OTG_STS  USB_OTG_EPActivate  (USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep);
USB_OTG_STS  USB_OTG_EPDeactivate(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep);
USB_OTG_STS  USB_OTG_EPStartXfer (USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep);
USB_OTG_STS  USB_OTG_EP0StartXfer(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep);
USB_OTG_STS  USB_OTG_EPSetStall          (USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep);
USB_OTG_STS  USB_OTG_EPClearStall        (USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep);
cyg_uint32     USB_OTG_ReadDevAllOutEp_itr (USB_OTG_CORE_HANDLE *pdev);
cyg_uint32     USB_OTG_ReadDevOutEP_itr    (USB_OTG_CORE_HANDLE *pdev , cyg_uint8 epnum);
cyg_uint32     USB_OTG_ReadDevAllInEPItr   (USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_InitDevSpeed        (USB_OTG_CORE_HANDLE *pdev , cyg_uint8 speed);
cyg_uint8      USBH_IsEvenFrame (USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_EP0_OutStart(USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_ActiveRemoteWakeup(USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_UngateClock(USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_StopDevice(USB_OTG_CORE_HANDLE *pdev);
void         USB_OTG_SetEPStatus (USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep , cyg_uint32 Status);
cyg_uint32     USB_OTG_GetEPStatus(USB_OTG_CORE_HANDLE *pdev ,USB_OTG_EP *ep);
#endif
/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif  /* __USB_CORE_H__ */


/**
  * @}
  */ 

/**
  * @}
  */ 
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

