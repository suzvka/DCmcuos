#ifndef API_H
#define API_H
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
void inputPU_Pin(void(*callback)(uint8_t));
// 配置下拉输入引脚
// @param uint8 pin 引脚编号
void inputPD_Pin(void(*callback)(uint8_t));
// 配置浮空输入引脚
// @param uint8 pin 引脚编号
void inputF_Pin(void(*callback)(uint8_t));
// 配置模拟输入引脚
// @param uint8 pin 引脚编号
void inputA_Pin(void(*callback)(uint8_t));

// 配置推挽输出引脚
// @param uint8 pin 引脚编号
void outputPP_Pin(void(*callback)(uint8_t));

// 配置开漏输出引脚
// @param uint8 pin 引脚编号
void outputOD_Pin(void(*callback)(uint8_t));

// 配置复用推挽输出引脚
// @param uint8 pin 引脚编号
void outputPPA_Pin(void(*callback)(uint8_t));

// 配置复用开漏输出引脚
// @param uint8 pin 引脚编号
void outputODA_Pin(void(*callback)(uint8_t));


// 配置 I2C
// @param uint8 SDA 引脚编号
void I2C_SDA(void(*callback)(uint8_t));
// @param uint8 SCL 引脚编号
void I2C_SCL(void(*callback)(uint8_t));

// 配置 SWD
// @param uint8 SWDIO 引脚编号
void SWD_DIO(void(*callback)(uint8_t));
// @param uint8 SWCLK 引脚编号
void SWD_CLK(void(*callback)(uint8_t));

//=========================================================
// 系统配置

// 软件中断回调
// 框架需要软件中断时调用这个函数
// 框架会注入一个 void foo() 来执行自定义过程
void __SWI(void(*callback)(void(*)()));

// 定时器
// 把这个放到相应定时器中
// @param uint32 us 定时器周期
void __timer(uint32_t us);

// 看门狗回调
void __watchdog(void(*callback)(void));

// 报错回调
void __errorCallback(void(*callback)(const char*));

// 禁用中断
void __disableInterrupts(void(*callback)(void));

// 启用中断
void __enableInterrupts(void(*callback)(void));

void init();

//=========================================================
// 任务管理

// 添加任务
void addTask(void(*task)());




#ifdef __cplusplus
}
#endif
#endif