#include "stm8s.h"
#include "gpio.h"

void GPIO_Config(void)
{
    // 初始化GPIOC的PIN3和PIN4为输出模式
    GPIO_Init(GPIOC, GPIO_PIN_3 | GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);
    // 初始化PC5为浮动输入带中断
    GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_PU_IT);
    // 初始化GPIOB的PIN4和PIN5为推挽输出模式，初始状态为高电平
    GPIO_Init(GPIOB, GPIO_PIN_4 | GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);
}
void GPIO_EXTI_Config(void)
{
    EXTI_DeInit();                                                         // 重置外部中断到默认状态
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_ONLY); // 设置PC5中断触发条件为下降沿
    enableInterrupts();                                                    // 全局使能中断
}