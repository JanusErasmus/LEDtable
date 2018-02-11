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

#include "animation_big_spiral.h"
#include "animation_color.h"
#include "animation_goomba.h"
#include "animation_runner.h"
#include "animation_spirals.h"
#include "output_port.h"
#include "spi_dev.h"
#include "ws281x_driver.h"
#include "ws281_receiver.h"
#include "blue_device.h"
#include "spi_flash.h"


#define TRACE(_x, ...) INFO_TRACE("cInit", _x,  ##__VA_ARGS__)

cInit * cInit::__instance = 0;

#include "char_printer.h"

cInit::cInit() : cDebug("init")
{
    __instance = this;

    cyg_thread_create(17,
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

void initBLE()
{
   cyg_uint8 mac[] = {0x1C, 0xB2, 0x84, 0xC2, 0x50, 0x00};
   char *BLEname = (char*)malloc(64);
   BLEname[0] = 0;
   sprintf(BLEname, "LED %d", mac[0]);
   cBlueNRG *blue = cBlueNRG::init(
         mac,
         BLEname,
         &BLE_spi_dev,
         CYGHWR_HAL_STM32_PIN_IN(A,  0, PULLDOWN),
         CYGHWR_HAL_STM32_PIN_OUT(A, 8, PUSHPULL, NONE, HIGH));

   while(!blue->ready())
      cyg_thread_delay(100);

   cBlueDevice::init(blue);

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

    SpiFlash::init();
//    initBLE();

    WS281receiver *receiver = new WS281receiver();
    cTerm::init((char *)"/dev/tty1",128,"LED>>");
    cTerm::setReceiver(receiver);

    cWS281xDriver *display = cWS281xDriver::get();
    CharPrinter Jprinter(display);
    Jprinter.setString("  Erasmus", yellow, red);

    CharPrinter Cprinter(display);
    Cprinter.setString("  Cherise", blue, green);

    Animation *animations[] = {
         &Jprinter,
       new ColorAnimation(display),
       new RunnerAnimation(display),
       new SpiralsAnimation(display),
       &Cprinter,
       new BigSpiralAnimation(display),
       new GoombaAnimation(display)
    };
    int index = 0;

    cyg_tick_count_t lastChange = cyg_current_time();

    cWS281xDriver::get()->setAll(off);
    cWS281xDriver::get()->paint();

//    cyg_uint8 buffer[1024];
//    memset(buffer, 0, 8);
//    buffer[0] = 0xF0;
//    cyg_spi_transfer(&stm32_flash_dev, false, 1, buffer, buffer);
//    buffer[0] = 0x30;
//    cyg_spi_transfer(&stm32_flash_dev, false, 1, buffer, buffer);
//    buffer[0] = 0x06;
//    cyg_spi_transfer(&stm32_flash_dev, false, 1, buffer, buffer);
//
//    memset(buffer, 0, 8);
//    buffer[0] = 0x01;
//    cyg_spi_transfer(&stm32_flash_dev, false, 3, buffer, buffer);
//    diag_dump_buf(buffer, 8);
//
//    memset(buffer, 0, 8);
//    buffer[0] = 0x05;
//    cyg_spi_transfer(&stm32_flash_dev, false, 8, buffer, buffer);
//    diag_dump_buf(buffer, 8);
//
//    memset(buffer, 0, 8);
//    buffer[0] = 0x07;
//    cyg_spi_transfer(&stm32_flash_dev, false, 8, buffer, buffer);
//    diag_dump_buf(buffer, 8);
//
//
//        animations[index]->run();
//        animations[index]->run();
//        cWS281xDriver::get()->getBuffer(buffer, 1024);
//        SpiFlash::get()->erase(0x400);
//        SpiFlash::get()->write(0x00, buffer, 1024);

    //       SpiFlash::get()->read(0, buffer, 1024);
    //       cWS281xDriver::get()->setBuffer(buffer, 1024);
    //       cWS281xDriver::get()->paint();
    //          cyg_thread_delay(50);
    //          SpiFlash::get()->read(0x400, buffer, 1024);
    //          cWS281xDriver::get()->setBuffer(buffer, 1024);
    //          cWS281xDriver::get()->paint();
//              cyg_thread_delay(50);

    while(1)
    {
       if(animations[index])
          animations[index]->run();
       //else

       if((cyg_current_time() - lastChange) > 6000)
       {
          lastChange = cyg_current_time();
          if(++index > 6)
             index = 0;
       }

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



