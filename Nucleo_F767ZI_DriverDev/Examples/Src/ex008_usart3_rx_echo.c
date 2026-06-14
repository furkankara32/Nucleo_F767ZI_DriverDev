/*
 * ex008_usart3_rx_echo.c
 *
 *  Created on: Jun 6, 2026
 *      Author: furkan
 */

#include "stm32f767xx_gpio_driver.h"
#include "stm32f767xx_usart_driver.h"
#include "stm32f767xx_systick_driver.h"
#include "examples_config.h"

#if ACTIVE_EXAMPLE == EXAMPLE_USART3_RX_ECHO

static USART_Handle_t USART3Handle;

static void USART3_GPIOInits(void)
{
	GPIO_Handle_t GpioUart;

	/*
	 * USART3_TX -> PD8
	 * USART3_RX -> PD9
	 * Alternate Function: AF7
	 */
	GpioUart.pGPIOx = GPIOD;
	GpioUart.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	GpioUart.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioUart.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	GpioUart.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioUart.GPIO_PinConfig.GPIO_PinAltFunMode = 7U;

	GpioUart.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
	GPIO_Init(&GpioUart);

	GpioUart.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
	GPIO_Init(&GpioUart);
}

static void USART3_Inits(void)
{
	USART3Handle.pUSARTx = USART3;

	USART3Handle.USART_Config.USART_Mode = USART_MODE_TXRX;
	USART3Handle.USART_Config.USART_Baud = 115200U;
	USART3Handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
	USART3Handle.USART_Config.USART_StopBits = USART_STOPBITS_1;
	USART3Handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	USART3Handle.USART_Config.USART_Oversampling = USART_OVERSAMPLING_16;
	USART3Handle.USART_Config.USART_PeripheralClockHz = 0U;

	USART_Init(&USART3Handle);
}

void EX008_USART3_RxEcho(void)
{
	uint8_t rx_byte;
	uint8_t status;

	uint8_t start_msg[] = "USART3 RX echo test started. Type characters...\r\n";
	uint8_t timeout_msg[] = "RX timeout\r\n";

	SysTick_Init(16000000U);

	USART3_GPIOInits();
	USART3_Inits();

	USART_SendDataWithTimeout(&USART3Handle,
							   start_msg,
							   sizeof(start_msg) - 1U,
							   100U);

	while (1)
	{
		status = USART_ReceiveDataWithTimeout(&USART3Handle,
											  &rx_byte,
											  1U,
											  3000U);

		if (status == USART_STATUS_OK)
		{
			USART_SendDataWithTimeout(&USART3Handle,
									   &rx_byte,
									   1U,
									   100U);
		}
		else if (status == USART_STATUS_TIMEOUT)
		{
			USART_SendDataWithTimeout(&USART3Handle,
									   timeout_msg,
									   sizeof(timeout_msg) - 1U,
									   100U);
		}
		else
		{

		}
	}
}

#endif
