#include "API.h"
#include "TaskManager.h"

void StartRun() {
	auto& task_manager = *RTOS::TaskManager::getInstance();
	task_manager.run();
}

void sleep(uint32_t ms) {
	RTOS::sleep(ms);
}

void AddTask(void(*task)(), uint16_t run_interval_ms) {
	auto& task_manager = *RTOS::TaskManager::getInstance();
	task_manager.addTask(ProcessCallback(task), run_interval_ms);
}

uint64_t RunningFreq(){
	auto& task_manager = *RTOS::TaskManager::getInstance();
	return task_manager.getRunningFreq();
}