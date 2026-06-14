/*
 * stm32f767xx_nvic_driver.c
 *
 *  Created on: Jun 6, 2026
 *      Author: furkan
 */
#include "stm32f767xx.h"
void NVIC_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	uint8_t reg_index;
	uint8_t bit_position;

	reg_index = IRQNumber / 32U;
	bit_position = IRQNumber % 32U;

	if (EnorDi == ENABLE)
	{
		*(NVIC_ISER_BASEADDR + reg_index) = (1U << bit_position);
	}
	else
	{
		*(NVIC_ICER_BASEADDR + reg_index) = (1U << bit_position);
	}
}

void NVIC_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4U;
	uint8_t iprx_section = IRQNumber % 4U;

	uint8_t shift_amount = (8U * iprx_section) + (8U - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASEADDR + iprx) &= ~((uint32_t)0xFFU << (8U * iprx_section));
	*(NVIC_PR_BASEADDR + iprx) |=  ((IRQPriority & 0x0FU) << shift_amount);
}
