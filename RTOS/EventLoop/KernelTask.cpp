#include "KernelTask.h"
#include "TaskManager.h"
#include "Timer.h"
#include "Handle.h"

namespace RTOS {
	TimerUpdata::TimerUpdata() : KernelTask(ProcessCallback([]() {
		getMainTimer().tick(getTime());
		updataTimer();
	})) {}


	RunningFreq::RunningFreq() : KernelTask(ProcessCallback(), 10) {}

	uint32_t RunningFreq::smoothControls(uint32_t now_freq){
		auto& task_manager = *TaskManager::getInstance();
		uint8_t now_percent = now_freq * 100 / MAX_CPU_FREQ_Hz;
		uint8_t target_percent = now_percent;

		if (task_manager.getRunningFreq() > CONTROL_FREQ_Hz + 50) {
			if (target_percent > MIN_CPU_FREQ_PERCENT) {
				target_percent--;
			}
		}
		else if (task_manager.getRunningFreq() < CONTROL_FREQ_Hz - 50) {
			if (target_percent < 100) {
				target_percent++;
			}
		}
		
		return target_percent * MAX_CPU_FREQ_Hz / 100;
	}

	void RunningFreq::run_impl() {
		uint64_t now = getTime();
		auto& task_manager = *TaskManager::getInstance();
		if (now - _last_check_time >= ADJUST_INTERVAL_MS) {
			_current_freq_percent = smoothControls(task_manager.getRunningFreq());
			_last_check_time = now;
		}
		task_manager.yield(false);
	}
}