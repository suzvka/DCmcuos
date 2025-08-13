#pragma once
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>

#include "HAL.h"
#include "API.h"
#include "TaskManager.h"
#include <test.h>
#include <Windows.h>

// 将当前任务的上下文（Fiber句柄）存储在线程局部存储中
static thread_local void* current_task_context = nullptr;

void simulated_setup_context(RTOS::UserTask::Context* ctx, void* stack_top, void* entry_point) {
	ctx->stack_top = CreateFiber(0, (LPFIBER_START_ROUTINE)entry_point, nullptr);
}
void simulated_switch_context(void** from_ctx_sp_ptr, const void* to_ctx) {
	*from_ctx_sp_ptr = GetCurrentFiber();
	const auto* task_context = static_cast<const RTOS::UserTask::Context*>(to_ctx);
	SwitchToFiber(task_context->stack_top);
}


void testTask_A() {
	RTOS::Timer timetest;

		std::cout << "任务 A 开始" << std::endl;
		RTOS::sleep(1000);
		std::cout << "执行时间 A ：" << timetest.now_ms() << std::endl;
		timetest.reset();

}

void testTask_B() {
	RTOS::Timer timetest;

		std::cout << "任务 B 开始" << std::endl;
		RTOS::sleep(251);
		std::cout << "执行时间 B ：" << timetest.now_ms() << std::endl;
		timetest.reset();

}

void testTask_C() {
	RTOS::Timer timetest;

		RTOS::sleep(200);
		timetest.reset();

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
	auto& testManager = *RTOS::TaskManager::getInstance();

	__time(_getTime);

	testManager.setSetupContext(simulated_setup_context);
	testManager.setSwitchContext(simulated_switch_context);

	testManager.addTask((testTask_A));
	testManager.addTask((testTask_B));
	testManager.addTask((testTask_C));

	std::cout << "\n--- Starting Scheduler Simulation ---\n" << std::endl;

	RTOS::sleep(200);

	testManager.run();

	return 0;
}