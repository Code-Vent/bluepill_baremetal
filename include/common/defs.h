#pragma once

#include<stdint.h>
#include<initializer_list>
#include"core/system.h"

using Base_Address = uint32_t;

using peripheral_register = uint32_t volatile;

struct peripheral_device {
protected:
	mcu::sys_clock::Bridge bridge;
	std::initializer_list<uint8_t> clock_bits;
	peripheral_device(mcu::sys_clock::Bridge b, std::initializer_list<uint8_t> ck)
		:bridge(b), clock_bits(ck)
	{
	}
	uint32_t enable();
	uint32_t disable();
public:
	uint32_t clear();
};