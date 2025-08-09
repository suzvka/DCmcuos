#include "KernelTask.h"

namespace RTOS {
	TimerUpdata::TimerUpdata() : KernelTask(ProcessCallback([]() {
		for (auto& timer : TimerTickList) {
			timer->tick();
		}
	})) {}



}