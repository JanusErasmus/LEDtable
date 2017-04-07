#include <cyg/kernel/diag.h>
#include <stdio.h>

#include "led.h"

cLEDdriver* cLEDdriver::__instance = NULL;

void cLEDdriver::init(cLED ** leds, cyg_uint8 ledCount)
{
    diag_printf("Initializing cLEDdriver\n");
    if(!__instance)
    {
        __instance = new cLEDdriver(leds, ledCount);
    }
}

cLEDdriver* cLEDdriver::get()
{
    return __instance;
}

cLEDdriver::cLEDdriver(cLED ** leds, cyg_uint8 ledCount)
{
    mStatus = CONNECTED;
    mLEDCnt = ledCount;
    mLEDs = new cLED*[ledCount];
    memcpy(mLEDs, leds, (sizeof(cLED*) * ledCount));

    setupLEDs(mLEDs, mLEDCnt);

    cyg_thread_create(15,
            cLEDdriver::led_thread,
            (cyg_addrword_t)this,
            (char *)"heartBeat",
            mLEDStack,
            CYGNUM_HAL_STACK_SIZE_MINIMUM,
            &mLEDThreadHandle,
            &mLEDThread);
    cyg_thread_resume(mLEDThreadHandle);
}

void cLEDdriver::setupLEDs(cLED ** leds, cyg_uint8 ledCount)
{
    if(!leds)
        return;

    for (int k = 0; k < ledCount; k++)
    {
        cLED * led = leds[k];
        if(led->greenPin)
        {
            CYGHWR_HAL_STM32_GPIO_SET(led->greenPin);
            CYGHWR_HAL_STM32_GPIO_OUT(led->greenPin, 0);
        }
        if(led->redPin)
        {
            CYGHWR_HAL_STM32_GPIO_SET(led->redPin);
            CYGHWR_HAL_STM32_GPIO_OUT(led->redPin, 1);
        }

    }
}

void cLEDdriver::led_thread(cyg_addrword_t args)
{
    cLEDdriver* _this = (cLEDdriver*)args;

    _this->animateTest();


    for(;;)
    {
        switch(_this->mStatus)
        {
        case UNCONNECTED:
            _this->animateUnconnected();
            break;
        case CONNECTED:
            _this->animateIdle();
            break;
        }
    }
}

void cLEDdriver::animateUnconnected()
{
    mLEDs[0]->setLED(cLED::red);
    cyg_thread_delay(8);
    mLEDs[0]->setLED(cLED::off);
    cyg_thread_delay(20);
    mLEDs[0]->setLED(cLED::red);
    cyg_thread_delay(8);
    mLEDs[0]->setLED(cLED::off);
    cyg_thread_delay(120);
}

void cLEDdriver::animateIdle()
{
    mLEDs[0]->setLED(cLED::green);
    cyg_thread_delay(8);
    mLEDs[0]->setLED(cLED::off);
    cyg_thread_delay(20);
    mLEDs[0]->setLED(cLED::green);
    cyg_thread_delay(8);
    mLEDs[0]->setLED(cLED::off);
    cyg_thread_delay(120);

}

void cLEDdriver::animateTest()
{
    cyg_thread_delay(50);

    for (int k = 0; k < mLEDCnt; k++)
        mLEDs[k]->setLED(cLED::green);

    cyg_thread_delay(50);

    for (int k = 0; k < mLEDCnt; k++)
            mLEDs[k]->setLED(cLED::off);
}

cLED::cLED(cyg_uint32 greenpin, cyg_uint32 redpin)
{
    greenPin = greenpin;
    redPin = redpin;
}

void cLED::setLED(eColor color)
{
    if(greenPin && redPin)
    {
        switch(color)
        {
        case cLED::green:
            CYGHWR_HAL_STM32_GPIO_OUT (greenPin, 1);
            CYGHWR_HAL_STM32_GPIO_OUT (redPin, 0);
            break;

        case cLED::red:
            CYGHWR_HAL_STM32_GPIO_OUT (greenPin, 0);
            CYGHWR_HAL_STM32_GPIO_OUT (redPin, 1);
            break;

        case cLED::off:
            CYGHWR_HAL_STM32_GPIO_OUT (greenPin, 0);
            CYGHWR_HAL_STM32_GPIO_OUT (redPin, 0);
            break;
        }

        return;
    }

    {
        cyg_uint8 state = 0;
        if(color == green)
            state = 1;

        if (greenPin)
        {
            CYGHWR_HAL_STM32_GPIO_OUT (greenPin, state);
        }
        else if (redPin)
        {
            CYGHWR_HAL_STM32_GPIO_OUT (redPin, state);
        }
    }
}

