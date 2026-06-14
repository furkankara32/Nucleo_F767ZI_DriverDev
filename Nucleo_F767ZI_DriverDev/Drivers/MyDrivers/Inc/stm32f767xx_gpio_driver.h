/*
 * stm32f767xx_gpio_driver.h
 *
 *  Created on: Jun 3, 2026
 *      Author: furkan
 */

#ifndef MYDRIVERS_INC_STM32F767XX_GPIO_DRIVER_H_
#define MYDRIVERS_INC_STM32F767XX_GPIO_DRIVER_H_

#include "stm32f767xx.h"

typedef struct
{
	uint8_t GPIO_PinNumber;
	uint8_t	GPIO_PinMode;
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFunMode;
}GPIO_PinConfig_t;

typedef struct
{
	GPIO_RegDef_t		*pGPIOx;
	GPIO_PinConfig_t 	GPIO_PinConfig;
}GPIO_Handle_t;

/*
 * Peripheral clock Setup
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

/*
 * Init and de-init GPIO
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOX);

/*
 * Data read and write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/*
 * IRQ configuration and ISR handling
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);
void GPIO_ApplicationEventCallback(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
#endif /* MYDRIVERS_INC_STM32F767XX_GPIO_DRIVER_H_ */
