/*
 * ex018_i2c1_ssd1306_command_test.c
 *
 *  Created on: Jun 14, 2026
 *      Author: furkan
 */

#include "stm32f767xx_gpio_driver.h"
#include "stm32f767xx_i2c_driver.h"
#include "stm32f767xx_systick_driver.h"
#include "examples_config.h"

#if ACTIVE_EXAMPLE == EXAMPLE_I2C1_SSD1306_COMMAND_TEST

#define SSD1306_I2C_ADDR        0x3CU
#define SSD1306_WIDTH           128U
#define SSD1306_HEIGHT          64U
#define SSD1306_PAGES           8U

static I2C_Handle_t I2C1Handle;

static volatile uint8_t g_oled_init_status = 0U;
static volatile uint8_t g_oled_clear_status = 0U;
static volatile uint8_t g_oled_fill_status = 0U;
static volatile uint8_t g_oled_test_pass = 0U;

static void I2C1_GPIOInits(void)
{
    GPIO_Handle_t GpioI2C;

    /*
     * I2C1 pins:
     * PB8 -> I2C1_SCL
     * PB9 -> I2C1_SDA
     * AF4
     */
    GpioI2C.pGPIOx = GPIOB;
    GpioI2C.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    GpioI2C.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
    GpioI2C.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    GpioI2C.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioI2C.GPIO_PinConfig.GPIO_PinAltFunMode = 4U;

    GpioI2C.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
    GPIO_Init(&GpioI2C);

    GpioI2C.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
    GPIO_Init(&GpioI2C);
}

static uint8_t I2C1_Inits(void)
{
    I2C1Handle.pI2Cx = I2C1;

    I2C1Handle.I2C_Config.I2C_Timing = I2C_TIMING_16MHZ_100KHZ;
    I2C1Handle.I2C_Config.I2C_AnalogFilter = I2C_ANALOG_FILTER_ENABLE;
    I2C1Handle.I2C_Config.I2C_DigitalFilter = 0U;
    I2C1Handle.I2C_Config.I2C_ClockStretch = I2C_CLOCK_STRETCH_ENABLE;

    return I2C_Init(&I2C1Handle);
}

static uint8_t SSD1306_WriteCommand(uint8_t command)
{
    uint8_t tx_data[2];

    /*
     * SSD1306 I2C control byte:
     * 0x00 -> following byte is command
     */
    tx_data[0] = 0x00U;
    tx_data[1] = command;

    return I2C_MasterTransmitWithTimeout(&I2C1Handle,
                                         tx_data,
                                         2U,
                                         SSD1306_I2C_ADDR,
                                         I2C_ADDR_MODE_7BIT,
                                         I2C_DISABLE_SR,
                                         100U);
}

static uint8_t SSD1306_WriteDataChunk(uint8_t *pData, uint32_t Len)
{
    /*
     * SSD1306 data packet:
     * first byte 0x40 means following bytes are display data.
     *
     * We send max 17 bytes here:
     * 1 control byte + 16 data bytes.
     */
    uint8_t tx_data[17];

    if ((pData == 0) || (Len == 0U) || (Len > 16U))
    {
        return I2C_STATUS_ERROR;
    }

    tx_data[0] = 0x40U;

    for (uint32_t i = 0U; i < Len; i++)
    {
        tx_data[i + 1U] = pData[i];
    }

    return I2C_MasterTransmitWithTimeout(&I2C1Handle,
                                         tx_data,
                                         Len + 1U,
                                         SSD1306_I2C_ADDR,
                                         I2C_ADDR_MODE_7BIT,
                                         I2C_DISABLE_SR,
                                         100U);
}

static uint8_t SSD1306_Init(void)
{
    uint8_t status;

    delay_ms(100U);

    status = SSD1306_WriteCommand(0xAEU);    /* Display OFF */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0x20U);    /* Memory addressing mode */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0x00U);    /* Horizontal addressing mode */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0xB0U);    /* Page start address */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0xC8U);    /* COM output scan direction remapped */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0x00U);    /* Low column address */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0x10U);    /* High column address */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0x40U);    /* Start line address */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0x81U);    /* Contrast control */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0x7FU);
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0xA1U);    /* Segment remap */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0xA6U);    /* Normal display */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0xA8U);    /* Multiplex ratio */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0x3FU);    /* 1/64 duty */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0xA4U);    /* Display follows RAM */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0xD3U);    /* Display offset */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0x00U);
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0xD5U);    /* Display clock divide */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0x80U);
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0xD9U);    /* Pre-charge period */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0xF1U);
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0xDAU);    /* COM pins hardware config */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0x12U);
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0xDBU);    /* VCOMH deselect level */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0x40U);
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0x8DU);    /* Charge pump */
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0x14U);
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand(0xAFU);    /* Display ON */
    if (status != I2C_STATUS_OK) return status;

    return I2C_STATUS_OK;
}

static uint8_t SSD1306_SetCursor(uint8_t page, uint8_t column)
{
    uint8_t status;

    if ((page >= SSD1306_PAGES) || (column >= SSD1306_WIDTH))
    {
        return I2C_STATUS_ERROR;
    }

    status = SSD1306_WriteCommand((uint8_t)(0xB0U + page));
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand((uint8_t)(0x00U + (column & 0x0FU)));
    if (status != I2C_STATUS_OK) return status;

    status = SSD1306_WriteCommand((uint8_t)(0x10U + ((column >> 4U) & 0x0FU)));
    if (status != I2C_STATUS_OK) return status;

    return I2C_STATUS_OK;
}

static uint8_t SSD1306_Fill(uint8_t pattern)
{
    uint8_t status;
    uint8_t chunk[16];

    for (uint32_t i = 0U; i < 16U; i++)
    {
        chunk[i] = pattern;
    }

    for (uint8_t page = 0U; page < SSD1306_PAGES; page++)
    {
        status = SSD1306_SetCursor(page, 0U);
        if (status != I2C_STATUS_OK) return status;

        for (uint8_t block = 0U; block < 8U; block++)
        {
            status = SSD1306_WriteDataChunk(chunk, 16U);
            if (status != I2C_STATUS_OK) return status;
        }
    }

    return I2C_STATUS_OK;
}

void EX018_I2C1_SSD1306_Command_Test(void)
{
    SysTick_Init(16000000U);

    I2C1_GPIOInits();

    if (I2C1_Inits() != I2C_STATUS_OK)
    {
        while (1)
        {
            /*
             * I2C init failed.
             */
        }
    }

    g_oled_init_status = SSD1306_Init();

    if (g_oled_init_status != I2C_STATUS_OK)
    {
        while (1)
        {
            /*
             * OLED init failed.
             */
        }
    }

    /*
     * Clear screen.
     */
    g_oled_clear_status = SSD1306_Fill(0x00U);

    delay_ms(500U);

    /*
     * Fill screen white.
     */
    g_oled_fill_status = SSD1306_Fill(0xFFU);

    delay_ms(500U);

    /*
     * Clear again.
     */
    g_oled_clear_status = SSD1306_Fill(0x00U);

    if ((g_oled_init_status == I2C_STATUS_OK) &&
        (g_oled_clear_status == I2C_STATUS_OK) &&
        (g_oled_fill_status == I2C_STATUS_OK))
    {
        g_oled_test_pass = 1U;
    }

    while (1)
    {
        /*
         * Expected:
         *
         * g_oled_init_status  = 0
         * g_oled_clear_status = 0
         * g_oled_fill_status  = 0
         * g_oled_test_pass    = 1
         *
         * OLED behavior:
         * screen clears
         * screen turns full white
         * screen clears again
         */
    }
}

#endif /* ACTIVE_EXAMPLE == EXAMPLE_I2C1_SSD1306_COMMAND_TEST */
