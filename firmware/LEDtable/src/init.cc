#include <cyg/kernel/diag.h>
#include <cyg/hal/hal_diag.h>
#include <cyg/kernel/kapi.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <cyg/io/ttyio.h>
#include <cyg/hal/var_io.h>
#include <cyg/error/strerror.h>
#include <cyg/io/flash.h>

#include "init.h"
#include "led.h"
#include "kses_term.h"
#include "output_port.h"
#include "spi_dev.h"
#include "ws281x_driver.h"
#include "BigSpiralAnimation.h"
#include "ColorAnimation.h"
#include "GoombaAnimation.h"
#include "SpiralsAnimation.h"

#define TRACE(_x, ...) INFO_TRACE("cInit", _x,  ##__VA_ARGS__)

cInit * cInit::__instance = 0;

cInit::cInit() : cDebug("init")
{
    __instance = this;

    cyg_thread_create(15,
            init_thread,
            (cyg_addrword_t)this,
            (char *)"cInit",
            mLEDStack,
            CYGNUM_HAL_STACK_SIZE_TYPICAL,
            &mLEDThreadHandle,
            &mLEDThread);
    cyg_thread_resume(mLEDThreadHandle);

    //diag_printf("pixelCount %d\n", mBitCount);
    //diag_dump_buf(mBuffer, mBitCount);

}

void cInit::init_thread(cyg_addrword_t args)
{
    cLED *statusLED  = new cLED(CYGHWR_HAL_STM32_PIN_OUT(A,  5, PUSHPULL, NONE, 2MHZ), 0);

    cLED *ledList[] = {
            statusLED,
    };
    cLEDdriver::init(ledList, 1);

    cyg_uint32 outputPortNumbers[] =
    {
          CYGHWR_HAL_STM32_PIN_OUT(C,  0, OPENDRAIN, NONE, 50MHZ),
          CYGHWR_HAL_STM32_PIN_OUT(C,  1, OPENDRAIN, NONE, 50MHZ),
          CYGHWR_HAL_STM32_PIN_OUT(C,  2, OPENDRAIN, NONE, 50MHZ),
          CYGHWR_HAL_STM32_PIN_OUT(C,  3, OPENDRAIN, NONE, 50MHZ),
          CYGHWR_HAL_STM32_PIN_OUT(C,  4, OPENDRAIN, NONE, 50MHZ),
          CYGHWR_HAL_STM32_PIN_OUT(C,  5, OPENDRAIN, NONE, 50MHZ),
          CYGHWR_HAL_STM32_PIN_OUT(C,  6, OPENDRAIN, NONE, 50MHZ),
          CYGHWR_HAL_STM32_PIN_OUT(C,  7, OPENDRAIN, NONE, 50MHZ),
    };
    cWS281xDriver::init(cWS281xDriver::WS2812, 32, outputPortNumbers, 8);


//    initFlash();

    cTerm::init((char *)"/dev/tty1",128,"iLED>>");

    Animation *animations[] = {
       new GoombaAnimation(),
       new SpiralsAnimation(),
       new ColorAnimation(),
       new BigSpiralAnimation()
    };
    int index = 1;

    while(1)
    {
       if(animations[index])
          animations[index]->run();
       else
          cyg_thread_delay(500);
    }
}

void printPinState()
{
    cyg_uint32 reg32;

    HAL_READ_UINT32(CYGHWR_HAL_STM32_GPIOA + CYGHWR_HAL_STM32_GPIO_MODER, reg32);
    diag_printf("MODER 0x%08X\n", reg32);

    HAL_READ_UINT32(CYGHWR_HAL_STM32_GPIOA + CYGHWR_HAL_STM32_GPIO_OTYPER, reg32);
    diag_printf("OTYPER 0x%08X\n", reg32);

    HAL_READ_UINT32(CYGHWR_HAL_STM32_GPIOA + CYGHWR_HAL_STM32_GPIO_OSPEEDR, reg32);
    diag_printf("OSPEEDR 0x%08X\n", reg32);

    HAL_READ_UINT32(CYGHWR_HAL_STM32_GPIOA + CYGHWR_HAL_STM32_GPIO_PUPDR, reg32);
    diag_printf("PUPDR 0x%08X\n", reg32);

    HAL_READ_UINT32(CYGHWR_HAL_STM32_GPIOA + CYGHWR_HAL_STM32_GPIO_AFRL, reg32);
    diag_printf("AFRL 0x%08X\n", reg32);
}



