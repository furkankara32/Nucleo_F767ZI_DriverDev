/*
 * stm32f767xx_rcc_driver.h
 *
 *  Created on: Jun 6, 2026
 *      Author: furkan
 */

#ifndef MYDRIVERS_INC_STM32F767XX_RCC_DRIVER_H_
#define MYDRIVERS_INC_STM32F767XX_RCC_DRIVER_H_

#include "stm32f767xx.h"

uint32_t RCC_GetSYSCLKValue(void);
uint32_t RCC_GetHCLKValue(void);
uint32_t RCC_GetPCLK1Value(void);
uint32_t RCC_GetPCLK2Value(void);

#endif /* MYDRIVERS_INC_STM32F767XX_RCC_DRIVER_H_ */
