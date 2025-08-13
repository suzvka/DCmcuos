#include "KernelTask.h"
#include "Timer.h"
#include "Handle.h"

namespace RTOS {
	TimerUpdata::TimerUpdata() : KernelTask(ProcessCallback([]() {
		getMainTimer().tick(getTime());
		updataTimer();
	})) {}


}