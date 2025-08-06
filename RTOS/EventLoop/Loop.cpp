#include "Loop.h"

namespace RTOS{
static TaskManager g_task_manager;

void (*TaskManager::_setupContext)(void*, void*) = nullptr;
void (*TaskManager::_switchContext)(void*, const void*) = nullptr;

inline void TaskManager::setSetupContext(void(*func)(void* stack_top, void* entry_point)){
	_setupContext = func;
}

void TaskManager::setSwitchContext(void(*func)(void* from_ctx, const void* to_ctx)){
	_switchContext = func;
}

TaskManager* TaskManager::getInstance(){
	return &g_task_manager;
}

bool TaskManager::addTask(Task* task){
	if (_taskCount >= MAX_TASKS || !task) return false;

	InterruptLock lock;
	_taskPool[_taskCount] = task;
	_taskCount++;

	task->start(); // 触发任务自身的初始化
	return true;
}

bool TaskManager::removeTask(size_t task_id){
	if (_taskCount >= MAX_TASKS || task_id >= _taskCount) return false;

	InterruptLock lock;
	_taskPool.erase(_taskPool.begin() + task_id);
	_taskCount--;

	return true;
}

void TaskManager::run(){
	if (_taskCount == 0) return;

	while (runStatus == RunStatus::Running) {
		checkSleepingTasks();

		size_t next_task_idx = findNextReadyTask();
		if (next_task_idx < _taskCount) {
			_nowTaskID = next_task_idx;

			InterruptLock lock;
			Task* current = _taskPool[_nowTaskID];
			lock.unlock();

			uint64_t start_time = Timer::getCurrentTime_us();

			if (dynamic_cast<UserTask*>(current)) {
				_switchContext(&_schedulerContext, &current->getContext());
			}
			else if (dynamic_cast<KernelTask*>(current)) {
				current->run();
			}

			uint64_t end_time = Timer::getCurrentTime_us();
			uint64_t execution_time = end_time - start_time;

			// 更新统计数据
			current->_last_run_time_us = execution_time;
			if (current->_average_run_time_us == 0) {
				current->_average_run_time_us = execution_time;
			}
			else {
				// 使用移动平均法，避免溢出并平滑数据
				current->_average_run_time_us = (current->_average_run_time_us * 7 + execution_time) / 8;
			}
		}
		else {
			// 没有就绪任务，可以进入低功耗模式
			// WFI(); // 例如：等待中断
		}
	}
}

void TaskManager::setupContext(Task::Context* ctx, void* stack_top){
	if (_setupContext) {
		_setupContext(stack_top, reinterpret_cast<void*>(task_entry_trampoline));
	}
}

void TaskManager::yield(bool is_sleeping, uint32_t wake_up_time){
	Task* current = getCurrentTask();
	if (!current) return;

	if (is_sleeping) {
		current->_state = Task::State::SLEEPING;
		current->_wake_up_time_us = wake_up_time * 1000;
	}

	// 切换回调度器
	_switchContext(&current->getContext(), &_schedulerContext);
}

Task* TaskManager::getCurrentTask(){
	return (_nowTaskID < _taskCount) ? _taskPool[_nowTaskID] : nullptr;
}

void TaskManager::checkSleepingTasks(){
	uint64_t now = Timer::getCurrentTime_us();
	for (size_t i = 0; i < _taskCount; ++i) {
		InterruptLock lock;
		if (_taskPool[i]->_state == Task::State::SLEEPING && now >= _taskPool[i]->_wake_up_time_us) {
			_taskPool[i]->_state = Task::State::READY;
		}
	}
}

size_t TaskManager::findNextReadyTask(){
	for (size_t i = 0; i < _taskCount; ++i) {
		InterruptLock lock;
		size_t idx = (_last_scheduled_id + 1 + i) % _taskCount;
		if (_taskPool[idx]->_state == Task::State::READY) {
			_last_scheduled_id = idx;
			return idx;
		}
	}
	return SIZE_MAX; // 没有找到就绪任务
}

void TaskManager::task_entry_trampoline() {
	Task* current = g_task_manager.getCurrentTask();
	if (current) {
		current->_task_func();
	}
	current->_state = Task::State::FINISHED;

	g_task_manager.yield(true, UINT32_MAX);
}

void UserTask::start(){
	// 计算栈顶
	void* stack_top = _context.stack_data + TASK_STACK_SIZE;
	// 请求 TaskManager 帮忙设置上下文
	g_task_manager.setupContext(&_context, stack_top);

	_state = State::READY;
}

}