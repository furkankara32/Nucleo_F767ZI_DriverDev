/*
 * ex004_gpio_deinit_test.c
 *
 *  Created on: Jun 4, 2026
 *      Author: furkan
 */

#include "stm32f767xx_gpio_driver.h"

static void delay(void)
{
	for (uint32_t i = 0; i < 500000; i++);
}

void EX004_GPIO_DeInitTest(void)
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

	GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_0, GPIO_PIN_SET);
	delay();

	GPIO_DeInit(GPIOB);

	while (1)
	{
		/*
		 * GPIOB resetlendiği için PB0 artık output modunda değildir.
		 * Bu toggle işlemi LED'i beklediğimiz şekilde sürmemelidir.
		 */
		GPIO_ToggleOutputPin(GPIOB, GPIO_PIN_NO_0);
		delay();
	}
}
