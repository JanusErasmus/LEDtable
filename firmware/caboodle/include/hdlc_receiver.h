#ifndef INCLUDE_HDLC_RECEIVER_H_
#define INCLUDE_HDLC_RECEIVER_H_
#include <hdlc_framer.h>

class cHDLCreceiver
{
   cHDLCframer mFramer;

public:
   cHDLCreceiver(cyg_uint32 len);
   virtual ~cHDLCreceiver();

   void pack(cyg_uint8 *buff, cyg_uint32 len);
};

#endif /* INCLUDE_HDLC_RECEIVER_H_ */
