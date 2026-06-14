/*
 * ex017_i2c1_scanner_test.c
 *
 *  Created on: Jun 14, 2026
 *      Author: furkan
 */

#include "stm32f767xx_gpio_driver.h"
#include "stm32f767xx_i2c_driver.h"
#include "stm32f767xx_systick_driver.h"
#include "examples_config.h"

#if ACTIVE_EXAMPLE == EXAMPLE_I2C1_SCANNER_TEST

static I2C_Handle_t I2C1Handle;

static volatile uint8_t g_i2c_scan_done = 0U;
static volatile uint8_t g_i2c_found_count = 0U;
static volatile uint8_t g_i2c_found_addresses[16];
static volatile uint8_t g_i2c_last_addr = 0U;
static volatile uint8_t g_i2c_last_status = 0U;
static volatile uint8_t g_i2c_status_3c = 0U;
static volatile uint8_t g_i2c_status_3d = 0U;
static volatile uint32_t g_gpiob_idr_before_scan = 0U;
static volatile uint32_t g_i2c_isr_after_3c = 0U;
static volatile uint32_t g_i2c_isr_after_3d = 0U;
static void I2C1_GPIOInits(void)
{
    GPIO_Handle_t GpioI2C;

    /*
     * I2C1 pins:
     * PB8 -> I2C1_SCL
     * PB9 -> I2C1_SDA
     * AF4
     *
     * I2C pins must be open-drain.
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

void EX017_I2C1_Scanner_Test(void)
{
    uint8_t status;

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

	g_gpiob_idr_before_scan = GPIOB->GPIO_IDR;

	g_i2c_status_3c = I2C_IsDeviceReadyWithTimeout(&I2C1Handle, 0x3CU,
	I2C_ADDR_MODE_7BIT, 100U);

	g_i2c_isr_after_3c = I2C1->I2C_ISR;

	delay_ms(5U);

	g_i2c_status_3d = I2C_IsDeviceReadyWithTimeout(&I2C1Handle, 0x3DU,
	I2C_ADDR_MODE_7BIT, 100U);

	g_i2c_isr_after_3d = I2C1->I2C_ISR;

	delay_ms(5U);
    /*
     * Standard usable 7-bit I2C address range:
     * 0x08 - 0x77
     */
    for (uint8_t addr = 0x08U; addr <= 0x77U; addr++)
    {
        g_i2c_last_addr = addr;

        status = I2C_IsDeviceReadyWithTimeout(&I2C1Handle,
                                              addr,
                                              I2C_ADDR_MODE_7BIT,
                                              20U);

        g_i2c_last_status = status;

        if (status == I2C_STATUS_OK)
        {
            if (g_i2c_found_count < 16U)
            {
                g_i2c_found_addresses[g_i2c_found_count] = addr;
                g_i2c_found_count++;
            }
        }

        delay_ms(2U);
    }

    g_i2c_scan_done = 1U;

    while (1)
    {
        /*
         * Debug expected:
         *
         * g_i2c_scan_done = 1
         *
         * If device exists:
         * g_i2c_found_count > 0
         *
         * Example addresses:
         * MPU6050 / MPU9250  -> 0x68 or 0x69
         * BMP280 / BME280    -> 0x76 or 0x77
         * 24Cxx EEPROM       -> 0x50
         *
         * If debugger shows decimal:
         * 104 decimal = 0x68
         * 118 decimal = 0x76
         * 119 decimal = 0x77
         */
    }
}

#endif /* ACTIVE_EXAMPLE == EXAMPLE_I2C1_SCANNER_TEST */
