/*
 * ex003_gpio_button_input.c
 *
 *  Created on: Jun 4, 2026
 *      Author: furkan
 */

#include "stm32f767xx_gpio_driver.h"

void EX003_GPIO_ButtonInput(void)
{
	GPIO_Handle_t GpioLed;
	GPIO_Handle_t GpioButton;

	/*
	 * LED configuration
	 * LD1 Green LED -> PB0
	 */
	GpioLed.pGPIOx = GPIOB;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GpioLed.GPIO_PinConfig.GPIO_PinAltFunMode = 0;

	GPIO_Init(&GpioLed);

	/*
	 * Button configuration
	 * USER Button -> PC13
	 */
	GpioButton.pGPIOx = GPIOC;
	GpioButton.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GpioButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GpioButton.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	GpioButton.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GpioButton.GPIO_PinConfig.GPIO_PinAltFunMode = 0;

	GPIO_Init(&GpioButton);

	while (1)
	{
		if (GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == GPIO_PIN_SET)
		{
			GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_0, GPIO_PIN_SET);
		}
		else
		{
			GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_0, GPIO_PIN_RESET);
		}
	}
}
