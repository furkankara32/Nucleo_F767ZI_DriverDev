/*
 * stm32f767xx_rcc_driver.c
 *
 *  Created on: Jun 6, 2026
 *      Author: furkan
 */


#include "stm32f767xx_rcc_driver.h"

/*
 * AHB prescaler table
 * HPRE encoding:
 * 0xxx: SYSCLK not divided
 * 1000: /2
 * 1001: /4
 * 1010: /8
 * 1011: /16
 * 1100: /64
 * 1101: /128
 * 1110: /256
 * 1111: /512
 */
static const uint16_t AHB_PreScaler[16] =
{
	1U, 1U, 1U, 1U,
	1U, 1U, 1U, 1U,
	2U, 4U, 8U, 16U,
	64U, 128U, 256U, 512U
};

/*
 * APB prescaler table
 * PPRE encoding:
 * 0xx: HCLK not divided
 * 100: /2
 * 101: /4
 * 110: /8
 * 111: /16
 */
static const uint8_t APB_PreScaler[8] =
{
	1U, 1U, 1U, 1U,
	2U, 4U, 8U, 16U
};

static uint32_t RCC_GetPLLClockValue(void)
{
	uint32_t pll_source;
	uint32_t pll_input_clock;
	uint32_t pllm;
	uint32_t plln;
	uint32_t pllp_bits;
	uint32_t pllp;
	uint32_t pll_vco;
	uint32_t pll_clk;

	/*
	 * PLL source:
	 * 0: HSI
	 * 1: HSE
	 */
	pll_source = (RCC->RCC_PLLCFGR >> RCC_PLLCFGR_PLLSRC) & 0x1U;

	if (pll_source == 0U)
	{
		pll_input_clock = HSI_VALUE;
	}
	else
	{
		pll_input_clock = HSE_VALUE;
	}

	/*
	 * PLLM = bits [5:0]
	 * PLLN = bits [14:6]
	 * PLLP = bits [17:16], encoded as:
	 * 00: /2
	 * 01: /4
	 * 10: /6
	 * 11: /8
	 */
	pllm = (RCC->RCC_PLLCFGR >> RCC_PLLCFGR_PLLM) & 0x3FU;
	plln = (RCC->RCC_PLLCFGR >> RCC_PLLCFGR_PLLN) & 0x1FFU;
	pllp_bits = (RCC->RCC_PLLCFGR >> RCC_PLLCFGR_PLLP) & 0x3U;

	pllp = 2U * (pllp_bits + 1U);

	if (pllm == 0U)
	{
		return 0U;
	}

	pll_vco = (pll_input_clock / pllm) * plln;
	pll_clk = pll_vco / pllp;

	return pll_clk;
}

uint32_t RCC_GetSYSCLKValue(void)
{
	uint32_t clock_source;
	uint32_t sysclk;

	/*
	 * SWS bits [3:2]
	 * 00: HSI
	 * 01: HSE
	 * 10: PLL
	 */
	clock_source = (RCC->RCC_CFGR >> RCC_CFGR_SWS) & 0x3U;

	if (clock_source == 0U)
	{
		sysclk = HSI_VALUE;
	}
	else if (clock_source == 1U)
	{
		sysclk = HSE_VALUE;
	}
	else if (clock_source == 2U)
	{
		sysclk = RCC_GetPLLClockValue();
	}
	else
	{
		sysclk = HSI_VALUE;
	}

	return sysclk;
}

uint32_t RCC_GetHCLKValue(void)
{
	uint32_t sysclk;
	uint32_t hpre;
	uint32_t hclk;

	sysclk = RCC_GetSYSCLKValue();

	hpre = (RCC->RCC_CFGR >> RCC_CFGR_HPRE) & 0xFU;

	hclk = sysclk / AHB_PreScaler[hpre];

	return hclk;
}

uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t hclk;
	uint32_t ppre1;
	uint32_t pclk1;

	hclk = RCC_GetHCLKValue();

	ppre1 = (RCC->RCC_CFGR >> RCC_CFGR_PPRE1) & 0x7U;

	pclk1 = hclk / APB_PreScaler[ppre1];

	return pclk1;
}

uint32_t RCC_GetPCLK2Value(void)
{
	uint32_t hclk;
	uint32_t ppre2;
	uint32_t pclk2;

	hclk = RCC_GetHCLKValue();

	ppre2 = (RCC->RCC_CFGR >> RCC_CFGR_PPRE2) & 0x7U;

	pclk2 = hclk / APB_PreScaler[ppre2];

	return pclk2;
}
