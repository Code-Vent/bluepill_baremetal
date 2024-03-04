#include"drivers/port.h"
#include<stddef.h>

using namespace mcu;

namespace mcu {
	struct gpio_registers {
		union {
			struct {
				peripheral_register CRL;
				peripheral_register CRH;
			};
			uint64_t CR;
		};
		peripheral_register IDR;
		peripheral_register ODR;
		peripheral_register BSRR;
		peripheral_register BRR;
		peripheral_register LCKR;
	};

	struct afio_registers {
		peripheral_register EVCR;
		peripheral_register MAP;
		peripheral_register EXTICR1;
		peripheral_register EXTICR2;
		peripheral_register EXTICR3;
		peripheral_register EXTICR4;
	};
}

static_assert(
	offsetof(gpio_registers, LCKR) == 0x18,
	"nvic_registers is not standard layout"
	);

static_assert(
	offsetof(afio_registers, EXTICR4) == 0x14,
	"nvic_registers is not standard layout"
	);

port::port(Base_Address address, sys_clock::Bridge b, uint8_t clock_bit)
	:peripheral_device(b, {clock_bit, 0})
{
	gpio = reinterpret_cast<gpio_registers*>(address);
	afio = reinterpret_cast<afio_registers*>(0x40010000);
}

port::~port() {
	disable();
}


void port::init(const std::initializer_list<pin_def>& list)
{	
	disable();
	for (auto iter = list.begin(); iter != list.end(); ++iter)
	{
		if (iter->mode == Mode_Option::IN)
			input(*iter);
		else if (iter->mode == Mode_Option::OUT)
			output(*iter);
		else if (iter->mode == Mode_Option::ALTERNATE_FUNC)
			alternate_func(*iter);
		else
			analog(*iter);
	}
	enable();
}

void port::input(const pin_def& p) {
	uint8_t lsb = p.pin_number * 4;
	gpio->CR &= ~(UINT64_C(1) << lsb);
	gpio->CR &= ~(UINT64_C(1) << (lsb + 1));
	if (p.in == Input_Option::NONE)
	{
		gpio->CR |= (UINT64_C(1) << (lsb + 2));
		gpio->CR &= ~(UINT64_C(1) << (lsb + 3));
	}
	else {
		gpio->CR &= ~(UINT64_C(1) << (lsb + 2));
		gpio->CR |= (UINT64_C(1) << (lsb + 3));
	}
}

void port::output(const pin_def& p)
{
	uint8_t lsb = p.pin_number * 4;
	switch (p.speed) {
	case Speed_Option::LOW:
		gpio->CR &= ~(UINT64_C(1) << lsb);
		gpio->CR |= (UINT64_C(1) << (lsb + 1));
		break;
	case Speed_Option::MEDIUM:
	case Speed_Option::HIGH:
		gpio->CR |= (UINT64_C(1) << lsb);
		gpio->CR &= ~(UINT64_C(1) << (lsb + 1));
		break;
	case Speed_Option::VERY_HIGH:
		gpio->CR |= (UINT64_C(1) << lsb);
		gpio->CR |= (UINT64_C(1) << (lsb + 1));
		break;
	}

	if (p.out == Output_Option::OPEN_DRAIN)
	{
		gpio->CR |= (UINT64_C(1) << (lsb + 2));
		gpio->CR &= ~(UINT64_C(1) << (lsb + 3));
	}
	else {
		gpio->CR &= ~(UINT64_C(1) << (lsb + 2));
		gpio->CR &= ~(UINT64_C(1) << (lsb + 3));
	}
}

void port::alternate_func(const pin_def& p) {
	uint8_t lsb = p.pin_number * 4;
	gpio->CR |= (UINT64_C(1) << lsb);
	gpio->CR |= (UINT64_C(1) << (lsb + 1));

	if (p.out == Output_Option::OPEN_DRAIN)
	{
		gpio->CR |= (UINT64_C(1) << (lsb + 2));
		gpio->CR |= (UINT64_C(1) << (lsb + 3));
	}
	else {
		gpio->CR &= ~(UINT64_C(1) << (lsb + 2));
		gpio->CR |= (UINT64_C(1) << (lsb + 3));
	}
}

void port::analog(const pin_def& p) {
	uint8_t lsb = p.pin_number * 4;
	gpio->CR &= ~(UINT64_C(1) << lsb);
	gpio->CR &= ~(UINT64_C(1) << (lsb + 1));
	gpio->CR &= ~(UINT64_C(1) << (lsb + 2));
	gpio->CR &= ~(UINT64_C(1) << (lsb + 3));
}

bool port::operator()(uint8_t pin_number) const
{
	return gpio->IDR & (UINT32_C(1) << pin_number);
}

void mcu::port::toggle(uint8_t pin_number)
{
	gpio->ODR ^= (UINT32_C(1) << pin_number);
}

void mcu::port::low(uint8_t pin_number)
{
	gpio->BSRR &= ~(UINT32_C(1) << pin_number);
	gpio->BSRR |= (UINT32_C(1) << (pin_number + 16));
}

void mcu::port::high(uint8_t pin_number)
{
	gpio->BSRR &= ~(UINT32_C(1) << (pin_number + 16));
	gpio->BSRR |= (UINT32_C(1) << pin_number);
}


port mcu::peripheral::PortA(0x40010800, sys_clock::Bridge::APB2, 2);
port mcu::peripheral::PortB(0x40010C00, sys_clock::Bridge::APB2, 3);
port mcu::peripheral::PortC(0x40011000, sys_clock::Bridge::APB2, 4);