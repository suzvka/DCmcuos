#include "Timer.h"
namespace RTOS {
	// 初始化全局定时器
	volatile uint32_t GlobalTimer::Timer_us = 0;
	volatile uint32_t GlobalTimer::TimerOverflowCount = 0;
}