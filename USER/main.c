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
#include "sys.h"
#include "stm8s_clk.h"
#include "stm8s_exti.h"
/* Private defines -----------------------------------------------------------*/
extern uint16_t adc_value;
extern float voltage;
volatile uint8_t work_flag = 0; // 使用volatile关键字确保编译器不会优化掉这个变量的读写操作
uint8_t work_start_once = 0;    // 用于控制一次性操作
uint8_t work_end_once = 0;      // 用于控制一次性操作
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void main(void)
{
    CLK_DeInit(); // 初始化时钟系统
    CLK_HSICmd(ENABLE);
    while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == FALSE)
        ;
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
    disableInterrupts(); // 关闭中断
    ADC_Init();          // 初始化ADC
    IWDG_Config();       // 初始化看门狗
    GPIO_Config();       // 配置GPIO
    GPIO_EXTI_Config();
    TIM4_Config(); // 配置定时器，但不启动
    TIM1_Configuration();
    TIM2_ConfigForDelay(); // 配置定时器
    enableInterrupts();    // 使能全局中断
    halt();                // 进入halt模式
    while (1)
    {
        if (work_flag == 1 && work_start_once == 0) // 电机开始，一次没执行
        {
            work_start_once = 1;               // 标记已执行一次性操作
            GPIO_WriteHigh(GPIOC, GPIO_PIN_3); // 设置PC3为高电平
            GPIO_WriteLow(GPIOC, GPIO_PIN_4);  // 设置PC4为低电平
            TIM4_Start();                      // 启动定时器开始倒计时
            ADC1_Cmd(ENABLE);
            GPIO_WriteHigh(GPIOD, GPIO_PIN_2); // 开启电解
        }
        if (work_flag == 0 && work_end_once == 0) // 电机关闭
        {
            work_end_once = 1;
            GPIO_WriteLow(GPIOC, GPIO_PIN_3);  // 关闭电机
            GPIO_WriteLow(GPIOD, GPIO_PIN_2);  // 关闭电解
            GPIO_WriteHigh(GPIOB, GPIO_PIN_4); // 关闭led0
            GPIO_WriteHigh(GPIOB, GPIO_PIN_5); // 关闭led1

            // Enter_LowPowerMode(); // 进入低功耗模式，等待外部中断唤醒
        }
        if (work_flag == 1)
        {
            adc_value = Read_ADC();                // 读取ADC值
            voltage = (float)adc_value * 5 / 1024; // 计算实际电压值，假设Vref=3.3V，10位ADC
            voltage = (float)adc_value * 3.3 * 4 / 3 / 1024;
            GPIO_WriteReverse(GPIOB, GPIO_PIN_5); // 反转PB4状态
        }
        Delay_ms(50); // 假设的延时函数，需要根据实际的时钟频率来调整这个值以达到200ms的延时
        IWDG_ReloadCounter();
        // GPIO_WriteLow(GPIOB, GPIO_PIN_4); // 反转led0
        // Delay_ms(1000); // 假设的延时函数，需要根据实际的时钟频率来调整这个值以达到200ms的延时
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
void assert_failed(u8 *file, u32 line)
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
