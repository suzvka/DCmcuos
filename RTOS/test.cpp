#pragma once
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>

#include "HAL.h"
#include "API.h"
#include <test.h>
#include <Windows.h>

#include "GPIO.h"

// 将当前任务的上下文（Fiber句柄）存储在线程局部存储中
static thread_local void* current_task_context = nullptr;

void simulated_setup_context(void** ctx_sp_ptr, void* initial_stack_top, void* entry_point) {
	(void)initial_stack_top; 
	*ctx_sp_ptr = CreateFiber(0, (LPFIBER_START_ROUTINE)entry_point, nullptr);
}

void simulated_switch_context(void** from_ctx_sp_ptr, const void* to_ctx_sp) {
	// 关键修复：保存当前 Fiber 的句柄到 from_ctx_sp_ptr 指向的位置。
	// 这正是“即时”初始化 _schedulerContext.stack_top 的地方。
	*from_ctx_sp_ptr = GetCurrentFiber();

	// to_ctx_sp 就是目标任务的 Fiber 句柄
	SwitchToFiber((LPVOID)to_ctx_sp);
}


void testTask_A() {
	sleep(1000);
}

void testTask_B() {
	sleep(251);
}

void testTask_C() {
	sleep(500);
	std::cout << "Running Freq: " << RunningFreq() << " Hz" << std::endl;
}

uint32_t last_us = 0;

uint32_t _getTime() {
	uint32_t now_us = static_cast<uint32_t>(
		std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::high_resolution_clock::now().time_since_epoch()
		).count()
		);
	uint32_t out = now_us - last_us;
	last_us = now_us;
	return out;
}

int main(int, char) { ConvertThreadToFiber(nullptr);
	__time(_getTime);

	__setupContext(simulated_setup_context);
	__switchContext(simulated_switch_context);

	AddTask((testTask_A));
	AddTask((testTask_B));
	AddTask((testTask_C));

	std::cout << "\n--- Starting Scheduler Simulation ---\n" << std::endl;

	sleep(200);

	StartRun();

	return 0;
}