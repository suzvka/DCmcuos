#pragma once
#include <cstdint>
#include <etl/vector.h>

#include "Config.h"
#include "Handle.h"
#include "Task.h"

namespace RTOS {
	enum class RunStatus : uint8_t {
		Stop,
		Running,
		Paused,
		Ready,
		Error
	};

	extern RunStatus runStatus;

	class TaskManager {
	public:
		static TaskManager* getInstance();

		bool checkInit();

		bool addTask(UserTask&& task);

		bool addTask(KernelTask&& task);

		bool addTask(ProcessCallback&& task, uint16_t run_interval_ms = 0);

		bool removeTask(size_t task_id);

		void run();

		void yield(bool is_sleeping, uint32_t wake_up_time = 0);

		uint64_t getRunningFreq() const;

		void idle();

		UserTask* getCurrentTask();

		void setSetupContext(void (*func)(UserTask::Context* ctx, void* stack_top, void* entry_point));

		void setSwitchContext(void(*func)(void** from_ctx_sp_ptr, const void* to_ctx));

	private:
		typedef struct {
			void* stack_top;
			uint8_t stack_data[SYSTEM_STACK_SIZE];
		} Context;

		void checkSleepingTasks();

		void initKernelTasks();

		void runKernelTasks();

		void setupContext(UserTask::Context* ctx, void* stack_top);

		void initUserTasks();

		void loopEnd();

		uint64_t findNextReadyTask();

		static void task_entry_trampoline();

		static void (*_setupContext)(UserTask::Context* ctx, void* stack_top, void* entry_point);
		static void (*_switchContext)(void** from_ctx_sp_ptr, const void* to_ctx);

		etl::vector<UserTask, MAX_TASKS> _userTaskPool;
		etl::vector<KernelTask, 4> _kernrlTaskPool;

		uint64_t _loop_count = 0;
		uint64_t _last_freq_calc_time_ms = 0;
		uint64_t _running_freq_hz = 0;

		uint64_t _taskCount = 0;
		uint64_t _nowTaskID = 0;
		uint64_t _last_scheduled_id = 0;
		Context _schedulerContext;
		Timer _timer;
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
	};
}