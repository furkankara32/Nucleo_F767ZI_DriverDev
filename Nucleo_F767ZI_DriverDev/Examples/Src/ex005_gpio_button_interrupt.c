/*
 * ex005_gpio_button_interrupt.c
 *
 *  Created on: Jun 5, 2026
 *      Author: furkan
 */

#include "stm32f767xx_gpio_driver.h"
#include "examples_config.h"

#if ACTIVE_EXAMPLE == EXAMPLE_GPIO_BUTTON_INTERRUPT

/*
 * This flag is set inside the GPIO callback.
 * The real application action is handled inside the main loop.
 */
static volatile uint8_t g_button_event = 0;

void EX005_GPIO_ButtonInterrupt(void)
{
	GPIO_Handle_t GpioLed;
	GPIO_Handle_t GpioButton;

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

	/*
	 * USER Button -> PC13
	 *
	 * Polling testinde butona basınca PC13 SET oluyorsa rising edge seç.
	 * Eğer interrupt gelmezse GPIO_MODE_IT_FT ile falling edge dene.
	 */
	GpioButton.pGPIOx = GPIOC;
	GpioButton.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GpioButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_RT;
	GpioButton.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	GpioButton.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GpioButton.GPIO_PinConfig.GPIO_PinAltFunMode = 0;

	GPIO_Init(&GpioButton);

	/*
	 * PC13 -> EXTI13 -> EXTI15_10 IRQ group
	 */
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10, 15U);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10, ENABLE);

	while (1)
	{
		if (g_button_event)
		{
			g_button_event = 0;

			GPIO_ToggleOutputPin(GPIOB, GPIO_PIN_NO_0);
		}
	}
}

/*
 * This function overrides the weak callback in the GPIO driver.
 */
void GPIO_ApplicationEventCallback(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	if ((pGPIOx == GPIOC) && (PinNumber == GPIO_PIN_NO_13))
	{
		g_button_event = 1;
	}
}

#endif /* ACTIVE_EXAMPLE == EXAMPLE_GPIO_BUTTON_INTERRUPT */
