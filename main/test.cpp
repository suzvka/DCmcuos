#include <etl/vector.h>
#include <etl/string.h>
#include <stdio.h>

#include "API.h"
#include "HAL.h"


static void test(uint8_t a){
	// 模拟输入引脚配置
	// 打印输入参数
	printf("Configuring input pull-up pin: %d\n", a);
}


int main(int, char) {

	__inputPU_Pin(test);



	return 0;
}