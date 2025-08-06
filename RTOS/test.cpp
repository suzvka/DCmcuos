#pragma once
#include <chrono>
#include <thread>

#include "Loop.h"

int main(int, char) {
	RTOS::TaskManager::getInstance()->run();

	while (true) {
		// —” ±10ms
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
	}

	return 0;
}