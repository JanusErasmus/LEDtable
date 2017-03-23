#ifndef WS281X_DRIVER_H_
#define WS281X_DRIVER_H_
#include <cyg/kernel/kapi.h>

#include "rgb.h"

#define WS281x_ODR (WS281x_OUTPUT_PORT + CYGHWR_HAL_STM32_GPIO_ODR)

#define DMA_CONTROLLER_RCC  CYGHWR_HAL_STM32_CLOCK(AHB1, DMA2)
#define DMA_CONTROLLER_REG  CYGHWR_HAL_STM32_DMA2
#define DMA_SET_STREAM      5
#define DMA_SET_CHANNEL     6
#define DMA_RESET_STREAM    1
#define DMA_RESET_CHANNEL   6
#define DMA_BUFFER_STREAM   2
#define DMA_BUFFER_CHANNEL  6

#define CC_TIMER_RCC        CYGHWR_HAL_STM32_CLOCK(APB2, TIM1)
#define CC_TIMER            CYGHWR_HAL_STM32_TIM1

#define WS281x_OUTPUT_PORT  CYGHWR_HAL_STM32_GPIOE

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
    cyg_interrupt mInterrupt;
    cyg_handle_t mIntHandle;
    static cyg_uint32 handleISR(cyg_vector_t vector,cyg_addrword_t data);
    static void handleDSR(cyg_vector_t vector,cyg_uint32 count,cyg_addrword_t data);

    cyg_uint32 mAutoReload;
    cyg_uint32 mRefreshAutoReload;
    cyg_interrupt mTM1interrupt;
    cyg_handle_t mTM1intHandle;
    static cyg_uint32 TIM1handleISR(cyg_vector_t vector,cyg_addrword_t data);
    static void TM1handleDSR(cyg_vector_t vector,cyg_uint32 count,cyg_addrword_t data);

    void getConstants(cyg_uint32 clockSpeed, cyg_uint32 &autoReload, cyg_uint32 &setCount, cyg_uint32 &resetCount, cyg_uint32 &refreshReload);
    void setupTimer(cyg_uint32 clockSpeed);
    void setupDMA();
    void setupDMA_MEM2MEM();

    cWS281xDriver(eWS281xModel model, cyg_uint32 *ports, cyg_uint8 count);
    virtual ~cWS281xDriver();

public:

    static void init(eWS281xModel model, cyg_uint32 *ports, cyg_uint8 count);
    static cWS281xDriver *get();

    void resetPixels();
    void setPixel(cyg_uint32 count, cRGB color);
};

#endif /* WS281X_DRIVER_H_ */
