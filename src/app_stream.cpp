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
	_read(fd, ch);
	return *this;
}

mcu::io_base& mcu::io_base::operator>>(wifi::file_server fs) {
	wifi::Esp8266 >> fs;
	return *this;
}

mcu::io_base::operator bool() {
	bool status = false;
	switch (fd) {
	case 1:
		status = mcu::peripheral::Uart1;
		break;
	case 2:
		break;
	case 3:
		status = wifi::Esp8266;
		break;
	default:
		break;
	}
	return status;
}


void mcu::_write(size_t fd, const char* str, size_t len) {
	for (int i = 0; i < len; ++i)
	{
		switch (fd) {
		case 1:
			mcu::peripheral::Uart1.put(str[i]);
			break;
		case 2:
			break;
		case 3:
			wifi::Esp8266.put(str[i]);
			break;
		default:
			break;
		}
	}
}

void mcu::_read(size_t fd, char& ch) {
	switch (fd) {
	case 1:
		mcu::peripheral::Uart1.get(ch);
		break;
	case 2:
		break;
	case 3:
		wifi::Esp8266.get(ch);
		break;
	default:
		break;
	}
}

mcu::io_base mcu::fstream::debug(1);
mcu::io_base mcu::fstream::swo(2);
mcu::io_base mcu::fstream::wifi(3);
