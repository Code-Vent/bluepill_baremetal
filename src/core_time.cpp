#include"core/time.h"
#include"common/defs.h"
#include<stddef.h>


using namespace mcu;

namespace mcu {
	struct tick_registers {
		peripheral_register CTRL;
		peripheral_register LOAD;
		peripheral_register VAL;
		peripheral_register CALIB;
	};
}

static_assert(
	offsetof(tick_registers, CALIB) == 0x0C,
	"scb_registers is not standard layout"
	);

uint64_t sys_time::cnt = 0;

void sys_time::configure() {
	stk->CTRL = 0b111;
	stk->LOAD = 0x1193F;
}

void sys_time::tick() {
	++cnt;
}