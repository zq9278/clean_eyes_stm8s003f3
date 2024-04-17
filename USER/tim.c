#include "tim.h"
void TIM4_Config(void)
{
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 124); // 设置定时器频率为125kHz，溢出时间为1ms
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE); // 使能更新中断
    // 注意：这里不再调用 TIM4_Cmd(ENABLE); 来启动定时器
}

// 新增一个函数用于启动定时器
void TIM4_Start(void)
{
    TIM4_SetCounter(0); // 重置定时器计数器
    TIM4_Cmd(ENABLE);   // 启动定时器
}
void TIM1_Configuration(void)
{ //
    // 以16MHz的系统时钟为例，计算TIM1的预分频和自动重载值产生50ms的中断
    // 这里的值可能需要根据实际情况进行调整
    TIM1_TimeBaseInit(16000 - 1, TIM1_COUNTERMODE_UP, 500 - 1, 0);
    TIM1_ClearFlag(TIM1_FLAG_UPDATE);      // 清除更新中断标志
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE); // 使能更新中断
    TIM1_Cmd(DISABLE);                     // 初始化时不启动定时器
}
// 假设TIM2用于延时，初始化TIM2
void TIM2_ConfigForDelay(void)
{
    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_128, 124); // 预分频128，计数至124
    TIM2_ClearFlag(TIM2_FLAG_UPDATE);
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
}
/**
 * @brief 初始化看门狗
 *
 * @details
 *      IWDG将用于检测系统是否响应，这里将初始化IWDG
 *
 */
void IWDG_Config(void)
{
    // 使能IWDG
    IWDG_Enable();
    // 允许访问IWDG_PR和IWDG_RLR寄存器
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    // 设置IWDG时钟预分频器为256，即IWDG的时钟为LSI/256
    IWDG_SetPrescaler(IWDG_Prescaler_256);//128000/256=50000
    // 设置IWDG重装载值为0xFF（最大值），即IWDG在计数至0xFF后产生中断
    // 根据实际需要调整这个值
    IWDG_SetReload(0xFF);//设置255，f=1/500（震荡频率），255*（1/500）=0.5s）
}