#include "Timer.h"
namespace RTOS {
	void BaseTimer::reset() { 
		num_us = 0;
	}

	void BaseTimer::tick(uint64_t us) {
		num_us += us;
	}
	uint64_t BaseTimer::now() const {
		return num_us;
	}

	void MainTimer::tick(uint64_t us) {
		MainTimer& instance = *getInstance();
		instance.last_tick_us = instance.num_us;
		instance.BaseTimer::tick(us);
	}

	void MainTimer::updataTimer() {
		MainTimer& instance = *getInstance();
		for (auto& timer : TimerTickList) {
			if (timer) {
				timer->BaseTimer::tick(instance.now() - instance.last_tick_us);
			}
		}
	}

	MainTimer* MainTimer::getInstance() {
		static MainTimer instance;
		return &instance;
	}

	MainTimer& getMainTimer() {
		return *MainTimer::getInstance();
	}

	void updataTimer() {
		MainTimer::updataTimer();
	}

	Timer::Timer(uint32_t us) : cycle_us(us){
		timer_id = PoolTimerID++;
		TimerTickList.push_back(this);
	}

	Timer::Timer() : Timer(1) {}

	Timer::~Timer() {
		for (auto it = TimerTickList.begin(); it != TimerTickList.end(); ++it) {
			if (*it == this) {
				TimerTickList.erase(it);
				break;
			}
		}
	}

	uint64_t Timer::now_us() const {
		return (num_us / cycle_us) * cycle_us;
	}

	uint64_t Timer::now_ms() const {
		return now_us() / 1000;
	}

	double Timer::now_s() const {
		return static_cast<double>(now_ms()) / 1000;
	}

	void Timer::tick(){
		BaseTimer::tick(cycle_us);
	}
}
