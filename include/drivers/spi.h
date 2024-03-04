#pragma once
#include<stdint.h>
#include<stddef.h>
#include"core/rcc.h"
#include"common/defs.h"

namespace mcu {
	struct spi_registers;

	class spi : public peripheral_device {
	public:
		enum class Mode :uint32_t { RECEIVE_ONLY, TRANSMIT_ONLY, BOTH };
		spi(Base_Address, sys_clock::Bridge bridge, uint8_t clock_bit);
		~spi();
		spi(const spi&) = delete;
		spi& operator= (const spi&) = delete;
		void* operator new(size_t) = delete;
		bool put(char ch);
		bool get(char& ch);
		operator bool()const;
		void init_slave(Mode);
		void init_master(Mode);
	private:
		void init(Mode);
		spi_registers* device;
		void enable_spi();
	};

	namespace peripheral {
		extern spi  Spi1;
		extern spi  Spi2;
		extern spi  Spi3;
	}
}