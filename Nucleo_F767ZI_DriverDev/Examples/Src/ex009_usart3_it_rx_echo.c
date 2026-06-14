/*
 * ex009_usart3_it_rx_echo.c
 *
 *  Created on: Jun 6, 2026
 *      Author: furkan
 */

#include "stm32f767xx_gpio_driver.h"
#include "stm32f767xx_usart_driver.h"
#include "stm32f767xx_systick_driver.h"
#include "stm32f767xx_nvic_driver.h"
#include "examples_config.h"

#if ACTIVE_EXAMPLE == EXAMPLE_USART3_IT_RX_ECHO

static USART_Handle_t USART3Handle;
static uint8_t USART3_RxRingBuffer[128];

static volatile uint32_t g_usart3_rx_overflow_count = 0;
static volatile uint32_t g_usart3_error_count = 0;

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

	USART_RxRingBufferInit(&USART3Handle,
							USART3_RxRingBuffer,
							sizeof(USART3_RxRingBuffer));
}

void EX009_USART3_InterruptRxEcho(void)
{
	uint8_t start_msg[] = "USART3 interrupt RX echo test started.\r\n";
	uint8_t rx_byte;

	SysTick_Init(16000000U);

	USART3_GPIOInits();
	USART3_Inits();

	USART_SendDataWithTimeout(&USART3Handle,
							   start_msg,
							   sizeof(start_msg) - 1U,
							   100U);

	/*
	 * Enable USART peripheral RX interrupt.
	 */
	USART_PeripheralInterruptControl(USART3,
									 USART_INTERRUPT_RXNE,
									 ENABLE);

	/*
	 * Enable USART error interrupt.
	 * This catches framing/noise/overrun errors.
	 */
	USART_PeripheralInterruptControl(USART3,
									 USART_INTERRUPT_ERR,
									 ENABLE);

	/*
	 * Enable USART3 interrupt in NVIC.
	 */
	NVIC_IRQPriorityConfig(IRQ_NO_USART3, 15U);
	NVIC_IRQInterruptConfig(IRQ_NO_USART3, ENABLE);

	while (1)
	{
		/*
		 * Main loop consumes RX ring buffer.
		 * No blocking receive here.
		 */
		while (USART_RxReadByte(&USART3Handle, &rx_byte) == USART_STATUS_OK)
		{
			/*
			 * Echo received byte back to PC.
			 * TX still uses timeout-protected polling.
			 */
			USART_SendDataWithTimeout(&USART3Handle,
									   &rx_byte,
									   1U,
									   10U);
		}

		/*
		 * Drone firmware would do other tasks here:
		 * IMU, PID, motor update, failsafe, parser, etc.
		 */
	}
}

void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,
									uint8_t Event)
{
	if (pUSARTHandle->pUSARTx == USART3)
	{
		if (Event == USART_EVENT_RX_OVERFLOW)
		{
			g_usart3_rx_overflow_count++;
		}
		else if ((Event == USART_EVENT_PARITY_ERROR) ||
				 (Event == USART_EVENT_FRAME_ERROR) ||
				 (Event == USART_EVENT_NOISE_ERROR) ||
				 (Event == USART_EVENT_OVERRUN_ERROR))
		{
			g_usart3_error_count++;
		}
	}
}

#endif /* ACTIVE_EXAMPLE == EXAMPLE_USART3_IT_RX_ECHO */
