#ifndef WS281X_DRIVER_H_
#define WS281X_DRIVER_H_
#include <cyg/kernel/kapi.h>

#include "rgb.h"

class cWS281xDriver
{
public:
    enum eWS281xModel
    {
        WS2811 = 400000,
        WS2812 = 800000
    };
private:
    static cWS281xDriver *__instance;

    cyg_atomic mBufferBusy;
    cyg_uint8 *mBuffer;
    cyg_uint32 mBitCount;
    cyg_uint32 mPixelCount;
    cyg_uint32 mPIO;
    cyg_interrupt mInterrupt;
    cyg_handle_t mIntHandle;
    static cyg_uint32 handleISR(cyg_vector_t vector,cyg_addrword_t data);
    static void handleDSR(cyg_vector_t vector,cyg_uint32 count,cyg_addrword_t data);

    cyg_uint32 mSilentCount;
    cyg_interrupt mTM1interrupt;
    cyg_handle_t mTM1intHandle;
    static cyg_uint32 TIM1handleISR(cyg_vector_t vector,cyg_addrword_t data);
    static void TM1handleDSR(cyg_vector_t vector,cyg_uint32 count,cyg_addrword_t data);

    void setupTimer(cyg_uint32 clockSpeed);
    void setupDMA();
    void setupDMA_MEM2MEM();

    cWS281xDriver(eWS281xModel model);
    virtual ~cWS281xDriver();

public:

    static void init(eWS281xModel model);
    static cWS281xDriver *get();

    void resetPixels();
    void setPixel(cyg_uint32 count, cRGB color);
};

#endif /* WS281X_DRIVER_H_ */
