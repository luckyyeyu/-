#include "stm32f10x.h"                  // Device header
#include "PID.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h> 
char Serial_RxPacket[100];
uint8_t Serial_RxFlag;
int32_t Serial_Value1 = 0;      // 存储第一个数值
int32_t Serial_Value2 = 0;      // 存储第二个数值

char Serial_Value1Buffer[20];  // 专门存第一个值的缓冲区
char Serial_Value2Buffer[20];  // 专门存第二个值的缓冲区
uint8_t RxState = 0;
uint8_t pRxValue = 0;
char Serial_ValueBuffer[32];
void Serial_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
     GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//分成员赋值
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//分成员赋值
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity =USART_Parity_No ;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel =USART1_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1 ;
	
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
uint16_t i;
	for(i = 0;i<Length;i++)
	{
	Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
uint8_t i;
	for(i = 0;String[i] != '\0';i++)
	{
	Serial_SendByte(String[i]);
	}
}


uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
uint32_t Result = 1;
	while(Y--)
	{
	Result *=X;
	}
	return Result;
}
void Serial_SendNumber(uint32_t Number,uint8_t length)
{
uint8_t i;
	for(i = 0;i<length;i++)
	{
		Serial_SendByte(Number/Serial_Pow(10,length - i -1)%10+'0');
	}

}

int fputc(int ch, FILE *f)
{
Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format,...)
{
char String[100];
	va_list arg;
	__va_start(arg,format);
    vsprintf(String,format,arg);
	__va_end(arg); 
	Serial_SendString(String);
}


uint8_t Serial_GetRxFlag(void)
{
if(Serial_RxFlag == 1)
{
Serial_RxFlag = 0;
	return 1;
}
return 0;
}

void USART1_IRQHandler(void)
{
    
    
    
if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
{
    uint8_t RxData = USART_ReceiveData(USART1);
    
    if(RxState == 0)  // 等待起始符@
    {
        if(RxData == '@')
        {
            RxState = 1;  // 开始接收数据
            pRxValue = 0;
            memset(Serial_ValueBuffer, 0, sizeof(Serial_ValueBuffer));
        }
    }
    else if(RxState == 1)  // 接收数值
    {
        if(RxData == ',')  // 逗号结束数值
        {
            Serial_ValueBuffer[pRxValue] = '\0';
            Serial_Value1 = atoi(Serial_ValueBuffer);  // 保存数值
            pRxValue = 0;
            RxState = 2;  // 等待\r\n
        }
        else
        {
            if(pRxValue < sizeof(Serial_ValueBuffer) - 1)
            {
                Serial_ValueBuffer[pRxValue++] = RxData;
            }
        }
    }
    else if(RxState == 2)  // 等待\r
    {
        if(RxData == '\r')
        {
            RxState = 3;  // 等待\n
        }
        else
        {
            RxState = 0;  // 格式错误，重置
        }
    }
    else if(RxState == 3)  // 等待\n
    {
        if(RxData == '\n')
        {
            RxState = 0;
            Serial_RxFlag = 1;  // 数据接收完成标志
        }
        else
        {
            RxState = 0;  // 格式错误，重置
        }
    }
    
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}


}
	int32_t Process_Serial_Data(void)
{
    if(Serial_GetRxFlag() == 1)
    {
//		add = Serial_Value1;
		int32_t s = Serial_Value1;
		return s;
	}
return 0;	
}	
