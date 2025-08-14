#pragma once
#include <cstdint>
#include <etl/vector.h>

#include "Config.h"
#include "Handle.h"
#include "Task.h"

namespace RTOS { class GPIO;
	class GPIO {
	public:
		enum class PinMode : uint8_t {
			Input,
			Output,
			Analog,
			PullUp,
			PullDown
		};
		enum class PinState : uint8_t {
			Low,
			High
		};

	private:
		uint8_t pin_number;
		PinMode mode;
		PinState state;

	};

	class AnalogGPIO : public GPIO {
	public:

	private:
		uint32_t value;
		uint32_t threshold;
	};

	static etl::vector<GPIO*, MAX_GPIO_PINS> GPIOList;
}