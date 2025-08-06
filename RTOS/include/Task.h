#pragma once
#include "CallbackManager.h"
#include "Timer.h"

namespace RTOS {
	class Task {
	public:
		enum class State {
			READY,
			SLEEPING,
			FINISHED,
			UNINITIALIZED
		};

		typedef struct {
			uint8_t stack_data[TASK_STACK_SIZE];
		} Context;

		Task(ProcessCallback task_func) : _task_func(task_func) {}
		virtual ~Task() = default;

		// start() 负责任务的初始化
		virtual void start() = 0;

		// run() 在不同任务类型中有不同含义
		virtual void run() = 0;

		// 获取任务的上下文存储区
		Context& getContext() { return _context; }

		// 公共成员变量，方便 TaskManager 访问
		State _state = State::UNINITIALIZED;
		uint64_t _wake_up_time_us = 0;
		uint64_t _last_run_time_us = 0;
		uint64_t _average_run_time_us = 0;

		ProcessCallback _task_func;

	protected:
		Context _context;
	};

	// 用户任务
	class UserTask : public Task {
	public:
		UserTask(ProcessCallback task_func) : Task(task_func) {}

		// UserTask 的 start() 负责设置上下文
		void start() override;

		// UserTask 的 run() 没有直接作用，因为执行流由上下文切换控制
		void run() override {
			// 这个函数在我们的模型中不被直接调用
			// 切换到此任务是通过 TaskManager::switchToTask 完成的
		}
	};

	// 内核任务
	class KernelTask : public Task {
	public:
		KernelTask(ProcessCallback task_func) : Task(task_func) {}

		// 内核任务的 start() 只是标记为就绪
		void start() override {
			_state = State::READY;
		}

		// 内核任务的 run() 是直接执行函数，无上下文切换
		void run() override {
			if (_state == State::READY && _task_func.has()) {
				_task_func();
			}
		}
	};
}