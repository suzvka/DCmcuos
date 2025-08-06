// 内部使用的硬件接口

#ifndef HANDLE_H
#define HANDLE_H

#include "API.h"
#include "CallbackManager.h"

static Uint8Callback setinputPU_Pin;
static Uint8Callback setinputPD_Pin;
static Uint8Callback setinputF_Pin;
static Uint8Callback setinputA_Pin;

static Uint8Callback setoutputPP_Pin;
static Uint8Callback setoutputOD_Pin;
static Uint8Callback setoutputPPA_Pin;
static Uint8Callback setoutputODA_Pin;

static Uint8Callback setI2C_SDA;
static Uint8Callback setI2C_SCL;

static Uint8Callback setSWD_DIO;
static Uint8Callback setSWD_CLK;

static TaskAgrntCallback triggerSWI;

static ProcessCallback OFF_Interrupts;
static ProcessCallback ON_Interrupts;

bool addInputPin(int pin, int mode);
bool addOutputPin(int pin, int mode);


#endif