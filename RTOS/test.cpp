#pragma once
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>

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
	while (true) {
		RTOS::sleep(1000);
		std::cout << "执行时间 A ：" << timetest.now_ms() << std::endl;
		timetest.reset();
	}
}

void testTask_B() {
	RTOS::Timer timetest;
	while (true) {
		RTOS::sleep(251);
		std::cout << "执行时间 B ：" << timetest.now_ms() << std::endl;
		timetest.reset();
	}
}

void testTask_C() {
	RTOS::Timer timetest;
	while (true) {
		RTOS::sleep(200);
		timetest.reset();
	}
}

void timeDrive() {
	auto last_time_point = std::chrono::high_resolution_clock::now();

	while (true) {
		auto current_time_point = std::chrono::high_resolution_clock::now();
		auto duration = current_time_point - last_time_point;

		uint64_t elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();

		if (elapsed_us > 0) {
			RTOS::getMainTimer().tick(elapsed_us);

			RTOS::updataTimer();

			last_time_point = current_time_point;
		}

		Sleep(1);
	}
}

int main(int, char) {

	ConvertThreadToFiber(nullptr);

	// 开一个线程来模拟时间驱动
	std::thread timeDriveThread(timeDrive);
	timeDriveThread.detach();

	auto& testManager = *RTOS::TaskManager::getInstance();

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