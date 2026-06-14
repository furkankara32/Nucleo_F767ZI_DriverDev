/*
 * ex002_gpio_led_set_reset.c
 *
 *  Created on: Jun 4, 2026
 *      Author: furkan
 */

#include "stm32f767xx_gpio_driver.h"

static void delay(void)
{
	for (uint32_t i = 0; i < 500000; i++);
}

void EX002_GPIO_LedSetReset(void)
{
	GPIO_Handle_t GpioLed;

	GpioLed.pGPIOx = GPIOB;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GpioLed.GPIO_PinConfig.GPIO_PinAltFunMode = 0;

	GPIO_Init(&GpioLed);

	while (1)
	{
		GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_0, GPIO_PIN_SET);
		delay();

		GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_0, GPIO_PIN_RESET);
		delay();
	}
}
