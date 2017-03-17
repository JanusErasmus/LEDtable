#ifndef USB_INTERFACE_H_
#define USB_INTERFACE_H_
#include <cyg/kernel/kapi.h>

class cSerial;

#include "activity_signaller.h"

#define USBRXBUFF_SIZE 512

class usbInterface
{
public:
    enum eUSBstate
    {
        unknown,
        reset,
        configured,
        suspended,
        resumed
    };

private:
    typedef struct
    {
        cyg_uint32 bitrate;
        cyg_uint8  format;
        cyg_uint8  paritytype;
        cyg_uint8  datatype;
    }LINE_CODING;

    static usbInterface * __instance;

    eUSBstate mUSBstatus;

    cyg_mutex_t mRXmutex;
    cyg_cond_t mRXcond;

    ActivitySignaller * mSignaller;

    cyg_uint32 mRXin;
    cyg_uint32 mRXout;
    static cyg_uint8 mRXbuf[USBRXBUFF_SIZE] __attribute__((section(".ccm")));
    cyg_uint32 mRXlen;

    static void setSerialCoding(LINE_CODING * coding);
    static LINE_CODING getSerialCoding();

    void fillReceiveBuffer(cyg_uint8 *buf, cyg_uint8 len);

    usbInterface(cyg_uint32 serial);

public:
    static void init(cyg_uint32 serial);
    static usbInterface* get(){ return __instance; };

    //USB device function call backs
    static void     USBD_USR_Init(void);
    static void     USBD_USR_DeviceReset (cyg_uint8 speed);
    static void     USBD_USR_DeviceConfigured (void);
    static void     USBD_USR_DeviceSuspended(void);
    static void     USBD_USR_DeviceResumed(void);

    static void     USBD_USR_DeviceConnected(void);
    static void     USBD_USR_DeviceDisconnected(void);


    //Communication device class functions
    static cyg_uint16 cdc_Init();
    static cyg_uint16 cdc_DeInit(void);
    static cyg_uint16 cdc_Ctrl(cyg_uint32 Cmd, cyg_uint8* Buf, cyg_uint32 Len);
    static cyg_uint16 cdc_DataTx (cyg_uint8* Buf, cyg_uint32 Len);
    static cyg_uint16 cdc_rxData(cyg_uint8* Buf, cyg_uint32 Len);

    void setUSBstate(eUSBstate state);
    eUSBstate getUSBstate(){ return mUSBstatus; };

    cyg_bool transmit(cyg_uint8 * buf,cyg_uint32 len);
    cyg_uint32 receive(cyg_uint8 * buf,cyg_uint32 len);

    void setSignaller(ActivitySignaller * signaller){ mSignaller = signaller; };

    virtual ~usbInterface();
};

#endif /* USB_INTERFACE_H_ */
