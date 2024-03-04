#include"app/pwm.h"


mcu::pwm::pwm(timer& t, timer_def td)
	:tim(t), channel(td.channel)
{
	t.pmw_init(td.channel, td.prescaler, td.period, td.duty_cycle);
	t.start();
}

mcu::pwm::~pwm()
{
}

void mcu::pwm::set_duty_cycle(uint16_t dc) {
	tim.pwm_duty_cycle(dc, channel);
}