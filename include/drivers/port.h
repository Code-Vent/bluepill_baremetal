#pragma once
#include<stdint.h>
#include<stddef.h>
#include"core/rcc.h"
#include<initializer_list>
#include"common/defs.h"

namespace mcu {
	struct gpio_registers;
	struct afio_registers;

	class port : peripheral_device{
	public:
		enum class Mode_Option :uint32_t { IN, OUT, ALTERNATE_FUNC, ANALOG };
		enum class Output_Option :uint32_t { PUSH_PULL, OPEN_DRAIN };
		enum class Input_Option :uint32_t { NONE, PULL_UP, PULL_DOWN };
		enum class Speed_Option :uint32_t { LOW, MEDIUM, HIGH, VERY_HIGH };

		struct pin_def {
			uint8_t pin_number;
			Mode_Option mode;
			Output_Option out;
			Input_Option in;
			Speed_Option speed;
		};

		port(Base_Address port, sys_clock::Bridge b, uint8_t clock_bit);
		~port();
		port(const port&) = delete;
		port& operator= (const port&) = delete;
		void* operator new(size_t) = delete;
		void toggle(uint8_t pin_number);
		void low(uint8_t pin_number);
		void high(uint8_t pin_number);
		bool operator ()(uint8_t)const;
		void init(const std::initializer_list<pin_def>& list);
	private:
		gpio_registers* gpio;
		afio_registers* afio;
		void input(const pin_def& p);
		void output(const pin_def& p);
		void alternate_func(const pin_def& p);
		void analog(const pin_def& p);
	};

	namespace peripheral {
		extern port PortA;
		extern port PortB;
		extern port PortC;
	}
}