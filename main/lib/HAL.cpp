#include "HAL.h"
#include "Handle.h"

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

void __SWI(void(*callback)(void(*)())) {
	triggerSWI = callback;
}

void __disableInterrupts(void(*callback)()) {
	OFF_Interrupts = callback;
}

void __enableInterrupts(void(*callback)()) {
	ON_Interrupts = callback;
} 