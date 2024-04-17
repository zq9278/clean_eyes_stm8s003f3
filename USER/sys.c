
#include "stm8s.h"
#include "sys.h"
volatile uint16_t _msCounter;
void Delay_ms(uint16_t ms)
{
    _msCounter = 0;
    TIM2_SetCounter(0); // 重置计数器
    TIM2_Cmd(ENABLE);   // 启动定时器

    while (_msCounter < ms)
        ; // 等待直到达到指定的毫秒数

    TIM2_Cmd(DISABLE); // 停止定时器
}

void delay_us(uint32_t us) {
    /* 一个大致的延时函数，具体的常数因子需要根据时钟频率和编译器优化调整 */
    volatile uint32_t nCount;
    for (; us != 0; us--)
    {
        nop();
        nop();
        nop();
        nop();
        nop();
    }
}

void delay_ms(uint32_t ms) {
    /* 一个大致的延时函数，具体的常数因子需要根据时钟频率和编译器优化调整 */
    volatile uint32_t nCount;
    for (; ms != 0; ms--)
        delay_us(1000);
}