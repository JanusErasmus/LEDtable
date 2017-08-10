#include <stdlib.h>
#include <string.h>

#include <hdlc_framer.h>

#include "utils.h"
#include "bluenrg_char.h"

extern "C" {
#include <bluenrg_gatt_aci.h>
}

#define NOTIFY_DELAY 5

#include "bluenrg_service.h"

#define TRACE(_x, ...) INFO_TRACE("cBlueNRGchar", _x,  ##__VA_ARGS__)

cBlueNRGchar::cBlueNRGchar(const uint8_t *uuid, uint8_t properties, uint8_t maxLen) :
                              mServiceHandle(0),
                              mNotify(0),
                              mListener(0),
                              mReader(0),
                              UUID(uuid),
                              Properties(properties),
                              Handle(0),
                              Length(maxLen)
{
}

void cBlueNRGchar::setServiceHandle(uint16_t handle)
{
   mServiceHandle = handle;
}

void cBlueNRGchar::modified(uint8_t *data, uint8_t len)
{
   //TRACE("%02X modified %d\n", UUID[15], len);

   if(mListener)
      mListener->handleData(data, len);
}

void cBlueNRGchar::setNotification(uint8_t state)
{
   TRACE("Set notification[%02X]: %d\n", UUID[15], state);

   mNotify = state;
}

tBleStatus cBlueNRGchar::update(uint8_t *data)
{
   if(!data)
      return ERR_COMMAND_DISALLOWED;

   if(mServiceHandle && Handle && mNotify)
   {
      //TRACE("Send notification(%dbytes) S%d %02X\n", Length, mServiceHandle, UUID[15]);
      //diag_dump_buf(data, Length);

      tBleStatus ret =  aci_gatt_update_char_value(mServiceHandle, Handle, 0, Length, data);
      cyg_thread_delay(NOTIFY_DELAY);

      return ret;
   }

   return ERR_COMMAND_DISALLOWED;
}

tBleStatus cBlueNRGchar::updateHDLC(uint8_t *data, uint32_t len)
{
   if(!data)
      return ERR_COMMAND_DISALLOWED;

   if(len && mServiceHandle && Handle && mNotify)
   {
      tBleStatus ret = BLE_STATUS_SUCCESS;
      uint32_t frameLen = len << 1;
      cyg_uint8 *frameBuff = (uint8_t*)malloc(frameLen);
      memset(frameBuff, 0, frameLen);
      cHDLCframer::frame(data, len, frameBuff, &frameLen);

      uint16_t idx = 0;
      while((ret == BLE_STATUS_SUCCESS) && frameLen)
      {
         uint16_t txLen = frameLen;
         if(txLen > Length)
            txLen = Length;

         ret = aci_gatt_update_char_value(mServiceHandle, Handle, 0, Length, &frameBuff[idx]);
         cyg_thread_delay(NOTIFY_DELAY);

         frameLen -= txLen;
         idx += txLen;

      }
      free(frameBuff);

      return ret;

   }
   return ERR_COMMAND_DISALLOWED;
}

tBleStatus cBlueNRGchar::readRequest(uint8_t length, uint16_t offset)
{
   TRACE("Read  S%d %02X\n",  mServiceHandle, UUID[15]);
   cyg_thread_delay(NOTIFY_DELAY);

   tBleStatus ret = BLE_STATUS_SUCCESS;

   if(mServiceHandle && Handle && mReader)
   {
      uint8_t *buff = (uint8_t*)malloc(1024);
      uint32_t len = mReader->readData(buff, 1024);

      if(len)
      {
         uint32_t frameLen = len << 1;
         uint8_t *frameBuff = (uint8_t*)malloc(frameLen);
         memset(frameBuff, 0, frameLen);
         cHDLCframer::frame(buff, len, frameBuff, &frameLen);

         uint16_t idx = 0;
         while((ret == BLE_STATUS_SUCCESS) && frameLen)
         {
            uint16_t txLen = frameLen;
            if(txLen > Length)
               txLen = Length;

            ret = aci_gatt_update_char_value(mServiceHandle, Handle, 0, Length, &frameBuff[idx]);
            cyg_thread_delay(NOTIFY_DELAY);

            frameLen -= txLen;
            idx += txLen;

         }
         free(frameBuff);
      }
      free(buff);

      return ret;
   }

   return ERR_COMMAND_DISALLOWED;
}

cBlueNRGchar::~cBlueNRGchar()
{
}

