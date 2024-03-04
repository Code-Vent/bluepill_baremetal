#pragma once

#include"../drivers/uart.h"
#include<string>
#include<stdlib.h>
#include<string.h>

namespace mcu {

	void _write(size_t, const char*, size_t);
	void _read(size_t, char&);

	class io_base {
	public:
		enum Radix{bin = 2, oct = 8, dec = 10, hex = 16};
		io_base(size_t fd, Radix r = dec);
		io_base& operator<<(const char* str);
		io_base& operator>>(char& ch);
		io_base& operator<<(Radix r) { 
			radix = r; 
			return *this;
		}

		template<typename T> requires std::integral<T>
		io_base& operator<<(T value) {
			char buffer[32];
			auto str = itoa(value, buffer, radix);
			operator<<(buffer);
			return *this;
		}

		template<typename T> requires std::floating_point<T>
		io_base& operator<<(T value) {
			auto str = std::to_string(value);
			operator<<(str.c_str());
			return *this;
		}
		operator bool();
	private:
		size_t fd;
		Radix radix;
	};

	namespace fstream {
		extern io_base debug;
		extern io_base swo;
	}
}