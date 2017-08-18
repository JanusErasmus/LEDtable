#ifndef WS281X_DRIVER_H_
#define WS281X_DRIVER_H_
#include <cyg/kernel/kapi.h>

#include "TermCMD.h"

#include "rgb.h"

#define WS281x_OUTPUT_PORT  CYGHWR_HAL_STM32_GPIOC
#define WS281x_ODR (WS281x_OUTPUT_PORT + CYGHWR_HAL_STM32_GPIO_ODR)

#define DMA_CONTROLLER_RCC  CYGHWR_HAL_STM32_CLOCK(AHB1, DMA2)
#define DMA_CONTROLLER_REG  CYGHWR_HAL_STM32_DMA2
#define DMA_UPDATE_STREAM   6
#define DMA_UPDATE_CHANNEL  6
#define DMA_RESET_STREAM    1
#define DMA_RESET_CHANNEL   6
#define DMA_BUFFER_STREAM   2
#define DMA_BUFFER_CHANNEL  6

#define CC_TIMER_RCC        CYGHWR_HAL_STM32_CLOCK(APB2, TIM1)
#define CC_TIMER            CYGHWR_HAL_STM32_TIM1

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

    cyg_uint8 *mBuffer;
    cyg_uint32 mBitCount;
    cyg_uint32 mPixelCount;

    void getConstants(cyg_uint32 clockSpeed, cyg_uint32 &autoReload, cyg_uint32 &setCount, cyg_uint32 &resetCount);
    void setupTimer(cyg_uint32 clockSpeed);
    void setupDMA();
    void setupDMA_MEM2MEM();

    cWS281xDriver(eWS281xModel model, cyg_uint32 pixel_count, cyg_uint32 *ports, cyg_uint8 count);
    virtual ~cWS281xDriver();

    void setBlackPixels(cRGB color);

public:

    static void init(eWS281xModel model, cyg_uint32 pixel_count, cyg_uint32 *ports, cyg_uint8 count);
    static cWS281xDriver *get();


    void setAll(cRGB color);
    void resetPixels();
    void setPixel(cyg_uint8 x, cyg_uint8 y, cRGB color);
    void paint();

    void setBuffer(cyg_uint8 *buffer, cyg_uint32 len);
    cyg_uint32 getBuffer(cyg_uint8 *buffer, cyg_uint32 len);

    static void paintDebug(cTerm & t,int argc,char *argv[]);
    static void setred(cTerm & t,int argc,char *argv[]);
    static void setgreen(cTerm & t,int argc,char *argv[]);
    static void setblue(cTerm & t,int argc,char *argv[]);
    static void setBlackred(cTerm & t,int argc,char *argv[]);
    static void setBlackgreen(cTerm & t,int argc,char *argv[]);
    static void setBlackblue(cTerm & t,int argc,char *argv[]);
};

extern const TermCMD::cmd_list_t wsCommands[];

#endif /* WS281X_DRIVER_H_ */
