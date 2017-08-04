#include <TermCMD.h>

#include "stm32cpu.h"
#include "ws281x_driver.h"

TermCMD::cmd_list_t *TermCMD::mCmdTable[] =
{
        (cmd_list_t*)shellCommands,
        (cmd_list_t*)cpuCommands,
		(cmd_list_t*)wsCommands,
        0
};
