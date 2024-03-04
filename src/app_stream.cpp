#include"app/stream.h"



mcu::io_base::io_base(size_t f, Radix r)
	:fd(f), radix(r)
{}

mcu::io_base& mcu::io_base::operator<<(const char* str) {
	size_t len = strlen(str);
	_write(fd, str, len);
	return *this;
}

mcu::io_base& mcu::io_base::operator>>(char& ch) {
	return *this;
}

mcu::io_base::operator bool() {
	return mcu::peripheral::Uart1;
}


void mcu::_write(size_t fd, const char* str, size_t len) {
	switch (fd) {
	case 1:
		for (int i = 0; i < len; ++i)
		{
			mcu::peripheral::Uart1.put(str[i]);
		}
		break;
	case 2:
		break;
	default:
		break;
	}
}

void mcu::_read(size_t fd, char& ch) {
	switch (fd) {
	case 1:
		mcu::peripheral::Uart1.get(ch);
		break;
	case 2:
		break;
	default:
		break;
	}
}

mcu::io_base mcu::fstream::debug(1);
mcu::io_base mcu::fstream::swo(2);
