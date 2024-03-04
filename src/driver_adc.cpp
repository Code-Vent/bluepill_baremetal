#include"drivers/adc.h"
#include"common/defs.h"
#include<stddef.h>

using namespace mcu;

namespace mcu {
	struct adc_registers {
		peripheral_register SR;
		peripheral_register CR1;
		peripheral_register CR2;
		peripheral_register SMPR1;
		peripheral_register SMPR2;
		peripheral_register JOFR1;
		peripheral_register JOFR2;
		peripheral_register JOFR3;
		peripheral_register JOFR4;
		peripheral_register HTR;
		peripheral_register LTR;
		peripheral_register SQR1;
		peripheral_register SQR2;
		peripheral_register SQR3;
		peripheral_register JSQR;
		peripheral_register JDR1;
		peripheral_register JDR2;
		peripheral_register JDR3;
		peripheral_register JDR4;
		peripheral_register DR;
	};
}

static_assert(
	offsetof(adc_registers, DR) == 0x04C,
	"nvic_registers is not standard layout"
	);


mcu::adc::adc(Base_Address address, mcu::sys_clock::Bridge bridge, uint8_t clock_bit)
	:peripheral_device(bridge, {clock_bit})
{
	device = reinterpret_cast<adc_registers*>(address);
}

void mcu::adc::init() {
	disable();
	device->CR2 &= ~UINT32_C(1);
	cal();
	device->CR2 |= UINT32_C(1);
	enable();
}

mcu::adc::~adc()
{
	disable();
}

void mcu::adc::cal(){
	device->SMPR1 = 0xffffff;
	device->SMPR2 = 0x3fffffff;
	device->CR2 = 0xC;
	while (device->CR2 & 0b100);
}

void mcu::adc::set_mode(Mode m) {
	switch (m) {
	case Mode::SINGLE:
		device->CR2 &= ~(UINT32_C(1) << 1);
		break;
	case Mode::CONTINOUS:
		device->CR2 |= (UINT32_C(1) << 1);
		break;
	}
}

float mcu::adc::convert(Channel l, Mode m)
{
	device->SQR3 = 0x1f & l;
	device->SQR1 = UINT32_C(1) << 20;
	set_mode(m);
	device->CR2 |= UINT32_C(1) << 22;//swstart
	return read();
}

float mcu::adc::jconvert(Channel l, Mode m)
{
	device->JSQR = 0x1f & l;
	device->JSQR |= (UINT32_C(1) << 20);
	set_mode(m);
	device->CR2 |= UINT32_C(1) << 21;//jswstart
	return jread();
}

float mcu::adc::read() {
	// Wait for conversion to complete
	while (!(device->SR & 0x01));
	// Read ADC result
	uint16_t adcValue = device->DR;
	// Convert ADC value to analog voltage
	float analogVoltage = (adcValue / 1024.0f) * 3.3f;
	device->SR = 0;
	return analogVoltage;
}

float mcu::adc::jread() {
	// Wait for conversion to complete
	while (!(device->SR & 0b100));
	// Read ADC result
	uint16_t adcValue = device->JDR1;
	// Convert ADC value to analog voltage
	float analogVoltage = (adcValue / 1024.0f) * 3.3f;
	device->SR = 0;
	return analogVoltage;
}


adc mcu::peripheral::Adc1(0x40012400, sys_clock::Bridge::APB2, 9);
adc mcu::peripheral::Adc2(0x40012800, sys_clock::Bridge::APB2, 10);
