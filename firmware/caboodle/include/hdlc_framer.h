#ifndef SRC_HDLC_FRAMER_H_
#define SRC_HDLC_FRAMER_H_
#include <cyg/kernel/kapi.h>

class cHDLCframer
{
	cyg_uint8 *mBuffer;
	cyg_uint32 mLength;
	cyg_uint32 mFrameIdx;
	cyg_bool mValidFrame;
	cyg_bool mEscapeChar;

	// --------------------------------------------------
    // Add a byte to a HDLC frame,
    // --------------------------------------------------
    static void add_byte(cyg_uint8 byte, cyg_uint8  * frame_ptr, cyg_uint32 * frame_length);

public:
	// ----------------- cHDLCframer ---------------------------
	// Use pack() to start packing bytes of an HDLC frame into buff_ptr
	//
	// When pack returns a value, a valid frame has been received
	cHDLCframer(cyg_uint32 buff_len);
	virtual ~cHDLCframer();

	// --------------------------------------------------
	// Create an HDLC frame from input pay load
	// --------------------------------------------------
	// payload_ptr points to a buffer which should contain
	// at least two command bytes. Up to 64 additional
	// data bytes may also be contained within the buffer,
	// but they are optional.
	// --------------------------------------------------
	// payload_length specifies how many bytes are in the
	// pay load buffer
	// --------------------------------------------------
	// frame_ptr points to a buffer which should be 138 bytes.
	// The pay load will be encapsulated within this frame,
	// along with two flag bytes and two FCS bytes.
	// --------------------------------------------------
	// frame_length returns the number of bytes within the frame
	// --------------------------------------------------
    static void frame(cyg_uint8 * payload_ptr,
    						cyg_uint32 payload_length,
							cyg_uint8 * frame_ptr,
							cyg_uint32 * frame_length);

    // --------------------------------------------------
	// Get the data in an HDLC frame buff with len length
	// --------------------------------------------------
   /* static cyg_uint32 unFrame(cyg_uint8* buff,
    							cyg_uint32 len,
								cyg_uint8* returnBuff);*/

    // --------------------------------------------------
	// Pack bytes from a input device into a HDLC frame
	// --------------------------------------------------
    // - Frames are packed in mBuffer, starting with a 0x7E and ended with a 0x7E,
	// - The last two bytes of the frame are the crc16 bytes
	// - Once a complete valid frame is packed, it returns the frame length
	//   and it is available where mBuffer pointed to
	// - If the frame had errors it returns -1
    int pack(cyg_uint8 byte);

    cyg_uint8* buffer(){ return mBuffer; };
};

#endif /* SRC_HDLC_FRAMER_H_ */
