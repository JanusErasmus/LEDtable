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
    cWS281xDriver::init(cWS281xDriver::WS2812, 58, outputPortNumbers, 2);



//    initFlash();

    cTerm::init((char *)"/dev/tty1",128,"iLED>>");

    cRGB off(0x0,0x0,0x0);
    cRGB white(255,255,255);
    cRGB blue(0x00, 0x00, 0xFF);
    cRGB red(0xFF, 0x00, 0x00);
    cRGB green(0x00, 0xFF, 0x00);
    cRGB mix1(0xFF, 0xFF, 0x00);
    cRGB mix2(0xFF, 0x00, 0xFF);
    cRGB mix3(0x00, 0xFF, 0xFF);

    cyg_uint8 cColor = 0;
    cRGB *pColor[] = {&off, &red, &green, &blue, &white, &mix1, &mix2, &mix3};

    cRGB color(0x02, 0x81, 0x81);

    cWS281xDriver::get()->setPixel(0, color);
    cWS281xDriver::get()->setPixel(1, color);
    cWS281xDriver::get()->setPixel(57, color);
//    cWS281xDriver::get()->paint();

    cyg_uint8 ledCount = 57;
    cyg_int8 diff = 1;
    cyg_int8 cnt = 0;
    while(1)
    {

        cWS281xDriver::get()->setPixel(cnt, *pColor[cColor]);
        cWS281xDriver::get()->paint();
        cyg_thread_delay(5);
		cWS281xDriver::get()->setPixel(cnt, off);

        cnt+= diff;

        if(cnt > ledCount)
        {
        	diff = -1;
        	cnt = ledCount - 1;

        	cColor++;
        }
        else if(cnt < 0)
        {
        	diff = 1;
        	cnt = 0;

        	cColor++;
        }

    	if(cColor > 8)
    		cColor = 1;

//		diag_printf("%d\n", cnt);
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



