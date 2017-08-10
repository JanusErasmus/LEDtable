#include <hdlc_receiver.h>
#include <utils.h>

#define TRACE(_x, ...) INFO_TRACE("cHDLCreceiver", _x,  ##__VA_ARGS__)

cHDLCreceiver::cHDLCreceiver(cyg_uint32 len) : mFramer(len)
{

}

void cHDLCreceiver::pack(cyg_uint8 *buff, cyg_uint32 len)
{
   for(cyg_uint32 k = 0; k < len; k++)
   {
      int rxLen = mFramer.pack(buff[k]);
      if(rxLen > 0)
      {
         cyg_uint8 *p = mFramer.buffer();
         cyg_uint32 frameLen = 0;
         cyg_uint8 data[1024];
         for(int k = 0; k < rxLen; k++)
         {
            if(p[k] == 0x7D)
            {
               data[frameLen++] = p[++k] ^ 0x20;
            }
            else
               data[frameLen++] = p[k];
         }
         handleData(data, frameLen);
      }
   }
}

void cHDLCreceiver::handleData(cyg_uint8 *buff, cyg_uint32 len)
{
   TRACE("RX %d\n", len);
   diag_dump_buf(buff, len);
}

cHDLCreceiver::~cHDLCreceiver()
{
}

