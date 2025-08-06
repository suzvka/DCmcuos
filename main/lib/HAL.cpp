#include "API.h"
#include "Handle.h"

void inputPU_Pin(void(*callback)(uint8_t)) {
	setinputPU_Pin = callback;
}

void inputPD_Pin(void(*callback)(uint8_t)) {
	setinputPD_Pin = callback;
}

void inputF_Pin(void(*callback)(uint8_t)) {
	setinputF_Pin = callback;
}

void inputA_Pin(void(*callback)(uint8_t)) {
	setinputA_Pin = callback;
}

void outputPP_Pin(void(*callback)(uint8_t)) {
	setoutputPP_Pin = callback;
}

void outputOD_Pin(void(*callback)(uint8_t)) {
	setoutputOD_Pin = callback;
}

void outputPPA_Pin(void(*callback)(uint8_t)) {
	setoutputPPA_Pin = callback;
}

void outputODA_Pin(void(*callback)(uint8_t)) {
	setoutputODA_Pin = callback;
}

void I2C_SDA(void(*callback)(uint8_t)) {
	setI2C_SDA = callback;
}

void I2C_SCL(void(*callback)(uint8_t)) {
	setI2C_SCL = callback;
}

void SWD_DIO(void(*callback)(uint8_t)) {
	setSWD_DIO = callback;
}

void SWD_CLK(void(*callback)(uint8_t)) {
	setSWD_CLK = callback;
}

void SWI(void(*callback)(void(*)())) {
	triggerSWI = callback;
}

void disableInterrupts(void(*callback)()) {
	OFF_Interrupts = callback;
}

void enableInterrupts(void(*callback)()) {
	ON_Interrupts = callback;
} 