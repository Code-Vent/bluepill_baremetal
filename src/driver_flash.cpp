#include"drivers/flash.h"
#include"common/defs.h"
#include<stddef.h>

namespace mcu {
	struct flash_registers {
		peripheral_register ACR;
		peripheral_register KEYR;
		peripheral_register OPTKEYR;
		peripheral_register SR;
		peripheral_register CR;
		peripheral_register AR;
		peripheral_register RESERVED;
		peripheral_register OBR;
		peripheral_register WRPR;
	};
}

static_assert(
	offsetof(mcu::flash_registers, WRPR) == 0x020,
	"nvic_registers is not standard layout"
	);

mcu::flash::flash(Base_Address address, mcu::sys_clock::Bridge bridge, uint8_t clock_bit) 
	:peripheral_device(bridge, {clock_bit}), length(0), buffer{0}
{
	device = reinterpret_cast<flash_registers*>(address);
}

mcu::flash::~flash() {

}

bool mcu::flash::flush(uint8_t page) {
	if (length == 0)return false;
	if (page < MIN_PAGE_NUM || page > MAX_PAGE_NUM) return false;
	uint32_t base_address = 0x08000000 + (page * 1024);	
	unlock();
	erase_page(base_address);

	if (!*this)return false;
	device->CR |= FLASH_CR_PG;
	for (size_t i = 0; i < length; ++i) {
		*(volatile uint8_t*)(base_address + i) = buffer[i];
		while (device->SR & FLASH_SR_BUSY);
		if (!*this)break;
	}
	lock();
	length = 0;
	return *this;
}

void mcu::flash::write(uint8_t byte) {
	if (length == MAX_SIZE)return;
	buffer[length++] = byte;
	return;
}

bool mcu::flash::eof() {
	return length >= MAX_SIZE;
}

void mcu::flash::erase_page(uint32_t address) {
	device->CR |= FLASH_CR_PER;
	device->AR = address;
	device->CR |= FLASH_CR_STRT;
	while (device->SR & FLASH_SR_BUSY);
	device->CR &= ~FLASH_CR_PER;
}

void mcu::flash::unlock() {
	while (device->SR & FLASH_SR_BUSY);
	device->KEYR = KEY1;
	device->KEYR = KEY2;
	while (device->SR & FLASH_SR_BUSY);
}

void mcu::flash::lock() {
	device->CR |= FLASH_CR_LOCK;
}

mcu::flash::operator bool() {
	bool pgm_err = device->SR & UINT32_C(4);
	bool wrt_err = device->SR & UINT32_C(16);
	bool opt_byte_err = device->SR & UINT32_C(65536);
	return pgm_err || wrt_err || opt_byte_err;
}









mcu::flash mcu::peripheral::Flash(0x40022000, mcu::sys_clock::Bridge::AHB, 4);
