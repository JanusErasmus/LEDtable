#ifndef _CRC_H_
#define _CRC_H_
#include <cyg/kernel/diag.h>

class cCrc
{
    static const cyg_uint16 cCrcTable[256];

    static void add_byte(cyg_uint8 byte, cyg_uint8  * frame_ptr, cyg_uint32 * frame_length);

public:
    static cyg_uint8 crc8(cyg_uint8 * data,cyg_uint32 len);
    static cyg_uint8 crc8_update(cyg_uint8 data,cyg_uint8 crc);
    static cyg_uint16 ccitt_crc16(cyg_uint8 data,cyg_uint16 crc_acc);
    static cyg_uint16 ccitt_crc16(cyg_uint8 * data_ptr,cyg_uint32 len);

    static const cyg_uint16 INIT_CRC;
    static const cyg_uint16 GOOD_CRC;
};




#endif //Include Guard
