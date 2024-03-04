#include"app/bootloader.h"
#include"app/stream.h"

void mcu::bootloader::load(uint8_t page) {
	loader(page);
}

void mcu::bootloader::load_and_run(uint8_t page) {
	loader(page);
	//set vector table offset
	//jump to entry routine
}

bool mcu::bootloader::loader(uint8_t page) {
	char ch;
	while (mcu::fstream::debug >> ch) {
		mcu::peripheral::Flash.write(ch);
		if (mcu::peripheral::Flash.eof()) {
			bool r = mcu::peripheral::Flash.flush(page++);
			if (!r)return false;
		}
	}
	return mcu::peripheral::Flash.flush(page);
}