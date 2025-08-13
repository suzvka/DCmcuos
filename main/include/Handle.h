// 内部使用的硬件接口

#ifndef HANDLE_H
#define HANDLE_H

#include "API.h"
#include "CallbackManager.h"
extern GetUint32Callback getTime;

extern Uint8Callback setinputPU_Pin;
extern Uint8Callback setinputPD_Pin;
extern Uint8Callback setinputF_Pin;
extern Uint8Callback setinputA_Pin;

extern Uint8Callback setoutputPP_Pin;
extern Uint8Callback setoutputOD_Pin;
extern Uint8Callback setoutputPPA_Pin;
extern Uint8Callback setoutputODA_Pin;

extern Uint8Callback setI2C_SDA;
extern Uint8Callback setI2C_SCL;

extern Uint8Callback setSWD_DIO;
extern Uint8Callback setSWD_CLK;

extern TaskAgrntCallback triggerSWI;

extern ProcessCallback OFF_Interrupts;
extern ProcessCallback ON_Interrupts;

bool addInputPin(int pin, int mode);
bool addOutputPin(int pin, int mode);


#endif