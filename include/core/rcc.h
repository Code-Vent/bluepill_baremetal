#pragma once
#include<stdint.h>

namespace mcu {
	struct rcc_registers;

	class sys_clock {
		friend class system;
	public:
		enum class Bridge:uint8_t { AHB, APB1, APB2 };
		enum class RTC_clk:uint8_t { LSI, LSE, HSE_div_128 };
		enum class SYS_clk:uint8_t { HSI, HSE, HSE_div_2 };
		
		void enable_peripheral(Bridge, uint8_t bit);
		void disable_peripheral(Bridge, uint8_t bit);
		void reset_peripheral(Bridge, uint8_t bit);
		uint32_t get_ahb_freq() { return ahb_freq; }
		uint32_t get_apb1_freq() { return apb1_freq; }
		uint32_t get_apb2_freq() { return apb2_freq; }
	private:
		static void reset(void);
		void configure(SYS_clk sysclk, RTC_clk rtcclk, uint16_t pllmul_flag);
		void rtcclk_setup(RTC_clk rtcclk);
		void sysclk_setup(SYS_clk sysclk);
		void pllmul_setup(uint8_t pllmul);
		sys_clock() = default;
		uint32_t ahb_freq;
		uint32_t apb1_freq;
		uint32_t apb2_freq;
		rcc_registers* rcc;
	};
}