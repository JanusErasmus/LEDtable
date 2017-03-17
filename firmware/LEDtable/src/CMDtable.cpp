#include <TermCMD.h>

#include "stm32cpu.h"

TermCMD::cmd_list_t *TermCMD::mCmdTable[] =
{
        (cmd_list_t*)shellCommands,
        (cmd_list_t*)cpuCommands,
        0
};
