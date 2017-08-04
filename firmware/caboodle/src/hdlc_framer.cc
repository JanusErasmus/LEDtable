#include <stdlib.h>
#include <cyg/kernel/diag.h>
#include <string.h>

#include <utils.h>
#include <hdlc_framer.h>
#include <crc.h>

const cyg_uint8 escapedChars[] =
{
      0x7E, 0x7D, 0x08, 0x7F
};


cHDLCframer::cHDLCframer(cyg_uint32 buff_len)
{
	mBuffer = (cyg_uint8*)malloc(buff_len);
	mLength = buff_len;
	mValidFrame = false;
	mEscapeChar = false;
	mFrameIdx = 0;
}

void cHDLCframer::frame(cyg_uint8 * payload_ptr,
						cyg_uint32 payload_length,
						cyg_uint8 * frame_ptr,
						cyg_uint32 * frame_length)
{
	// Perform CRC on payload
	cyg_uint16 crc;
    crc = cCrc::ccitt_crc16(payload_ptr, payload_length);
    crc ^= 0xFFFF;

	// Opening flag
	frame_ptr[0] = 0x7E;
	*frame_length = 1;

	// Add each byte of the payload
	for (cyg_uint32 cntr = 0; cntr < payload_length; cntr++)
		add_byte(*payload_ptr++, frame_ptr, frame_length);

	// Add FCS and flag bytes
	add_byte((crc >> 0) & 0xFF, frame_ptr, frame_length);
	add_byte((crc >> 8) & 0xFF, frame_ptr, frame_length);

	// Closing flag
	frame_ptr[*frame_length] = 0x7E;
	(*frame_length) += 1;
}

int cHDLCframer::pack(cyg_uint8 byte)
{
	int packedFrame = 0;

	if(!mValidFrame && mFrameIdx == 0)  //first byte
	{
		if(byte == 0x7E)//frame start
			mValidFrame = true;
	}
	else //middle or last bytes
	{
		if(byte == 0x7E)// && var->frameLen > 2)  //frame end
		{
			if(mFrameIdx < 3) //start of a new frame
				return 0;

			//diag_printf("Data received %d\n",frameLen);
			cyg_uint16 calc_crc = cCrc::ccitt_crc16(mBuffer,mFrameIdx);
			if(calc_crc == cCrc::GOOD_CRC)
			{
				//dataFrame Received
				packedFrame = mFrameIdx - 2;
			}
			else
			{
	            mFrameIdx = 0;
	            mValidFrame = false;
	            mEscapeChar = false;

				INFO_TRACE("cHDLCframer", "FrameErr\n");
				//diag_dump_buf(mBuffer,mFrameIdx);

				return -1;
			}
			mFrameIdx = 0;
			mValidFrame = false;
			mEscapeChar = false;
		}
		else if( byte == 0x7D)
		{
			mEscapeChar = true;  //escape next byte
		}
		else if(mEscapeChar)
		{
			mBuffer[mFrameIdx++] = byte  ^ 0x20; //xor to get original byte
			mEscapeChar = false;
		}
		else
		{
			mBuffer[mFrameIdx++] = byte;

			if(mFrameIdx >= mLength)
			{
				mFrameIdx = 0;
				mValidFrame = false;
			}
		}
	}

	return packedFrame;
}

/*cyg_uint32 cHDLCframer::unFrame(cyg_uint8* buff,
								cyg_uint32 len,
								cyg_uint8* returnBuff)
{
	cyg_bool await_new_frame = true;
	cyg_bool escaping = false;
	cyg_uint32 BuffLen = 0;

	for ( cyg_uint32 i = 0 ; i < len ; i++ )
	{
		//diag_printf( " 0x%02X", buff[i] );
		switch (buff[i])
		{
			// Receive a flag
			case 0x7E:
				// Is this the start of a new frame?
				if (await_new_frame)
				{
					// New frame has started
					await_new_frame = false;

					BuffLen = 0;
				} // if
				// Or was it the end of the current frame?
				else
				{
					// Current frame has ended
					await_new_frame = 1;
					// Process the received command
					//dbg_printf(2,"Buffer %x\n",BuffLen);
					//dbg_dump_buf(2,buff,BuffLen);
					return BuffLen;
			} // else
			break;

			// Receive a escape character
			case 0x7D:
				// Next byte will be escaped
				escaping = 1;
			break;

			default:
				// Should byte be escaped?
				if (escaping)	// Yes
				{
					// XOR with 0x20 to get original byte
					returnBuff[BuffLen++] = buff[i] ^ 0x20;
					// Done escaping
					escaping = 0;
				} // if
				else			// No
				{
					// Normal byte
					returnBuff[BuffLen++] = buff[i];
				} // else
			break;
		}
	}
	return BuffLen;
}*/

// --------------------------------------------------
// Add a byte to the frame, escaping as necessary
// --------------------------------------------------
void cHDLCframer::add_byte(cyg_uint8 byte, cyg_uint8  * frame_ptr, cyg_uint32 * frame_length)
{
    cyg_bool to_escape = false;

    for (cyg_uint32 k = 0 ; k < sizeof(escapedChars) ; k++)
    {
        if (byte == escapedChars[k])
        {
            to_escape = true;
        }
    }

    if (to_escape == true)
    {
		// Insert escape sequence
		frame_ptr[*frame_length] = 0x7D;
		(*frame_length)++;
		frame_ptr[*frame_length] = 0x20^byte;
		(*frame_length)++;
    }
    else
	{
		frame_ptr[*frame_length] = byte;
		(*frame_length)++;
	}
}

cHDLCframer::~cHDLCframer()
{
	free(mBuffer);
}
