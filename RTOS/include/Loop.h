#pragma once
#include "Config.h"

#include "Global.h"
#include "Task.h"


#include <cstdint>

namespace RTOS {
	class TaskManager {
	public:
		static TaskManager* getInstance();

		bool addTask(Task* task);

		bool removeTask(size_t task_id);

		void run();

		void setupContext(Task::Context* ctx, void* stack_top);

		void yield(bool is_sleeping, uint32_t wake_up_time = 0);

		Task* getCurrentTask();

		static void setSetupContext(void (*func)(void* stack_top, void* entry_point));

		static void setSwitchContext(void (*func)(void* from_ctx, const void* to_ctx));

	private:
		void checkSleepingTasks();

		size_t findNextReadyTask();

		static void task_entry_trampoline();

		static void (*_setupContext)(void* stack_top, void* entry_point);
		static void (*_switchContext)(void* from_ctx, const void* to_ctx);

		etl::vector<Task*, MAX_TASKS> _taskPool;
		size_t _taskCount = 0;
		size_t _nowTaskID = 0;
		size_t _last_scheduled_id = -1;
		Task::Context _schedulerContext;
		Timer timer;
	};

	class InterruptLock {
	public:
		InterruptLock() {
			lock();
		}

		~InterruptLock() {
			unlock();
		}

		void lock() {
			OFF_Interrupts();
		}

		void unlock() {
			ON_Interrupts();
		}
	};

	static void _Sleep111(uint32_t ms) {
		TaskManager::getInstance()->yield(true, ms);
	}

	// 这个函数将由定时器中断触发
	// 用户会把这个函数放在与传入参数一致的定时器中断处理函数中
	static void _Timer111(uint32_t us) {
		GlobalTimer::timer(us);
	}
}