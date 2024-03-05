#include"drivers/wifi.h"
#include"drivers/uart.h"


wifi::esp8266::esp8266(uint32_t baud)
	:at_cmd{0}
{
	mcu::peripheral::Uart1.init(mcu::uart::Option::UART, baud);
}

void wifi::esp8266::conn() {
	//establish connection
}

wifi::esp8266::operator bool() {
	return false;
}

bool wifi::esp8266::put(char ch) {
	return mcu::peripheral::Uart1.put(ch);
}

bool wifi::esp8266::get(char& ch) {
	return mcu::peripheral::Uart1.get(ch);;
}


wifi::esp8266 wifi::Esp8266(9600);
