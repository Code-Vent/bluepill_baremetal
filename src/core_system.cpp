#include "..\include\core\system.h"
#include"common/defs.h"
#include<stdint.h>
#include<algorithm>
#include<chrono>

using namespace mcu;


mcu::system system::sys;

namespace std::chrono {
	time_point<high_resolution_clock> high_resolution_clock::now() {
		auto cnt = system::get_instance().get_timer_count();
		return time_point(
			nanoseconds(1000ull*static_cast<nanoseconds::rep>(cnt))
		);
	}
}

mcu::system::system() 
	:power_control(), 
	clock(), 
	control_block(),
	interrupt_controller(),
	time()
{
	power_control.pwr = reinterpret_cast<pwr_registers*>(0x40007000);
	clock.rcc = reinterpret_cast<rcc_registers*>(0x40021000);
	control_block.scb = reinterpret_cast<scb_registers*>(0xE000ED00);// E000ED00);
	interrupt_controller.nvic = reinterpret_cast<nvic_registers*>(0xE000E100);
	interrupt_controller.nvic_stir = reinterpret_cast<nvic_registers*>(0xE000EF00);
	time.stk = reinterpret_cast<tick_registers*>(0xE000E010);

	power_control.configure();
	power_control.disable_write_protection();
	clock.configure(sys_clock::SYS_clk::HSE, sys_clock::RTC_clk::HSE_div_128, 9);
	clock.ahb_freq = 8'000'000;
	control_block.configure();
	interrupt_controller.configure();
	time.configure();
}

mcu::system& system::get_instance() {
	return sys;
}

void system::nmi() {

}

constexpr uint32_t sram_start = 0x20000000;
constexpr uint32_t sram_size = (20 * 1024); //20KB
constexpr uint32_t sram_end = sram_start + sram_size;
constexpr uint32_t stack_start = sram_end;

extern "C" {
	void user_defined_handler() {

	}
}

void debug_monitor()	 __attribute__((weak, alias("user_defined_handler")));
void wwdg_IRQ()			 __attribute__((weak, alias("user_defined_handler")));
void pvd_IRQ()			 __attribute__((weak, alias("user_defined_handler")));
void tamper_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void rtc_IRQ()			 __attribute__((weak, alias("user_defined_handler")));
void flash_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void rcc_IRQ()			 __attribute__((weak, alias("user_defined_handler")));
void exti0_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void exti1_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void exti2_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void exti3_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void exti4_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void dma1_channel1_IRQ() __attribute__((weak, alias("user_defined_handler")));
void dma1_channel2_IRQ() __attribute__((weak, alias("user_defined_handler")));
void dma1_channel3_IRQ() __attribute__((weak, alias("user_defined_handler")));
void dma1_channel4_IRQ() __attribute__((weak, alias("user_defined_handler")));
void dma1_channel5_IRQ() __attribute__((weak, alias("user_defined_handler")));
void dma1_channel6_IRQ() __attribute__((weak, alias("user_defined_handler")));
void dma1_channel7_IRQ() __attribute__((weak, alias("user_defined_handler")));
void adc1_2_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void can1_tx_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void can1_rx0_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void can1_rx1_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void can1_sce_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void exti9_5_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void tim1_brk_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void tim1_up_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void tim1_trg_com_IRQ()	 __attribute__((weak, alias("user_defined_handler")));
void tim1_cc_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void tim2_IRQ()			 __attribute__((weak, alias("user_defined_handler")));
void tim3_IRQ()			 __attribute__((weak, alias("user_defined_handler")));
void tim4_IRQ()			 __attribute__((weak, alias("user_defined_handler")));
void i2c1_ev_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void i2c1_er_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void i2c2_ev_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void i2c2_er_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void spi1_IRQ()			 __attribute__((weak, alias("user_defined_handler")));
void spi2_IRQ()			 __attribute__((weak, alias("user_defined_handler")));
void uart1_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void uart2_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void uart3_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void exti5_10_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void rtc_alarm_IRQ()	 __attribute__((weak, alias("user_defined_handler")));
void otg_fs_wkup_IRQ()	 __attribute__((weak, alias("user_defined_handler")));
void tim5_IRQ()			 __attribute__((weak, alias("user_defined_handler")));
void spi3_IRQ()			 __attribute__((weak, alias("user_defined_handler")));
void uart4_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void uart5_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void tim6_IRQ()			 __attribute__((weak, alias("user_defined_handler")));
void tim7_IRQ()			 __attribute__((weak, alias("user_defined_handler")));
void dma2_channel1_IRQ() __attribute__((weak, alias("user_defined_handler")));
void dma2_channel2_IRQ() __attribute__((weak, alias("user_defined_handler")));
void dma2_channel3_IRQ() __attribute__((weak, alias("user_defined_handler")));
void dma2_channel4_IRQ() __attribute__((weak, alias("user_defined_handler")));
void dma2_channel5_IRQ() __attribute__((weak, alias("user_defined_handler")));
void eth_IRQ()			 __attribute__((weak, alias("user_defined_handler")));
void eth_wkup_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void can2_tx_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void can2_rx0_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void can2_rx1_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void can2_sce_IRQ()		 __attribute__((weak, alias("user_defined_handler")));
void otg_fs_IRQ()		 __attribute__((weak, alias("user_defined_handler")));

uint32_t system::vectors[] __attribute__((section(".isr_vector"))) = {
	stack_start,
	reinterpret_cast<uint32_t>(sys_clock::reset),
	reinterpret_cast<uint32_t>(system::nmi),
	reinterpret_cast<uint32_t>(sys_control_block::hard_fault),
	reinterpret_cast<uint32_t>(sys_control_block::mem_manage_fault),//Memory Manage
	reinterpret_cast<uint32_t>(sys_control_block::bus_fault),//Bus fault
	reinterpret_cast<uint32_t>(sys_control_block::usage_fault),//usage fault
	0,
	0,
	0,
	0,
	reinterpret_cast<uint32_t>(sys_nvic::sv_call),
	reinterpret_cast<uint32_t>(debug_monitor),
	0,
	reinterpret_cast<uint32_t>(sys_nvic::pend_sv),
	reinterpret_cast<uint32_t>(sys_time::tick),
	reinterpret_cast<uint32_t>(wwdg_IRQ),
	reinterpret_cast<uint32_t>(pvd_IRQ),
	reinterpret_cast<uint32_t>(tamper_IRQ),
	reinterpret_cast<uint32_t>(rtc_IRQ),
	reinterpret_cast<uint32_t>(flash_IRQ),
	reinterpret_cast<uint32_t>(rcc_IRQ),
	reinterpret_cast<uint32_t>(exti0_IRQ),
	reinterpret_cast<uint32_t>(exti1_IRQ),
	reinterpret_cast<uint32_t>(exti2_IRQ),
	reinterpret_cast<uint32_t>(exti3_IRQ),
	reinterpret_cast<uint32_t>(exti4_IRQ),
	reinterpret_cast<uint32_t>(dma1_channel1_IRQ),
	reinterpret_cast<uint32_t>(dma1_channel2_IRQ),
	reinterpret_cast<uint32_t>(dma1_channel3_IRQ),
	reinterpret_cast<uint32_t>(dma1_channel4_IRQ),
	reinterpret_cast<uint32_t>(dma1_channel5_IRQ),
	reinterpret_cast<uint32_t>(dma1_channel6_IRQ),
	reinterpret_cast<uint32_t>(dma1_channel7_IRQ),
	reinterpret_cast<uint32_t>(adc1_2_IRQ),
	reinterpret_cast<uint32_t>(can1_tx_IRQ),
	reinterpret_cast<uint32_t>(can1_rx0_IRQ),
	reinterpret_cast<uint32_t>(can1_rx1_IRQ),
	reinterpret_cast<uint32_t>(can1_sce_IRQ),
	reinterpret_cast<uint32_t>(exti9_5_IRQ),
	reinterpret_cast<uint32_t>(tim1_brk_IRQ),
	reinterpret_cast<uint32_t>(tim1_up_IRQ),
	reinterpret_cast<uint32_t>(tim1_trg_com_IRQ),
	reinterpret_cast<uint32_t>(tim1_cc_IRQ),
	reinterpret_cast<uint32_t>(tim2_IRQ),
	reinterpret_cast<uint32_t>(tim3_IRQ),
	reinterpret_cast<uint32_t>(tim4_IRQ),
	reinterpret_cast<uint32_t>(i2c1_ev_IRQ),
	reinterpret_cast<uint32_t>(i2c1_er_IRQ),
	reinterpret_cast<uint32_t>(i2c2_ev_IRQ),
	reinterpret_cast<uint32_t>(i2c2_er_IRQ),
	reinterpret_cast<uint32_t>(spi1_IRQ),
	reinterpret_cast<uint32_t>(spi2_IRQ),
	reinterpret_cast<uint32_t>(uart1_IRQ),
	reinterpret_cast<uint32_t>(uart2_IRQ),
	reinterpret_cast<uint32_t>(uart3_IRQ),
	reinterpret_cast<uint32_t>(exti5_10_IRQ),
	reinterpret_cast<uint32_t>(rtc_alarm_IRQ),
	reinterpret_cast<uint32_t>(otg_fs_wkup_IRQ),
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	reinterpret_cast<uint32_t>(tim5_IRQ),
	reinterpret_cast<uint32_t>(spi3_IRQ),
	reinterpret_cast<uint32_t>(uart4_IRQ),
	reinterpret_cast<uint32_t>(uart5_IRQ),
	reinterpret_cast<uint32_t>(tim6_IRQ),
	reinterpret_cast<uint32_t>(tim7_IRQ),
	reinterpret_cast<uint32_t>(dma2_channel1_IRQ),
	reinterpret_cast<uint32_t>(dma2_channel2_IRQ),
	reinterpret_cast<uint32_t>(dma2_channel3_IRQ),
	reinterpret_cast<uint32_t>(dma2_channel4_IRQ),
	reinterpret_cast<uint32_t>(dma2_channel5_IRQ),
	reinterpret_cast<uint32_t>(eth_IRQ),
	reinterpret_cast<uint32_t>(eth_wkup_IRQ),
	reinterpret_cast<uint32_t>(can2_tx_IRQ),
	reinterpret_cast<uint32_t>(can2_rx0_IRQ),
	reinterpret_cast<uint32_t>(can2_rx1_IRQ),
	reinterpret_cast<uint32_t>(can2_sce_IRQ),
	reinterpret_cast<uint32_t>(otg_fs_IRQ),
};

