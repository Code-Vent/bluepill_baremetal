#include"common/defs.h"


uint32_t peripheral_device::clear()
{
	auto& clock = mcu::system::get_instance().get_clock();
	for (auto iter = clock_bits.begin(); iter != clock_bits.end(); ++iter)
		clock.reset_peripheral(bridge, *iter);
	return 0;
}

uint32_t peripheral_device::enable()
{
	auto& clock = mcu::system::get_instance().get_clock();
	for (auto iter = clock_bits.begin(); iter != clock_bits.end(); ++iter)
		clock.enable_peripheral(bridge, *iter);
	return 0;
}

uint32_t peripheral_device::disable()
{
	auto& clock = mcu::system::get_instance().get_clock();
	for (auto iter = clock_bits.begin(); iter != clock_bits.end(); ++iter)
		clock.disable_peripheral(bridge, *iter);
	return 0;
}
