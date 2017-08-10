#ifndef INCLUDE_STM32_EXT_INTERRUPT_H_
#define INCLUDE_STM32_EXT_INTERRUPT_H_

#ifdef __cplusplus
extern "C"  {
#endif

cyg_uint32 setupInterrupt(cyg_uint32 extiPin, cyg_bool rising, cyg_bool falling);

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_STM32_EXT_INTERRUPT_H_ */
