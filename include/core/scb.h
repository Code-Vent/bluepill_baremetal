#pragma once
#include<stdint.h>

namespace mcu {
	struct scb_registers;

	class sys_control_block {
		friend class system;
	public:
		void set_vector_table_offset(uint32_t);
	private:
		scb_registers* scb;
		sys_control_block() = default;
		void configure();
		static void hard_fault();
		static void bus_fault();
		static void usage_fault();
		static void mem_manage_fault();
	};
}