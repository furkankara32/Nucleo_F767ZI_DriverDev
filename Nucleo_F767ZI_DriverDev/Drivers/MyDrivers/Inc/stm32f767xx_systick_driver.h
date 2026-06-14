/*
 * stm32f767xx_systick_driver.h
 *
 *  Created on: Jun 5, 2026
 *      Author: furkan
 */

#ifndef MYDRIVERS_INC_STM32F767XX_SYSTICK_DRIVER_H_
#define MYDRIVERS_INC_STM32F767XX_SYSTICK_DRIVER_H_

#include "stm32f767xx.h"

/*
 * SysTick API
 */
void SysTick_Init(uint32_t system_clock_hz);
void SysTick_IncTick(void);
uint32_t millis(void);
void delay_ms(uint32_t delay);


#endif /* MYDRIVERS_INC_STM32F767XX_SYSTICK_DRIVER_H_ */
