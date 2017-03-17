#ifndef _INIT_H_
#define _INIT_H_
#include <cyg/kernel/kapi.h>

#include "definitions.h"
#include "kses_term.h"
#include "debug.h"


class cInit : public cDebug
{
    static cInit * __instance;

    cyg_uint8 mLEDStack[INIT_STACK_SIZE];
    cyg_thread mLEDThread;
    cyg_handle_t mLEDThreadHandle;
    static void init_thread(cyg_addrword_t args);

public:

    cInit();

};

#endif //Include Guard

