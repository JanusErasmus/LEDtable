#ifndef VAR_IO_H_
#define VAR_IO_H_
// Some defines specific to the STM32 connectivity line not available in the current eCos source


#define CYGHWR_HAL_STM32_RCC_CFGR2 0x2C

#define CYGHWR_HAL_STM32_RCC_CR_PLL3RDY         BIT_(29)
#define CYGHWR_HAL_STM32_RCC_CR_PLL3ON          BIT_(28)

#define CYGHWR_HAL_STM32_RCC_CFGR_MCO_PLL2      VALUE_(24,8)
#define CYGHWR_HAL_STM32_RCC_CFGR_MCO_PLL3DIV2  VALUE_(24,9)
#define CYGHWR_HAL_STM32_RCC_CFGR_MCO_XT1       VALUE_(24,10)
#define CYGHWR_HAL_STM32_RCC_CFGR_MCO_PLL3      VALUE_(24,11)

#define CYGHWR_HAL_STM32_RCC_AHBENR_ETHMACRX    BIT_(16)
#define CYGHWR_HAL_STM32_RCC_AHBENR_ETHMACTX    BIT_(15)
#define CYGHWR_HAL_STM32_RCC_AHBENR_ETHMAC      BIT_(14)
#define CYGHWR_HAL_STM32_RCC_AHBENR_OTGFS       BIT_(12)

#define CYGHWR_HAL_STM32_RCC_CFGR2_I2S3SRC_SYSCLK    0
#define CYGHWR_HAL_STM32_RCC_CFGR2_I2S3SRC_PLL3      BIT_(18)
#define CYGHWR_HAL_STM32_RCC_CFGR2_I2S2SRC_SYSCLK    0
#define CYGHWR_HAL_STM32_RCC_CFGR2_I2S2SRC_PLL3      BIT_(17)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PREDIV1SRC_HSE    0
#define CYGHWR_HAL_STM32_RCC_CFGR2_PREDIV1SRC_PLL2   BIT_(16)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL3MUL_8    VALUE_(12,6)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL3MUL_9    VALUE_(12,7)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL3MUL_10   VALUE_(12,8)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL3MUL_11   VALUE_(12,9)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL3MUL_12   VALUE_(12,10)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL3MUL_13   VALUE_(12,11)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL3MUL_14   VALUE_(12,12)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL3MUL_16   VALUE_(12,14)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL3MUL_20   VALUE_(12,15)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL2MUL_8    VALUE_(8,6)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL2MUL_9    VALUE_(8,7)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL2MUL_10   VALUE_(8,8)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL2MUL_11   VALUE_(8,9)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL2MUL_12   VALUE_(8,10)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL2MUL_13   VALUE_(8,11)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL2MUL_14   VALUE_(8,12)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL2MUL_16   VALUE_(8,14)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PLL2MUL_20   VALUE_(8,15)
#define CYGHWR_HAL_STM32_RCC_CFGR2_PREDIV2(__x) VALUE_(4,(__x))

#define CYGHWR_HAL_STM32_ADC_SR                 0x00
#define CYGHWR_HAL_STM32_ADC_CR1                0x04
#define CYGHWR_HAL_STM32_ADC_CR2                0x08
#define CYGHWR_HAL_STM32_ADC_SMPR1              0x0C
#define CYGHWR_HAL_STM32_ADC_SMPR2              0x10
#define CYGHWR_HAL_STM32_ADC_JOFR1              0x14
#define CYGHWR_HAL_STM32_ADC_JOFR2              0x18
#define CYGHWR_HAL_STM32_ADC_JOFR3              0x1C
#define CYGHWR_HAL_STM32_ADC_JOFR4              0x20
#define CYGHWR_HAL_STM32_ADC_HTR                0x24
#define CYGHWR_HAL_STM32_ADC_LTR                0x28
#define CYGHWR_HAL_STM32_ADC_SQR1               0x2C
#define CYGHWR_HAL_STM32_ADC_SQR2               0x30
#define CYGHWR_HAL_STM32_ADC_SQR3               0x34
#define CYGHWR_HAL_STM32_ADC_JSQR               0x38
#define CYGHWR_HAL_STM32_ADC_JDR1               0x3C
#define CYGHWR_HAL_STM32_ADC_JDR2               0x40
#define CYGHWR_HAL_STM32_ADC_JDR3               0x44
#define CYGHWR_HAL_STM32_ADC_JDR4               0x48
#define CYGHWR_HAL_STM32_ADC_DR                 0x4C

#define CYGHWR_HAL_STM32_ADC_SR_EOC             BIT_(1)

#define CYGHWR_HAL_STM32_ADC_CR1_DISCEN			BIT_(11)
#define CYGHWR_HAL_STM32_ADC_CR1_DISCNUM(__x)	VALUE_(13,(__x))

#define CYGHWR_HAL_STM32_ADC_CR2_ADON           BIT_(0)
#define CYGHWR_HAL_STM32_ADC_CR2_CONT           BIT_(1)
#define CYGHWR_HAL_STM32_ADC_CR2_CAL            BIT_(2)
#define CYGHWR_HAL_STM32_ADC_CR2_RSTCAL         BIT_(3)
#define CYGHWR_HAL_STM32_ADC_CR2_EXTSEL(__x)	VALUE_(17,(__x))
#define CYGHWR_HAL_STM32_ADC_CR2_EXTTRIG        BIT_(20)
#define CYGHWR_HAL_STM32_ADC_CR2_SWSTART        BIT_(22)
#define CYGHWR_HAL_STM32_ADC_CR2_TSVREFE        BIT_(23)

#define CYGHWR_HAL_STM32_ADC_SMPR1_SMP10(__x)	VALUE_(0,(__x))
#define CYGHWR_HAL_STM32_ADC_SMPR1_SMP11(__x)	VALUE_(3,(__x))
#define CYGHWR_HAL_STM32_ADC_SMPR1_SMP12(__x)	VALUE_(6,(__x))
#define CYGHWR_HAL_STM32_ADC_SMPR1_SMP13(__x)	VALUE_(9,(__x))
#define CYGHWR_HAL_STM32_ADC_SMPR1_SMP14(__x)	VALUE_(12,(__x))
#define CYGHWR_HAL_STM32_ADC_SMPR1_SMP15(__x)	VALUE_(15,(__x))
#define CYGHWR_HAL_STM32_ADC_SMPR1_SMP16(__x)	VALUE_(18,(__x))
#define CYGHWR_HAL_STM32_ADC_SMPR1_SMP17(__x)	VALUE_(21,(__x))
#define CYGHWR_HAL_STM32_ADC_SMPR2_SMP0(__x)	VALUE_(0,(__x))
#define CYGHWR_HAL_STM32_ADC_SMPR2_SMP1(__x)	VALUE_(3,(__x))
#define CYGHWR_HAL_STM32_ADC_SMPR2_SMP2(__x)	VALUE_(6,(__x))
#define CYGHWR_HAL_STM32_ADC_SMPR2_SMP3(__x)	VALUE_(9,(__x))
#define CYGHWR_HAL_STM32_ADC_SMPR2_SMP4(__x)	VALUE_(12,(__x))
#define CYGHWR_HAL_STM32_ADC_SMPR2_SMP5(__x)	VALUE_(15,(__x))
#define CYGHWR_HAL_STM32_ADC_SMPR2_SMP6(__x)	VALUE_(18,(__x))
#define CYGHWR_HAL_STM32_ADC_SMPR2_SMP7(__x)	VALUE_(21,(__x))
#define CYGHWR_HAL_STM32_ADC_SMPR2_SMP8(__x)	VALUE_(24,(__x))
#define CYGHWR_HAL_STM32_ADC_SMPR2_SMP9(__x)	VALUE_(27,(__x))

#define CYGHWR_HAL_STM32_ADC_SQR1_L(__x)		VALUE_(20,(__x))
#define CYGHWR_HAL_STM32_ADC_SQR1_SQ13(__x)		VALUE_(0,(__x))
#define CYGHWR_HAL_STM32_ADC_SQR1_SQ14(__x)		VALUE_(5,(__x))
#define CYGHWR_HAL_STM32_ADC_SQR1_SQ15(__x)		VALUE_(10,(__x))
#define CYGHWR_HAL_STM32_ADC_SQR1_SQ16(__x)		VALUE_(15,(__x))
#define CYGHWR_HAL_STM32_ADC_SQR2_SQ7(__x)		VALUE_(0,(__x))
#define CYGHWR_HAL_STM32_ADC_SQR2_SQ8(__x)		VALUE_(5,(__x))
#define CYGHWR_HAL_STM32_ADC_SQR2_SQ9(__x)		VALUE_(10,(__x))
#define CYGHWR_HAL_STM32_ADC_SQR2_SQ10(__x)		VALUE_(15,(__x))
#define CYGHWR_HAL_STM32_ADC_SQR2_SQ11(__x)		VALUE_(20,(__x))
#define CYGHWR_HAL_STM32_ADC_SQR2_SQ12(__x)		VALUE_(25,(__x))
#define CYGHWR_HAL_STM32_ADC_SQR3_SQ1(__x)		VALUE_(0,(__x))
#define CYGHWR_HAL_STM32_ADC_SQR3_SQ2(__x)		VALUE_(5,(__x))
#define CYGHWR_HAL_STM32_ADC_SQR3_SQ3(__x)		VALUE_(10,(__x))
#define CYGHWR_HAL_STM32_ADC_SQR3_SQ4(__x)		VALUE_(15,(__x))
#define CYGHWR_HAL_STM32_ADC_SQR3_SQ5(__x)		VALUE_(20,(__x))
#define CYGHWR_HAL_STM32_ADC_SQR3_SQ6(__x)		VALUE_(25,(__x))

#define CYGHWR_HAL_STM32_TIM_SMCR                0x08
#define CYGHWR_HAL_STM32_TIM_CCMR1               0x18
#define CYGHWR_HAL_STM32_TIM_CCMR2               0x1C
#define CYGHWR_HAL_STM32_TIM_CCER                0x20
#define CYGHWR_HAL_STM32_TIM_CCR1                0x34
#define CYGHWR_HAL_STM32_TIM_CCR2                0x38
#define CYGHWR_HAL_STM32_TIM_CCR3                0x3C
#define CYGHWR_HAL_STM32_TIM_CCR4                0x40
#define CYGHWR_HAL_STM32_TIM_DCR                 0x48
#define CYGHWR_HAL_STM32_TIM_DMAR                0x4C

#define CYGHWR_HAL_STM32_TIM_CR1_CKD_1			VALUE_(8,0)
#define CYGHWR_HAL_STM32_TIM_CR1_CKD_2			VALUE_(8,1)
#define CYGHWR_HAL_STM32_TIM_CR1_CKD_4			VALUE_(8,2)

#define CYGHWR_HAL_STM32_TIM_DIER_CC1IE			BIT_(1)
#define CYGHWR_HAL_STM32_TIM_DIER_CC2IE			BIT_(2)
#define CYGHWR_HAL_STM32_TIM_DIER_CC3IE			BIT_(3)
#define CYGHWR_HAL_STM32_TIM_DIER_CC4IE			BIT_(4)

#define CYGHWR_HAL_STM32_TIM_SR_CC1IF			BIT_(1)
#define CYGHWR_HAL_STM32_TIM_SR_CC2IF			BIT_(2)
#define CYGHWR_HAL_STM32_TIM_SR_CC3IF			BIT_(3)
#define CYGHWR_HAL_STM32_TIM_SR_CC4IF			BIT_(4)
#define CYGHWR_HAL_STM32_TIM_SR_CC1OF			BIT_(9)
#define CYGHWR_HAL_STM32_TIM_SR_CC2OF			BIT_(10)
#define CYGHWR_HAL_STM32_TIM_SR_CC3OF			BIT_(11)
#define CYGHWR_HAL_STM32_TIM_SR_CC4OF			BIT_(12)


#define CYGHWR_HAL_STM32_TIM_CCMR1_CC1S_OUTPUT	VALUE_(0,0)
#define CYGHWR_HAL_STM32_TIM_CCMR1_CC1S_T1		VALUE_(0,1)
#define CYGHWR_HAL_STM32_TIM_CCMR1_CC1S_T2		VALUE_(0,2)
#define CYGHWR_HAL_STM32_TIM_CCMR1_CC1S_TRC		VALUE_(0,3)
#define CYGHWR_HAL_STM32_TIM_CCMR1_CC2S_T1		VALUE_(8,2)
#define CYGHWR_HAL_STM32_TIM_CCMR1_CC2S_T2		VALUE_(8,1)
#define CYGHWR_HAL_STM32_TIM_CCMR1_CC2S_TRC		VALUE_(8,3)
#define CYGHWR_HAL_STM32_TIM_CCMR1_IC1PSC_1		VALUE_(2,0)
#define CYGHWR_HAL_STM32_TIM_CCMR1_IC1PSC_2		VALUE_(2,1)
#define CYGHWR_HAL_STM32_TIM_CCMR1_IC1PSC_4		VALUE_(2,2)
#define CYGHWR_HAL_STM32_TIM_CCMR1_IC1PSC_8		VALUE_(2,3)
#define CYGHWR_HAL_STM32_TIM_CCMR1_IC2PSC_1		VALUE_(10,0)
#define CYGHWR_HAL_STM32_TIM_CCMR1_IC2PSC_2		VALUE_(10,1)
#define CYGHWR_HAL_STM32_TIM_CCMR1_IC2PSC_4		VALUE_(10,2)
#define CYGHWR_HAL_STM32_TIM_CCMR1_IC2PSC_8		VALUE_(10,3)
#define CYGHWR_HAL_STM32_TIM_CCMR1_IC1F_(__x)	VALUE_(4,(__x))
#define CYGHWR_HAL_STM32_TIM_CCMR1_IC2F_(__x)	VALUE_(12,(__x))

#define CYGHWR_HAL_STM32_TIM_CCER_CC1E			BIT_(0)
#define CYGHWR_HAL_STM32_TIM_CCER_CC1P			BIT_(1)
#define CYGHWR_HAL_STM32_TIM_CCER_CC2E			BIT_(4)
#define CYGHWR_HAL_STM32_TIM_CCER_CC2P			BIT_(5)

#define CYGHWR_HAL_STM32_IWDG_KR			0x00		//Independent Watchdog Key register
#define CYGHWR_HAL_STM32_IWDG_PR			0x04		//Independent Watchdog Prescale register
#define CYGHWR_HAL_STM32_IWDG_RLR			0x08		//Independent Watchdog Reload register
#define CYGHWR_HAL_STM32_IWDG_SR			0x0C		//Independent Watchdog Status register
#define CYGHWR_HAL_STM32_IWDG_SR_PVU		BIT_(0)		//Independent Watchdog prescaler value update
#define CYGHWR_HAL_STM32_IWDG_SR_RVU		BIT_(0)		//Independent Watchdog counter reload value update

#define CYGHWR_HAL_STM32_WWDG_CR			0x00		//Window Watchdog Control Register
#define CYGHWR_HAL_STM32_WWDG_CFR			0x04		//Window Watchdog Configuration Register
#define CYGHWR_HAL_STM32_WWDG_SR			0x08		//Window Watchdog Status Register


//http://www.ecoscentric.com/ecospro/doc.cgi/html/ecospro-ref/cortexm-stm32-gpio.html
//Short desciption of macros

#define setup_pin(pin)               CYGHWR_HAL_STM32_GPIO_SET(pin)
#define set_pinH(pin)                CYGHWR_HAL_STM32_GPIO_OUT(pin, 1 )
#define set_pinL(pin)                CYGHWR_HAL_STM32_GPIO_OUT(pin, 0 )
#define read_pin(pin, pointerval)    CYGHWR_HAL_STM32_GPIO_IN(pin,  pointerval )

//These macros should not be used, pins should be defined below and above macros should be used.
#define setup_pin_in(port, bit)      CYGHWR_HAL_STM32_GPIO_SET(CYGHWR_HAL_STM32_GPIO(port, bit, IN, PULL ))
#define setup_pin_out(port, bit)     CYGHWR_HAL_STM32_GPIO_SET(CYGHWR_HAL_STM32_GPIO(port, bit, OUT_50MHZ, GPOPP ))
#define set_pinHi(port, bit)          CYGHWR_HAL_STM32_GPIO_OUT(CYGHWR_HAL_STM32_GPIO(port, bit, OUT_50MHZ, GPOPP), 1 )
#define set_pinLo(port, bit)          CYGHWR_HAL_STM32_GPIO_OUT(CYGHWR_HAL_STM32_GPIO(port, bit, OUT_50MHZ, GPOPP), 0 )
#define read_pinb(port, bit, pointerval) CYGHWR_HAL_STM32_GPIO_IN(pin,  pointerval ) //Not tested yet, pointers

#endif /* VAR_IO_H_ */
