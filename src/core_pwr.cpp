#include"core/pwr.h"
#include"common/defs.h"
#include<stddef.h>

using namespace mcu;

namespace mcu {
	struct pwr_registers {
		peripheral_register CR;
		peripheral_register CSR;
	};
}

static_assert(
	offsetof(pwr_registers, CSR) == 0x04,
	"pwr_registers is not standard layout"
);

void sys_power_control::configure() {

}

void sys_power_control::disable_write_protection() {
	pwr->CR |= (1u << 8); //access to rtc and backup registers enabled
}