#include <cyg/kernel/kapi.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/hal/hal_diag.h>
#include <cyg/kernel/diag.h>

#include "stm32cpu.h"
#include "init.h"

void * _impure_ptr;    // g++ compatibility

extern "C" void cyg_user_start(void);

cyg_uint32 unitVersion = 0x000100;

void cyg_user_start(void)
{
    diag_printf("LED table\nCompiled on %s %s\n", __DATE__, __TIME__);
    stm32cpu::info();
    stm32cpu::sysResetStatus();

    new cInit();
}
