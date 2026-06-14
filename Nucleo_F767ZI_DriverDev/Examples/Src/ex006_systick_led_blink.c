/*
 * ex006_systick_led_blink.c
 *
 *  Created on: Jun 5, 2026
 *      Author: furkan
 */
#include "stm32f767xx_gpio_driver.h"
#include "stm32f767xx_systick_driver.h"
#include "examples_config.h"

#if ACTIVE_EXAMPLE == EXAMPLE_SYSTICK_LED_BLINK

void EX006_SysTickLedBlink(void)
{
	GPIO_Handle_t GpioLed;

	/*
	 * Default reset clock is usually HSI = 16 MHz.
	 * We are not configuring PLL yet.
	 */
	SysTick_Init(16000000U);

	/*
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

	while (1)
	{
		GPIO_ToggleOutputPin(GPIOB, GPIO_PIN_NO_0);
		delay_ms(500);
	}
}

#endif /* ACTIVE_EXAMPLE == EXAMPLE_SYSTICK_LED_BLINK */
