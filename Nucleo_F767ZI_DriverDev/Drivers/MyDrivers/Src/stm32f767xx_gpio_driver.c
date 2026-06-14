/*
 * stm32f767xx_gpio_driver.c
 *
 *  Created on: Jun 3, 2026
 *      Author: furkan
 */

#include "stm32f767xx_gpio_driver.h"

static GPIO_RegDef_t *GPIO_EXTI_PortMap[16] = {0};
/*
 * Peripheral Clock Setup
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		if (pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}
		else if (pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}
		else if (pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}
		else if (pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}
		else if (pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}
		else if (pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}
		else if (pGPIOx == GPIOG)
		{
			GPIOG_PCLK_EN();
		}
		else if (pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}
		else if (pGPIOx == GPIOI)
		{
			GPIOI_PCLK_EN();
		}
		else if (pGPIOx == GPIOJ)
		{
			GPIOJ_PCLK_EN();
		}
		else if (pGPIOx == GPIOK)
		{
			GPIOK_PCLK_EN();
		}
	}
	else
	{
		if (pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		}
		else if (pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}
		else if (pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}
		else if (pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}
		else if (pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DI();
		}
		else if (pGPIOx == GPIOF)
		{
			GPIOF_PCLK_DI();
		}
		else if (pGPIOx == GPIOG)
		{
			GPIOG_PCLK_DI();
		}
		else if (pGPIOx == GPIOH)
		{
			GPIOH_PCLK_DI();
		}
		else if (pGPIOx == GPIOI)
		{
			GPIOI_PCLK_DI();
		}
		else if (pGPIOx == GPIOJ)
		{
			GPIOJ_PCLK_DI();
		}
		else if (pGPIOx == GPIOK)
		{
			GPIOK_PCLK_DI();
		}
	}
}

/*
 * GPIO Init
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp = 0;
	/*
	 * 1. Enable peripheral clock
	 */
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	/*
	 * 2. Configure GPIO mode
	 * MODER register: 2 bits for each pin
	 */
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		temp = ((uint32_t)pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2U * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->GPIO_MODER &= ~(0x3U << (2U * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->GPIO_MODER |= temp;
	}else
	{
		/*
		 * Interrupt Mode
		 */

		/*
		 *  1. Confiure rising/falling edge trigger
		 */
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			EXTI->EXTI_FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->EXTI_RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			EXTI->EXTI_RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->EXTI_FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			EXTI->EXTI_RTSR |= (1U << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->EXTI_FTSR |= (1U << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		/*
		 * 2. Configure the GPIO port seelction in SYSCFG_EXTICR
		 */
		uint8_t exticr_index = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4U;
		uint8_t exticr_position = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4U;

		uint8_t port_code = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);

		SYSCFG_PCLK_EN();

		SYSCFG->SYSCFG_EXTICR[exticr_index] &= ~((uint32_t)0xFU << 4U * exticr_position);
		SYSCFG->SYSCFG_EXTICR[exticr_index] |= ((uint32_t)port_code << 4U * exticr_position);

		/*
		 * Store port mapping for callback layer
		 */
		GPIO_EXTI_PortMap[pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber] = pGPIOHandle->pGPIOx;

		//Enable the EXTI interrupt delivery using IMR
		EXTI->EXTI_IMR |= (1U << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}

	/*
	 * 3. Configure output type
	 * OTYPER register: 1 bit for each pin
	 */
	temp = 0;

	temp = ((uint32_t)pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->GPIO_OTYPER &= ~(0x1U << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->GPIO_OTYPER |= temp;

	/*
	 * 4. Configure output speed
	 * OSPEEDR register: 2 bits for each pin
	 */
	temp = 0;

	temp = ((uint32_t)pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2U * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->GPIO_OSPEEDR &= ~(0x3U << 2U * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->GPIO_OSPEEDR |= temp;
	/*
	 * 5. Configure pull-up / pull-down
	 * PUPDR register: 2 bits for each pin
	 */
	temp = 0;

	temp = ((uint32_t)pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2U * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->GPIO_PUPDR &= ~(0x3U << 2U * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->GPIO_PUPDR |= temp;

	/*
	 * 6. Configure alternate function if needed
	 * AFR[0] = AFRL for pins 0-7
	 * AFR[1] = AFRH for pins 8-15
	 */
	temp = 0;


	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		uint8_t register_index = 0;
		uint8_t pin_index = 0;

		register_index = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8U;
		pin_index = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8U;

		temp = ((uint32_t)pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode
				<< (4U * pin_index));

		pGPIOHandle->pGPIOx->GPIO_AFR[register_index] &= ~((uint32_t)0xFU
				<< (4U * pin_index));

		pGPIOHandle->pGPIOx->GPIO_AFR[register_index] |= temp;
	}


}

/*
 * GPIO DeInit
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if (pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}
	else if (pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}
	else if (pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}
	else if (pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}
	else if (pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}
	else if (pGPIOx == GPIOF)
	{
		GPIOF_REG_RESET();
	}
	else if (pGPIOx == GPIOG)
	{
		GPIOG_REG_RESET();
	}
	else if (pGPIOx == GPIOH)
	{
		GPIOH_REG_RESET();
	}
	else if (pGPIOx == GPIOI)
	{
		GPIOI_REG_RESET();
	}
	else if (pGPIOx == GPIOJ)
	{
		GPIOJ_REG_RESET();
	}
	else if (pGPIOx == GPIOK)
	{
		GPIOK_REG_RESET();
	}
}

/*
 * Data Read
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t)((pGPIOx->GPIO_IDR >> PinNumber) & 0x1U);
	return value;
}

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;

	value = (uint16_t)((pGPIOx->GPIO_IDR) & 0xFFFFU);
	return value;
}

/*
 * Data Write
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if(Value == GPIO_PIN_SET)
	{
		pGPIOx->GPIO_BSRR = (1U << PinNumber);
	}else
	{
		pGPIOx->GPIO_BSRR = (1U << (PinNumber + 16U) );
	}
}

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->GPIO_ODR = Value;
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->GPIO_ODR ^= (1U << PinNumber);
}

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4U;
	uint8_t iprx_section = IRQNumber % 4U;

	uint8_t shift_amount = (8U * iprx_section) + (8U - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASEADDR + iprx) &= ~((uint32_t)0xFFU << (8U * iprx_section));
	*(NVIC_PR_BASEADDR + iprx) |=  ((IRQPriority & 0x0FU) << shift_amount);
}


void GPIO_IRQHandling(uint8_t PinNumber)
{
	if (EXTI->EXTI_PR & (1U << PinNumber))
	{
		/*
		 * Clear pending bit by writing 1
		 */
		EXTI->EXTI_PR = (1U << PinNumber);

		/*
		 * Notify application layer with port and pin info
		 */
		GPIO_ApplicationEventCallback(GPIO_EXTI_PortMap[PinNumber], PinNumber);
	}
}
/*
 * Application layer Function
 */
__attribute__((weak)) void GPIO_ApplicationEventCallback(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	(void)pGPIOx;
	(void)PinNumber;
}
