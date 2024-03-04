#include"../include/drivers/uart.h"
#include"../include/drivers/port.h"
#include"../include/drivers/adc.h"
#include"../include/drivers/timer.h"

mcu::port::pin_def rx = {
	.pin_number = 7,
	.mode = mcu::port::Mode_Option::IN,
	.in = mcu::port::Input_Option::PULL_UP,
};

mcu::port::pin_def tx = {
	.pin_number = 6,
	.mode = mcu::port::Mode_Option::ALTERNATE_FUNC,
	.out = mcu::port::Output_Option::OPEN_DRAIN,
	.speed = mcu::port::Speed_Option::VERY_HIGH
};

int main() {
	using namespace mcu::peripheral;
	Uart1.init(mcu::uart::Option::UART, 115200);
	PortB.init({ rx, tx });
	Timer1.clear();
	char ch;
	for (;;) {
		auto f = Adc1.convert(mcu::adc::VREF, mcu::adc::Mode::CONTINOUS);
		if (Uart1) {
			if (Uart1.get(ch)) {
				Uart1.put(ch);
			}
		}
		else {
			Uart1.clear();
		}
	}
}