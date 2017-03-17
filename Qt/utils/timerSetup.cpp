#include <cyg/kernel/diag.h>
#include <cyg/hal/hal_diag.h>
#include <cyg/kernel/kapi.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <cyg/io/ttyio.h>
#include <cyg/hal/var_io.h>
#include <cyg/error/strerror.h>

#include "init.h"
#include "led.h"
#include "kses_term.h"
#include "output_port.h"

cInit * cInit::__instance = 0;

cyg_uint8 SetData = 0xFF;
cyg_uint8 ResetData = 0x00;


/**
 * This function is the system init function.
 * It is conctructed in such a way as to be only
 * execute the constructor once, thus only initialising
 * the system once
 *
 * @example cInit::init();
 */
void cInit::init()
{
    if (__instance == 0) //Allow instance to be created only once.
    {
        __instance = new cInit();
    }
}

cInit::cInit() : cDebug("init")
{
    cyg_thread_create(INIT_PRIOR,
               init_thread,
               (cyg_addrword_t)this,
               (char *)"cInit",
               mLEDStack,
               INIT_STACK_SIZE,
               &mLEDThreadHandle,
               &mLEDThread);
       cyg_thread_resume(mLEDThreadHandle);

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

       mSilentCount = 0;
       mPixelCount = 1;
       mPIO = CYGHWR_HAL_STM32_GPIOE + CYGHWR_HAL_STM32_GPIO_ODR;

       //mBuffer = (cyg_uint8*)malloc(mPixelCount * 24);

       memset(mBuffer,0x00, mPixelCount * 24);

       mBuffer[1] = 0xFF;
       mBuffer[2] = 0xFF;
       mBuffer[3] = 0xFF;
       mBuffer[5] = 0xFF;
       mBuffer[7] = 0xFF;

       diag_printf("mBuffer: %p\n", mBuffer);
       diag_dump_buf(mBuffer, (mPixelCount * 24));

       cyg_uint32 outputPortNumbers[] =
       {
               CYGHWR_HAL_STM32_PIN_OUT(E,  2, PUSHPULL, NONE, 2MHZ),
       };
       cOutput::init(outputPortNumbers, 1);
}

void cInit::setupDMA_MEM2MEM()
{
    cyg_uint32 reg32;
    CYGHWR_HAL_STM32_CLOCK_ENABLE(CYGHWR_HAL_STM32_CLOCK(AHB1, DMA2));

    reg32 = 1;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SNDTR(5), reg32);

    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SM0AR(5), (cyg_uint32)&SetData);

    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SPAR(5), (cyg_uint32)mPIO);

    reg32 = CYGHWR_HAL_STM32_DMA_FCR_DMDIS;
    //    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA1 + CYGHWR_HAL_STM32_DMA_SFCR(1), reg32);

    cyg_uint32 CR = CYGHWR_HAL_STM32_DMA_CCR_CHSEL(6)   |
            CYGHWR_HAL_STM32_DMA_CCR_MEM2P    |
            CYGHWR_HAL_STM32_DMA_CCR_CIRC;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SCR(5), CR);

    CR |= CYGHWR_HAL_STM32_DMA_CCR_EN;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SCR(5), CR);

    //------------------------------------------------------------------------------------------------------

    reg32 = 1;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SNDTR(1), reg32);

    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SM0AR(1), (cyg_uint32)&ResetData);

    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SPAR(1), (cyg_uint32)mPIO);

    reg32 = CYGHWR_HAL_STM32_DMA_FCR_DMDIS;
    //    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA1 + CYGHWR_HAL_STM32_DMA_SFCR(1), reg32);

    CR = CYGHWR_HAL_STM32_DMA_CCR_CHSEL(6)    |
            CYGHWR_HAL_STM32_DMA_CCR_MEM2P    |
            CYGHWR_HAL_STM32_DMA_CCR_CIRC;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SCR(1), CR);

    CR |= CYGHWR_HAL_STM32_DMA_CCR_EN;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SCR(1), CR);

    //------------------------------------------------------------------------------------------------------

    reg32 = (mPixelCount * 8);
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SNDTR(2), reg32);

    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SM0AR(2), (cyg_uint32)mBuffer);

    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SPAR(2), (cyg_uint32)mPIO);

    reg32 = CYGHWR_HAL_STM32_DMA_FCR_DMDIS;
    //    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA1 + CYGHWR_HAL_STM32_DMA_SFCR(1), reg32);

    CR = CYGHWR_HAL_STM32_DMA_CCR_CHSEL(6)    |
            CYGHWR_HAL_STM32_DMA_CCR_MEM2P    |
            CYGHWR_HAL_STM32_DMA_CCR_MINC |
            CYGHWR_HAL_STM32_DMA_CCR_TCIE;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SCR(2), CR);

    CR |= CYGHWR_HAL_STM32_DMA_CCR_EN;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_DMA2 + CYGHWR_HAL_STM32_DMA_SCR(2), CR);

    PRINT_REG(CYGHWR_HAL_STM32_DMA2, CYGHWR_HAL_STM32_DMA_SCR(2));
}

void setupTimer()
{
    cyg_uint32 reg32;
    CYGHWR_HAL_STM32_CLOCK_ENABLE(CYGHWR_HAL_STM32_CLOCK(APB2, TIM1));

    reg32 = 0x0;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_PSC, reg32);

    reg32 = 0x12A;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_ARR, reg32);

    reg32 = 0xA6;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_CCR1, reg32);

    reg32 = 0x53;
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

void cInit::init_thread(cyg_addrword_t args)
{
    cLED *statusLED  = new cLED(
            CYGHWR_HAL_STM32_PIN_OUT(E,  9, PUSHPULL, NONE, 2MHZ),
            CYGHWR_HAL_STM32_PIN_OUT(E,  10, PUSHPULL, NONE, 2MHZ));

    cLED *ledList[] = {
            statusLED,
    };
    cLEDdriver::init(ledList, 1);

    ((cInit*)args)->setupDMA_MEM2MEM();
    setupTimer();

    cKSESterminal::init((char *)"/dev/tty0",128,"iLED>>");

    while(1)
    {
        //PRINT_REG(CYGHWR_HAL_STM32_TIM1, CYGHWR_HAL_STM32_TIM_CNT);

        //PRINT_REG(CYGHWR_HAL_STM32_DMA2, CYGHWR_HAL_STM32_DMA_SCR(1));
        //PRINT_REG(CYGHWR_HAL_STM32_DMA2, CYGHWR_HAL_STM32_DMA_SFCR(5));


        //PRINT_REG(CYGHWR_HAL_STM32_DMA2, CYGHWR_HAL_STM32_DMA_SCR(2));
        cyg_thread_delay(20);

//        if(((cInit*)args)->mBuffer[0] == 0xFF)
//            ((cInit*)args)->mBuffer[0] = 0xEE;
    }
}

cyg_uint32 cInit::handleISR(cyg_vector_t vector, cyg_addrword_t data)
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

    if(lisr)
    {
        return (CYG_ISR_CALL_DSR | CYG_ISR_HANDLED);
    }

    cyg_interrupt_unmask(vector);
    return CYG_ISR_HANDLED;
}

void cInit::handleDSR(cyg_vector_t vector,cyg_uint32 count,cyg_addrword_t data)
{
    //clear flags
    cyg_uint32 reg32 = 0;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_SR, reg32);

    //enable timer Update interrupt
    HAL_READ_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_DIER, reg32);
    reg32 |= CYGHWR_HAL_STM32_TIM_DIER_UIE;
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_DIER, reg32);

    ((cInit*)data)->mSilentCount = 20;

    cyg_interrupt_unmask(vector);
}

cyg_uint32 cInit::TIM1handleISR(cyg_vector_t vector, cyg_addrword_t data)
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

        if(!(((cInit*)data)->mSilentCount)--)
        {
            return (CYG_ISR_CALL_DSR | CYG_ISR_HANDLED);
        }
        else
        {
            reg32 = 0;
            HAL_WRITE_UINT32(((cInit*)data)->mPIO, reg32);
        }
    }

    cyg_interrupt_unmask(vector);
    return CYG_ISR_HANDLED;
}

void cInit::TM1handleDSR(cyg_vector_t vector,cyg_uint32 count,cyg_addrword_t data)
{
    cyg_uint32 CR, reg32;


    //PRINT_REG(CYGHWR_HAL_STM32_TIM1, CYGHWR_HAL_STM32_TIM_DIER);
    //diag_printf("UP_ITR 0x%08X\n", SR);
    //PRINT_REG(CYGHWR_HAL_STM32_DMA2, CYGHWR_HAL_STM32_DMA_SNDTR(2));

    //stop timer
    HAL_READ_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_CR1, CR);
    CR &= ~(CYGHWR_HAL_STM32_TIM_CR1_CEN);
    HAL_WRITE_UINT32(CYGHWR_HAL_STM32_TIM1 + CYGHWR_HAL_STM32_TIM_CR1, CR);

    reg32 = (((cInit*)data)->mPixelCount * 8);
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



