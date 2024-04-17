#include "stm8s.h"
#include "stm8s_iwdg.h"
void TIM4_Config(void);


/**
 * @brief 启动定时器4
 *
 * @details
 *      TIM4将用于游戏任务的计时，这里将启动定时器4
 *
 */
void TIM4_Start(void); // 新增一个函数用于启动定时器
void TIM1_Configuration(void) ;
/**
 * @brief Initialize TIM2 for delay
 *
 * @details
 *      TIM2 will be used for delay,
 *      this function initializes TIM2,
 *      sets it to auto-reload mode and
 *      starts counting from 0x0000
 *
 */
void TIM2_ConfigForDelay(void);

void IWDG_Config(void);

