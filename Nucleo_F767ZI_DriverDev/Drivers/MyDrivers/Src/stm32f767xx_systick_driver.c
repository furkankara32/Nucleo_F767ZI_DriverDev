/*
 * stm32f767xx_systick_driver.c
 *
 *  Created on: Jun 5, 2026
 *      Author: furkan
 */

#include "stm32f767xx_systick_driver.h"

static volatile uint32_t g_systick_ms = 0;

void SysTick_Init(uint32_t system_clock_hz)
{
	uint32_t reload_value;

	/*
	 * SysTick generates interrupt every 1 ms.
	 *
	 * reload_value = system_clock_hz / 1000
	 *
	 * Example:
	 * system_clock_hz = 16 MHz
	 * reload_value = 16000
	 * SYST_RVR = 16000 - 1
	 */
	reload_value = system_clock_hz / 1000U;

	/*
	 * SysTick is a 24-bit timer.
	 * Maximum reload value is 0x00FFFFFF.
	 */
	if ((reload_value == 0U) || (reload_value > 0x00FFFFFFU))
	{
		return;
	}

	/*
	 * 1. Disable SysTick during configuration
	 */
	SYSTICK->SYST_CSR = 0U;

	/*
	 * 2. Set reload value for 1 ms tick
	 */
	SYSTICK->SYST_RVR = reload_value - 1U;

	/*
	 * 3. Clear current value register
	 */
	SYSTICK->SYST_CVR = 0U;

	/*
	 * 4. Select processor clock, enable SysTick interrupt, enable SysTick
	 */
	SYSTICK->SYST_CSR |= (1U << SYSTICK_CSR_CLKSOURCE);
	SYSTICK->SYST_CSR |= (1U << SYSTICK_CSR_TICKINT);
	SYSTICK->SYST_CSR |= (1U << SYSTICK_CSR_ENABLE);
}

void SysTick_IncTick(void)
{
	g_systick_ms++;
}

uint32_t millis(void)
{
	return g_systick_ms;
}

void delay_ms(uint32_t delay)
{
	uint32_t start_tick = millis();

	while ((millis() - start_tick) < delay)
	{
		/*
		 * Wait until delay time expires
		 */
	}
}
