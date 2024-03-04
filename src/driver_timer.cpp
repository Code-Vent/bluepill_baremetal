#include"drivers/timer.h"
#include<stddef.h>
#include<type_traits>
#include"common/defs.h"
#include"core/system.h"

using namespace std;


namespace mcu {
	struct timer_registers {
		peripheral_register CR1;
		peripheral_register CR2;
		peripheral_register SMCR;
		peripheral_register DIER;
		peripheral_register SR;
		peripheral_register EGR;
		peripheral_register CCMR1;
		peripheral_register CCMR2;
		peripheral_register CCER;
		peripheral_register CNT;
		peripheral_register PSC;
		peripheral_register ARR;
		peripheral_register RCR;
		peripheral_register CCR1;
		peripheral_register CCR2;
		peripheral_register CCR3;
		peripheral_register CCR4;
		peripheral_register BDTR;
		peripheral_register DCR;
		peripheral_register DMAR;
	};
}

//using namespace mcu;

static_assert(
	offsetof(mcu::timer_registers, DMAR) == 0x4C,
	"timer_registers is not standard layout"
	);



void mcu::timer::pmw_init(Channel channel, uint16_t prescaler, uint16_t period, uint16_t duty_cycle)
{
	uint16_t ccmr = 0b1100100;
	disable();
	//Code here
	pwm_duty_cycle(duty_cycle, channel);
	if (channel == Channel::ONE) {
		device->CCMR1 |= ccmr;
		device->CCER |= 0xF;
	}
	else if (channel == Channel::TWO) {
		device->CCMR1 |= (ccmr << 8);
		device->CCER |= (0xF << 4);
	}
	else if (channel == Channel::THREE) {
		device->CCMR2 |= ccmr;
		device->CCER |= (0xF << 8);
	}
	else {
		device->CCMR2 |= (ccmr << 8);
		device->CCER |= (0xF << 12);
	}
	device->ARR = period;
	device->PSC = prescaler;
	enable();
}

mcu::timer::timer(Base_Address address, sys_clock::Bridge b, uint8_t clock_bit)
	:peripheral_device(b, {clock_bit})
{
	device = reinterpret_cast<timer_registers*>(address);	
}

mcu::timer::~timer()
{
	disable();
}

void mcu::timer::start() {
	device->CR1 |= UINT32_C(1);
}

void mcu::timer::pwm_duty_cycle(uint16_t duty_cycle, Channel channel) {
	if (channel == Channel::ONE) {
		device->CCR1 = duty_cycle;
	}
	else if (channel == Channel::TWO) {
		device->CCR2 = duty_cycle;
	}
	else if (channel == Channel::THREE) {
		device->CCR3 = duty_cycle;
	}
	else {
		device->CCR4 = duty_cycle;
	}
}

mcu::timer mcu::peripheral::Timer1(0x40012C00, sys_clock::Bridge::APB2, 11);
mcu::timer mcu::peripheral::Timer2(0x40000000, sys_clock::Bridge::APB1, 0);
mcu::timer mcu::peripheral::Timer3(0x40000400, sys_clock::Bridge::APB1, 1);
mcu::timer mcu::peripheral::Timer4(0x40000800, sys_clock::Bridge::APB1, 2);