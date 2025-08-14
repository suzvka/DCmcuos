#include <stdint.h>

// ARM Cortex-M0

/**
 * @brief 初始化任务的上下文
 * @param ctx_sp_ptr        用于存储初始化后栈顶指针的地址
 * @param initial_stack_top 任务栈的最高地址（初始栈顶）
 * @param entry_point       任务的入口函数指针
 */
void setup_context(void** ctx_sp_ptr, void* initial_stack_top, void* entry_point) {
    // 将栈顶指针转换为 uint32_t 指针以便于操作
    uint32_t* stack_ptr = (uint32_t*)initial_stack_top;

    // 任务栈向下生长，首先将指针向下移动以对齐
    // ARM Cortex-M 要求栈指针在异常进入/退出时是 8 字节对齐的
    stack_ptr = (uint32_t*)((uint32_t)stack_ptr & 0xFFFFFFF8);

    // 伪造一个 ARM Cortex-M 的异常栈帧 (Exception Stack Frame)
    // 寄存器入栈顺序是固定的，从高地址到低地址：
    // xPSR, PC, LR, R12, R3, R2, R1, R0 (由硬件自动处理)
    // R11, R10, R9, R8, R7, R6, R5, R4 (由软件保存)

    // 1. 硬件自动保存的寄存器
    *(--stack_ptr) = 0x01000000;  // xPSR: T-bit 置 1，表示 Thumb 状态
    *(--stack_ptr) = (uint32_t)entry_point; // PC: 程序计数器，指向任务入口
    *(--stack_ptr) = 0xFFFFFFFF;  // LR: 链接寄存器，可设为任务结束哨兵值
    *(--stack_ptr) = 0x12121212;  // R12
    *(--stack_ptr) = 0x03030303;  // R3
    *(--stack_ptr) = 0x02020202;  // R2
    *(--stack_ptr) = 0x01010101;  // R1
    *(--stack_ptr) = 0x00000000;  // R0: 第一个参数，可以为 0

    // 2. 软件需要保存的寄存器
    *(--stack_ptr) = 0x11111111;  // R11
    *(--stack_ptr) = 0x10101010;  // R10
    *(--stack_ptr) = 0x09090909;  // R9
    *(--stack_ptr) = 0x08080808;  // R8
    *(--stack_ptr) = 0x07070707;  // R7
    *(--stack_ptr) = 0x06060606;  // R6
    *(--stack_ptr) = 0x05050505;  // R5
    *(--stack_ptr) = 0x04040404;  // R4

    // 3. 保存最终计算出的新栈顶指针
    *ctx_sp_ptr = stack_ptr;
}

/**
 * @brief 切换任务上下文 (Keil ARMCC v6 / Clang 版本)
 * @note  此函数是 "naked" 函数，因为它会手动操作栈和寄存器。
 *        它通常在 PendSV_Handler 中被调用。
 * @param from_ctx_sp_ptr (传入 R0) 保存当前任务栈顶指针的地址
 * @param to_ctx_sp       (传入 R1) 要切换到的任务的栈顶指针
 */
__attribute__((naked))
void switch_context(void** from_ctx_sp_ptr, const void* to_ctx_sp)
{
    __asm(
    // R0 = from_ctx_sp_ptr, R1 = to_ctx_sp

    // 1. 获取当前任务的进程栈指针 (PSP)
    "MRS     R2, PSP \n\t"

        // 2. 保存当前任务的软件上下文 (R4-R11)
        "SUB     R2, R2, #32 \n\t"
        "STR     R4, [R2, #0] \n\t"
        "STR     R5, [R2, #4] \n\t"
        "STR     R6, [R2, #8] \n\t"
        "STR     R7, [R2, #12] \n\t"
        "STR     R8, [R2, #16] \n\t"
        "STR     R9, [R2, #20] \n\t"
        "STR     R10, [R2, #24] \n\t"
        "STR     R11, [R2, #28] \n\t"

        // 3. 保存当前任务更新后的栈顶指针
        "STR     R2, [R0] \n\t"

        // 4. 加载新任务的栈顶指针
        "MOV     R2, R1 \n\t"

        // 5. 恢复新任务的软件上下文 (R4-R11)
        "LDR     R4, [R2, #0] \n\t"
        "LDR     R5, [R2, #4] \n\t"
        "LDR     R6, [R2, #8] \n\t"
        "LDR     R7, [R2, #12] \n\t"
        "LDR     R8, [R2, #16] \n\t"
        "LDR     R9, [R2, #20] \n\t"
        "LDR     R10, [R2, #24] \n\t"
        "LDR     R11, [R2, #28] \n\t"
        "ADD     R2, R2, #32 \n\t"

        // 6. 将新任务的栈顶指针写入 PSP
        "MSR     PSP, R2 \n\t"

        // 7. 函数返回
        "BX      LR \n\t"
        );
}