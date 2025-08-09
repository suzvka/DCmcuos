#include "Task.h"

namespace RTOS{
	// 刷新定时器任务
	class TimerUpdata : public KernelTask {
	public: TimerUpdata();
	};

	// 钟摆回调
	class PendulumCallback : public KernelTask {
	public: PendulumCallback();
	};

}