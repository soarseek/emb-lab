#include "stm32f10x.h"                  // 设备头文件

/**
  * @brief  LED初始化函数
  * @note   配置PA0、PA1、PA2为推挽输出模式，初始状态为熄灭（高电平）
  *         LED0 -> PA0（边上蓝色）
  *         LED1 -> PA1（中间蓝色）
  *         LED2 -> PA2（橙色）
  * @retval None
  */
void LED_Init(void)
{
	// 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 配置GPIO结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // 推挽输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;  // PA0 PA1 PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // 输出速度50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);              // 初始化GPIOA
	
	// 默认熄灭所有LED（LED为低电平点亮）
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
}

/**
  * @brief  LED0点亮函数
  * @note   控制PA0输出低电平，点亮LED0（边上蓝色）
  * @retval None
  */
void LED0_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}

/**
  * @brief  LED0熄灭函数
  * @note   控制PA0输出高电平，熄灭LED0（边上蓝色）
  * @retval None
  */
void LED0_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

/**
  * @brief  LED1点亮函数
  * @note   控制PA1输出低电平，点亮LED1（中间蓝色）
  * @retval None
  */
void LED1_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

/**
  * @brief  LED1熄灭函数
  * @note   控制PA1输出高电平，熄灭LED1（中间蓝色）
  * @retval None
  */
void LED1_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

/**
  * @brief  LED1翻转函数
  * @note   切换LED1的状态（亮->灭 或 灭->亮）
  * @retval None
  */
void LED1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	}
}

/**
  * @brief  LED2点亮函数
  * @note   控制PA2输出低电平，点亮LED2（橙色）
  * @retval None
  */
void LED2_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

/**
  * @brief  LED2熄灭函数
  * @note   控制PA2输出高电平，熄灭LED2（橙色）
  * @retval None
  */
void LED2_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
}

/**
  * @brief  LED2翻转函数
  * @note   切换LED2的状态（亮->灭 或 灭->亮）
  * @retval None
  */
void LED2_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	}
}