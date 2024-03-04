#pragma once
#include<stdint.h>
#include<stddef.h>
#include"stream.h"
#include"../drivers/flash.h"
#include"common/defs.h"


namespace mcu {
	class bootloader {
	public:
		void load_bin_file(uint8_t page, io_base& io);
		void load_and_run_bin_file(uint8_t page, io_base& io);
	private:
		bool bin_file_loader(uint8_t page, io_base& io);
		void jump_to_start(uint32_t start);
	};
}