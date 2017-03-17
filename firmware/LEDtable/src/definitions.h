#ifndef DEFINITIONS_H
#define    DEFINITIONS_H
#include <cyg/hal/hal_arch.h>

//Stack and thread definitions
#define SER_PRIOR   15
#define INIT_PRIOR  19
#define WD_PRIOR    20
#define LED_PRIOR   24
#define TERM_PRIOR  25

//Stack thread sizes
#define INIT_STACK_SIZE     CYGNUM_HAL_STACK_SIZE_TYPICAL
#define TERM_STACK_SIZE     CYGNUM_HAL_STACK_SIZE_TYPICAL
#define SER_STACK_SIZE      CYGNUM_HAL_STACK_SIZE_MINIMUM
#define LED_STACK_SIZE      CYGNUM_HAL_STACK_SIZE_MINIMUM
#define WD_STACK_SIZE       CYGNUM_HAL_STACK_SIZE_MINIMUM

#define KMSG_BUFF_SIZE 1024

//Test points
#define TP1            CYGHWR_HAL_STM32_GPIO( C,12, IN , PULLDOWN)
#define TP2            CYGHWR_HAL_STM32_GPIO( D, 4, IN , PULLDOWN)

#endif    /* DEFINITIONS_H */

