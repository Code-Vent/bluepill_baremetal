#pragma once
#include<stdint.h>
#include<chrono>
#include"rcc.h"
#include"scb.h"
#include"pwr.h"
#include"nvic.h"
#include"time.h"
//Set up the peripheral buses


namespace mcu {
	
	class system {
	public:
		static system& get_instance();
		system(const system&) = delete;
		sys_clock& get_clock() { return clock; }
		sys_control_block& get_control_block() { return control_block; }
		uint64_t get_timer_count() { return time.cnt; }
	private:
		static system sys;
		sys_control_block control_block;
		sys_clock clock;
		sys_power_control power_control;
		sys_nvic interrupt_controller;
		sys_time time;
		system();
		static void nmi();
		static uint32_t vectors[84];
	};
}