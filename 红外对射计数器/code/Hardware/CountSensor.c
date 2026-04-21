#include "stm32f10x.h"

uint16_t CountSensor_Count = 0;

void CountSensor_Init(void)
{
    // 初始化计数传感器
    // 使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    // 使能AFIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;                    // 定义GPIO初始化结构体
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;           // 设置为输入拉模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;              // 选择引脚14
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // 设置速度为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);                  // 初始化GPIOB

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
    //初始化EXTI
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line14;            // 选择EXTI线14
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 设置为下降沿触发
    EXTI_Init(&EXTI_InitStructure);                         // 初始化EXTI

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    //初始化NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; // 选择EXTI15_10中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   // 抢占优先级为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          // 响应优先级为1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             // 使能中断
    NVIC_Init(&NVIC_InitStructure);                             // 初始化NVIC
}

uint16_t CountSensor_Get(void)
{
    return CountSensor_Count;
}

// EXTI15_10_IRQHandler中断函数
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line14) == SET)
    {
        CountSensor_Count++;
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
}
