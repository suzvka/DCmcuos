#include "HAL.h"
#include "Handle.h"
#include "TaskManager.h"

void __inputPU_Pin(void(*callback)(uint8_t)) {
	setinputPU_Pin = callback;
}

void __inputPD_Pin(void(*callback)(uint8_t)) {
	setinputPD_Pin = callback;
}

void __inputF_Pin(void(*callback)(uint8_t)) {
	setinputF_Pin = callback;
}

void __inputA_Pin(void(*callback)(uint8_t)) {
	setinputA_Pin = callback;
}

void __outputPP_Pin(void(*callback)(uint8_t)) {
	setoutputPP_Pin = callback;
}

void __outputOD_Pin(void(*callback)(uint8_t)) {
	setoutputOD_Pin = callback;
}

void __outputPPA_Pin(void(*callback)(uint8_t)) {
	setoutputPPA_Pin = callback;
}

void __outputODA_Pin(void(*callback)(uint8_t)) {
	setoutputODA_Pin = callback;
}

void __I2C_SDA(void(*callback)(uint8_t)) {
	setI2C_SDA = callback;
}

void __I2C_SCL(void(*callback)(uint8_t)) {
	setI2C_SCL = callback;
}

void __setSystemClock(void(*callback)(uint8_t)){
	setSystemClock = callback;
}

void __SWI(void(*callback)(void(*)())) {
	triggerSWI = callback;
}

void __disableInterrupts(void(*callback)()) {
	OFF_Interrupts = callback;
}

void __enableInterrupts(void(*callback)()) {
	ON_Interrupts = callback;
} 

void __time(uint32_t(*callback)(void)) {
	getTime = callback;
}

static void(*g_setupContextCallback)(void**, void*, void*) = nullptr;
void __setupContext(void(*callback)(void**, void*, void*)) {
	auto& task_manager = *RTOS::TaskManager::getInstance();

	// 1. 将用户传入的回调保存到全局变量中
	g_setupContextCallback = callback;

	// 2. 创建一个不捕获任何变量的 lambda 作为包装器
	auto wrapper = [](RTOS::UserTask::Context* ctx, void* initial_stack_top, void* entry_point) {
		if (g_setupContextCallback) {
			// 3. 调用全局回调，并进行类型转换以匹配 HAL 接口
			g_setupContextCallback(
				reinterpret_cast<void**>(&ctx->stack_top), // 从 Context* 提取 void**
				initial_stack_top,
				entry_point
			);
		}
		};

	// 4. 将 lambda 隐式转换为函数指针并注册到任务管理器
	task_manager.setSetupContext(wrapper);
}

static void(*g_switchContextCallback)(void**, const void*) = nullptr;
void __switchContext(void(*callback)(void**, const void*)) {
	auto& task_manager = *RTOS::TaskManager::getInstance();

	g_switchContextCallback = callback;

	auto wrapper = [](void** from_ctx_sp_ptr, const void* to_ctx) {
		if (g_switchContextCallback) {
			// 在 switchContext 中，类型是匹配的，但我们仍然通过包装器调用全局回调
			// 这样做可以保持与 __setupContext 实现模式的一致性
			const auto* task_ctx = static_cast<const RTOS::UserTask::Context*>(to_ctx);
			g_switchContextCallback(from_ctx_sp_ptr, task_ctx->stack_top);
		}
		};

	task_manager.setSwitchContext(wrapper);
}