#include"../include/drivers/port.h"
#include"core/system.h"


mcu::port::pin_def led = {
	.pin_number = 13,
	.mode       = mcu::port::Mode_Option::OUT,
	.out        = mcu::port::Output_Option::OPEN_DRAIN,
	.in         = mcu::port::Input_Option::NONE,
	.speed      = mcu::port::Speed_Option::MEDIUM
};

int main() {
	
	mcu::peripheral::PortC.init({ led });
	std::chrono::high_resolution_clock::now();
	for (;;) {
		mcu::peripheral::PortC.toggle(led.pin_number);
		for (int i = 0; i < 400000; ++i)
			__asm__ volatile("nop");
	}
}