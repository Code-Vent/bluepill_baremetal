#pragma once
#include<stdint.h>
#include<stddef.h>
#include<string>
#include"uart.h"


namespace wifi {
	struct endpiont {
		endpiont(const char* url);
		uint32_t operator()();
	};

	class esp8266 {
	public:
		esp8266();
		operator bool();
		bool put(char ch);
		bool get(char& ch);
		esp8266& operator>>(endpiont);
		void change_baud_rate(uint32_t baud_rate);
	private:
		void conn();//establish connection
		uint32_t received_data_length;
	};

	extern esp8266 Esp8266;
}