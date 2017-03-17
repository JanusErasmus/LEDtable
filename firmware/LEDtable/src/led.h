#ifndef LED_H_
#define LED_H_
#include <cyg/kernel/kapi.h>

#include "definitions.h"

class cLED
{
public :
    enum eColor
    {
        red,
        green,
        off
    };

    cyg_uint32 greenPin;
    cyg_uint32 redPin;

    cLED(cyg_uint32 greenpin = 0, cyg_uint32 redpin = 0);


    void setLED(eColor);
};

class cLEDdriver
{
public:
    enum eLEDstate
    {
        UNCONNECTED,
        CONNECTED
    };

private:
    static cLEDdriver * __instance;

    eLEDstate mStatus;
    cyg_uint8 mLEDCnt;
    cLED ** mLEDs;

    cyg_uint8 mLEDStack[LED_STACK_SIZE];
    cyg_thread mLEDThread;
    cyg_handle_t mLEDThreadHandle;
    static void led_thread(cyg_addrword_t args);

    cLEDdriver(cLED ** leds, cyg_uint8 ledCount);

    void setupLEDs(cLED ** leds, cyg_uint8 ledCount);

    void animateTest();
    void animateUnconnected();
    void animateIdle();


public:
    static void init(cLED ** leds, cyg_uint8 ledCount);
    static cLEDdriver * get();

    void setState(eLEDstate state){ mStatus = state; };
};

#endif /* LED_H_ */
