#pragma once
#include<stdint.h>
#include<stddef.h>
#include"core/rcc.h"
#include"common/defs.h"

namespace mcu {
	struct uart_registers;

	class uart : public peripheral_device{
	public:
		enum class Option :uint32_t { UART, USART };
		uart(Base_Address, sys_clock::Bridge bridge, uint8_t clock_bit);
		~uart();
		uart(const uart&) = delete;
		uart& operator= (const uart&) = delete;
		void* operator new(size_t) = delete;
		bool put(char ch);
		bool get(char& ch);
		operator bool()const;
		void init(Option opt, uint32_t baud);
	private:
		bool is_usart;
		uart_registers* device;
		void usart_settings(Option opt);
		void common_settings(Option opt);
		void enable_uart();
		uint16_t uart_div(const uint32_t freq, const uint32_t buad);
	};

	namespace peripheral {
		extern uart  Uart1;
		extern uart  Uart2;
		extern uart  Uart3;
	}
}