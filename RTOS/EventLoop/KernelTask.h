#include "Task.h"

namespace RTOS{
	// 刷新定时器任务
	class TimerUpdata : public KernelTask {
	public: TimerUpdata();
	};

	class RunningFreq : public KernelTask {
	public: RunningFreq();
	private:
		uint32_t _last_check_time = 0;
		uint8_t _current_freq_percent = 0;
		static constexpr uint16_t ADJUST_INTERVAL_MS = 100; // 频率调整间隔

		uint32_t smoothControls(uint32_t now_freq);

		void run_impl();
		
	};

}