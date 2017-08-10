#include <stdlib.h>
#include <string.h>

#include "bluenrg_interface.h"

#define TRACE(_x, ...) INFO_TRACE("cBlueNRGIF", _x,  ##__VA_ARGS__)

cBlueNRGInterface::cBlueNRGInterface(cBlueNRGchar *ifChar, cyg_uint32 max_length, cyg_bool hdlc)
{
   mRXlength = 0;
   mIFchar = ifChar;
   mLength = max_length;
   mBuffer = (cyg_uint8*)malloc(mLength);
   mFramer = 0;

   if(hdlc)
      mFramer = new cHDLCframer(mLength);

   cyg_mutex_init(&mMutex);
   cyg_cond_init(&mCondition, &mMutex);

   mIFchar->setListener(this);
}

void cBlueNRGInterface::handleData(uint8_t *data, uint8_t len)
{
   if(mFramer)
      handleHDLCData(data, len);
   else
      handlePlainData(data, len);
}

void cBlueNRGInterface::handleHDLCData(uint8_t *data, uint8_t len)
{
   cyg_mutex_lock(&mMutex);

   for(cyg_uint8 k = 0; k < len; k++)
   {
      int frameLen = mFramer->pack(data[k]);
      if(frameLen > 0)
      {
         //TRACE("Valid frame\n");

         //TRACE("RX %d\n", frameLen);
         cyg_uint8 *frame = mFramer->buffer();
         //diag_dump_buf(frame, frameLen);
         if((cyg_uint32)frameLen < mLength)
         {
            memcpy(mBuffer, frame, frameLen);
            mRXlength = frameLen;
         }
         else
         {
            mRXlength = 0;
         }
         cyg_cond_signal(&mCondition);

      }
   }

   cyg_mutex_unlock(&mMutex);
}

void cBlueNRGInterface::handlePlainData(uint8_t *data, uint8_t len)
{
   cyg_mutex_lock(&mMutex);
   if(len < mLength)
   {
      memcpy(mBuffer, data, len);
      mRXlength = len;
   }
   else
   {
      mRXlength = 0;
   }
   cyg_cond_signal(&mCondition);
   cyg_mutex_unlock(&mMutex);

}

cyg_uint32 cBlueNRGInterface::transaction(cyg_uint8* buffer, cyg_uint32 data_len, cyg_uint32 buffer_len, cyg_tick_count_t timeout)
{
   cyg_uint32 rxLen = 0;
   cyg_mutex_lock(&mMutex);


   if(mFramer)
      mIFchar->updateHDLC(buffer, data_len);
   else
      mIFchar->update(buffer);

   if(cyg_cond_timed_wait(&mCondition, cyg_current_time() + timeout))
   {
      if(mRXlength && (mRXlength < buffer_len))
      {
         memcpy(buffer, mBuffer, mRXlength);
         rxLen = mRXlength;
      }
   }
   cyg_mutex_unlock(&mMutex);

   return rxLen;
}

cyg_uint32 cBlueNRGInterface::transmit(cyg_uint8* buffer, cyg_uint32 len)
{
   tBleStatus status = ERR_COMMAND_DISALLOWED;

   cyg_mutex_lock(&mMutex);

   //TRACE("TX %d\n", len);
   if(mFramer)
      status = mIFchar->updateHDLC(buffer, len);
   else
      status = mIFchar->update(buffer);

   cyg_mutex_unlock(&mMutex);

   if(status == BLE_STATUS_SUCCESS)
      return len;

   return 0;
}

cBlueNRGInterface::~cBlueNRGInterface()
{
   free(mBuffer);
}

