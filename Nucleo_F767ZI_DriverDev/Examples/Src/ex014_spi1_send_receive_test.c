/*
 * ex014_spi1_send_receive_test.c
 *
 *  Created on: Jun 11, 2026
 *      Author: furkan
 */

#include "stm32f767xx_gpio_driver.h"
#include "stm32f767xx_spi_driver.h"
#include "stm32f767xx_systick_driver.h"
#include "examples_config.h"

#if ACTIVE_EXAMPLE == EXAMPLE_SPI1_SEND_RECEIVE_TEST

static SPI_Handle_t SPI1Handle;

static volatile uint8_t g_spi_rx_status = 0U;
static volatile uint8_t g_spi_tx_status = 0U;
static volatile uint8_t g_spi_error_before_clear = 0U;
static volatile uint8_t g_spi_error_after_clear = 0U;
static volatile uint8_t g_spi_receive_pass = 0U;
static volatile uint8_t g_spi_send_pass = 0U;
static volatile uint8_t g_spi_deinit_done = 0U;

static volatile uint8_t g_spi_rx_buffer[5];

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

static uint8_t SPI_CheckAllBytes(volatile uint8_t *buffer,
                                 uint8_t expected,
                                 uint32_t len)
{
    for (uint32_t i = 0U; i < len; i++)
    {
        if (buffer[i] != expected)
        {
            return 0U;
        }
    }

    return 1U;
}

void EX014_SPI1_SendReceive_Test(void)
{
    uint8_t tx_buffer[5] =
    {
        0xA1U, 0xB2U, 0xC3U, 0xD4U, 0xE5U
    };

    SysTick_Init(16000000U);

    SPI1_GPIOInits();

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
     * TEST-1:
     * SPI_ReceiveDataWithTimeout()
     *
     * Bu fonksiyon master olduğu için clock üretmek amacıyla
     * içeride dummy byte olarak 0xFF gönderiyor.
     *
     * PA7 MOSI -> PA6 MISO loopback bağlı olduğu için
     * RX buffer içinde 0xFF bekliyoruz.
     */
    g_spi_rx_status = SPI_ReceiveDataWithTimeout(&SPI1Handle,
                                                 (uint8_t*)g_spi_rx_buffer,
                                                 sizeof(g_spi_rx_buffer),
                                                 100U);

    if (g_spi_rx_status == SPI_STATUS_OK)
    {
        g_spi_receive_pass = SPI_CheckAllBytes(g_spi_rx_buffer,
                                               0xFFU,
                                               sizeof(g_spi_rx_buffer));
    }

    /*
     * TEST-2:
     * SPI_SendDataWithTimeout()
     *
     * Bu fonksiyon veri gönderir.
     * Full-duplex modda gelen dummy veriyi de okuyup discard eder.
     * Burada asıl kontrol:
     * - timeout olmaması
     * - OVR oluşmaması
     * - fonksiyonun SPI_STATUS_OK dönmesi
     */
    g_spi_tx_status = SPI_SendDataWithTimeout(&SPI1Handle,
                                              tx_buffer,
                                              sizeof(tx_buffer),
                                              100U);

    if (g_spi_tx_status == SPI_STATUS_OK)
    {
        g_spi_send_pass = 1U;
    }

    /*
     * TEST-3:
     * Error status kontrolü.
     */
    g_spi_error_before_clear = SPI_GetErrorStatus(SPI1);

    if (g_spi_error_before_clear != SPI_ERROR_NONE)
    {
        SPI_ClearErrorFlags(SPI1);
    }

    g_spi_error_after_clear = SPI_GetErrorStatus(SPI1);

    /*
     * Transferlerden sonra SPI'ı kapat.
     */
    SPI_PeripheralControl(SPI1, DISABLE);

    /*
     * TEST-4:
     * DeInit test.
     */
    SPI_DeInit(SPI1);
    g_spi_deinit_done = 1U;

    while (1)
    {
        /*
         * Expected:
         *
         * g_spi_rx_status          = SPI_STATUS_OK
         * g_spi_tx_status          = SPI_STATUS_OK
         * g_spi_receive_pass       = 1
         * g_spi_send_pass          = 1
         * g_spi_error_after_clear  = SPI_ERROR_NONE
         * g_spi_deinit_done        = 1
         *
         * g_spi_rx_buffer:
         * FF FF FF FF FF
         */
    }
}

#endif /* ACTIVE_EXAMPLE == EXAMPLE_SPI1_SEND_RECEIVE_TEST */
