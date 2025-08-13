#pragma once
#include "CallbackManager.h"
#include "Timer.h"

#include <cstdint>

namespace RTOS {
	class TaskBase {
	public:
		enum class State : uint8_t {
			READY,
			SLEEPING,
			FINISHED,
			UNINITIALIZED
		};

		virtual ~TaskBase() = default;
		virtual void run() = 0;

		State _state = State::UNINITIALIZED;
		uint32_t active_ms = 0;
	};

	template <typename Derived>
	class Task : public TaskBase {
	public:
		Task(ProcessCallback task_func) : _task_func(task_func) {}
		Task(void(*task_func)()) : _task_func(task_func) {}

		void start() {
			static_cast<Derived*>(this)->start_impl();
		}

		void run() override {
			static_cast<Derived*>(this)->run_impl();
		}

		ProcessCallback _task_func;
	};

	// 用户任务
	class UserTask : public Task<UserTask> {
	public:
		UserTask(ProcessCallback task_func);

		typedef struct {
			void* stack_top;
			uint8_t stack_data[TASK_STACK_SIZE];
		} Context;

		void start_impl();

		void run_impl();

		Context& getContext() { return _context; }

		uint64_t getMaxStackDepth() const;

		Context _context;
	};

	// 内核任务
	class KernelTask : public Task<KernelTask> {
	public:
		KernelTask(ProcessCallback task_func) : Task<KernelTask>(task_func) {}

		void start_impl() {
			_state = State::READY;
		}

		void run_impl() {
			if (_state == State::READY && _task_func.has()) {
				_task_func();
			}
		}
	};
}