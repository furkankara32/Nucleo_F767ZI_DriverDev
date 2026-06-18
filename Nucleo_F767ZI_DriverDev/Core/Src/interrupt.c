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
#include "stm32f767xx_spi_driver.h"
#include "stm32f767xx_i2c_driver.h"
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
void SPI1_IRQHandler(void)
{
	SPI_IRQHandling(SPI1);
}

void SPI2_IRQHandler(void)
{
	SPI_IRQHandling(SPI2);
}

void SPI3_IRQHandler(void)
{
	SPI_IRQHandling(SPI3);
}

void SPI4_IRQHandler(void)
{
	SPI_IRQHandling(SPI4);
}

void SPI5_IRQHandler(void)
{
	SPI_IRQHandling(SPI5);
}

void SPI6_IRQHandler(void)
{
	SPI_IRQHandling(SPI6);
}
void I2C1_EV_IRQHandler(void)
{
    I2C_EV_IRQHandling(I2C1);
}

void I2C1_ER_IRQHandler(void)
{
    I2C_ER_IRQHandling(I2C1);
}

void I2C2_EV_IRQHandler(void)
{
    I2C_EV_IRQHandling(I2C2);
}

void I2C2_ER_IRQHandler(void)
{
    I2C_ER_IRQHandling(I2C2);
}

void I2C3_EV_IRQHandler(void)
{
    I2C_EV_IRQHandling(I2C3);
}

void I2C3_ER_IRQHandler(void)
{
    I2C_ER_IRQHandling(I2C3);
}

void I2C4_EV_IRQHandler(void)
{
    I2C_EV_IRQHandling(I2C4);
}

void I2C4_ER_IRQHandler(void)
{
    I2C_ER_IRQHandling(I2C4);
}
