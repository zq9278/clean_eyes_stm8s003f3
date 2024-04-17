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
volatile uint8_t work_flag = 0; // ʹ��volatile�ؼ���ȷ�������������Ż�����������Ķ�д����
uint8_t work_start_once = 0;    // ���ڿ���һ���Բ���
uint8_t work_end_once = 0;      // ���ڿ���һ���Բ���
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void main(void)
{
    CLK_DeInit(); // ��ʼ��ʱ��ϵͳ
    CLK_HSICmd(ENABLE);
    while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == FALSE)
        ;
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
    disableInterrupts(); // �ر��ж�
    ADC_Init();          // ��ʼ��ADC
    IWDG_Config();       // ��ʼ�����Ź�
    GPIO_Config();       // ����GPIO
    GPIO_EXTI_Config();
    TIM4_Config(); // ���ö�ʱ������������
    TIM1_Configuration();
    TIM2_ConfigForDelay(); // ���ö�ʱ��
    enableInterrupts();    // ʹ��ȫ���ж�
    halt();                // ����haltģʽ
    while (1)
    {
        if (work_flag == 1 && work_start_once == 0) // �����ʼ��һ��ûִ��
        {
            work_start_once = 1;               // �����ִ��һ���Բ���
            GPIO_WriteHigh(GPIOC, GPIO_PIN_3); // ����PC3Ϊ�ߵ�ƽ
            GPIO_WriteLow(GPIOC, GPIO_PIN_4);  // ����PC4Ϊ�͵�ƽ
            TIM4_Start();                      // ������ʱ����ʼ����ʱ
            ADC1_Cmd(ENABLE);
            GPIO_WriteHigh(GPIOD, GPIO_PIN_2); // �������
        }
        if (work_flag == 0 && work_end_once == 0) // ����ر�
        {
            work_end_once = 1;
            GPIO_WriteLow(GPIOC, GPIO_PIN_3);  // �رյ��
            GPIO_WriteLow(GPIOD, GPIO_PIN_2);  // �رյ��
            GPIO_WriteHigh(GPIOB, GPIO_PIN_4); // �ر�led0
            GPIO_WriteHigh(GPIOB, GPIO_PIN_5); // �ر�led1

            // Enter_LowPowerMode(); // ����͹���ģʽ���ȴ��ⲿ�жϻ���
        }
        if (work_flag == 1)
        {
            adc_value = Read_ADC();                // ��ȡADCֵ
            voltage = (float)adc_value * 5 / 1024; // ����ʵ�ʵ�ѹֵ������Vref=3.3V��10λADC
            voltage = (float)adc_value * 3.3 * 4 / 3 / 1024;
            GPIO_WriteReverse(GPIOB, GPIO_PIN_5); // ��תPB4״̬
        }
        Delay_ms(50); // �������ʱ��������Ҫ����ʵ�ʵ�ʱ��Ƶ�����������ֵ�Դﵽ200ms����ʱ
        IWDG_ReloadCounter();
        // GPIO_WriteLow(GPIOB, GPIO_PIN_4); // ��תled0
        // Delay_ms(1000); // �������ʱ��������Ҫ����ʵ�ʵ�ʱ��Ƶ�����������ֵ�Դﵽ200ms����ʱ
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
