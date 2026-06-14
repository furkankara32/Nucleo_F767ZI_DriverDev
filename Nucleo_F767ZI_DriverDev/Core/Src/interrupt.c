/*
 * interrupt.c
 *
 *  Created on: Jun 5, 2026
 *      Author: furkan
 */
#include <stdint.h>
#include "stm32f767xx_gpio_driver.h"
#include "stm32f767xx_systick_driver.h"
#include "stm32f767xx_usart_driver.h"
void EXTI0_IRQHandler(void)
{
	GPIO_IRQHandling(GPIO_PIN_NO_0);
}

void EXTI1_IRQHandler(void)
{
	GPIO_IRQHandling(GPIO_PIN_NO_1);
}

void EXTI2_IRQHandler(void)
{
	GPIO_IRQHandling(GPIO_PIN_NO_2);
}

void EXTI3_IRQHandler(void)
{
	GPIO_IRQHandling(GPIO_PIN_NO_3);
}

void EXTI4_IRQHandler(void)
{
	GPIO_IRQHandling(GPIO_PIN_NO_4);
}

void EXTI9_5_IRQHandler(void)
{
	for (uint8_t pin = GPIO_PIN_NO_5; pin <= GPIO_PIN_NO_9; pin++)
	{
		GPIO_IRQHandling(pin);
	}
}

void EXTI15_10_IRQHandler(void)
{
	for (uint8_t pin = GPIO_PIN_NO_10; pin <= GPIO_PIN_NO_15; pin++)
	{
		GPIO_IRQHandling(pin);
	}
}
void SysTick_Handler(void)
{
	SysTick_IncTick();
}
void USART1_IRQHandler(void)
{
	USART_IRQHandling(USART1);
}
void USART2_IRQHandler(void)
{
	USART_IRQHandling(USART2);
}
void USART3_IRQHandler(void)
{
	USART_IRQHandling(USART3);
}

void UART4_IRQHandler(void)
{
	USART_IRQHandling(UART4);
}
void UART5_IRQHandler(void)
{
	USART_IRQHandling(UART5);
}
void USART6_IRQHandler(void)
{
	USART_IRQHandling(USART6);
}
void UART7_IRQHandler(void)
{
	USART_IRQHandling(UART7);
}
void UART8_IRQHandler(void)
{
	USART_IRQHandling(UART8);
}
