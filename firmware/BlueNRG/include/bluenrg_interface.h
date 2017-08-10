#ifndef SRC_BLUENRG_INTERFACE_H_
#define SRC_BLUENRG_INTERFACE_H_
#include <bluenrg.h>
#include <hdlc_framer.h>

class cBlueNRGInterface : public cBlueNRGlistener
{
protected:
   cBlueNRGchar *mIFchar;

   cyg_mutex_t mMutex;
   cyg_cond_t mCondition;
   cyg_uint8 *mBuffer;
   cHDLCframer *mFramer;
   cyg_uint32 mLength;
   cyg_uint32 mRXlength;

   void handleData(uint8_t *data, uint8_t len);

   void handlePlainData(uint8_t *data, uint8_t len);
   void handleHDLCData(uint8_t *data, uint8_t len);

public:
   cBlueNRGInterface(cBlueNRGchar *ifChar, cyg_uint32 max_length, cyg_bool hdlc = false);
   virtual ~cBlueNRGInterface();

   cyg_uint32 transaction(cyg_uint8* buffer, cyg_uint32 data_len, cyg_uint32 buffer_len, cyg_tick_count_t timeout = 500);
   cyg_uint32 transmit(cyg_uint8* buffer, cyg_uint32 len);

};

#endif /* SRC_BLUENRG_INTERFACE_H_ */
