/*
 * ex010_usart3_packet_10byte.c
 *
 *  Created on: Jun 6, 2026
 *      Author: furkan
 */


#include "stm32f767xx_gpio_driver.h"
#include "stm32f767xx_usart_driver.h"
#include "stm32f767xx_systick_driver.h"
#include "stm32f767xx_nvic_driver.h"
#include "examples_config.h"

#if ACTIVE_EXAMPLE == EXAMPLE_USART3_PACKET_10BYTE

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

void EX010_USART3_Packet10Byte(void)
{
	uint8_t start_msg[] = "USART3 10-byte packet test started.\r\n";
	uint8_t info_msg[]  = "Type 10 ASCII characters. Example: furkankara\r\n";
	uint8_t prefix[]    = "PACKET[10] = ";
	uint8_t newline[]   = "\r\n";

	uint8_t packet[10];
	uint16_t read_count;

	SysTick_Init(16000000U);

	USART3_GPIOInits();
	USART3_Inits();

	USART_SendDataWithTimeout(&USART3Handle,
							   start_msg,
							   sizeof(start_msg) - 1U,
							   100U);

	USART_SendDataWithTimeout(&USART3Handle,
							   info_msg,
							   sizeof(info_msg) - 1U,
							   100U);

	/*
	 * Enable USART peripheral RX interrupt.
	 */
	USART_PeripheralInterruptControl(USART3,
									 USART_INTERRUPT_RXNE,
									 ENABLE);

	/*
	 * Enable USART error interrupt.
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
		 * Main loop does not wait/block.
		 * It only checks whether at least 10 bytes are available.
		 */
		if (USART_RxAvailable(&USART3Handle) >= 10U)
		{
			read_count = USART_RxReadBytes(&USART3Handle,
										   packet,
										   10U);

			if (read_count == 10U)
			{
				USART_SendDataWithTimeout(&USART3Handle,
										   prefix,
										   sizeof(prefix) - 1U,
										   100U);

				USART_SendDataWithTimeout(&USART3Handle,
										   packet,
										   10U,
										   100U);

				USART_SendDataWithTimeout(&USART3Handle,
										   newline,
										   sizeof(newline) - 1U,
										   100U);
			}
		}

		/*
		 * Drone firmware would do other jobs here:
		 * IMU read, control loop, telemetry parser, failsafe, etc.
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

#endif /* ACTIVE_EXAMPLE == EXAMPLE_USART3_PACKET_10BYTE */
