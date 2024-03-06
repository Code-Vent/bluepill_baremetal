#include"drivers/wifi.h"
#include"drivers/uart.h"

wifi::endpiont::endpiont(const char* url) 
{
	//send ATcommands to GET from the endpoint
}

uint32_t wifi::endpiont::operator()() {
	//sent ATcommands to receive data from server
	//process response
	return 0;
}

wifi::esp8266::esp8266()
	:received_data_length(0)
{
	mcu::peripheral::Uart3.init(mcu::uart::Option::UART, 9600);
	conn();
}

void wifi::esp8266::conn() {
	//ATcommands to establish connection
}

wifi::esp8266& wifi::esp8266::operator>>(endpiont e) {
	received_data_length = e();
	return *this;
}

void wifi::esp8266::change_baud_rate(uint32_t baud_rate) {
	mcu::peripheral::Uart3.init(mcu::uart::Option::UART, baud_rate);
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
