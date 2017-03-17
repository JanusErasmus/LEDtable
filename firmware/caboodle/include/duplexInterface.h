#ifndef DUPLEXINTERFACE_H_
#define DUPLEXINTERFACE_H_
#include <cyg/kernel/kapi.h>

#include <string.h>

class duplexInterface
{
public:
    duplexInterface(){}

    virtual cyg_uint32 transaction(cyg_uint8* buffer, cyg_uint32 data_len, cyg_uint32 buffer_len, cyg_tick_count_t timeout = 500) = 0;
    virtual cyg_uint32 transmit(cyg_uint8* buffer, cyg_uint32 len) = 0;

    virtual ~duplexInterface(){}
};

#endif /* DUPLEXINTERFACE_H_ */
