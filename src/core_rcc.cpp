#include"core/rcc.h"
#include"common/defs.h"
#include<stddef.h>
#include<algorithm>

using namespace mcu;

extern  uint32_t _edata, _sdata, _la_data, _sbss, _ebss;

extern int main(void);

namespace mcu {

	struct rcc_registers {
		peripheral_register CR;
		peripheral_register CFGR;
		peripheral_register CIR;
		peripheral_register APB2RSTR;
		peripheral_register APB1RSTR;
		peripheral_register AHBENR;
		peripheral_register APB2ENR;
		peripheral_register APB1ENR;
		peripheral_register BDCR;
		peripheral_register CSR;
	};
}

static_assert(
	offsetof(rcc_registers, CSR) == 0x024,
	"rcc_registers is not standard layout"
);

typedef void(*function_type)();
// Linker-defined begin and end of the ctors.
extern function_type __init_array_start[];
extern function_type __init_array_end[];

void init_ctors()
{
	std::for_each(__init_array_start, __init_array_end,
		[](const function_type pf)
		{
			pf();
		});
}


void sys_clock::enable_peripheral(Bridge bridge, uint8_t bit) {
	switch (bridge) {
	case Bridge::AHB:
		rcc->AHBENR |= (1u << bit);
		break;
	case Bridge::APB1:
		rcc->APB1ENR |= (1u << bit);
		break;
	case Bridge::APB2:
		rcc->APB2ENR |= (1u << bit);
		break;
	default:
		break;
	}
}

void sys_clock::disable_peripheral(Bridge bridge, uint8_t bit) {
	switch (bridge) {
	case Bridge::AHB:
		rcc->AHBENR &= ~(1u << bit);
		break;
	case Bridge::APB1:
		rcc->APB1ENR &= ~(1u << bit);
		break;
	case Bridge::APB2:
		rcc->APB2ENR &= ~(1u << bit);
		break;
	default:
		break;
	}
}

void sys_clock::reset_peripheral(Bridge bridge, uint8_t bit) {
	switch (bridge) {
	case Bridge::APB1:
		rcc->APB1RSTR |= (1u << bit);
		break;
	case Bridge::APB2:
		rcc->APB2RSTR |= (1u << bit);
		break;
	default:
		break;
	}
}

void sys_clock::configure(SYS_clk sysclk, RTC_clk rtcclk, uint16_t pllmul_flag) {
	sysclk_setup(sysclk);
	rtcclk_setup(rtcclk);
	pllmul_setup(pllmul_flag);
}

void sys_clock::rtcclk_setup(RTC_clk rtcclk) {
	//RTCSEL = 1:0
	rcc->BDCR |= (1u << 15);
	switch (rtcclk) {
	case RTC_clk::LSI:
		rcc->BDCR |= (1u << 9);
		break;
	case RTC_clk::LSE:
		rcc->BDCR |= (1u << 0);
		rcc->BDCR |= (1u << 8);
		while (0 == (rcc->BDCR & (1u << 1))); //lse ready
		break;
	case RTC_clk::HSE_div_128:
		rcc->BDCR |= (1u << 8);
		rcc->BDCR |= (1u << 9);
		break;
	default:
		break;
	}

}

void sys_clock::sysclk_setup(SYS_clk sysclk) {
	
	switch (sysclk)
	{	
	case SYS_clk::HSI:
		rcc->CR |= (1u << 0);	//hsi on
		while (0 == (rcc->CR & (1u << 1)));	//hsi ready
		ahb_freq /= 2;
		break;
	case SYS_clk::HSE_div_2:
		rcc->CFGR |= (1u << 17);		//PLLXTPRE
		ahb_freq /= 2;
	case SYS_clk::HSE:
		rcc->CFGR |= (1u << 16);		//PLLSRC
		rcc->CR |= (1u << 16);	//hse on
		rcc->CR |= (1u << 19);
		while (0 == (rcc->CR & (1u << 17))); //hse ready
		break;
	default:
		break;
	}
}

void sys_clock::pllmul_setup(uint8_t pllmul) {
	if (pllmul > 16) {
		pllmul = 16;
	}
	ahb_freq *= pllmul;

	rcc->CR |= (1u << 24);

	rcc->CFGR |= (1u << 1); //selects pll as system clock (Bits 1:0 = 10)
	rcc->CFGR |= (1u << 10); //apb1 clock = sysclk/2, apb2 clock = sysclk
	rcc->CFGR |= (1u << 15); //adc clock = apb2 clock/6
	
	apb1_freq = ahb_freq;
	apb2_freq = ahb_freq / 2;

	rcc->CFGR |= (((pllmul >> 0) & 1u) << 18);
	rcc->CFGR |= (((pllmul >> 0) & 1u) << 19);
	rcc->CFGR |= (((pllmul >> 0) & 1u) << 20);
	rcc->CFGR |= (((pllmul >> 0) & 1u) << 21);
		
	rcc->CFGR |= (1u << 26); //mco = pll / 2
	rcc->CFGR |= (1u << 25); //mco = pll / 2
	rcc->CFGR |= (1u << 24); //mco = pll / 2

	while (0 == (rcc->CR & (1u << 25)));	//pll ready
}

void sys_clock::reset(void) {
	constexpr uint32_t SRAM_START = UINT32_C(0x20000000);
	constexpr uint32_t SRAM_SIZE = UINT32_C(128 * 1024);
	constexpr uint32_t STACK_START = SRAM_START + SRAM_SIZE;

	uint32_t size = reinterpret_cast<uint32_t>(&_edata) - reinterpret_cast<uint32_t>(&_sdata);
	uint32_t* src = &_la_data,
		* dest = &_sdata;
	for (uint32_t i = 0; i < size; ++i) {
		dest[i] = src[i];
	}

	size = reinterpret_cast<uint32_t>(&_ebss) - reinterpret_cast<uint32_t>(&_sbss);
	dest = &_sbss;
	for (uint32_t i = 0; i < size; ++i)
		dest[i] = 0;
	init_ctors();
	main();
}