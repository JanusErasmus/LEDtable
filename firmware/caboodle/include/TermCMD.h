#ifndef TERMCMD_H_
#define TERMCMD_H_
#include <cyg/hal/hal_tables.h>
#include <cyg/kernel/kapi.h>
#include "debug.h"
#include "kses_term.h"
#include "modem.h"

class TermCMD
{
    static cModem * mMdm;

public:
    typedef void (*func_t)(cTerm & t,int argc,char *argv[]);

    typedef struct
    {
        char* cmd;
        char* argDesc;
        char* desc;
        func_t f;
    } cmd_list_t;

    static cmd_list_t *mCmdTable[];

    TermCMD();
    static void setModem(cModem * mdm){ mMdm = mdm; };

    static void process(cTerm & term, int argc,char * argv[]);
    static void help(cTerm & t,int argc,char *argv[]);
};

extern const TermCMD::cmd_list_t shellCommands[];

class System : cDebug
{
public:
    static void setDebugLvl(cTerm & term, int argc,char * argv[]);
};
#endif /* TERMCMD_H_ */
