
#include "adc.h"
uint16_t adc_value;
float voltage;
void ADC_Init(void)
{
    // 重置ADC1的所有寄存器到默认值
    ADC1_DeInit();

    // 初始化ADC1
    // 使用单次转换模式，选择AIN4通道，即PD4
    // 设置ADC时钟为fCPU/2，无外部触发，右对齐数据格式
    // AIN4不使用施密特触发器
    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_5, ADC1_PRESSEL_FCPU_D2,
              ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL4,
              DISABLE);
}
uint16_t Read_ADC(void)
{
    // 开始ADC转换
    ADC1_StartConversion();

    // 等待转换完成
    while (ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET)
        ;

    // 获取并返回ADC转换结果
    return ADC1_GetConversionValue();
}