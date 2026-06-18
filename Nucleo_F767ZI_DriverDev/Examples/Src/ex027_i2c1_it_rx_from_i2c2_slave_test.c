/*
 * ex027_i2c1_it_rx_from_i2c2_slave_test.c
 *
 *  Created on: Jun 18, 2026
 *      Author: furkan
 */
/*
 * ex027_i2c1_it_rx_from_i2c2_slave_test.c
 *
 * I2C1 interrupt master RX <- I2C2 polling slave TX test
 *
 * Wiring:
 * PB8  I2C1_SCL  ---->  PB10 I2C2_SCL
 * PB9  I2C1_SDA  ---->  PB11 I2C2_SDA
 *
 * Internal pull-up is used for this test.
 */

#include "stm32f767xx_gpio_driver.h"
#include "stm32f767xx_i2c_driver.h"
#include "stm32f767xx_nvic_driver.h"
#include "stm32f767xx_systick_driver.h"
#include "examples_config.h"

#if ACTIVE_EXAMPLE == EXAMPLE_I2C1_IT_RX_FROM_I2C2_SLAVE_TEST

#define I2C2_SLAVE_ADDR              0x32U
#define I2C_OAR1_OA1EN_LOCAL         15U

static I2C_Handle_t I2C1Handle;
static I2C_Handle_t I2C2Handle;

static uint8_t g_i2c2_tx_buffer[] =
{
    0xA1U, 0xB2U, 0xC3U, 0xD4U, 0xE5U
};

static uint8_t g_i2c1_rx_buffer[sizeof(g_i2c2_tx_buffer)];

static volatile uint8_t  g_i2c1_transfer_status = 0U;
static volatile uint8_t  g_i2c1_transfer_done = 0U;
static volatile uint8_t  g_i2c1_callback_event = 0xFFU;

static volatile uint32_t g_i2c1_rx_complete_count = 0U;
static volatile uint32_t g_i2c1_nack_count = 0U;
static volatile uint32_t g_i2c1_error_count = 0U;
static volatile uint32_t g_i2c1_berr_count = 0U;
static volatile uint32_t g_i2c1_arlo_count = 0U;
static volatile uint32_t g_i2c1_ovr_count = 0U;
static volatile uint32_t g_i2c1_timeout_count = 0U;

static volatile uint8_t  g_i2c2_slave_done = 0U;
static volatile uint8_t  g_i2c1_compare_ok = 0U;
static volatile uint32_t g_i2c2_addr_count = 0U;
static volatile uint32_t g_i2c2_txis_count = 0U;
static volatile uint32_t g_i2c2_nack_count = 0U;
static volatile uint32_t g_i2c2_stop_count = 0U;
static volatile uint32_t g_i2c2_error_count = 0U;
static volatile uint32_t g_i2c2_tx_index = 0U;

static volatile uint8_t  g_test_done = 0U;
static volatile uint8_t  g_test_timeout = 0U;

static volatile uint8_t  g_i2c1_final_error_code = 0U;
static volatile uint32_t g_i2c1_final_isr = 0U;
static volatile uint32_t g_i2c1_final_cr1 = 0U;
static volatile uint32_t g_i2c1_final_cr2 = 0U;

static volatile uint32_t g_i2c2_final_isr = 0U;
static volatile uint32_t g_i2c2_final_cr1 = 0U;
static volatile uint32_t g_i2c2_final_cr2 = 0U;

static volatile uint32_t g_i2c2_txis_data_count = 0U;
static volatile uint32_t g_i2c2_txis_extra_count = 0U;
static void I2C_GPIOInits(void)
{
    GPIO_Handle_t GpioI2C;

    /*
     * I2C1:
     * PB8 -> SCL
     * PB9 -> SDA
     *
     * I2C2:
     * PB10 -> SCL
     * PB11 -> SDA
     *
     * AF4, open-drain, internal pull-up.
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

    GpioI2C.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
    GPIO_Init(&GpioI2C);

    GpioI2C.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_11;
    GPIO_Init(&GpioI2C);
}

static void I2C1_MasterInits(void)
{
    I2C1Handle.pI2Cx = I2C1;

    I2C1Handle.I2C_Config.I2C_Timing = I2C_TIMING_16MHZ_100KHZ;
    I2C1Handle.I2C_Config.I2C_AnalogFilter = I2C_ANALOG_FILTER_ENABLE;
    I2C1Handle.I2C_Config.I2C_DigitalFilter = 0U;
    I2C1Handle.I2C_Config.I2C_ClockStretch = I2C_CLOCK_STRETCH_ENABLE;

    I2C_Init(&I2C1Handle);
}

static void I2C2_SlaveInits(void)
{
    I2C2Handle.pI2Cx = I2C2;

    I2C2Handle.I2C_Config.I2C_Timing = I2C_TIMING_16MHZ_100KHZ;
    I2C2Handle.I2C_Config.I2C_AnalogFilter = I2C_ANALOG_FILTER_ENABLE;
    I2C2Handle.I2C_Config.I2C_DigitalFilter = 0U;
    I2C2Handle.I2C_Config.I2C_ClockStretch = I2C_CLOCK_STRETCH_ENABLE;

    I2C_Init(&I2C2Handle);

    /*
     * Configure I2C2 own address 1 as 7-bit slave address.
     *
     * OAR1[7:1] = own address
     * OAR1[15]  = OA1EN
     */
    I2C_PeripheralControl(I2C2, DISABLE);

    I2C2->I2C_OAR1 = (((uint32_t)I2C2_SLAVE_ADDR & 0x7FU) << 1U) |
                     (1U << I2C_OAR1_OA1EN_LOCAL);

    I2C_ClearNACKFlag(I2C2);
    I2C_ClearSTOPFlag(I2C2);
    I2C_ClearErrorFlags(I2C2);

    I2C_PeripheralControl(I2C2, ENABLE);
}

static uint8_t I2C_CompareBuffers(uint8_t *pExpected,
                                  uint8_t *pActual,
                                  uint32_t Len)
{
    for (uint32_t i = 0U; i < Len; i++)
    {
        if (pExpected[i] != pActual[i])
        {
            return 0U;
        }
    }

    return 1U;
}

static void I2C2_SlavePollTx(void)
{
    uint32_t isr;

    isr = I2C2->I2C_ISR;

    /*
     * Address matched.
     * Clear ADDR so slave transfer can continue.
     */
    if (isr & I2C_FLAG_ADDR)
    {
        I2C2->I2C_ICR = (1U << I2C_ICR_ADDRCF);
        g_i2c2_addr_count++;
    }

    /*
     * Master is reading. Slave TXDR is requested.
     */
    if (isr & I2C_FLAG_TXIS)
    {
        if (g_i2c2_tx_index < sizeof(g_i2c2_tx_buffer))
        {
            I2C2->I2C_TXDR = g_i2c2_tx_buffer[g_i2c2_tx_index];
            g_i2c2_tx_index++;

            g_i2c2_txis_count++;
            g_i2c2_txis_data_count++;
        }
        else
        {
            /*
             * Extra TXIS can occur around the end of slave-read transfer.
             * Write filler to release the peripheral, but count separately.
             */
            I2C2->I2C_TXDR = 0xFFU;

            g_i2c2_txis_count++;
            g_i2c2_txis_extra_count++;
        }
    }

    /*
     * Master normally NACKs after the last byte in read transfer.
     * Clear NACKF on slave side.
     */
    if (isr & I2C_FLAG_NACKF)
    {
        I2C_ClearNACKFlag(I2C2);
        g_i2c2_nack_count++;
    }

    /*
     * STOP detected by slave.
     */
    if (isr & I2C_FLAG_STOPF)
    {
        I2C_ClearSTOPFlag(I2C2);
        g_i2c2_stop_count++;
        g_i2c2_slave_done = 1U;
    }

    /*
     * Slave-side error flags.
     */
    if (isr & (I2C_FLAG_BERR |
               I2C_FLAG_ARLO |
               I2C_FLAG_OVR |
               I2C_FLAG_TIMEOUT))
    {
        I2C_ClearErrorFlags(I2C2);
        g_i2c2_error_count++;
        g_i2c2_slave_done = 1U;
    }
}

void EX027_I2C1_IT_Rx_From_I2C2_Slave_Test(void)
{
    uint8_t status;
    uint32_t start_tick;

    SysTick_Init(16000000U);

    I2C_GPIOInits();

    I2C1_MasterInits();
    I2C2_SlaveInits();

    for (uint32_t i = 0U; i < sizeof(g_i2c1_rx_buffer); i++)
    {
        g_i2c1_rx_buffer[i] = 0U;
    }

    I2C_ClearNACKFlag(I2C1);
    I2C_ClearSTOPFlag(I2C1);
    I2C_ClearErrorFlags(I2C1);

    I2C_ClearNACKFlag(I2C2);
    I2C_ClearSTOPFlag(I2C2);
    I2C_ClearErrorFlags(I2C2);

    NVIC_IRQPriorityConfig(IRQ_NO_I2C1_EV, 15U);
    NVIC_IRQPriorityConfig(IRQ_NO_I2C1_ER, 15U);

    g_i2c1_transfer_done = 0U;
    g_i2c1_callback_event = 0xFFU;

    g_i2c2_slave_done = 0U;
    g_i2c2_tx_index = 0U;
    g_i2c1_compare_ok = 0U;

    g_test_done = 0U;
    g_test_timeout = 0U;

    /*
     * I2C1 master reads bytes from I2C2 slave address.
     */
    status = I2C_MasterReceiveIT(&I2C1Handle,
                                 g_i2c1_rx_buffer,
                                 sizeof(g_i2c1_rx_buffer),
                                 I2C2_SLAVE_ADDR,
                                 I2C_ADDR_MODE_7BIT,
                                 I2C_DISABLE_SR);

    g_i2c1_transfer_status = status;

    if (status != I2C_STATUS_OK)
    {
        g_test_done = 1U;
    }
    else
    {
        /*
         * Enable only I2C1 master IRQs.
         * I2C2 slave is polled in main loop.
         */
        NVIC_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);
        NVIC_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
    }

    start_tick = millis();

    while (1)
    {
        /*
         * Poll I2C2 slave TX side fast.
         */
        I2C2_SlavePollTx();

        if ((g_i2c1_transfer_done == 1U) &&
            (g_i2c2_slave_done == 1U))
        {
            g_i2c1_compare_ok = I2C_CompareBuffers(g_i2c2_tx_buffer,
                                                   g_i2c1_rx_buffer,
                                                   sizeof(g_i2c2_tx_buffer));

            g_i2c1_final_error_code = I2C1Handle.ErrorCode;
            g_i2c1_final_isr = I2C1->I2C_ISR;
            g_i2c1_final_cr1 = I2C1->I2C_CR1;
            g_i2c1_final_cr2 = I2C1->I2C_CR2;

            g_i2c2_final_isr = I2C2->I2C_ISR;
            g_i2c2_final_cr1 = I2C2->I2C_CR1;
            g_i2c2_final_cr2 = I2C2->I2C_CR2;

            g_test_done = 1U;
        }

        if ((millis() - start_tick) >= 1000U)
        {
            g_test_timeout = 1U;

            g_i2c1_final_error_code = I2C1Handle.ErrorCode;
            g_i2c1_final_isr = I2C1->I2C_ISR;
            g_i2c1_final_cr1 = I2C1->I2C_CR1;
            g_i2c1_final_cr2 = I2C1->I2C_CR2;

            g_i2c2_final_isr = I2C2->I2C_ISR;
            g_i2c2_final_cr1 = I2C2->I2C_CR1;
            g_i2c2_final_cr2 = I2C2->I2C_CR2;

            g_test_done = 1U;
        }

        if (g_test_done == 1U)
        {
            /*
             * Stop here for debugger inspection.
             *
             * Expected:
             * g_test_timeout = 0
             *
             * g_i2c1_transfer_status = I2C_STATUS_OK
             * g_i2c1_rx_complete_count = 1
             * g_i2c1_nack_count = 0
             * g_i2c1_error_count = 0
             *
             * g_i2c2_addr_count = 1
             * g_i2c2_txis_count = sizeof(g_i2c2_tx_buffer)
             * g_i2c2_stop_count = 1
             * g_i2c1_compare_ok = 1
             *
             * g_i2c1_rx_buffer = 0xA1, 0xB2, 0xC3, 0xD4, 0xE5
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

        if (Event == I2C_EVENT_RX_CMPLT)
        {
            g_i2c1_rx_complete_count++;
            g_i2c1_transfer_done = 1U;
        }
        else if (Event == I2C_EVENT_NACK)
        {
            g_i2c1_nack_count++;
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

#endif /* ACTIVE_EXAMPLE == EXAMPLE_I2C1_IT_RX_FROM_I2C2_SLAVE_TEST */

