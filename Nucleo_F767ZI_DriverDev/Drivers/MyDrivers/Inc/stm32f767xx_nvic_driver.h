/*
 * stm32f767xx_nvic_driver.h
 *
 *  Created on: Jun 6, 2026
 *      Author: furkan
 */

#ifndef MYDRIVERS_INC_STM32F767XX_NVIC_DRIVER_H_
#define MYDRIVERS_INC_STM32F767XX_NVIC_DRIVER_H_

#include "stm32f767xx.h"

void NVIC_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void NVIC_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

#endif /* MYDRIVERS_INC_STM32F767XX_NVIC_DRIVER_H_ */
