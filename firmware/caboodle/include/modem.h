#ifndef MODEM_H_
#define MODEM_H_
#include "kses_term.h"

class cModem
{
public:
	cModem(){};
	virtual ~cModem(){};

	virtual cyg_bool power(cyg_bool stat){ return false; };
	virtual cyg_uint16 write(const char* str){ return 0; };
	virtual cyg_uint16 write(void* buff, cyg_uint32 len){ return 0; };

	virtual void ATcmd(cTerm & term, int argc,char * argv[]){};
};



#endif /* MODEM_H_ */
