#ifndef _CRC_H_
#define _CRC_H_

class cCrc
{
	static cyg_uint8 polynomial;
    static const cyg_uint16 cCrcTable[256];

public:
    static cyg_uint8 crc8(cyg_uint8 * data,cyg_uint32 len);
	static cyg_uint8 crc8_update(cyg_uint8 data,cyg_uint8 crc);
	static cyg_uint16 ccitt_crc16(cyg_uint8 data,cyg_uint16 crc_acc);
    static cyg_uint16 ccitt_crc16(cyg_uint8 * data_ptr,cyg_uint32 len);

	static const cyg_uint16 INIT_CRC;
    static const cyg_uint16 GOOD_CRC;

    static void setPoly(cyg_uint8 p){ polynomial = p; }
};




#endif //Include Guard
