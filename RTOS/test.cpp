#pragma once
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>

#include "Loop.h"
#include <test.h>
//#include <Windows.h>
//
//// 将当前任务的上下文（Fiber句柄）存储在线程局部存储中
//static thread_local void* current_task_context = nullptr;
//
//void simulated_setup_context(RTOS::UserTask::Context* ctx, void* stack_top, void* entry_point) {
//	// Fiber API 会自行管理栈，因此 stack_top 在此实现中未使用
//	// 创建一个新的 Fiber，并将其句柄直接写入 UserTask 的上下文中
//	ctx->stack_top = CreateFiber(0, (LPFIBER_START_ROUTINE)entry_point, nullptr);
//}
//void simulated_switch_context(void** from_ctx_sp_ptr, const void* to_ctx) {
//	// from_ctx_sp_ptr 是指向存储当前任务上下文指针的指针
//	// to_ctx 是下一个要切换到的任务的上下文
//	*from_ctx_sp_ptr = GetCurrentFiber();
//	// 从 UserTask::Context 结构中提取纤程句柄
//	const auto* task_context = static_cast<const RTOS::UserTask::Context*>(to_ctx);
//	SwitchToFiber(task_context->stack_top);
//}


void testTask_A() {
	RTOS::Timer timetest;
	while (true) {
		RTOS::sleep(1000);
		std::cout << "执行时间 A ：" << timetest.timing_ms() << std::endl;
		timetest.reset();
	}
}

void testTask_B() {
	RTOS::Timer timetest;
	while (true) {
		RTOS::sleep(251);
		std::cout << "执行时间 B ：" << timetest.timing_ms() << std::endl;
		timetest.reset();
	}
}

void testTask_C() {
	RTOS::Timer timetest;
	while (true) {
		RTOS::sleep(200);
		//std::cout << "执行时间 C ：" << timetest.timing_ms() << std::endl;
		timetest.reset();
	}
}

void timeDrive() {
	int count = 0;
	while (true){
		RTOS::GlobalTimer::tick(10000);
		//Sleep(10);

		count++;
	}
}

int main(int, char) {
	//ConvertThreadToFiber(nullptr);

	// 开一个线程来模拟时间驱动
	//std::thread timeDriveThread(timeDrive);
	//timeDriveThread.detach(); // 分离线程，使其在后台运行

	auto& testManager = *RTOS::TaskManager::getInstance();

	//testManager.setSetupContext(simulated_setup_context);
	//testManager.setSwitchContext(simulated_switch_context);

	testManager.addTask((testTask_A));
	testManager.addTask((testTask_B));
	testManager.addTask((testTask_C));

	//std::cout << "\n--- Starting Scheduler Simulation ---\n" << std::endl;

	RTOS::sleep(200);

	testManager.run();

	return 0;
}