#pragma once
#include<stdint.h>
#include<stddef.h>
#include<initializer_list>
#include"core/rcc.h"
#include"common/defs.h"

namespace mcu {
	struct adc_registers;

	class adc : peripheral_device{
	public:
		enum Channel :uint8_t { ADC0, ADC1, ADC2, ADC3, ADC4, ADC5, ADC6, ADC7, ADC8, ADC9, TEMP=16, VREF};
		enum class Mode :uint32_t { SINGLE, CONTINOUS };
		adc(Base_Address, mcu::sys_clock::Bridge, uint8_t);
		~adc();
		adc(const adc&) = delete;
		adc& operator= (const adc&) = delete;
		void* operator new(size_t) = delete;
		float convert(Channel l, Mode m = Mode::SINGLE);
		float jconvert(Channel l, Mode m = Mode::SINGLE);
		void init();
	private:
		adc_registers* device;
		uint8_t adc_clock_bit;
		void cal();
		float read();
		float jread();
		void set_mode(Mode);
	};

	namespace peripheral {
		extern adc Adc1;
		extern adc Adc2;
	}
}