#include"drivers/wifi.h"
#include"drivers/uart.h"
#include<string.h>

static constexpr char RESPONSE_SIZE = 30;
static char response[RESPONSE_SIZE];

wifi::file_server::file_server(const char* f)
	:filename(nullptr)
{
	//send ATcommands to GET from the endpoint
	//192.168.43.1:33455
	if (wifi::esp8266::send_at_command("AT+CIPSTART=\"TCP\",\"192.168.43.1\",33455,7200\r\n",
		response, RESPONSE_SIZE) == wifi::esp8266::Result::OK) {
		filename = const_cast<char*>(f);
	}
}

uint32_t wifi::file_server::operator()() {
	//sent ATcommands to receive data from server
	//process response
	if (filename == nullptr)
		return 0;
	esp8266::send_at_command("", response, RESPONSE_SIZE);
	return 0;
}

wifi::esp8266::esp8266()
	:received_data_length(0), is_connected(false)
{
	mcu::peripheral::Uart3.init(mcu::uart::Option::USART, 115200);;	
	if (send_at_command("AT\r\n", response, RESPONSE_SIZE) == Result::NOT_OK) return;
	if (send_at_command("AT+UART_CUR=115200,8,1,1,3\r\n", response, RESPONSE_SIZE) == Result::NOT_OK) return;
	if (send_at_command("AT+CWMODE_CUR=1\r\n", response, RESPONSE_SIZE) == Result::NOT_OK) return;
	is_connected = conn();
}

wifi::esp8266::~esp8266() {
	disconn();
}

bool wifi::esp8266::reset() {
	if (send_at_command("AT+RST\r\n", response, RESPONSE_SIZE) == Result::OK) {
		return true;
	}
	return false;
}

wifi::esp8266::Result wifi::esp8266::send_at_command(const char* cmd, char* response, uint8_t res_len) {
	auto length = strlen(cmd);
	size_t i;
	for (i = 0; i < length;)
	{
		if(mcu::peripheral::Uart3.put(cmd[i]))
			++i;
	}
	//100 msec delay
	i = 0;
	char c;
	do {
		if (mcu::peripheral::Uart3.get(c))
		{
			response[i++] = c;
		}
	} while (c != '\r' && i < res_len);
	response[--i] = 0;

	if (strcmp("OK", response) == 0) {
		return esp8266::Result::OK;
	}
	else {

		return esp8266::Result::NOT_OK;
	}
}

bool wifi::esp8266::conn() {
	//ATcommands to establish connection
	if (send_at_command("AT+CWJAP_CUR=\"ABC\",\"123456789\"\r\n", 
		response, RESPONSE_SIZE) == Result::NOT_OK) return false;
	return true;
}

bool wifi::esp8266::disconn() {
	//ATcommands to establish connection
	if (send_at_command("AT+CWQAP\r\n",response, RESPONSE_SIZE) == Result::NOT_OK) return false;
	return true;
}

wifi::esp8266& wifi::esp8266::operator>>(file_server fs) {
	received_data_length = fs();
	return *this;
}

wifi::esp8266::operator bool() {
	return received_data_length > 0;
}

bool wifi::esp8266::put(char ch) {
	return mcu::peripheral::Uart3.put(ch);
}

bool wifi::esp8266::get(char& ch) {
	if (received_data_length == 0)return false;
	if (mcu::peripheral::Uart3.get(ch)) {
		--received_data_length;
		return true;
	}
	return false;
}


wifi::esp8266 wifi::Esp8266;
