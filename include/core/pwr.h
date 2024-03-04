#pragma once
#include<stdint.h>

namespace mcu {
	struct pwr_registers;

	class sys_power_control {
		friend class system;
	public:
	private:
		pwr_registers* pwr;
		sys_power_control() = default;
		void configure();
		void disable_write_protection();
	};
}