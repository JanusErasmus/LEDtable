#ifndef STM32CPU_H_
#define STM32CPU_H_

#include "kses_term.h"

class stm32cpu
{
public:

    static cyg_uint64 uniqueID();
    static cyg_uint16 flashSize();
    static cyg_uint16 ramSize();
    static void info();
    static void sysResetStatus();
   static void pStatus(cKSESterminal & term, int argc,char * argv[]);
};

#endif /* STM32CPU_H_ */
