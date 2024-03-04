#include"core/nvic.h"
#include"common/defs.h"
#include<stddef.h>

using namespace mcu;

namespace mcu {
	struct nvic_registers {
		peripheral_register ISER[3];
		peripheral_register unused_1[29];
		peripheral_register ICER[3];
		peripheral_register unused_2[29];
		peripheral_register ISPR[3];
		peripheral_register unused_3[29];
		peripheral_register ICPR[3];
		peripheral_register unused_4[29];
		peripheral_register IABR[3];
		peripheral_register unused_5[61];
		peripheral_register IPR[21];
		peripheral_register unused_6[683];
		peripheral_register STIR;
	};
}

static_assert(
	offsetof(nvic_registers, ICER[0]) == 0x80,
	"nvic_registers is not standard layout"
);

static_assert(
	offsetof(nvic_registers, ICPR[0]) == 0x180,
	"nvic_registers is not standard layout"
	);

static_assert(
	offsetof(nvic_registers, ISPR[0]) == 0x100,
	"nvic_registers is not standard layout"
	);

static_assert(
	offsetof(nvic_registers, IABR[0]) == 0x200,
	"nvic_registers is not standard layout"
	);

static_assert(
	offsetof(nvic_registers, IPR[0]) == 0x300,
	"nvic_registers is not standard layout"
	);

static_assert(
	offsetof(nvic_registers, STIR) == 0xE00,
	"nvic_registers is not standard layout"
	);

void sys_nvic::configure() {

}

void sys_nvic::sv_call() {

}

void sys_nvic::pend_sv() {

}