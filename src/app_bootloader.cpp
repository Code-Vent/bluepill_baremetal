#include"app/bootloader.h"
#include"app/stream.h"
#include"core/system.h"

void mcu::bootloader::load_bin_file(uint8_t page, io_base& io) {
	bin_file_loader(page, io);
}

void mcu::bootloader::load_and_run_bin_file(uint8_t page, io_base& io) {
	uint32_t vector_address = 0x08000000 + (page * 1024);
	bin_file_loader(page, io);
	//set vector table offset
	auto scb = mcu::system::get_instance().get_control_block();
	scb.set_vector_table_offset(vector_address);
	//jump to entry routine
	jump_to_start(vector_address + 4);
}

void mcu::bootloader::jump_to_start(uint32_t start) {
	__asm__ volatile("BX R0");
}

bool mcu::bootloader::bin_file_loader(uint8_t page, io_base& io) {
	char ch;
	while (io >> ch) {
		mcu::peripheral::Flash.write(ch);
		if (mcu::peripheral::Flash.end_of_page()) {
			bool r = mcu::peripheral::Flash.flush(page++);
			if (!r)return false;
		}
	}
	return mcu::peripheral::Flash.flush(page);
}