///===========================================================================
// DCmcuos HAL - 硬件抽象层
//-------------------------------------------------------------
// 2025.8.8
// v 0.0.1
//-------------------------------------------------------------
// 按照需求实现这些函数
// 通过这些接口将函数指针传入框架
//
//=====================================================================

#ifndef HAL_H
#define HAL_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

//=========================================================
// 引脚配置
// 需要注册一个新的引脚时，框架会调用这里传入的函数
// 回调格式：
// @param pin 引脚编号

// 配置上拉输入引脚
// @param uint8 pin 引脚编号
void __inputPU_Pin(void(*callback)(uint8_t));

// 配置下拉输入引脚
// @param uint8 pin 引脚编号
void __inputPD_Pin(void(*callback)(uint8_t));

// 配置浮空输入引脚
// @param uint8 pin 引脚编号
void __inputF_Pin(void(*callback)(uint8_t));

// 配置模拟输入引脚
// @param uint8 pin 引脚编号
void __inputA_Pin(void(*callback)(uint8_t));

// 配置推挽输出引脚
// @param uint8 pin 引脚编号
void __outputPP_Pin(void(*callback)(uint8_t));

// 配置开漏输出引脚
// @param uint8 pin 引脚编号
void outputOD_Pin(void(*callback)(uint8_t));

// 配置复用推挽输出引脚
// @param uint8 pin 引脚编号
void __outputPPA_Pin(void(*callback)(uint8_t));

// 配置复用开漏输出引脚
// @param uint8 pin 引脚编号
void __outputODA_Pin(void(*callback)(uint8_t));

// 配置 I2C
// @param uint8 SDA 引脚编号
void __I2C_SDA(void(*callback)(uint8_t));

// @param uint8 SCL 引脚编号
void __I2C_SCL(void(*callback)(uint8_t));

//=========================================================
// 系统配置

// 设置主频
// @param uint32 freq 主频 kHz
void __setSystemClock(void(*callback)(uint8_t));

// 软件中断回调
// 框架需要软件中断时调用这个函数
// 框架会注入一个 void foo() 来执行自定义过程
void __SWI(void(*callback)(void(*)()));

// 自动周期定时器
// 把这个放到相应定时器中断里
// @param uint32 us 定时器周期
void __tick(uint32_t us);

// 手动递增定时器
// 获取两次调用的间隔
// @return uint32_t 间隔 us
void __time(uint32_t(*callback)(void));

// 看门狗回调
void __watchdog(void(*callback)(void));

// 报错回调
void __errorCallback(void(*callback)(const char*));

// 禁用中断
void __disableInterrupts(void(*callback)(void));

// 启用中断
void __enableInterrupts(void(*callback)(void));

// 注册任务上下文初始化函数
// @param callback
//   - arg1: void** ctx_sp_ptr, 用于保存初始化后栈顶指针的地址
//   - arg2: void* initial_stack_top, 任务栈的最高地址（初始栈顶）
//   - arg3: void* entry_point, 任务入口函数指针
void __setupContext(void(*callback)(void**, void*, void*));

// 注册任务上下文切换函数
// @param callback
//   - arg1: void** from_ctx_sp_ptr, 保存当前任务栈顶指针的地址
//   - arg2: const void* to_ctx_sp, 要切换到的任务的栈顶指针
void __switchContext(void(*callback)(void**, const void*));

#ifdef __cplusplus
}
#endif
#endif