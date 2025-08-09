#ifndef API_H
#define API_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>



void init();

//=========================================================
// 任务管理

// 添加任务
void addTask(void(*task)());




#ifdef __cplusplus
}
#endif
#endif