#pragma once
#include "CallbackManager.h"
#include "Timer.h"

#include <cstdint>

namespace RTOS {
	class Task {
	public:
		enum class State {
			READY,
			SLEEPING,
			FINISHED,
			UNINITIALIZED
		};

		Task(ProcessCallback task_func) : _task_func(task_func) {}
		Task(void(*task_func)()) : _task_func(task_func) {}
		virtual ~Task() = default;

		virtual void start() = 0;
		virtual void run() = 0;

		State _state = State::UNINITIALIZED;
		uint32_t active_cycle = 0;

		ProcessCallback _task_func;
	};

	// 用户任务
	class UserTask : public Task {
	public:
		UserTask(ProcessCallback task_func) : Task(task_func) {
			for (int i = 0; i < TASK_STACK_SIZE - 16; ++i) {
				_context.stack_data[i] = 0xa5;
			}
			for (int i = TASK_STACK_SIZE - 16; i < TASK_STACK_SIZE; ++i) {
				_context.stack_data[i] = 0x5a;
			}

			_context.stack_top = _context.stack_data + TASK_STACK_SIZE;
		}

		typedef struct {
			void* stack_top;
			uint8_t stack_data[TASK_STACK_SIZE];
		} Context;

		void start() override;

		void run() override {
			_task_func();
		}

		Context& getContext() { return _context; }

		uint64_t getMaxStackDepth() const {
			uint64_t out = TASK_STACK_SIZE;

			for (int i = 0; i < TASK_STACK_SIZE - 16; ++i) {
				if (_context.stack_data[i] != 0xa5) {
					out = TASK_STACK_SIZE - i;
					break;
				}
			}

			for (int i = TASK_STACK_SIZE - 16; i < TASK_STACK_SIZE; ++i) {
				if (_context.stack_data[i] != 0x5a) {
					out = TASK_STACK_SIZE - i - 16;
					break;
				}
			}
			
			return out;
		}

		Context _context;
	};

	// 内核任务
	class KernelTask : public Task {
	public:
		KernelTask(ProcessCallback task_func) : Task(task_func) {}

		void start() override {
			_state = State::READY;
		}

		void run() override {
			if (_state == State::READY && _task_func.has()) {
				_task_func();
			}
		}
	};
}