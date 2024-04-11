/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
 volatile uint16_t _msCounter;
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void GPIO_Config(void) {
    // 初始化GPIOC的PIN3和PIN4为输出模式
    GPIO_Init(GPIOC, GPIO_PIN_3 | GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);
// 初始化PC5为浮动输入带中断
    GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_FL_IT); 
     // 初始化GPIOB的PIN4和PIN5为推挽输出模式，初始状态为高电平  
    GPIO_Init(GPIOB, GPIO_PIN_4 | GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
}

void TIM4_Config(void) {
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 124); // 设置定时器频率为125kHz，溢出时间为1ms
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE); // 使能更新中断
    // 注意：这里不再调用 TIM4_Cmd(ENABLE); 来启动定时器
}
void GPIO_EXTI_Config(void) {
    EXTI_DeInit(); // 重置外部中断到默认状态
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_LOW); // 设置PC5中断触发条件为下降沿和低电平
    enableInterrupts(); // 全局使能中断
}

// 新增一个函数用于启动定时器
void TIM4_Start(void) {
    TIM4_SetCounter(0); // 重置定时器计数器
    TIM4_Cmd(ENABLE); // 启动定时器
}
// 假设TIM2用于延时，初始化TIM2
void TIM2_ConfigForDelay(void) {
     TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_128, 124); // 预分频128，计数至124
    TIM2_ClearFlag(TIM2_FLAG_UPDATE);
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
}

void Delay_ms(uint16_t ms) {
    _msCounter = 0;
    TIM2_SetCounter(0); // 重置计数器
    TIM2_Cmd(ENABLE); // 启动定时器

    while (_msCounter < ms); // 等待直到达到指定的毫秒数

    TIM2_Cmd(DISABLE); // 停止定时器
}
void Enter_LowPowerMode(void) {
    /* 配置低功耗模式前的准备 */
    // 可以在这里配置外设进入低功耗模式，如关闭未使用的外设时钟

    /* 进入低功耗模式 */
    halt();  // 进入halt模式
}
void main(void)
{
 disableInterrupts();
    GPIO_Config(); // 配置GPIO
    TIM4_Config(); // 配置定时器，但不启动
    TIM2_ConfigForDelay(); // 配置定时器
    enableInterrupts(); // 使能全局中断
Enter_LowPowerMode(); // 进入低功耗模式，等待外部中断唤醒
    while (1) {
        //halt(); // 进入低功耗模式，等待外部中断唤醒
       
         GPIO_WriteReverse(GPIOB, GPIO_PIN_4); // 反转PB4状态
        GPIO_WriteReverse(GPIOB, GPIO_PIN_5); // 反转PB5状态
        Delay_ms(50); // 假设的延时函数，需要根据实际的时钟频率来调整这个值以达到200ms的延时
    }
  
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
    
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
