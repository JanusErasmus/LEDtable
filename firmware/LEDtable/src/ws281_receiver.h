#ifndef SRC_WS281_RECEIVER_H_
#define SRC_WS281_RECEIVER_H_
#include <hdlc_receiver.h>

class WS281receiver : public cHDLCreceiver
{
public:
   WS281receiver();
   virtual ~WS281receiver();

   void handleData(cyg_uint8 *buff, cyg_uint32 len);
};

#endif /* SRC_WS281_RECEIVER_H_ */
