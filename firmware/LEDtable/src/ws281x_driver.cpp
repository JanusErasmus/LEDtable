#include <cyg/kernel/diag.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#include "ws281x_driver.h"

cyg_uint8 SetData = 0xFF;
cyg_uint8 ResetData = 0x00;

cWS281xDriver *cWS281xDriver::__instance = 0;

void cWS281xDriver::init(eWS281xModel model)
{
    if(!__instance)
        __instance = new cWS281xDriver(model);
}

cWS281xDriver *cWS281xDriver::get()
{
    return __instance;
}

cWS281xDriver::cWS281xDriver(eWS281xModel model)
{
    mBufferBusy = 0;
    mSilentCount = 0;
    mPixelCount = 100;
    mBitCount = (24 * mPixelCount) + 1;
    mPIO = CYGHWR_HAL_STM32_GPIOE + CYGHWR_HAL_STM32_GPIO_ODR;

    mBuffer = (cyg_uint8*)malloc(mBitCount);
    resetPixels();

    setupDMA_MEM2MEM();
    setupTimer(model);

    cyg_interrupt_mask(CYGNUM_HAL_INTERRUPT_DMA2_STR2);
    cyg_interrupt_create(CYGNUM_HAL_INTERRUPT_DMA2_STR2,
            7,
            (cyg_addrword_t)this,
            handleISR,
            handleDSR,
            &mIntHandle,
            &mInterrupt);
    cyg_interrupt_attach(mIntHandle);
    cyg_interrupt_unmask(CYGNUM_HAL_INTERRUPT_DMA2_STR2);

    cyg_interrupt_mask(CYGNUM_HAL_INTERRUPT_TIM1_UP);
    cyg_interrupt_create(CYGNUM_HAL_INTERRUPT_TIM1_UP,
            7,
            (cyg_addrword_t)this,
            TIM1handleISR,
            TM1handleDSR,
            &mTM1intHandle,
            &mTM1interrupt);
    cyg_interrupt_attach(mTM1intHandle);
    cyg_interrupt_unmask(CYGNUM_HAL_INTERRUPT_TIM1_UP);
}

void cWS281xDriver::resetPixels()
{
    memset(mBuffer, 0x00, mBitCount);
}

void cWS281xDriver::setupDMA_MEM2MEM()
{
    cyg_uint32 reg32;
    CYGHWR_HAL_STM32_CLOCK_ENABLE(CYGHWR_HAL_STM32_CLOCK(AHB1, DMA2));


//----------------- Setup GPIO Set transfer ------------------------------------
    reg32 = 1;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SNDTR(5), reg32);

    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SM0AR(5), (cyg_uint32)&SetData);

    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SPAR(5), (cyg_uint32)mPIO);

    cyg_uint32 CR = CYGHWR_HAL_STM32_DMA_CCR_CHSEL(6)   |
            CYGHWR_HAL_STM32_DMA_CCR_MEM2P    |
            CYGHWR_HAL_STM32_DMA_CCR_CIRC;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SCR(5), CR);

    CR |= CYGHWR_HAL_STM32_DMA_CCR_EN;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SCR(5), CR);


//----------------- Setup GPIO Reset transfer ----------------------------------
    reg32 = 1;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SNDTR(1), reg32);

    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SM0AR(1), (cyg_uint32)&ResetData);

    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SPAR(1), (cyg_uint32)mPIO);

    CR = CYGHWR_HAL_STM32_DMA_CCR_CHSEL(6)    |
            CYGHWR_HAL_STM32_DMA_CCR_MEM2P    |
            CYGHWR_HAL_STM32_DMA_CCR_CIRC;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SCR(1), CR);

    CR |= CYGHWR_HAL_STM32_DMA_CCR_EN;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SCR(1), CR);


//----------------- Setup pixel buffer transfer --------------------------------
    reg32 = mBitCount;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SNDTR(2), reg32);

    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SM0AR(2), (cyg_uint32)mBuffer);

    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SPAR(2), (cyg_uint32)mPIO);


    CR = CYGHWR_HAL_STM32_DMA_CCR_CHSEL(6)    |
            CYGHWR_HAL_STM32_DMA_CCR_MEM2P    |
            CYGHWR_HAL_STM32_DMA_CCR_MINC |
            CYGHWR_HAL_STM32_DMA_CCR_TCIE;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SCR(2), CR);

    CR |= CYGHWR_HAL_STM32_DMA_CCR_EN;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SCR(2), CR);

    PRINT_REG(CYGHWR_HAL_STM32_DMA2, CYGHWR_HAL_STM32_DMA_SCR(2));
}

void cWS281xDriver::setupTimer(cyg_uint32 clockSpeed)
{
    cyg_uint32 reg32;
    CYGHWR_HAL_STM32_CLOCK_ENABLE(CYGHWR_HAL_STM32_CLOCK(APB2, TIM1));

//----------------- Setup Timer pre-scaler -------------------------------------
    if(clockSpeed == 800000)
        reg32 = 0x00;
    else
        reg32 = 0x01;

    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_PSC, reg32);

//----------------- Setup reset  timing for a ONE ------------------------------
    reg32 = 0x95;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_ARR, reg32);

//----------------- Setup pixel set trigger ------------------------------------
    reg32 = 0x53;;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_CCR1, reg32);

//----------------- Setup reset timing for a ZERO ------------------------------
    reg32 = 0x29;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_CCR2, reg32);

    reg32 =
            (1 << 8)                        |   //update DMA request
            (1 << 9)                        |   //CC1 DMA request
            (1 << 10);                          //CC2 DMA request
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_DIER, reg32);

    reg32 =  CYGHWR_HAL_STM32_TIM_CR1_ARPE   | //enable auto reload register
             CYGHWR_HAL_STM32_TIM_CR1_CEN;
   HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_CR1, reg32);

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
    setString(buffer        , string, color.G);
    setString(&buffer[8]    , string, color.R);
    setString(&buffer[16]   , string, color.B);
}

void cWS281xDriver::setPixel(cyg_uint32 count, cRGB color)
{
    while(mBufferBusy) cyg_thread_delay(1);

     cyg_uint8 *buffer = &mBuffer[(count * 24) + 1];
     setStringColor(buffer, 2, color);

     mBuffer[0] = 0;
}

cyg_uint32 cWS281xDriver::handleISR(cyg_vector_t vector, cyg_addrword_t data)
{
    cyg_interrupt_mask(vector);
    cyg_interrupt_acknowledge(vector);


    cyg_uint32 lisr;
    HAL_READ_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_LISR, lisr);

    cyg_uint32 reg32 = 0xFFFFFFFF;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_LIFCR, reg32);
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_HIFCR, reg32);

    HAL_READ_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_DIER, reg32);
    reg32 &= ~((1 << 8) | (1 << 9) | (1 << 10));
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_DIER, reg32);


    reg32 = 0;
    HAL_WRITE_UINT32(((cWS281xDriver*)data)->mPIO, reg32);


    if(lisr)
    {
        return (CYG_ISR_CALL_DSR | CYG_ISR_HANDLED);
    }

    cyg_interrupt_unmask(vector);
    return CYG_ISR_HANDLED;
}

void cWS281xDriver::handleDSR(cyg_vector_t vector,cyg_uint32 count,cyg_addrword_t data)
{
    //clear flags
    cyg_uint32 reg32 = 0;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_SR, reg32);

    //enable timer Update interrupt
    HAL_READ_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_DIER, reg32);
    reg32 |= CYGHWR_HAL_STM32_TIM_DIER_UIE;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_DIER, reg32);


    reg32 = 0;
    HAL_WRITE_UINT32(((cWS281xDriver*)data)->mPIO, reg32);

    ((cWS281xDriver*)data)->mBufferBusy = 0;
    ((cWS281xDriver*)data)->mSilentCount = 10000;

    cyg_interrupt_unmask(vector);
}

cyg_uint32 cWS281xDriver::TIM1handleISR(cyg_vector_t vector, cyg_addrword_t data)
{
    cyg_interrupt_mask(vector);
    cyg_interrupt_acknowledge(vector);

    cyg_uint32 SR, reg32;
    HAL_READ_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_SR, SR);

    //clear flags
    reg32 = 0;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_SR, reg32);

    if(SR & 0x01)
    {

        if(!(((cWS281xDriver*)data)->mSilentCount)--)
        {
            ((cWS281xDriver*)data)->mBufferBusy = 1;
            return (CYG_ISR_CALL_DSR | CYG_ISR_HANDLED);
        }
        else
        {
            reg32 = 0;
            HAL_WRITE_UINT32(((cWS281xDriver*)data)->mPIO, reg32);
        }
    }

    cyg_interrupt_unmask(vector);
    return CYG_ISR_HANDLED;
}

void cWS281xDriver::TM1handleDSR(cyg_vector_t vector,cyg_uint32 count,cyg_addrword_t data)
{
    cyg_uint32 CR, reg32;

    //PRINT_REG(CYGHWR_HAL_STM32_TIM1, CYGHWR_HAL_STM32_TIM_DIER);
    //diag_printf("UP_ITR 0x%08X\n", SR);
    //PRINT_REG(CYGHWR_HAL_STM32_DMA2, CYGHWR_HAL_STM32_DMA_SNDTR(2));

    //stop timer
    HAL_READ_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_CR1, CR);
    CR &= ~(CYGHWR_HAL_STM32_TIM_CR1_CEN);
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_CR1, CR);

    reg32 = (((cWS281xDriver*)data)->mBitCount);
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SNDTR(2), reg32);


    HAL_READ_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_SR, reg32);
    reg32 = 0;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_SR, reg32);

    //PRINT_REG(CYGHWR_HAL_STM32_DMA2, CYGHWR_HAL_STM32_DMA_SCR(2));

    //disable timer Update interrupt
    HAL_READ_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_DIER, reg32);
    reg32 &= ~(CYGHWR_HAL_STM32_TIM_DIER_UIE);
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_DIER, reg32);

    //enable DMA capture event
    HAL_READ_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_DIER, reg32);
    reg32 |= (1 << 8) | (1 << 9) | (1 << 10);
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_DIER, reg32);

    //start the timer again
    reg32 = 0;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_SR, reg32);

    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_CNT, reg32);
    CR |= CYGHWR_HAL_STM32_TIM_CR1_CEN;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_CR1, CR);

    //enable the DMA for pixels again
    reg32 = 0xFFFFFFFF;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_LIFCR, reg32);
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_HIFCR, reg32);

    cyg_uint32 SCR;
    HAL_READ_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SCR(2), SCR);
    do
    {
        SCR |= CYGHWR_HAL_STM32_DMA_CCR_EN;
        HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SCR(2), SCR);
        HAL_READ_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SCR(2), reg32);
        //diag_printf("SCR 0x%08X\n", reg32);
    }while(!(reg32 & CYGHWR_HAL_STM32_DMA_CCR_EN));

    //diag_printf("Start timer");
    cyg_interrupt_unmask(vector);
}


cWS281xDriver::~cWS281xDriver()
{
    free(mBuffer);
}

