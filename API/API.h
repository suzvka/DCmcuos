#ifndef API_H
#define API_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>



void StartRun();
void sleep(uint32_t ms);
uint64_t RunningFreq();

//=========================================================
// 任务管理

// 添加任务
void AddTask(void(*task)(), uint16_t run_interval_ms = 0);




#ifdef __cplusplus
}
#endif
#endif