/*
 * ex022_spi1_send_it_test.c
 *
 *  Created on: Jun 16, 2026
 *      Author: furkan
 */

#include "stm32f767xx_gpio_driver.h"
#include "stm32f767xx_spi_driver.h"
#include "stm32f767xx_nvic_driver.h"
#include "stm32f767xx_systick_driver.h"
#include "examples_config.h"

#if ACTIVE_EXAMPLE == EXAMPLE_SPI1_SEND_IT_TEST

static SPI_Handle_t SPI1Handle;

static uint8_t g_spi1_tx_buffer[] =
{
	0xA1U, 0xB2U, 0xC3U, 0xD4U,
	0xE5U, 0xF6U, 0x12U, 0x34U
};

static volatile uint8_t  g_spi1_transfer_done = 0U;
static volatile uint8_t  g_spi1_transfer_status = 0U;

static volatile uint32_t g_spi1_tx_complete_count = 0U;
static volatile uint32_t g_spi1_error_count = 0U;
static volatile uint32_t g_spi1_ovr_error_count = 0U;
static volatile uint32_t g_spi1_modf_error_count = 0U;
static volatile uint32_t g_spi1_crc_error_count = 0U;
static volatile uint32_t g_spi1_fre_error_count = 0U;

static void SPI1_GPIOInits(void)
{
	GPIO_Handle_t GpioSPI;

	/*
	 * SPI1 pins:
	 * PA5 -> SCK
	 * PA6 -> MISO
	 * PA7 -> MOSI
	 * AF5
	 */
	GpioSPI.pGPIOx = GPIOA;

	GpioSPI.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	GpioSPI.GPIO_PinConfig.GPIO_PinAltFunMode = 5U;
	GpioSPI.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioSPI.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GpioSPI.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GpioSPI.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GPIO_Init(&GpioSPI);

	GpioSPI.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&GpioSPI);

	GpioSPI.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&GpioSPI);
}

static void SPI1_Inits(void)
{
	SPI1Handle.pSPIx = SPI1;

	SPI1Handle.SPI_Config.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI1Handle.SPI_Config.SPI_BusConfig = SPI_BUS_CONFIG_FULL_DUPLEX;
	SPI1Handle.SPI_Config.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV64;
	SPI1Handle.SPI_Config.SPI_DataSize = SPI_DATA_SIZE_8BIT;
	SPI1Handle.SPI_Config.SPI_CPOL = SPI_CPOL_LOW;
	SPI1Handle.SPI_Config.SPI_CPHA = SPI_CPHA_FIRST_EDGE;
	SPI1Handle.SPI_Config.SPI_SSM = SPI_SSM_ENABLE;

	SPI_Init(&SPI1Handle);
}

void EX022_SPI1_Send_IT_Test(void)
{
	uint8_t status;

	SysTick_Init(16000000U);

	SPI1_GPIOInits();
	SPI1_Inits();

	/*
	 * Clear possible old SPI errors before starting.
	 */
	SPI_ClearErrorFlags(SPI1);

	/*
	 * Enable SPI peripheral.
	 */
	SPI_PeripheralControl(SPI1, ENABLE);

	/*
	 * Set IRQ priority first.
	 * We enable NVIC after SPI_SendDataIT() for easier debugging.
	 */
	NVIC_IRQPriorityConfig(IRQ_NO_SPI1, 15U);

	g_spi1_transfer_done = 0U;

	status = SPI_SendDataIT(&SPI1Handle,
							g_spi1_tx_buffer,
							sizeof(g_spi1_tx_buffer));

	g_spi1_transfer_status = status;

	if (status != SPI_STATUS_OK)
	{
		g_spi1_transfer_done = 1U;
	}
	else
	{
		/*
		 * Enable SPI1 interrupt in NVIC after transfer state is prepared.
		 * SPI1_IRQHandler() is already inside interrupt.c.
		 */
		NVIC_IRQInterruptConfig(IRQ_NO_SPI1, ENABLE);
	}

	while (1)
	{
		if (g_spi1_transfer_done == 1U)
		{
			/*
			 * Stop here for debugger inspection.
			 *
			 * Expected:
			 * g_spi1_transfer_status = SPI_STATUS_OK
			 * g_spi1_tx_complete_count = 1
			 * g_spi1_error_count = 0
			 * SPI1Handle.TxState = SPI_READY
			 * SPI1Handle.RxState = SPI_READY
			 */
			while (1)
			{
			}
		}
	}
}

void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,
								  uint8_t Event)
{
	if (pSPIHandle->pSPIx == SPI1)
	{
		if (Event == SPI_EVENT_TX_CMPLT)
		{
			g_spi1_tx_complete_count++;
			g_spi1_transfer_done = 1U;
		}
		else if (Event == SPI_EVENT_OVR_ERR)
		{
			g_spi1_ovr_error_count++;
			g_spi1_error_count++;
			g_spi1_transfer_done = 1U;
		}
		else if (Event == SPI_EVENT_MODF_ERR)
		{
			g_spi1_modf_error_count++;
			g_spi1_transfer_done = 1U;
		}
		else if (Event == SPI_EVENT_CRC_ERR)
		{
			g_spi1_crc_error_count++;
			g_spi1_error_count++;
			g_spi1_transfer_done = 1U;
		}
		else if (Event == SPI_EVENT_FRE_ERR)
		{
			g_spi1_fre_error_count++;
			g_spi1_error_count++;
			g_spi1_transfer_done = 1U;
		}
	}
}

#endif /* ACTIVE_EXAMPLE == EXAMPLE_SPI1_SEND_IT_TEST */
