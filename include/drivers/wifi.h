#pragma once
#include<stdint.h>
#include<stddef.h>
#include"uart.h"


namespace wifi {
	class esp8266 {
	public:
		esp8266(uint32_t baud_rate);
		void init();
		operator bool();
		bool put(char ch);
		bool get(char& ch);
	private:
	};

	extern esp8266 Esp8266;
}