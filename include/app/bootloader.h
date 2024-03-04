#pragma once
#include<stdint.h>
#include<stddef.h>
#include"stream.h"
#include"../drivers/flash.h"
#include"common/defs.h"


namespace mcu {
	class bootloader {
	public:
		void load(uint8_t page);
		void load_and_run(uint8_t page);
	private:
		bool loader(uint8_t page);
	};
}