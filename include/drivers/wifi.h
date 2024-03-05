#pragma once
#include<stdint.h>
#include<stddef.h>
#include<string>
#include"uart.h"


namespace wifi {
	class esp8266 {
	public:
		esp8266(uint32_t baud_rate);
		void conn();//establish connection
		operator bool();
		bool put(char ch);
		bool get(char& ch);
	private:
		static constexpr uint8_t BUF_SIZE = 50;
		char at_cmd[BUF_SIZE];
	};

	extern esp8266 Esp8266;
}