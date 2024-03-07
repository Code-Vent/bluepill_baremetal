#include"drivers/uart.h"
//#include<iostream>
#include<type_traits>
#include"common/defs.h"
#include"core/system.h"

using namespace std;


namespace mcu {
	struct uart_registers {
		peripheral_register SR;
		peripheral_register DR;
		peripheral_register BRR;
		peripheral_register CR1;
		peripheral_register CR2;
		peripheral_register CR3;
		peripheral_register GTPR;
	};
}

using namespace mcu;

static_assert(
	is_standard_layout<uart_registers>::value,
	"uart_registers is not standard layout"
	);

static_assert(
	sizeof(uart_registers) == 7 * sizeof(peripheral_register),
	"uart_registers is not standard layout"
	);


void uart::init(Option opt, uint32_t baud)
{
	clear();
	disable();
	auto& clock = system::get_instance().get_clock();
	auto freq = (bridge == sys_clock::Bridge::APB1) ? clock.get_apb1_freq() : clock.get_apb2_freq();
	uint16_t bd = uart_div(freq, baud);
	this->device->BRR = bd;
	if (opt == Option::USART) {
		usart_settings(opt);
	}
	common_settings(opt);
	enable_uart();
	enable();
}

void uart::usart_settings(Option opt) {
	//rts, CTS, CLK PINS
	//CLKEN, CLKPOL, CLKPHASE, LBCL, 
	//CTSEN, RTSEN, 
	this->device->CR2 = UINT32_C(0xE00);
	this->device->CR3 = UINT32_C(0x300);
	//HDSEL, 
}

void uart::common_settings(Option opt) {
	//TX, RX PINS
	
	//PCE, PARITY SEL, WAKE, TE, RE, 
	//1-start-bit, 1-stop-bit
	//8 data bits
	//odd parity
	this->device->CR1 = UINT32_C(0x2604);
	//STOP BIT, 
}

void uart::enable_uart() {
	this->device->CR1 |= UINT32_C(1) << 13;
}

uart::uart(Base_Address address, sys_clock::Bridge bridge, uint8_t clock_bit)
	:peripheral_device(bridge, {clock_bit})
{
	device = reinterpret_cast<uart_registers*>(address);
}

uart::~uart()
{
	disable();
}

bool uart::put(char ch)
{
	bool res = false;
	device->CR1 |= UINT32_C(8);
	if (device->SR & UINT32_C(128)) {
		device->DR = UINT32_C(0xFF) & ch;
		res = true;
	}
	device->CR1 &= ~UINT32_C(8);
	return res;
}

bool uart::get(char& ch)
{
	bool rxne = device->SR & UINT32_C(32);
	if (rxne) {
		ch = static_cast<char>(device->DR);
		return true;
	}
	return false;
}

uint16_t uart::uart_div(const uint32_t freq, const uint32_t buad) {
	uint32_t div = (buad << 4);
	uint32_t mantissa = freq / div;
	uint32_t frac = 16 * ((freq % div) / (float)div);
	uint16_t uartdiv = frac + ((mantissa < 255) ? (mantissa << 4) : mantissa);
	return uartdiv;
}

uart::operator bool() const{
	bool overrun_err = device->SR | (1u << 3);
	bool noise_err   = device->SR | (1u << 2);
	bool frame_err   = device->SR | (1u << 1);
	bool parity_err  = device->SR | (1u << 0);
	return overrun_err || noise_err || frame_err || parity_err;
}

uart mcu::peripheral::Uart1(0x40013800, sys_clock::Bridge::APB2, 14);
uart mcu::peripheral::Uart2(0x40004400, sys_clock::Bridge::APB1, 17);
uart mcu::peripheral::Uart3(0x40004800, sys_clock::Bridge::APB1, 18);