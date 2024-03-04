#pragma once
#include<stdint.h>
#include<stddef.h>
#include"core/rcc.h"
#include"common/defs.h"

namespace mcu {
	struct flash_registers;

	class flash : peripheral_device{		
	public:
		flash(Base_Address address, mcu::sys_clock::Bridge bridge, uint8_t clock_bit);
		~flash();
		flash(const flash&) = delete;
		flash& operator= (const flash&) = delete;
		void* operator new(size_t) = delete;
		bool flush(uint8_t page);
		void write(uint8_t byte);
		bool eof();
		operator bool();
	private:
		static constexpr size_t MAX_SIZE = 1024;
		static constexpr size_t MAX_PAGE_NUM = 63;
		static constexpr size_t MIN_PAGE_NUM = 32;
		static constexpr uint32_t KEY1 = 0x45670123;
		static constexpr uint32_t KEY2 = 0xCDEF89AB;
		static constexpr uint32_t FLASH_SR_BUSY = UINT32_C(1);
		static constexpr uint32_t FLASH_SR_EOP = UINT32_C(32);
		static constexpr uint32_t FLASH_CR_LOCK = UINT32_C(64);

		static constexpr uint32_t FLASH_CR_PG = UINT32_C(1);
		static constexpr uint32_t FLASH_CR_PER = UINT32_C(2);
		static constexpr uint32_t FLASH_CR_STRT = UINT32_C(2048);
		uint8_t buffer[MAX_SIZE];
		size_t length;
		flash_registers* device;
		void erase_page(uint32_t);
		void unlock();
		void lock();
	};

	namespace peripheral {
		extern flash Flash;
	}
}