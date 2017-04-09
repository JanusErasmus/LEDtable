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
#include "runner.h"
#include "Spiral.h"

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
            CYGHWR_HAL_STM32_PIN_OUT(C,  5, OPENDRAIN, NONE, 2MHZ),
            CYGHWR_HAL_STM32_PIN_OUT(C,  6, OPENDRAIN, NONE, 2MHZ),
    };
    cWS281xDriver::init(cWS281xDriver::WS2812, 32, outputPortNumbers, 2);



//    initFlash();

    cTerm::init((char *)"/dev/tty1",128,"iLED>>");


    Spiral string1(3, 12, 3, 15, 0, 12);
    Spiral string2(3, 8 , 3, 11, 0, 8);
    string2.startColor(6);
    Spiral string3(3, 4 , 3, 7 , 0, 4);
    string3.startColor(2);
    Spiral string4(3, 0 , 3, 3 , 0, 0);
    string4.startColor(4);
//    Runner string1(59);

    cRGB color(0x00, 0xFF, 0x00);
    cRGB red(0xFF, 0, 0);
    cRGB off(0, 0, 0);


    cRGB blue(0x00, 0, 0x80);

    cRGB white(100, 20, 0);//(159, 0, 80);//(0x7, 0x7, 0x7);

//    cWS281xDriver::get()->setPixel(0, 0, cRGB(0x00, 0x80, 0x00));
//    cWS281xDriver::get()->setPixel(1, 0, cRGB(0x00, 0x80, 0x00));
//    cWS281xDriver::get()->setPixel(2, 0, cRGB(0x00, 0x00, 0x80));
//    cWS281xDriver::get()->setPixel(0, 10, cRGB(0x00, 0x00, 0x80));
//    cWS281xDriver::get()->paint();



    while(1)
    {
       string1.run();
       string2.run();
       string3.run();
       string4.run();
       cWS281xDriver::get()->paint();
       cyg_thread_delay(5);
       string1.next();
       string2.next();
       string3.next();
       string4.next();

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



