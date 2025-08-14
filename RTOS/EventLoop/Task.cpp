#include "Task.h"
#include "TaskManager.h"

namespace RTOS {
	UserTask::UserTask(ProcessCallback task_func, uint16_t run_interval_ms) : Task<UserTask>(task_func, run_interval_ms) {
		for (int i = 0; i < TASK_STACK_SIZE - 16; ++i) {
			_context.stack_data[i] = 0xa5;
		}
		for (int i = TASK_STACK_SIZE - 16; i < TASK_STACK_SIZE; ++i) {
			_context.stack_data[i] = 0x5a;
		}

		_context.stack_top = _context.stack_data + TASK_STACK_SIZE;
	}

	void UserTask::start_impl() {
		_state = State::READY;
	}

	void UserTask::run_impl() {
		_task_func();
	}

	uint64_t UserTask::getMaxStackDepth() const {
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
}