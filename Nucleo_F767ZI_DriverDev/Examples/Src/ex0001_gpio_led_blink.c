/*
 * ex0001_gpio_led_blink.c
 *
 *  Created on: Jun 4, 2026
 *      Author: furkan
 */

#include "stm32f767xx_gpio_driver.h"

static void delay(void)
{
	for (uint32_t i = 0; i < 500000; i++);
}

void EX001_GPIO_LedBlink(void)
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

	while(1)
	{
		GPIO_ToggleOutputPin(GPIOB, GPIO_PIN_NO_0);
		delay();
	}
}
