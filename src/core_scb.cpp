#include"core/scb.h"
#include"common/defs.h"
#include<stddef.h>

using namespace mcu;

namespace mcu {
	struct scb_registers {
		peripheral_register CPUID;
		peripheral_register ICSR;
		peripheral_register VTOR;
		peripheral_register AIRCR;
		peripheral_register SCR;
		peripheral_register CCR;
		peripheral_register SHPR1;
		peripheral_register SHPR2;
		peripheral_register SHPR3;
		peripheral_register SHCRS;
		peripheral_register CFSR;
		peripheral_register HFSR;
		peripheral_register MMFAR;
		peripheral_register BFAR;
	};
}

static_assert(
	offsetof(scb_registers, BFAR) == 0x34,
	"scb_registers is not standard layout"
);

void sys_control_block::configure() {

}

void sys_control_block::hard_fault() {

}

void sys_control_block::bus_fault() {

}

void sys_control_block::usage_fault() {

}

void sys_control_block::mem_manage_fault() {

}

void sys_control_block::set_vector_table_offset(uint32_t offset) {

}