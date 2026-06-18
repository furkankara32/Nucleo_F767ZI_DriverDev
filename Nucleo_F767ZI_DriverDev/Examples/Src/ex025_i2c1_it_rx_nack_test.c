/*
 * ex025_i2c1_it_rx_nack_test.c
 *
 *  Created on: Jun 17, 2026
 *      Author: furkan
 */

/*
 * ex025_i2c1_it_rx_nack_test.c
 *
 * I2C1 interrupt based RX NACK test
 *
 * Pins:
 * PB8 -> I2C1_SCL
 * PB9 -> I2C1_SDA
 * AF4
 *
 * This test tries to read 1 byte from address 0x55.
 * Expected case: no device at 0x55, so NACK interrupt occurs.
 */

#include "stm32f767xx_gpio_driver.h"
#include "stm32f767xx_i2c_driver.h"
#include "stm32f767xx_nvic_driver.h"
#include "stm32f767xx_systick_driver.h"
#include "examples_config.h"

#if ACTIVE_EXAMPLE == EXAMPLE_I2C1_IT_RX_NACK_TEST

static I2C_Handle_t I2C1Handle;

static uint8_t g_i2c1_rx_buffer[1];

static volatile uint8_t  g_i2c1_transfer_done = 0U;
static volatile uint8_t  g_i2c1_transfer_status = 0U;
static volatile uint8_t  g_i2c1_callback_event = 0xFFU;

static volatile uint32_t g_i2c1_nack_count = 0U;
static volatile uint32_t g_i2c1_rx_complete_count = 0U;
static volatile uint32_t g_i2c1_stop_count = 0U;

static volatile uint32_t g_i2c1_error_count = 0U;
static volatile uint32_t g_i2c1_berr_count = 0U;
static volatile uint32_t g_i2c1_arlo_count = 0U;
static volatile uint32_t g_i2c1_ovr_count = 0U;
static volatile uint32_t g_i2c1_timeout_count = 0U;

static volatile uint8_t  g_i2c1_final_error_code = 0U;
static volatile uint32_t g_i2c1_final_isr = 0U;
static volatile uint32_t g_i2c1_final_cr1 = 0U;
static volatile uint32_t g_i2c1_final_cr2 = 0U;

static void I2C1_GPIOInits(void)
{
    GPIO_Handle_t GpioI2C;

    /*
     * I2C1:
     * PB8 -> SCL
     * PB9 -> SDA
     * AF4
     */
    GpioI2C.pGPIOx = GPIOB;

    GpioI2C.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    GpioI2C.GPIO_PinConfig.GPIO_PinAltFunMode = 4U;
    GpioI2C.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
    GpioI2C.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    GpioI2C.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    GpioI2C.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
    GPIO_Init(&GpioI2C);

    GpioI2C.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
    GPIO_Init(&GpioI2C);
}

static void I2C1_Inits(void)
{
    I2C1Handle.pI2Cx = I2C1;

    I2C1Handle.I2C_Config.I2C_Timing = I2C_TIMING_16MHZ_100KHZ;
    I2C1Handle.I2C_Config.I2C_AnalogFilter = I2C_ANALOG_FILTER_ENABLE;
    I2C1Handle.I2C_Config.I2C_DigitalFilter = 0U;
    I2C1Handle.I2C_Config.I2C_ClockStretch = I2C_CLOCK_STRETCH_ENABLE;

    I2C_Init(&I2C1Handle);
}

void EX025_I2C1_IT_RxNACK_Test(void)
{
    uint8_t status;

    SysTick_Init(16000000U);

    I2C1_GPIOInits();
    I2C1_Inits();

    g_i2c1_rx_buffer[0] = 0x00U;

    I2C_ClearNACKFlag(I2C1);
    I2C_ClearSTOPFlag(I2C1);
    I2C_ClearErrorFlags(I2C1);

    NVIC_IRQPriorityConfig(IRQ_NO_I2C1_EV, 15U);
    NVIC_IRQPriorityConfig(IRQ_NO_I2C1_ER, 15U);

    g_i2c1_transfer_done = 0U;
    g_i2c1_callback_event = 0xFFU;
    g_i2c1_final_error_code = 0U;

    /*
     * Try to read 1 byte from an address where no device is expected.
     * Expected async result: I2C_EVENT_NACK.
     */
    status = I2C_MasterReceiveIT(&I2C1Handle,
                                 g_i2c1_rx_buffer,
                                 sizeof(g_i2c1_rx_buffer),
                                 0x55U,
                                 I2C_ADDR_MODE_7BIT,
                                 I2C_DISABLE_SR);

    g_i2c1_transfer_status = status;

    if (status != I2C_STATUS_OK)
    {
        g_i2c1_transfer_done = 1U;
    }
    else
    {
        NVIC_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);
        NVIC_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
    }

    while (1)
    {
        if (g_i2c1_transfer_done == 1U)
        {
            g_i2c1_final_error_code = I2C1Handle.ErrorCode;
            g_i2c1_final_isr = I2C1->I2C_ISR;
            g_i2c1_final_cr1 = I2C1->I2C_CR1;
            g_i2c1_final_cr2 = I2C1->I2C_CR2;

            /*
             * Stop here for debugger inspection.
             *
             * Expected no-device result:
             * g_i2c1_transfer_status = I2C_STATUS_OK
             * g_i2c1_nack_count = 1
             * g_i2c1_rx_complete_count = 0
             * g_i2c1_error_count = 0
             * I2C1Handle.TxRxState = I2C_READY
             * I2C1Handle.ErrorCode has I2C_ERROR_NACK bit set
             */
            while (1)
            {
            }
        }
    }
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,
                                  uint8_t Event)
{
    if (pI2CHandle->pI2Cx == I2C1)
    {
        g_i2c1_callback_event = Event;

        if (Event == I2C_EVENT_NACK)
        {
            g_i2c1_nack_count++;
            g_i2c1_transfer_done = 1U;
        }
        else if (Event == I2C_EVENT_RX_CMPLT)
        {
            /*
             * This means some device ACKed address 0x55
             * and returned 1 byte.
             */
            g_i2c1_rx_complete_count++;
            g_i2c1_transfer_done = 1U;
        }
        else if (Event == I2C_EVENT_STOP)
        {
            g_i2c1_stop_count++;
            g_i2c1_transfer_done = 1U;
        }
        else if (Event == I2C_EVENT_BERR)
        {
            g_i2c1_berr_count++;
            g_i2c1_error_count++;
            g_i2c1_transfer_done = 1U;
        }
        else if (Event == I2C_EVENT_ARLO)
        {
            g_i2c1_arlo_count++;
            g_i2c1_error_count++;
            g_i2c1_transfer_done = 1U;
        }
        else if (Event == I2C_EVENT_OVR)
        {
            g_i2c1_ovr_count++;
            g_i2c1_error_count++;
            g_i2c1_transfer_done = 1U;
        }
        else if (Event == I2C_EVENT_TIMEOUT)
        {
            g_i2c1_timeout_count++;
            g_i2c1_error_count++;
            g_i2c1_transfer_done = 1U;
        }
    }
}

#endif /* ACTIVE_EXAMPLE == EXAMPLE_I2C1_IT_RX_NACK_TEST */
