#include <stdlib.h>
#include <cyg/kernel/diag.h>
#include <stdlib.h>
#include <string.h>

#include <utils.h>
#include "ws281x_driver.h"


#define TRACE(_x, ...) INFO_TRACE("cWS281xDriver", _x,  ##__VA_ARGS__)
extern cyg_uint32 hal_stm32_pclk2;

cyg_uint8 SetData = 0xFF;
cyg_uint8 ResetData = 0x00;

cWS281xDriver *cWS281xDriver::__instance = 0;

void cWS281xDriver::init(eWS281xModel model, cyg_uint32 pixel_count, cyg_uint32 *ports, cyg_uint8 count)
{
    if(!__instance)
        __instance = new cWS281xDriver(model, pixel_count, ports, count);
}

cWS281xDriver *cWS281xDriver::get()
{
    return __instance;
}

cWS281xDriver::cWS281xDriver(eWS281xModel model, cyg_uint32 pixel_count, cyg_uint32 *ports, cyg_uint8 count)
{
    mPixelCount = pixel_count + 1;
    mBitCount = (24 * mPixelCount);

    mBuffer = (cyg_uint8*)malloc(mBitCount + 8);
    resetPixels();

    for (int k = 0; k < count; k++)
    {
        CYGHWR_HAL_STM32_GPIO_SET (ports[k]);
    }

    setupDMA_MEM2MEM();
    setupTimer(model);
}

void cWS281xDriver::resetPixels()
{
    memset(mBuffer, 0x00, mBitCount + 8);
}

void cWS281xDriver::setupDMA_MEM2MEM()
{
    CYGHWR_HAL_STM32_CLOCK_ENABLE(DMA_CONTROLLER_RCC);

//----------------- Setup GPIO Set transfer ------------------------------------

    HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SM0AR( DMA_UPDATE_STREAM ), (cyg_uint32)&SetData);

    HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SPAR( DMA_UPDATE_STREAM ), (cyg_uint32)WS281x_ODR);

    cyg_uint32 CR = CYGHWR_HAL_STM32_DMA_CCR_CHSEL( DMA_UPDATE_STREAM )   |
    		CYGHWR_HAL_STM32_DMA_CCR_MEM2P;
    HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SCR( DMA_UPDATE_STREAM ), CR);


//----------------- Setup GPIO Reset transfer ----------------------------------

    HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SM0AR( DMA_RESET_STREAM ), (cyg_uint32)&ResetData);

    HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SPAR( DMA_RESET_STREAM ), (cyg_uint32)WS281x_ODR);

    CR = CYGHWR_HAL_STM32_DMA_CCR_CHSEL( DMA_RESET_CHANNEL )    |
            CYGHWR_HAL_STM32_DMA_CCR_MEM2P;
    HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SCR( DMA_RESET_STREAM ), CR);


//----------------- Setup pixel buffer transfer --------------------------------

    HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SPAR( DMA_BUFFER_STREAM ), (cyg_uint32)WS281x_ODR);


    CR = CYGHWR_HAL_STM32_DMA_CCR_CHSEL( DMA_BUFFER_CHANNEL )    |
            CYGHWR_HAL_STM32_DMA_CCR_MEM2P    |
            CYGHWR_HAL_STM32_DMA_CCR_MINC;
    HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SCR( DMA_BUFFER_STREAM ), CR);

    //PRINT_REG(DMA_CONTROLLER_REG, CYGHWR_HAL_STM32_DMA_SCR( DMA_BUFFER_STREAM ));
}


extern cyg_uint32 hal_stm32_pclk2;

void cWS281xDriver::getConstants(cyg_uint32 clockSpeed, cyg_uint32 &autoReload, cyg_uint32 &setCount, cyg_uint32 &resetCount)
{
    float TIM1tick = 1.0 / ( (float)hal_stm32_pclk2);
    float TIM1_ARRvalue = ( (float)hal_stm32_pclk2) / (float)clockSpeed;

    autoReload = TIM1_ARRvalue;
    if(clockSpeed == WS2812)
    {
        setCount = (float)800e-9 / TIM1tick;
        resetCount = (float)400e-9 / TIM1tick;
    }
    else
    {
        setCount = (float)1200e-9 / TIM1tick;
        resetCount = (float)500e-9 / TIM1tick;
    }

}

void cWS281xDriver::setupTimer(cyg_uint32 clockSpeed)
{
    cyg_uint32 reg32, setCount, resetCount, autoReload;

    getConstants(clockSpeed, autoReload, setCount, resetCount);

    TRACE("TIM1 Setup:\n");
    TRACE(" - ARR    : 0x%08X\n", autoReload);
    TRACE(" - SET    : 0x%08X\n", setCount);
    TRACE(" - RESET  : 0x%08X\n", resetCount);

    CYGHWR_HAL_STM32_CLOCK_ENABLE(CC_TIMER_RCC);

//----------------- Setup timer frequency --------------------------------------
    HAL_WRITE_UINT32(CC_TIMER + CYGHWR_HAL_STM32_TIM_ARR, autoReload);

//----------------- Setup reset timing for a ONE ------------------------------
    reg32 = 10;
    HAL_WRITE_UINT32(CC_TIMER + CYGHWR_HAL_STM32_TIM_CCR3, reg32);

//----------------- Setup pixel reset trigger when ZERO ------------------------
    HAL_WRITE_UINT32(CC_TIMER + CYGHWR_HAL_STM32_TIM_CCR1, setCount);

//----------------- Setup reset timing for a ONE ------------------------------
    HAL_WRITE_UINT32(CC_TIMER + CYGHWR_HAL_STM32_TIM_CCR2, resetCount);

    reg32 = (1 << 1) |
            (1 << 11)                        |   //update DMA request
            (1 << 9)                        |   //CC1 DMA request
            (1 << 10);                          //CC2 DMA request
    HAL_WRITE_UINT32(CC_TIMER + CYGHWR_HAL_STM32_TIM_DIER, reg32);

    reg32 =  CYGHWR_HAL_STM32_TIM_CR1_ARPE;//   | //enable auto reload register
             //CYGHWR_HAL_STM32_TIM_CR1_CEN;
   HAL_WRITE_UINT32(CC_TIMER + CYGHWR_HAL_STM32_TIM_CR1, reg32);

}

void setString(cyg_uint8 *buffer, cyg_uint8 string, cyg_uint8 colorByte)
{
    //diag_printf("setString %02X\n", colorByte);
    cyg_uint8 idx = 7;
    for(cyg_uint8 k = 0; k < 8; k++)
    {
        if(colorByte & (1 << k))
            buffer[idx--] |= (1 << string);
        else
            buffer[idx--] &= ~(1 << string);
    }
}

void setStringColor(cyg_uint8 *buffer, cyg_uint8 string, cRGB color)
{
    setString(buffer        , string, color.G );
    setString(&buffer[8]    , string, color.R );
    setString(&buffer[16]   , string, color.B );
}

void cWS281xDriver::setPixel(cyg_uint8 x, cyg_uint8 y, cRGB color)
{

	cyg_uint8 string = x/2;
	cyg_uint8 count = y;

	if(x%2)
	{
		count = 31;
		count -= y;
	}
	string += 5;
//	diag_printf("string %d, count %d\n", string, count);

     cyg_uint8 *buffer = &mBuffer[(count * 24)];
     setStringColor(buffer, string, color);

}

void cWS281xDriver::paint()
{
	cyg_uint32 TIM_CR;
	HAL_READ_UINT32(CC_TIMER + CYGHWR_HAL_STM32_TIM_CR1, TIM_CR);
    TIM_CR &= ~(CYGHWR_HAL_STM32_TIM_CR1_CEN);
    HAL_WRITE_UINT32(CC_TIMER + CYGHWR_HAL_STM32_TIM_CR1, TIM_CR);

    cyg_uint32 reg32;
//    do
//    {
//       HAL_READ_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SCR( DMA_RESET_STREAM ), reg32);
//    }while(reg32 & CYGHWR_HAL_STM32_DMA_CCR_EN);

//    PRINT_REG(DMA_CONTROLLER_REG, CYGHWR_HAL_STM32_DMA_SM0AR( 6 ));
//    PRINT_REG(DMA_CONTROLLER_REG, CYGHWR_HAL_STM32_DMA_SM0AR( DMA_BUFFER_STREAM ));
//    PRINT_REG(DMA_CONTROLLER_REG, CYGHWR_HAL_STM32_DMA_SM0AR( DMA_RESET_STREAM ));

	HAL_READ_UINT32(CC_TIMER + CYGHWR_HAL_STM32_TIM_SR, reg32);
	reg32 = 0;
	HAL_WRITE_UINT32(CC_TIMER + CYGHWR_HAL_STM32_TIM_SR, reg32);
    HAL_WRITE_UINT32(CC_TIMER + CYGHWR_HAL_STM32_TIM_CNT, reg32);


    reg32 = 0xFFFFFFFF;
    HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_LIFCR, reg32);
    HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_HIFCR, reg32);

//	HAL_WRITE_UINT32(WS281x_ODR, reg32);
	cyg_thread_delay(1);

    reg32 = 0xFFFFFFFF;
    HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_LIFCR, reg32);
    HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_HIFCR, reg32);

	reg32 = mBitCount;
	HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SNDTR( DMA_RESET_STREAM ), reg32);
	HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SNDTR( DMA_UPDATE_STREAM ), reg32);
	HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SNDTR( DMA_BUFFER_STREAM ), reg32);
    HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SM0AR( DMA_BUFFER_STREAM ), (cyg_uint32)mBuffer);


	cyg_uint32 CR;
	HAL_READ_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SCR( DMA_UPDATE_STREAM ), CR);
	CR |= CYGHWR_HAL_STM32_DMA_CCR_EN;
	HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SCR( DMA_UPDATE_STREAM ), CR);

	HAL_READ_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SCR( DMA_RESET_STREAM ), CR);
	CR |= CYGHWR_HAL_STM32_DMA_CCR_EN;
	HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SCR( DMA_RESET_STREAM ), CR);

	HAL_READ_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SCR( DMA_BUFFER_STREAM ), CR);
	CR |= CYGHWR_HAL_STM32_DMA_CCR_EN;
	HAL_WRITE_UINT32(DMA_CONTROLLER_REG + CYGHWR_HAL_STM32_DMA_SCR( DMA_BUFFER_STREAM ), CR);



    TIM_CR |= CYGHWR_HAL_STM32_TIM_CR1_CEN;
    HAL_WRITE_UINT32(CC_TIMER + CYGHWR_HAL_STM32_TIM_CR1, TIM_CR);

}

cWS281xDriver::~cWS281xDriver()
{
    free(mBuffer);
}

void cWS281xDriver::paint(cTerm & t,int argc,char **argv)
{
	if(__instance)
		__instance->paint();
}

const TermCMD::cmd_list_t wsCommands[] =
{
      {"WS218x"    ,0,0,0},
      {"paint",       "",            "Repaint pixels", cWS281xDriver::paint},
};
