/*
 * ex015_i2c1_init_test.c
 *
 *  Created on: Jun 12, 2026
 *      Author: furkan
 */

#include "stm32f767xx_gpio_driver.h"
#include "stm32f767xx_i2c_driver.h"
#include "stm32f767xx_systick_driver.h"
#include "examples_config.h"

#if ACTIVE_EXAMPLE == EXAMPLE_I2C1_INIT_TEST

static I2C_Handle_t I2C1Handle;

static volatile uint8_t g_i2c_init_status = 0U;
static volatile uint8_t g_i2c_init_pass = 0U;

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
     * External pull-up resistors are recommended for real bus communication.
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

    /*
     * First version:
     * I2C kernel clock = 16 MHz
     * Standard mode = 100 kHz
     */
    I2C1Handle.I2C_Config.I2C_Timing = I2C_TIMING_16MHZ_100KHZ;
    I2C1Handle.I2C_Config.I2C_AnalogFilter = I2C_ANALOG_FILTER_ENABLE;
    I2C1Handle.I2C_Config.I2C_DigitalFilter = 0U;
    I2C1Handle.I2C_Config.I2C_ClockStretch = I2C_CLOCK_STRETCH_ENABLE;

    return I2C_Init(&I2C1Handle);
}

void EX015_I2C1_Init_Test(void)
{
    SysTick_Init(16000000U);

    I2C1_GPIOInits();

    g_i2c_init_status = I2C1_Inits();

    if (g_i2c_init_status == I2C_STATUS_OK)
    {
        if ((I2C1->I2C_CR1 & (1U << I2C_CR1_PE)) &&
            (I2C1->I2C_TIMINGR == I2C_TIMING_16MHZ_100KHZ))
        {
            g_i2c_init_pass = 1U;
        }
    }

    while (1)
    {
        /*
         * Expected:
         *
         * g_i2c_init_status = I2C_STATUS_OK
         * g_i2c_init_pass   = 1
         *
         * SFR checks:
         * RCC_APB1ENR bit 21 = 1
         * GPIOB_MODER PB8/PB9 = AF mode
         * GPIOB_OTYPER PB8/PB9 = open-drain
         * GPIOB_AFRH PB8/PB9 = AF4
         * I2C1_TIMINGR = 0x30420F13
         * I2C1_CR1 PE = 1
         */
    }
}

#endif /* ACTIVE_EXAMPLE == EXAMPLE_I2C1_INIT_TEST */
