#pragma once
#include <cstdint>
#include <etl/vector.h>

#include "Config.h"

namespace RTOS {
	class MainTimer;
	class Timer;
	static etl::vector<Timer*, MAX_TIMERS> TimerTickList;
	static uint32_t PoolTimerID = 0;

	class BaseTimer {
	protected: uint64_t num_us = 0;
	public:
		void tick(uint64_t us);
		uint64_t now() const;

		void reset();
		
		uint32_t timer_id = 0;
	};

	class MainTimer : public BaseTimer {
	public:
		static MainTimer* getInstance();
		static void tick(uint64_t us);
		static void updataTimer();

	private:
		MainTimer(){}
		MainTimer(const MainTimer&) = delete;
		MainTimer& operator=(const MainTimer&) = delete;

		uint64_t last_tick_us = 0;
	};

	MainTimer& getMainTimer();
	void updataTimer();

	class Timer : public BaseTimer {
	public: Timer(uint32_t us); Timer(); ~Timer();
		[[nodiscard]] uint64_t now_us() const;
		[[nodiscard]] uint64_t now_ms() const;
		[[nodiscard]] double now_s() const;

		void tick();
	private:
		uint32_t cycle_us = 0;

	};
}