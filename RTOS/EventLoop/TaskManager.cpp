#include "TaskManager.h"
#include "KernelTask.h"

namespace RTOS {
void (*TaskManager::_setupContext)(UserTask::Context* ctx, void* initial_stack_top, void* entry_point) = nullptr;
void (*TaskManager::_switchContext)(void** from_ctx_sp_ptr, const void* to_ctx) = nullptr;
RunStatus runStatus = RunStatus::Stop;

void TaskManager::run() {
	runStatus = RunStatus::Running;

	initKernelTasks();
	initUserTasks();
	// if (!checkInit()) runStatus = RunStatus::Error;

	while (runStatus == RunStatus::Running || runStatus == RunStatus::Paused) {
		checkSleepingTasks();
		runKernelTasks();

		size_t next_task_idx = findNextReadyTask();
		if (next_task_idx < _taskCount && runStatus == RunStatus::Running) {
			_nowTaskID = next_task_idx;

			UserTask& current = _userTaskPool[_nowTaskID];

			InterruptLock lock;
			_switchContext(
				reinterpret_cast<void**>(&_schedulerContext.stack_top),
				&current.getContext()
			);
		}
		else {
			//idle();
			// 没有就绪任务，可以进入低功耗模式
			// WFI(); // 例如：等待中断
		}

		loopEnd();
	}
}

void RTOS::TaskManager::setSetupContext(void (*func)(UserTask::Context* ctx, void* initial_stack_top, void* entry_point)) {
	_setupContext = func;
}

void TaskManager::setSwitchContext(void(*func)(void** from_ctx_sp_ptr, const void* to_ctx)){
	_switchContext = func;
}

TaskManager* TaskManager::getInstance(){
	static TaskManager g_task_manager;
	return &g_task_manager;
}

bool TaskManager::checkInit(){
	return 
		(_setupContext != nullptr) && 
		(_switchContext != nullptr) &&
		(OFF_Interrupts.has()) &&
		(ON_Interrupts.has())
		;
}

bool TaskManager::addTask(UserTask&& task) {
	if (_taskCount >= MAX_TASKS) return false;
	InterruptLock lock;
	_userTaskPool.emplace_back(std::move(task));
	auto& t = _userTaskPool.back();
	t.start();

	void* initial_stack_top_addr = t._context.stack_data + TASK_STACK_SIZE;
	setupContext(&t._context, initial_stack_top_addr);

	_taskCount++;
	return true;
}

bool TaskManager::addTask(KernelTask&& task){
	InterruptLock lock;
	_kernrlTaskPool.emplace_back(std::move(task));
	_kernrlTaskPool.back().start();

	return true;
}

bool TaskManager::addTask(ProcessCallback&& task, uint16_t run_interval_ms) {
	return addTask(UserTask(task, run_interval_ms));
}

bool TaskManager::removeTask(size_t task_id){
	if (task_id >= _taskCount) return false;

	InterruptLock lock;
	_userTaskPool.erase(_userTaskPool.begin() + task_id);
	_taskCount--;

	return true;
}

void RTOS::TaskManager::setupContext(UserTask::Context* ctx, void* initial_stack_top) {
	if (_setupContext) {
		InterruptLock lock;
		_setupContext(
			ctx, 
			initial_stack_top, 
			(void*)task_entry_trampoline
		);
	}
}

void TaskManager::initUserTasks() {
	for (auto& task : _userTaskPool) {
		if (!_setupContext) continue;
		if (task.getContext().stack_top == nullptr) {
			void* initial_stack_top_addr = task._context.stack_data + TASK_STACK_SIZE;
			setupContext(&task._context, initial_stack_top_addr);
		}
	}
}

void TaskManager::loopEnd() {
	_loop_count++;
	uint64_t now = _timer.now_ms();
	uint64_t elapsed = now - _last_freq_calc_time_ms;

	if (elapsed >= 100) {
		_running_freq_hz = _loop_count * 1000 / elapsed;
		_loop_count = 0;
		_last_freq_calc_time_ms = now;
	}
}

uint64_t TaskManager::getRunningFreq() const {
	return _running_freq_hz;
}

void TaskManager::task_entry_trampoline() {
	while (runStatus == RunStatus::Running) {
		UserTask& current = *getInstance()->getCurrentTask();

		current.run();

		getInstance()->yield(current.run_interval_ms > 0, current.run_interval_ms);
	}
}

void TaskManager::yield(bool is_sleeping, uint32_t wake_up_time){
	TaskBase* current = getCurrentTask();
	if (runStatus != RunStatus::Running || !current) return;

	if (is_sleeping) {
		current->_state = TaskBase::State::SLEEPING;
		current->active_ms = _timer.now_ms() + wake_up_time;
	}

    auto* task = static_cast<UserTask*>(current);
    {
        InterruptLock lock;
        _switchContext(
            reinterpret_cast<void**>(&task->getContext().stack_top),
            &_schedulerContext
        );
    }
}

UserTask* TaskManager::getCurrentTask(){
	return (_nowTaskID < _taskCount) ? &_userTaskPool[_nowTaskID] : nullptr;
}

void TaskManager::checkSleepingTasks(){
	uint64_t now = _timer.now_ms();
	for (size_t i = 0; i < _taskCount; ++i) {
		auto& task = _userTaskPool[i];
		if (task._state == TaskBase::State::SLEEPING) {
			if (now >= task.active_ms) {
				task._state = TaskBase::State::READY;
			}
		}
	}	
}

void TaskManager::initKernelTasks(){
	addTask(TimerUpdata());
}

void TaskManager::runKernelTasks(){
	for (auto& task : _kernrlTaskPool) {
		if (task._state == TaskBase::State::READY) {
			task.run();
			task.active_ms += task.run_interval_ms;
		}
		else if (task._state == TaskBase::State::SLEEPING) {
			if (_timer.now_ms() >= task.active_ms) {
				task._state = TaskBase::State::READY;
			}
		}
	}
}

uint64_t TaskManager::findNextReadyTask(){
	for (uint64_t i = 0; i < _taskCount; ++i) {
		uint64_t idx = (_last_scheduled_id + i) % _taskCount;
		if (_userTaskPool[idx]._state == TaskBase::State::READY) {
			_last_scheduled_id = idx;
			return idx;
		}
	}
	return UINT64_MAX;
}

}

