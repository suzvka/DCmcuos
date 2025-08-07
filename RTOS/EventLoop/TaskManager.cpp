#include "TaskManager.h"
#include "API.h"

namespace RTOS{
static TaskManager g_task_manager;

void (*TaskManager::_setupContext)(UserTask::Context* ctx, void* stack_top, void* entry_point) = nullptr;
void (*TaskManager::_switchContext)(void** from_ctx_sp_ptr, const void* to_ctx) = nullptr;

void RTOS::TaskManager::setSetupContext(void (*func)(UserTask::Context* ctx, void* stack_top, void* entry_point)) {
	_setupContext = func;
}

void TaskManager::setSwitchContext(void(*func)(void** from_ctx_sp_ptr, const void* to_ctx)){
	_switchContext = func;
}

TaskManager* TaskManager::getInstance(){
	return &g_task_manager;
}

bool TaskManager::addTask(UserTask&& task) {
	if (_taskCount >= MAX_TASKS) return false;

	InterruptLock lock;
	_userTaskPool.emplace_back(std::move(task));
	_userTaskPool.back().start();

	_taskCount++;
	return true;
}

bool TaskManager::addTask(ProcessCallback&& task) {
	return addTask(UserTask(task));
}

bool TaskManager::removeTask(size_t task_id){
	if (task_id >= _taskCount) return false;

	InterruptLock lock;
	_userTaskPool.erase(_userTaskPool.begin() + task_id);
	_taskCount--;

	return true;
}

void TaskManager::run(){
	if (_taskCount == 0) return;
	runStatus = RunStatus::Running;

	while (runStatus == RunStatus::Running) {
		checkSleepingTasks();

		size_t next_task_idx = findNextReadyTask();
		if (next_task_idx < _taskCount) {
			_nowTaskID = next_task_idx;

			UserTask& current = _userTaskPool[_nowTaskID];

			_switchContext(reinterpret_cast<void**>(&_schedulerContext.stack_top), &current.getContext());
		}
		else {
			// 没有就绪任务，可以进入低功耗模式
			// WFI(); // 例如：等待中断
		}
	}
}

void RTOS::TaskManager::setupContext(UserTask::Context* ctx, void* stack_top) {
	if (_setupContext) {
		_setupContext(ctx, stack_top, (void*)task_entry_trampoline);
	}
}

void TaskManager::yield(bool is_sleeping, uint32_t wake_up_time){
	Task* current = getCurrentTask();
	if (runStatus != RunStatus::Running || !current) return;

	if (is_sleeping) {
		current->_state = Task::State::SLEEPING;
		current->_wake_up_time_ms = __timer.getCurrentTime_ms() + static_cast<uint64_t>(wake_up_time);
	}

	if (static_cast<UserTask*>(current)) {
		auto* task = static_cast<UserTask*>(current);
		_switchContext(reinterpret_cast<void**>(&task->getContext().stack_top), &_schedulerContext);
	}
}

UserTask* TaskManager::getCurrentTask(){
	return (_nowTaskID < _taskCount) ? &_userTaskPool[_nowTaskID] : nullptr;
}

void TaskManager::checkSleepingTasks(){
	uint64_t now = __timer.getCurrentTime_ms();
	for (size_t i = 0; i < _taskCount; ++i) {
		if (_userTaskPool[i]._state == Task::State::SLEEPING && now >= _userTaskPool[i]._wake_up_time_ms) {
			InterruptLock lock;
			_userTaskPool[i]._state = Task::State::READY;
		}
	}
}

size_t TaskManager::findNextReadyTask(){
	for (size_t i = 0; i < _taskCount; ++i) {
		size_t idx = (_last_scheduled_id + 1 + i) % _taskCount;
		if (_userTaskPool[idx]._state == Task::State::READY) {
			_last_scheduled_id = idx;
			return idx;
		}
	}
	return SIZE_MAX;
}

void TaskManager::task_entry_trampoline() {
	UserTask& current = *g_task_manager.getCurrentTask();
	current.run(); // 执行用户任务
	current._state = Task::State::FINISHED;

	// 任务结束，永久休眠并让出CPU
	g_task_manager.yield(true, UINT32_MAX);
}

void UserTask::start(){
	void* stack_top_addr = _context.stack_data + TASK_STACK_SIZE;
	g_task_manager.setupContext(&_context, stack_top_addr);

	_state = State::READY;
}



}

void init() {
	auto& i = *RTOS::TaskManager::getInstance();
}