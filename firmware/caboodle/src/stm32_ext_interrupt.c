#include <cyg/kernel/kapi.h>


#ifdef CYGHWR_HAL_CORTEXM_STM32_F4

static const cyg_uint32 interrupt_exti[] =
{
        CYGNUM_HAL_INTERRUPT_EXTI0,    //0
        CYGNUM_HAL_INTERRUPT_EXTI1,    //1
        CYGNUM_HAL_INTERRUPT_EXTI2,    //2
        CYGNUM_HAL_INTERRUPT_EXTI3,    //3
        CYGNUM_HAL_INTERRUPT_EXTI4,    //4
        CYGNUM_HAL_INTERRUPT_EXTI5,    //5
        CYGNUM_HAL_INTERRUPT_EXTI6,    //6
        CYGNUM_HAL_INTERRUPT_EXTI7,    //7
        CYGNUM_HAL_INTERRUPT_EXTI8,    //8
        CYGNUM_HAL_INTERRUPT_EXTI9,    //9
        CYGNUM_HAL_INTERRUPT_EXTI10,   //10
        CYGNUM_HAL_INTERRUPT_EXTI11,   //11
        CYGNUM_HAL_INTERRUPT_EXTI12,   //12
        CYGNUM_HAL_INTERRUPT_EXTI13,   //13
        CYGNUM_HAL_INTERRUPT_EXTI14,   //14
        CYGNUM_HAL_INTERRUPT_EXTI15,   //15
};

void enableInterrupt(cyg_uint8 port, cyg_uint8 pin, cyg_bool rising, cyg_bool falling)
{
   cyg_uint32 reg32;
   cyg_uint32 exticr = 0;
   cyg_uint32 shiftLeft = 0;
   switch(pin)
   {
   case 0:
   case 1:
   case 2:
   case 3:
#ifdef CYGHWR_HAL_CORTEXM_STM32_F4
      exticr = CYGHWR_HAL_STM32_SYSCFG + CYGHWR_HAL_STM32_SYSCFG_EXTICR1;
      shiftLeft = (pin & 0x0F) * 4;
#else
      exticr = CYGHWR_HAL_STM32_AFIO+CYGHWR_HAL_STM32_AFIO_EXTICR1;
#endif
      shiftLeft = (pin & 0x0F) * 4;
      break;
   case 4:
   case 5:
   case 6:
   case 7:
#ifdef CYGHWR_HAL_CORTEXM_STM32_F4
      exticr = CYGHWR_HAL_STM32_SYSCFG + CYGHWR_HAL_STM32_SYSCFG_EXTICR2;
      shiftLeft = ((pin - 4) & 0x0F) * 4;
#else
      exticr = CYGHWR_HAL_STM32_AFIO+CYGHWR_HAL_STM32_AFIO_EXTICR2;
#endif
      shiftLeft = ((pin - 4) & 0x0F) * 4;
      break;
   case 8:
   case 9:
   case 10:
   case 11:
#ifdef CYGHWR_HAL_CORTEXM_STM32_F4
      exticr = CYGHWR_HAL_STM32_SYSCFG + CYGHWR_HAL_STM32_SYSCFG_EXTICR3;
      shiftLeft = ((pin - 8) & 0x0F) * 4;
#else
      exticr = CYGHWR_HAL_STM32_AFIO+CYGHWR_HAL_STM32_AFIO_EXTICR3;
#endif
      shiftLeft = ((pin - 8) & 0x0F) * 4;
      break;
   case 12:
   case 13:
   case 14:
   case 15:
#ifdef CYGHWR_HAL_CORTEXM_STM32_F4
      exticr = CYGHWR_HAL_STM32_SYSCFG + CYGHWR_HAL_STM32_SYSCFG_EXTICR4;
      shiftLeft = ((pin - 12) & 0x0F) * 4;
#else
      exticr = CYGHWR_HAL_STM32_AFIO+CYGHWR_HAL_STM32_AFIO_EXTICR4;
#endif
      shiftLeft = ((pin - 12) & 0x0F) * 4;
      break;
   default:
      break;
   }

   if(exticr)
   {
      HAL_READ_UINT32(exticr, reg32);
      reg32 |= port << shiftLeft;
      HAL_WRITE_UINT32(exticr, reg32);
   }

   //enable trigger edge of interrupt
   if(rising)
   {
      HAL_READ_UINT32( CYGHWR_HAL_STM32_EXTI + CYGHWR_HAL_STM32_EXTI_RTSR, reg32 );
      reg32 |= (1 << pin);
      HAL_WRITE_UINT32( CYGHWR_HAL_STM32_EXTI + CYGHWR_HAL_STM32_EXTI_RTSR, reg32 );
   }

   if(falling)
   {
      HAL_READ_UINT32( CYGHWR_HAL_STM32_EXTI + CYGHWR_HAL_STM32_EXTI_FTSR, reg32 );
      reg32 |= (1 << pin);
      HAL_WRITE_UINT32( CYGHWR_HAL_STM32_EXTI + CYGHWR_HAL_STM32_EXTI_FTSR, reg32 );
   }
}

cyg_uint32 setupInterrupt(cyg_uint32 extiPin, cyg_bool rising, cyg_bool falling)
{
    cyg_uint32 pin = CYGHWR_HAL_STM32_GPIO_BIT(extiPin);
    cyg_uint32 port = (((extiPin) & 0xF0000) >> 16);//((extiPin >> 8) & 0xFF) >> 2;

#ifdef CYGHWR_HAL_CORTEXM_STM32_F4
    cyg_uint32 regIMR;

    CYGHWR_HAL_STM32_CLOCK_ENABLE(CYGHWR_HAL_STM32_CLOCK(APB2, SYSCFG));
    HAL_READ_UINT32( CYGHWR_HAL_STM32_EXTI + CYGHWR_HAL_STM32_EXTI_IMR, regIMR);
    regIMR |= (1 << pin);
    HAL_WRITE_UINT32( CYGHWR_HAL_STM32_EXTI + CYGHWR_HAL_STM32_EXTI_IMR, regIMR);
#else
    CYGHWR_HAL_STM32_CLOCK_ENABLE(CYGHWR_HAL_STM32_CLOCK(APB2, AFIO));
#endif

    enableInterrupt(port, pin, rising, falling);
    cyg_uint32 extInterruptVector = interrupt_exti[pin];

    return extInterruptVector;
}

#endif // CYGHWR_HAL_CORTEXM_STM32_F4
