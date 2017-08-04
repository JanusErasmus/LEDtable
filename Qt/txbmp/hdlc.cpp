#include "hdlc.h"
#include "stdio.h"

uint8_t cCrc::crc8_update(uint8_t data, uint8_t crc)
{
   uint8_t i;

   crc = crc ^ data;
   for (i = 0; i < 8; i++)
   {
       if (crc & 0x01)
           crc = (crc >> 1) ^ 0x8C;
       else
           crc >>= 1;
   }

   return crc;

}

uint8_t cCrc::crc8(uint8_t * data_ptr, uint32_t len)
{
   uint8_t crc = 0;
   while(len--)
   {
      crc = crc8_update(*data_ptr,crc);
      data_ptr++;
   }
   return crc;
}

/** Returns the checksum according to the lookup table
@param data cCrcTable index of the value to lookup.
@param crc Input value
@retval Calculated check sum
*/
unsigned short int checkSum::ccitt_crc16( char data, unsigned short int crc)
{
    return ((crc >> 8) ^ cCrcTable[(crc ^ data) & 0xff]);
}

/** Calculates the chechsum of the input buffer.
@param data_ptr The data for which the check sum should be calculated.
@param the length of data_ptr.
@retval crc Calculated checksum.
*/
unsigned short int checkSum::ccitt_crc16(unsigned char * data_ptr, unsigned long int len)
{
   unsigned short int crc = INIT_CRC;
   while(len--)
   {
      crc = ccitt_crc16(*data_ptr,crc);
      data_ptr++;
   }
   return crc;
}

/** Create an HDLC frame for input payload
*
//@param payload_ptr Points to a buffer which should contain at least two command bytes. Up to 64 additional
// data bytes may also be contained within the buffer, but they are optional.
//
//@param payload_length Specifies how many bytes are in the payload buffer.
//
//@param frame_ptr Points to a buffer which should be 138 bytes.\n
// The payload will be encapsulated within this frame, along with two flag bytes and two FCS bytes.\n
//
//@retval frame_length Returns the number of bytes within the frame.
//
*/
void HDLC::HDLC_Frame(unsigned char * payload_ptr, int payload_length,
				unsigned char * frame_ptr, int * frame_length)
{

	

	// Perform CRC on payload
    unsigned short int crc = checkSum::ccitt_crc16(payload_ptr, payload_length);
	crc ^= 0xffff;

	// Opening flag
	frame_ptr[0] = 0x7E;
	*frame_length = 1;

	// Add each byte of the payload
	for (int cntr = 0; cntr < payload_length; cntr++)
		add_byte(*payload_ptr++, frame_ptr, frame_length);

	// Add FCS and flag bytes
	add_byte((crc >> 0) & 0xFF, frame_ptr, frame_length);
	add_byte((crc >> 8) & 0xFF, frame_ptr, frame_length);

	// Closing flag
	frame_ptr[*frame_length] = 0x7E;
    (*frame_length) += 1;

} // HDLC_Frame


/**
* Add a byte to the HDLC frame, escaping as necessary
*/
void HDLC::add_byte(unsigned char byte, unsigned char  * frame_ptr, int * frame_length)
{
	// Flag byte?
	if (byte == 0x7E)
	{
		// Escape the flag sequence
		frame_ptr[*frame_length] = 0x7D;
		(*frame_length)++;
		frame_ptr[*frame_length] = 0x5E;
		(*frame_length)++;
	} // if
	// Escape appears within payload
	else if (byte == 0x7D)
	{
		// Escape the escape sequence
		frame_ptr[*frame_length] = 0x7D;
		(*frame_length)++;
		frame_ptr[*frame_length] = 0x5D;
		(*frame_length)++;
	} // else if
	else
	{
		frame_ptr[*frame_length] = byte;
		(*frame_length)++;
	} // else
} // add_byte


/** Add a byte to a HDLC frame,
 *  - Frames are packed starting with a 0x7E and ended with a 0x7E,
 *  - The last two bytes of the frame are the crc16 bytes
 */
bool HDLC::HDLC_pack(char byte, unsigned char * buff,int * len, unsigned int maxBuffLen, sFrameVar* var)
{
    bool stat = false;

    if(!var->validFrame && var->frameLen == 0)  //first byte
    {
        if(byte == 0x7E)//frame start
            var->validFrame = true;
    }
    else //middle or last bytes
    {
        if(byte == 0x7E && var->frameLen > 2)  //frame end
        {
            //diag_printf("Data received %d\n",frameLen);
            //diag_dump_buf(mRXbuff,frameLen-2);

            unsigned short int calc_crc = checkSum::ccitt_crc16(buff,var->frameLen);
            //diag_printf("crc calculated 0x%X\n",calc_crc);
            if(calc_crc == GOOD_CRC)
            {
                var->frameLen-=2;
                *len = var->frameLen;
                //diag_printf("handle data \n");
                //dataFrame Received
                stat = true;
            }
            else
            {
                printf("\x1b[5m");
                printf("frameErr\n");
                //diag_dump_buf(buff,var->frameLen);
                printf("\x1b[m");
            }
            var->frameLen = 0;
            var->validFrame = false;
        }
        else if( byte == 0x7D)
        {
            var->escapeChar = true;  //escape next byte
        }
        else if(var->escapeChar)
        {
            buff[var->frameLen++] = byte  ^ 0x20; //xor to get original byte
            var->escapeChar = false;
        }
        else
        {
            buff[var->frameLen++] = byte;

            if(var->frameLen >= maxBuffLen)
            {
                var->frameLen = 0;
                var->validFrame = false;
            }
        }
    }

    return stat;
}
