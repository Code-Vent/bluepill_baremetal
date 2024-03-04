#pragma once
#include<stdint.h>
#include<stddef.h>
#include"core/rcc.h"
#include"common/defs.h"

namespace mcu {
	struct timer_registers;

	class timer : public peripheral_device{
		friend class pwm;
		enum class Direction :uint32_t {};
		enum class Channel:uint8_t{ONE, TWO, THREE, FOUR};
	public:
		timer(Base_Address address, sys_clock::Bridge bridge, uint8_t clock_bit);
		~timer();
		timer(const timer&) = delete;
		timer& operator= (const timer&) = delete;
		void* operator new(size_t) = delete;
		operator bool()const;
	private:
		timer_registers* device;
		void pmw_init(Channel channel, uint16_t prescaler, uint16_t period, uint16_t duty_cycle);
		void pwm_duty_cycle(uint16_t value, Channel c);
		void start();
	};

	namespace peripheral {
		extern timer Timer1;
		extern timer Timer2;
		extern timer Timer3;
		extern timer Timer4;
	}
}