#include <Handle.h>

GetUint32Callback getTime = nullptr;

Uint8Callback setinputPU_Pin = nullptr;
Uint8Callback setinputPD_Pin = nullptr;
Uint8Callback setinputF_Pin = nullptr;
Uint8Callback setinputA_Pin = nullptr;

Uint8Callback setoutputPP_Pin = nullptr;
Uint8Callback setoutputOD_Pin = nullptr;
Uint8Callback setoutputPPA_Pin = nullptr;
Uint8Callback setoutputODA_Pin = nullptr;

Uint8Callback setI2C_SDA = nullptr;
Uint8Callback setI2C_SCL = nullptr;

Uint8Callback setSWD_DIO = nullptr;
Uint8Callback setSWD_CLK = nullptr;

TaskAgrntCallback triggerSWI = nullptr;

ProcessCallback OFF_Interrupts = nullptr;
ProcessCallback ON_Interrupts = nullptr;

bool addInputPin(int pin, int mode){
	return false;
}

bool addOutputPin(int pin, int mode){
	return false;
}