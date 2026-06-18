/*
 * ex020_usart3_idle_rx_test.c
 *
 *  Created on: Jun 15, 2026
 *      Author: furkan
 */

#include "stm32f767xx_gpio_driver.h"
#include "stm32f767xx_usart_driver.h"
#include "stm32f767xx_systick_driver.h"
#include "stm32f767xx_nvic_driver.h"
#include "examples_config.h"

#if ACTIVE_EXAMPLE == EXAMPLE_USART3_IDLE_RX_TEST

static USART_Handle_t USART3Handle;

static uint8_t USART3_RxRingBuffer[128];

static uint8_t g_last_packet[64];

static volatile uint32_t g_rx_byte_count = 0U;
static volatile uint32_t g_idle_count = 0U;
static volatile uint32_t g_packet_count = 0U;
static volatile uint32_t g_packet_overflow_count = 0U;
static volatile uint32_t g_rx_ring_overflow_count = 0U;
static volatile uint32_t g_error_count = 0U;

static volatile uint8_t  g_packet_ready = 0U;
static volatile uint16_t g_last_packet_len = 0U;

static volatile uint8_t  g_tx_done = 1U;
static volatile uint32_t g_tx_complete_count = 0U;
static volatile uint32_t g_tx_busy_count = 0U;

static uint8_t g_start_msg[] =
	"USART3 IDLE RX test started.\r\n"
	"Send text from terminal. IDLE interrupt marks packet end.\r\n";

static uint8_t g_packet_msg[] =
	"[IDLE] packet received.\r\n";

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

static void USART3_ReadPacketFromRingBuffer(void)
{
	uint8_t rx_byte;
	uint16_t len = 0U;

	while (USART_RxReadByte(&USART3Handle, &rx_byte) == USART_STATUS_OK)
	{
		if (len < (sizeof(g_last_packet) - 1U))
		{
			g_last_packet[len] = rx_byte;
			len++;
		}
		else
		{
			g_packet_overflow_count++;
		}
	}

	g_last_packet[len] = '\0';
	g_last_packet_len = len;

	if (len > 0U)
	{
		g_packet_count++;
	}
}

void EX020_USART3_IdleRx_Test(void)
{
	uint8_t status;

	SysTick_Init(16000000U);

	USART3_GPIOInits();
	USART3_Inits();

	/*
	 * Startup message can be blocking.
	 * The tested part is RXNE + IDLE interrupt.
	 */
	USART_SendDataWithTimeout(&USART3Handle,
							   g_start_msg,
							   sizeof(g_start_msg) - 1U,
							   100U);

	/*
	 * Clear possible old IDLE flag before enabling IDLE interrupt.
	 */
	USART3->USART_ICR = (1U << USART_ICR_IDLECF);

	/*
	 * Enable USART peripheral interrupts.
	 */
	USART_PeripheralInterruptControl(USART3,
									 USART_INTERRUPT_RXNE,
									 ENABLE);

	USART_PeripheralInterruptControl(USART3,
									 USART_INTERRUPT_IDLE,
									 ENABLE);

	USART_PeripheralInterruptControl(USART3,
									 USART_INTERRUPT_ERR,
									 ENABLE);

	/*
	 * Enable USART3 interrupt in NVIC.
	 * USART3_IRQHandler() is already in interrupt.c.
	 */
	NVIC_IRQPriorityConfig(IRQ_NO_USART3, 15U);
	NVIC_IRQInterruptConfig(IRQ_NO_USART3, ENABLE);

	while (1)
	{
		if (g_packet_ready == 1U)
		{
			g_packet_ready = 0U;

			USART3_ReadPacketFromRingBuffer();

			/*
			 * Send a short non-blocking notification.
			 * Actual received packet can be inspected in g_last_packet.
			 */
			if ((g_last_packet_len > 0U) && (g_tx_done == 1U))
			{
				g_tx_done = 0U;

				status = USART_SendDataIT(&USART3Handle,
										  g_packet_msg,
										  sizeof(g_packet_msg) - 1U);

				if (status == USART_STATUS_BUSY)
				{
					g_tx_busy_count++;
					g_tx_done = 1U;
				}
				else if (status != USART_STATUS_OK)
				{
					g_tx_done = 1U;
				}
			}
		}

		/*
		 * Other application tasks can run here.
		 */
	}
}

void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,
									uint8_t Event)
{
	if (pUSARTHandle->pUSARTx == USART3)
	{
		if (Event == USART_EVENT_RX_BYTE_RECEIVED)
		{
			g_rx_byte_count++;
		}
		else if (Event == USART_EVENT_IDLE)
		{
			g_idle_count++;
			g_packet_ready = 1U;
		}
		else if (Event == USART_EVENT_RX_OVERFLOW)
		{
			g_rx_ring_overflow_count++;
		}
		else if (Event == USART_EVENT_TX_CMPLT)
		{
			g_tx_complete_count++;
			g_tx_done = 1U;
		}
		else if ((Event == USART_EVENT_PARITY_ERROR) ||
				 (Event == USART_EVENT_FRAME_ERROR) ||
				 (Event == USART_EVENT_NOISE_ERROR) ||
				 (Event == USART_EVENT_OVERRUN_ERROR))
		{
			g_error_count++;
		}
	}
}

#endif /* ACTIVE_EXAMPLE == EXAMPLE_USART3_IDLE_RX_TEST */
