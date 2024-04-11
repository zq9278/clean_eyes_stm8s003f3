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
    // ��ʼ��GPIOC��PIN3��PIN4Ϊ���ģʽ
    GPIO_Init(GPIOC, GPIO_PIN_3 | GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);
// ��ʼ��PC5Ϊ����������ж�
    GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_FL_IT); 
     // ��ʼ��GPIOB��PIN4��PIN5Ϊ�������ģʽ����ʼ״̬Ϊ�ߵ�ƽ  
    GPIO_Init(GPIOB, GPIO_PIN_4 | GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
}

void TIM4_Config(void) {
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 124); // ���ö�ʱ��Ƶ��Ϊ125kHz�����ʱ��Ϊ1ms
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE); // ʹ�ܸ����ж�
    // ע�⣺���ﲻ�ٵ��� TIM4_Cmd(ENABLE); ��������ʱ��
}
void GPIO_EXTI_Config(void) {
    EXTI_DeInit(); // �����ⲿ�жϵ�Ĭ��״̬
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_FALL_LOW); // ����PC5�жϴ�������Ϊ�½��غ͵͵�ƽ
    enableInterrupts(); // ȫ��ʹ���ж�
}

// ����һ����������������ʱ��
void TIM4_Start(void) {
    TIM4_SetCounter(0); // ���ö�ʱ��������
    TIM4_Cmd(ENABLE); // ������ʱ��
}
// ����TIM2������ʱ����ʼ��TIM2
void TIM2_ConfigForDelay(void) {
     TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_128, 124); // Ԥ��Ƶ128��������124
    TIM2_ClearFlag(TIM2_FLAG_UPDATE);
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
}

void Delay_ms(uint16_t ms) {
    _msCounter = 0;
    TIM2_SetCounter(0); // ���ü�����
    TIM2_Cmd(ENABLE); // ������ʱ��

    while (_msCounter < ms); // �ȴ�ֱ���ﵽָ���ĺ�����

    TIM2_Cmd(DISABLE); // ֹͣ��ʱ��
}
void Enter_LowPowerMode(void) {
    /* ���õ͹���ģʽǰ��׼�� */
    // ���������������������͹���ģʽ����ر�δʹ�õ�����ʱ��

    /* ����͹���ģʽ */
    halt();  // ����haltģʽ
}
void main(void)
{
 disableInterrupts();
    GPIO_Config(); // ����GPIO
    TIM4_Config(); // ���ö�ʱ������������
    TIM2_ConfigForDelay(); // ���ö�ʱ��
    enableInterrupts(); // ʹ��ȫ���ж�
Enter_LowPowerMode(); // ����͹���ģʽ���ȴ��ⲿ�жϻ���
    while (1) {
        //halt(); // ����͹���ģʽ���ȴ��ⲿ�жϻ���
       
         GPIO_WriteReverse(GPIOB, GPIO_PIN_4); // ��תPB4״̬
        GPIO_WriteReverse(GPIOB, GPIO_PIN_5); // ��תPB5״̬
        Delay_ms(50); // �������ʱ��������Ҫ����ʵ�ʵ�ʱ��Ƶ�����������ֵ�Դﵽ200ms����ʱ
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
