#include <cyg/kernel/diag.h>
#include <stdio.h>

#include "../usb_dev/inc/usb_conf.h"
#include "../usb_dev/inc/usbd_desc.h"
#include "../usb_otg/inc/usbd_cdc_core.h"


#define INFO_TRACE(_class, _string, ...)    diag_printf( "%15s : " _string, _class, ##__VA_ARGS__)

#include <usb_interface.h>

#define USB_TRACE(_string, ...)	INFO_TRACE("USBiface", _string, ##__VA_ARGS__)

usbInterface * usbInterface::__instance = 0;
cyg_uint8 usbInterface::mRXbuf[USBRXBUFF_SIZE] __attribute__((section(".ccm")));

__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;
extern  USBD_DEVICE USR_desc;

extern cyg_uint8  APP_Rx_Buffer []; /* Write CDC received data in this buffer.
                                     These data will be sent over USB IN endpoint
                                     in the CDC core functions. */
extern cyg_uint32 APP_Rx_ptr_in;    /* Increment this pointer or roll it back to
                                     start address when writing received data
                                    in the buffer APP_Rx_Buffer. */

CDC_IF_Prop_TypeDef cdc_fops =
{
		usbInterface::cdc_Init,
		usbInterface::cdc_DeInit,
		usbInterface::cdc_Ctrl,
		usbInterface::cdc_DataTx,
		usbInterface::cdc_rxData
};

USBD_Usr_cb_TypeDef USR_cb =
{
		usbInterface::USBD_USR_Init,
		usbInterface::USBD_USR_DeviceReset,
		usbInterface::USBD_USR_DeviceConfigured,
		usbInterface::USBD_USR_DeviceSuspended,
		usbInterface::USBD_USR_DeviceResumed,

		usbInterface::USBD_USR_DeviceConnected,
		usbInterface::USBD_USR_DeviceDisconnected,

};


void usbInterface::init(cyg_uint32 serial)
{
	if(!__instance)
	{
		__instance = new usbInterface(serial);
	}
}

usbInterface::usbInterface(cyg_uint32 serial)
{
    mRXin = 0;
    mRXout = 0;
	mRXlen = 0;
	mUSBstatus = unknown;
	mSignaller = 0;

	cyg_mutex_init(&mRXmutex);
	cyg_cond_init(&mRXcond, &mRXmutex);

	USBD_Init(serial,
			&USB_OTG_dev,
			USB_OTG_FS_CORE_ID,
			&USR_desc,
			&USBD_CDC_cb,
			&USR_cb);

}

/**
 * @brief  USBD_USR_Init
 *         Displays the message on LCD for host lib initialization
 * @param  None
 * @retval None
 */
void  usbInterface::USBD_USR_Init(void)
{
	USB_TRACE("USR_init\n");
}

/**
 * @brief  USBD_USR_DeviceReset
 *         Displays the message on LCD on device Reset Event
 * @param  speed : device speed
 * @retval None
 */
void  usbInterface::USBD_USR_DeviceReset(cyg_uint8 speed )
{
	//	switch (speed)
	//	{
	//	case USB_OTG_SPEED_HIGH:
	//		diag_printf ("     USB Device Library v1.0.0 [HS]\n" );
	//		break;
	//
	//	case USB_OTG_SPEED_FULL:
	//		diag_printf("     USB Device Library v1.0.0 [FS]\n" );
	//		break;
	//	default:
	//		diag_printf("     USB Device Library v1.0.0 [??]\n" );
	//	}

	if(__instance)
		__instance->setUSBstate(reset);
}


/**
 * @brief  USBD_USR_DeviceConfigured
 *         Displays the message on LCD on device configuration Event
 * @param  None
 * @retval Staus
 */
void  usbInterface::USBD_USR_DeviceConfigured (void)
{
	if(__instance)
		__instance->setUSBstate(configured);
}

/**
 * @brief  USBD_USR_DeviceSuspended
 *         Displays the message on LCD on device suspend Event
 * @param  None
 * @retval None
 */
void  usbInterface::USBD_USR_DeviceSuspended(void)
{
	if(__instance)
		__instance->setUSBstate(suspended);
}


/**
 * @brief  USBD_USR_DeviceResumed
 *         Displays the message on LCD on device resume Event
 * @param  None
 * @retval None
 */
void  usbInterface::USBD_USR_DeviceResumed(void)
{
	if(__instance)
		__instance->setUSBstate(resumed);
}

/**
 * @brief  USBD_USR_DeviceConnected
 *         Displays the message on LCD on device connection Event
 * @param  None
 * @retval Staus
 */
void  usbInterface::USBD_USR_DeviceConnected (void)
{
	USB_TRACE("Connected.\n");
}


/**
 * @brief  USBD_USR_DeviceDisonnected
 *         Displays the message on LCD on device disconnection Event
 * @param  None
 * @retval Staus
 */
void  usbInterface::USBD_USR_DeviceDisconnected (void)
{
	USB_TRACE("Disconnected.\n");
}

/**
 * @brief  cdc_Init
 *         Initializes the Media on the STM32
 * @param  None
 * @retval Result of the opeartion (USBD_OK in all cases)
 */
cyg_uint16 usbInterface::cdc_Init()
{
	USB_TRACE("cdc init\n");

	return USBD_OK;
}


cyg_uint16 usbInterface::cdc_DeInit(void)
{
	USB_TRACE("De- initialized\n");

	return USBD_OK;
}

usbInterface::LINE_CODING usbInterface::getSerialCoding()
{
    LINE_CODING coding = {
            115200,
            0,
            0,
            8,
    };

    diag_printf("Get cyg_serial_info_t NOT implemented\n");

    return coding;
}

void usbInterface::setSerialCoding(LINE_CODING * coding)
{
    diag_printf("Coding:\n");
    diag_printf(" - baud %d\n", coding->bitrate);
    diag_printf(" - format %d\n", coding->format);
    diag_printf(" - parity %d\n",  coding->paritytype);
    diag_printf(" - data %d\n", coding->datatype);


}

/**
 * @brief  cdc_Ctrl
 *         Manage the CDC class requests
 * @param  Cmd: Command code
 * @param  Buf: Buffer containing command data (request parameters)
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the opeartion (USBD_OK in all cases
 */
//not really necessary for this example
cyg_uint16 usbInterface::cdc_Ctrl (cyg_uint32 Cmd, cyg_uint8* Buf, cyg_uint32 Len)
{
	//USB_TRACE("cdc_Ctrl %02X, len %d\n", Cmd, Len);

	switch (Cmd)
	{
	case SEND_ENCAPSULATED_COMMAND:
		/* Not  needed for this driver */
		break;

	case GET_ENCAPSULATED_RESPONSE:
		/* Not  needed for this driver */
		break;

	case SET_COMM_FEATURE:
		/* Not  needed for this driver */
		break;

	case GET_COMM_FEATURE:
		/* Not  needed for this driver */
		break;

	case CLEAR_COMM_FEATURE:
		/* Not  needed for this driver */
		break;

	case SET_LINE_CODING:
		//USB_TRACE("set line coding\n");
		{
		usbInterface::LINE_CODING linecoding;

		linecoding.bitrate = (cyg_uint32)(Buf[0] | (Buf[1] << 8) | (Buf[2] << 16) | (Buf[3] << 24));
		linecoding.format = Buf[4];
		linecoding.paritytype = Buf[5];
		linecoding.datatype = Buf[6];

		/* Set the new configuration */
        if(__instance)
            setSerialCoding(&linecoding);
	}
		break;

	case GET_LINE_CODING:
		//USB_TRACE("get line coding\n");
		{
		usbInterface::LINE_CODING linecoding =
		        {
		           115200,
		           0,0,8
		        };

		if(__instance)
		    linecoding = __instance->getSerialCoding();

		Buf[0] = (cyg_uint8)(linecoding.bitrate);
		Buf[1] = (cyg_uint8)(linecoding.bitrate >> 8);
		Buf[2] = (cyg_uint8)(linecoding.bitrate >> 16);
		Buf[3] = (cyg_uint8)(linecoding.bitrate >> 24);
		Buf[4] = linecoding.format;
		Buf[5] = linecoding.paritytype;
		Buf[6] = linecoding.datatype;
		}
		break;

	case SET_CONTROL_LINE_STATE:
		/* Not  needed for this driver */
		break;

	case SEND_BREAK:
		/* Not  needed for this driver */
		break;

	default:
		break;
	}

	return USBD_OK;
}


/**
 * @brief  cdc_DataTx
 *         CDC received data to be send over USB IN endpoint are managed in
 *         this function.
 * @param  Buf: Buffer of data to be sent
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the opeartion: USBD_OK
 */
cyg_uint16 usbInterface::cdc_DataTx (cyg_uint8* Buf, cyg_uint32 Len)
{
	cyg_uint32 i;
	//loop through buffer
	for( i = 0; i < Len; i++ )
	{
		//push data into transfer buffer
		APP_Rx_Buffer[APP_Rx_ptr_in] = Buf[i] ;
		//increase pointer value
		APP_Rx_ptr_in++;
		/* To avoid buffer overflow */
		if(APP_Rx_ptr_in == APP_RX_DATA_SIZE)
		{
			APP_Rx_ptr_in = 0;
		}
	}

	return USBD_OK;
}

cyg_uint32 usbInterface::receive(cyg_uint8 * buf,cyg_uint32 len)
{
	cyg_mutex_lock(&mRXmutex);
	while(!mRXlen)
	    cyg_cond_wait(&mRXcond);

	cyg_scheduler_lock();
	mRXlen = 0;

	cyg_uint32 txLength = 0;

	while(mRXout != mRXin)
	{
	    if(mRXout < mRXin)
	    {
	        txLength = mRXin - mRXout;
	        if(txLength > len)
	            txLength = len;

	        memcpy(buf, &mRXbuf[mRXout], txLength);


	        mRXout += txLength;
	        if(mRXout >= USBRXBUFF_SIZE)
	        {
	            mRXout = 0;
	            break;
	        }
	    }
	    else
	    {
	        txLength = USBRXBUFF_SIZE - mRXout;


            memcpy(buf, &mRXbuf[mRXout], txLength);

            cyg_uint32 headLength = mRXin;
            memcpy(&buf[txLength], mRXbuf, headLength);
            txLength += headLength;


            mRXout = mRXin;

	    }
//	    diag_printf("usbInterface: RX CDC %d\n", txLength);
//	    diag_printf("usbInterface: mRXout %d\n", mRXout);
//	    diag_printf("usbInterface: mRXin %d\n", mRXin);
	}

	if(mSignaller)
	    mSignaller->signalRx();

	cyg_scheduler_unlock();
	cyg_mutex_unlock(&mRXmutex);

	return txLength;
}

cyg_bool usbInterface::transmit(cyg_uint8 * buf,cyg_uint32 len)
{
	if(mUSBstatus != configured)
		return 0;

	cyg_uint32 sentLen = 0;
	cyg_uint32 transmitLen = len;
	while(transmitLen)
	{
	    cyg_uint32 txLen = 0;

		//transmit packets of 63 bytes;
		if(transmitLen > CDC_DATA_MAX_PACKET_SIZE)
			txLen = CDC_DATA_MAX_PACKET_SIZE;
		else
			txLen = transmitLen;

		//diag_printf("usbInterface: TX CDC %d\n", txLen);
		if(cdc_DataTx(&buf[sentLen], txLen) != USBD_OK)
			break;

		transmitLen -= txLen;
		sentLen += txLen;
	}

    if(mSignaller)
        mSignaller->signalTx();

	return sentLen;
}

cyg_uint16 usbInterface::cdc_rxData(cyg_uint8* buf, cyg_uint32 len)
{
	if(!__instance)
		return USBD_FAIL;

    //diag_printf("cdc rx %d\n", len);

    cyg_uint32 i;
    //loop through buffer
    for( i = 0; i < len; i++ )
    {
        //push data into transfer buffer
        __instance->mRXbuf[__instance->mRXin] = buf[i] ;

        //increase pointer value
        __instance->mRXin++;
        /* To avoid buffer overflow */
        if(__instance->mRXin >= USBRXBUFF_SIZE)
        {
            __instance->mRXin = 0;
        }
    }
//
	__instance->mRXlen = len;
//	memcpy(__instance->mRXbuf, buf, len);
	cyg_cond_signal(&(__instance->mRXcond));

	return USBD_OK;
}

void usbInterface::setUSBstate(usbInterface::eUSBstate state)
{
	//diag_printf("Setting state %d\n", state);
	switch(state)
	{

	case reset:
	{
		USB_TRACE("Reset\n");
	}
	break;
	case configured:
	{
		USB_TRACE("Configured.\n");
	}
	break;
	case suspended:
	{
		USB_TRACE("Suspend Mode.\n");
	}
	break;
	case resumed:
	{
		USB_TRACE("Resumed\n");
	}
	break;

	default:
		break;
	}
	mUSBstatus = state;
}


usbInterface::~usbInterface()
{
}

