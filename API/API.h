#ifndef API_H
#define API_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

// 开始运行
void StartRun();

// 睡眠指定毫秒数
void sleep(uint32_t ms);

// 获取当前运行频率，单位 Hz
uint64_t RunningFreq();

//=========================================================
// 任务管理

// 添加任务
void AddTask(void(*task)(), uint16_t run_interval_ms = 0);

#ifdef __cplusplus
}
#endif
#endif