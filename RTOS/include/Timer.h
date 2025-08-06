#pragma once
#include <cstdint>

#include "Handle.h"

namespace RTOS {
	// 单例全局定时器
	class GlobalTimer {
	public:
		static uint64_t timing() noexcept {
			uint32_t overflow_count;
			uint32_t us;

			OFF_Interrupts();
			overflow_count = TimerOverflowCount;
			us = Timer_us;
			ON_Interrupts();

			return (static_cast<uint64_t>(overflow_count) << 32) | us;
		}

		// 这个函数保持不变，由用户的定时器中断调用
		static void timer(uint32_t us) noexcept {
			uint32_t old_us = Timer_us;
			Timer_us += us;
			if (Timer_us < old_us) { // 检测溢出
				TimerOverflowCount++;
			}
		}

	private:
		GlobalTimer() = default;
		GlobalTimer(const GlobalTimer&) = delete;
		GlobalTimer& operator=(const GlobalTimer&) = delete;

		static volatile uint32_t Timer_us;
		static volatile uint32_t TimerOverflowCount;
	};


	// 主要用于提供两次方法调用的时间差
	class Timer {
	public:
		Timer() noexcept {
			reset();
		}

		static uint64_t getCurrentTime_us() noexcept {
			return GlobalTimer::timing();
		}

		static uint64_t getCurrentTime_ms() noexcept {
			constexpr uint64_t us_per_ms = 1000;
			return getCurrentTime_us() / us_per_ms;
		}

		static double getCurrentTime_s() noexcept {
			constexpr double us_per_s = 1000000.0;
			return static_cast<double>(getCurrentTime_us()) / us_per_s;
		}

		// 重置计时器
		void reset() noexcept {
			StartTime_us = getCurrentTime_us();
		}

		[[nodiscard]] uint64_t timing_us() const noexcept {
			return getCurrentTime_us() - StartTime_us;
		}

		[[nodiscard]] uint64_t timing_ms() const noexcept {
			constexpr uint64_t us_per_ms = 1000;
			return timing_us() / us_per_ms;
		}

		[[nodiscard]] double timing_s() const noexcept {
			constexpr double us_per_s = 1000000.0;
			return static_cast<double>(timing_us()) / us_per_s;
		}

	private:
		uint64_t StartTime_us;
	};
}