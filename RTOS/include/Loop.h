#pragma once
#include "Config.h"

#include "Global.h"
#include "Task.h"


#include <cstdint>

namespace RTOS {
	void sleep(uint32_t ms);

	class TaskManager {
	public:
		static TaskManager* getInstance();

		bool addTask(UserTask&& task);

		bool addTask(ProcessCallback&& task);

		bool removeTask(size_t task_id);

		void run();

		void setupContext(UserTask::Context* ctx, void* stack_top);

		void yield(bool is_sleeping, uint32_t wake_up_time = 0);

		UserTask* getCurrentTask();

		static void setSetupContext(void (*func)(UserTask::Context* ctx, void* stack_top, void* entry_point));

		static void setSwitchContext(void(*func)(void** from_ctx_sp_ptr, const void* to_ctx));

	private:
		typedef struct {
			void* stack_top;
			uint8_t stack_data[SYSTEM_STACK_SIZE];
		} Context;

		void checkSleepingTasks();

		size_t findNextReadyTask();

		static void task_entry_trampoline();

		static void (*_setupContext)(UserTask::Context* ctx, void* stack_top, void* entry_point);
		static void (*_switchContext)(void** from_ctx_sp_ptr, const void* to_ctx);

		etl::vector<UserTask, MAX_TASKS> _userTaskPool;

		size_t _taskCount = 0;
		size_t _nowTaskID = 0;
		size_t _last_scheduled_id = -1;
		Context _schedulerContext;
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

	static void sleep(uint32_t ms) {
		TaskManager::getInstance()->yield(true, ms);
	}

	// 这个函数将由定时器中断触发
	// 用户会把这个函数放在与传入参数一致的定时器中断处理函数中
	static void tick(uint32_t us) {
		GlobalTimer::tick(us);
	}
}