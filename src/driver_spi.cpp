#include"drivers/spi.h"
#include<stddef.h>
#include<type_traits>
#include"common/defs.h"
#include"core/system.h"

using namespace std;

namespace mcu {
	struct spi_registers {
		peripheral_register CR1;
		peripheral_register CR2;
		peripheral_register SR;
		peripheral_register DR;
		peripheral_register CRCPR;
		peripheral_register RXCRCR;
		peripheral_register TXCRCR;
		peripheral_register I2SCFGR;
		peripheral_register I2SPR;
	};
}

static_assert(
	offsetof(mcu::spi_registers, I2SPR) == 0x20,
	"timer_registers is not standard layout"
	);



mcu::spi::spi(Base_Address address, sys_clock::Bridge bridge, uint8_t clock_bit)
	:peripheral_device(bridge, {clock_bit})
{
	device = reinterpret_cast<spi_registers*>(address);
}

mcu::spi::~spi()
{
	disable();
}

void mcu::spi::init(Mode m) {
	switch (m) {
	case Mode::RECEIVE_ONLY:
		device->CR1 = 0x8444;
		break;
	case Mode::TRANSMIT_ONLY:
		device->CR1 = 0xC044;
		break;
	default:
		device->CR1 = 0x0044;
		break;
	}
	device->CR2 = 0x4;
}

void mcu::spi::init_master(Mode m) {
	disable();
	init(m);
	device->CR1 |= (UINT32_C(1) << 2);
	enable();
}

void mcu::spi::init_slave(Mode m) {
	disable();
	init(m);
	device->CR1 &= ~(UINT32_C(1) << 2);
	enable();
}

bool mcu::spi::put(char ch)
{
	bool res = false;
	if (device->SR & (UINT32_C(1) << 1)) {
		device->DR = UINT32_C(0xFF) & ch;
		res = true;
	}
	return res;
}

bool mcu::spi::get(char& ch)
{
	bool rxne = device->SR & (UINT32_C(1) << 0);
	if (rxne) {
		ch = static_cast<char>(device->DR);
		return true;
	}
	return false;
}

mcu::spi::operator bool() const
{
	bool overrun_err = device->SR | (1u << 6);
	bool mode_err = device->SR | (1u << 5);
	bool crc_err = device->SR | (1u << 4);
	bool underrun_err = device->SR | (1u << 3);
	
	return overrun_err || mode_err || crc_err || underrun_err;
}
mcu::spi mcu::peripheral::Spi1(0x40013000, mcu::sys_clock::Bridge::APB2, 12);
mcu::spi mcu::peripheral::Spi2(0x40003800, mcu::sys_clock::Bridge::APB1, 14);
mcu::spi mcu::peripheral::Spi3(0x40003C00, mcu::sys_clock::Bridge::APB1, 15);