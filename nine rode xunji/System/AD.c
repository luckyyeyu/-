#include "stm32f10x.h"                  // Device header
uint16_t AD_Value[5];
void AD_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
		GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

	  ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);//规则组的输入通道（哪个adc，哪个通道，跟哪个gpio有关，规则组序列中的次序（pa0一个通道非扫描），采样时间//更快的转换就选更小的参数，更稳定的转换就选跟大的参数）
      ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_55Cycles5);
      ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_55Cycles5);
      ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_55Cycles5);
      ADC_RegularChannelConfig(ADC1,ADC_Channel_4,5,ADC_SampleTime_55Cycles5);

	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode =ADC_Mode_Independent;//adc的工作模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对齐，左对齐还是右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//外部触发转化选择
	ADC_InitStructure.ADC_ContinuousConvMode =ENABLE ;//连续转化还是单次转换
	ADC_InitStructure.ADC_NbrOfChannel = 5;//指定在扫描模式下总共会用到几个通道
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式还是非扫描模式
	ADC_Init(ADC1,&ADC_InitStructure);
	
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_BufferSize = 5;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;
	DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_HalfWord;   
	DMA_InitStructure.DMA_MemoryInc =DMA_MemoryInc_Enable ;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
    ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);//复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);//返回复位校准状态//如果没有校准完成就在空循环里一直等待
	ADC_StartCalibration(ADC1);//开始校准
	while(ADC_GetCalibrationStatus(ADC1)== SET);//获取校准状态
	  ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件触发转换

}



//uint8_t result[5];
//void Binarization(void)
//{
//for(uint8_t i = 0;i<=4;i++)
//	{
//		result[i] = (AD_Value[i]<450)? 1:0;
//	}
//}





