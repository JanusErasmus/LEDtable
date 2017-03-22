#include <cyg/kernel/diag.h>
#include <cyg/hal/hal_diag.h>
#include <cyg/kernel/kapi.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <cyg/io/ttyio.h>
#include <cyg/hal/var_io.h>
#include <cyg/error/strerror.h>

#include "init.h"
#include "led.h"
#include "kses_term.h"
#include "output_port.h"

#include "ws281x_driver.h"

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
    cLED *statusLED  = new cLED(
            CYGHWR_HAL_STM32_PIN_OUT(E,  9, PUSHPULL, NONE, 2MHZ),
            CYGHWR_HAL_STM32_PIN_OUT(E,  10, PUSHPULL, NONE, 2MHZ));

    cLED *ledList[] = {
            statusLED,
    };
    cLEDdriver::init(ledList, 1);

    cyg_uint32 outputPortNumbers[] =
    {
            CYGHWR_HAL_STM32_PIN_OUT(E,  2, OPENDRAIN, NONE, 2MHZ), //OPENDRAIN
    };
    cWS281xDriver::init(cWS281xDriver::WS2811, outputPortNumbers, 1);


    cTerm::init((char *)"/dev/tty0",128,"iLED>>");

    cRGB off(0x0,0x0,0x0);
    cRGB color(255,255,255);
    cRGB blue(0x00, 0x00, 0xFF);
    cRGB red(0xFF, 0x00, 0x00);
    cRGB green(0x00, 0xFF, 0x00);
    //((cInit*)args)->setPixels(color);

    cyg_uint8 cnt = 0;
    while(1)
    {
        cWS281xDriver::get()->setPixel(cnt++, off);
        cWS281xDriver::get()->setPixel(cnt, color);

        if(cnt > 57)
            cnt = 0;

        cyg_thread_delay(10);
//        cWS281xDriver::get()->setPixel(57, red);
//        cyg_thread_delay(50);
//        cWS281xDriver::get()->setPixel(57, green);
//        cyg_thread_delay(50);
//        cWS281xDriver::get()->setPixel(57, blue);
//        cyg_thread_delay(50);

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



