#ifndef STM32CPU_H_
#define STM32CPU_H_

#include "TermCMD.h"

class stm32cpu
{
public:

    static cyg_uint64 uniqueID();
    static cyg_uint16 flashSize();
    static cyg_uint16 ramSize();
    static void info();
    static void sysResetStatus();
   static void pStatus(cTerm & term, int argc,char * argv[]);
};

extern const TermCMD::cmd_list_t cpuCommands[];
#endif /* STM32CPU_H_ */
