#pragma once

#include"../drivers/port.h"
#include"../drivers/timer.h"


namespace mcu {
	class pwm {
	public:
		struct timer_def {
			timer::Channel channel;
			uint16_t prescaler;
			uint16_t period;
			uint16_t duty_cycle;
		};
		pwm(timer&, timer_def);
		~pwm();
		pwm(const pwm&) = delete;
		pwm& operator= (const pwm&) = delete;
		void* operator new(size_t) = delete;
		void set_duty_cycle(uint16_t dc);
	private:
		timer& tim;
		timer::Channel channel;
	};
}