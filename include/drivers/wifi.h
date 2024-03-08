#pragma once
#include<stdint.h>
#include<stddef.h>
#include<string>
#include"uart.h"


namespace wifi {
	struct file_server {
		file_server(const char* filename);
		uint32_t operator()();
	private:
		char* filename;
	};

	class esp8266 {
	public:
		enum class Result :uint8_t { OK, NOT_OK };
		esp8266();
		~esp8266();
		operator bool();
		bool put(char ch);
		bool get(char& ch);
		esp8266& operator>>(file_server);
		bool reset();
		static Result send_at_command(const char* cmd, char* response, uint8_t res_len);
		static uint16_t parse_next_int(char**);
	private:
		bool conn();//establish connection
		bool disconn();
		static constexpr uint8_t RESULT_SIZE = 25;
		uint32_t received_data_length;
		bool is_connected;
	};

	extern esp8266 Esp8266;
}