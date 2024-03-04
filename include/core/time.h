#pragma once
#include<stdint.h>

namespace mcu {
	struct tick_registers;

	class sys_time {
		friend class system;
	public:

	private:
		static uint64_t cnt;
		sys_time() = default;
		void configure();
		tick_registers* stk;
		static void tick();
	};
}