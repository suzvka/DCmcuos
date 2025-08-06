#pragma once
#include <Config.h>
#include "etl/vector.h"
#include "etl/function.h"

#include "Task.h"

namespace RTOS {
	enum class RunStatus{
		Stop,
		Running,
		Ready,
		Error
	};

	static RunStatus runStatus = RunStatus::Stop;
}