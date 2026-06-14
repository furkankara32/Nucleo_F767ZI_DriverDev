/*
 * ex013_spi1_loopback_with_cs.c
 *
 *  Created on: Jun 11, 2026
 *      Author: furkan
 */

#include "stm32f767xx_gpio_driver.h"
#include "stm32f767xx_spi_driver.h"
#include "stm32f767xx_systick_driver.h"
#include "examples_config.h"

#if ACTIVE_EXAMPLE == EXAMPLE_SPI1_LOOPBACK_WITH_CS

static SPI_Handle_t SPI1Handle;

static volatile uint8_t g_spi_status = 0U;
static volatile uint8_t g_spi_test_pass = 0U;
static volatile uint8_t g_spi_rx_buffer[10];

/*
 * CS pin:
 * PD14 selected as software chip select.
 */
#define SPI1_CS_PORT      GPIOD
#define SPI1_CS_PIN       GPIO_PIN_NO_14

static void SPI1_CS_LOW(void)
{
    GPIO_WriteToOutputPin(SPI1_CS_PORT, SPI1_CS_PIN, GPIO_PIN_RESET);
}

static void SPI1_CS_HIGH(void)
{
    GPIO_WriteToOutputPin(SPI1_CS_PORT, SPI1_CS_PIN, GPIO_PIN_SET);
}

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
    GpioSPI.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GpioSPI.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    GpioSPI.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioSPI.GPIO_PinConfig.GPIO_PinAltFunMode = 5U;

    GpioSPI.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
    GPIO_Init(&GpioSPI);

    GpioSPI.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
    GPIO_Init(&GpioSPI);

    GpioSPI.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
    GPIO_Init(&GpioSPI);
}

static void SPI1_CS_GPIOInit(void)
{
    GPIO_Handle_t GpioCS;

    GpioCS.pGPIOx = SPI1_CS_PORT;
    GpioCS.GPIO_PinConfig.GPIO_PinNumber = SPI1_CS_PIN;
    GpioCS.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioCS.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GpioCS.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    GpioCS.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioCS.GPIO_PinConfig.GPIO_PinAltFunMode = 0U;

    GPIO_Init(&GpioCS);

    /*
     * Default inactive state for most SPI devices is CS HIGH.
     */
    SPI1_CS_HIGH();
}

static uint8_t SPI1_Inits(void)
{
    SPI1Handle.pSPIx = SPI1;

    SPI1Handle.SPI_Config.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    SPI1Handle.SPI_Config.SPI_BusConfig = SPI_BUS_CONFIG_FULL_DUPLEX;
    SPI1Handle.SPI_Config.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV64;
    SPI1Handle.SPI_Config.SPI_DataSize = SPI_DATA_SIZE_8BIT;
    SPI1Handle.SPI_Config.SPI_CPOL = SPI_CPOL_LOW;
    SPI1Handle.SPI_Config.SPI_CPHA = SPI_CPHA_FIRST_EDGE;
    SPI1Handle.SPI_Config.SPI_SSM = SPI_SSM_ENABLE;

    return SPI_Init(&SPI1Handle);
}

static uint8_t SPI_BufferCompare(uint8_t *tx, volatile uint8_t *rx, uint32_t len)
{
    for (uint32_t i = 0U; i < len; i++)
    {
        if (tx[i] != rx[i])
        {
            return 0U;
        }
    }

    return 1U;
}

void EX013_SPI1_Loopback_WithCS(void)
{
    uint8_t tx_buffer[10] =
    {
        0x11U, 0x22U, 0x33U, 0x44U, 0x55U,
        0x66U, 0x77U, 0x88U, 0x99U, 0xAAU
    };

    SysTick_Init(16000000U);

    SPI1_GPIOInits();
    SPI1_CS_GPIOInit();

    if (SPI1_Inits() != SPI_STATUS_OK)
    {
        while (1)
        {
            /*
             * SPI init failed.
             */
        }
    }

    SPI_PeripheralControl(SPI1, ENABLE);

    /*
     * SPI transaction starts.
     */
    SPI1_CS_LOW();

    g_spi_status = SPI_TransmitReceiveWithTimeout(&SPI1Handle,
                                                  tx_buffer,
                                                  (uint8_t*)g_spi_rx_buffer,
                                                  sizeof(tx_buffer),
                                                  100U);

    SPI1_CS_HIGH();

    /*
     * SPI transaction ends.
     */

    if (g_spi_status == SPI_STATUS_OK)
    {
        g_spi_test_pass = SPI_BufferCompare(tx_buffer,
                                            g_spi_rx_buffer,
                                            sizeof(tx_buffer));
    }

    SPI_PeripheralControl(SPI1, DISABLE);

    while (1)
    {
        /*
         * Expected:
         * g_spi_status    = SPI_STATUS_OK
         * g_spi_test_pass = 1
         * g_spi_rx_buffer = tx_buffer
         */
    }
}

#endif /* ACTIVE_EXAMPLE == EXAMPLE_SPI1_LOOPBACK_WITH_CS */
