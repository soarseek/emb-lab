#include "stm32f10x.h"                  // 设备头文件
#include <stdio.h>
#include <stdarg.h>
#include "LED.h"
#include "Delay.h"

uint8_t Serial_RxData;                    // 串口接收数据缓冲区
uint8_t Serial_RxFlag;                    // 串口接收标志位

/**
  * @brief  串口初始化函数
  * @note   配置USART1，波特率115200，8位数据位，1位停止位，无校验
  *         PA9 -> TX（复用推挽输出）
  *         PA10 -> RX（上拉输入）
  *         同时配置DMA1通道4用于串口发送
  * @retval None
  */
void Serial_Init(void)
{
	// 使能USART1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	// 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// 使能DMA1时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	// 配置TX引脚 PA9 为复用推挽输出
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       // 复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;             // PA9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     // 输出速度50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 配置RX引脚 PA10 为上拉输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         // 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;            // PA10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 配置USART1参数
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;                           // 波特率115200
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // 无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;       // 收发模式
	USART_InitStructure.USART_Parity = USART_Parity_No;                    // 无校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                 // 1位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;           // 8位数据位
	USART_Init(USART1, &USART_InitStructure);
	
	// 启用USART1的DMA发送请求
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	
	// 配置NVIC优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	// 配置USART1中断
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;         // USART1中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 使能中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        // 子优先级1
	NVIC_Init(&NVIC_InitStructure);
	
	// 启用USART1接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	// 启用USART1
	USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  串口DMA发送数组函数
  * @note   使用DMA1通道4将数据从内存发送到USART1
  *         DMA配置步骤：
  *         1. 设置DMA请求源为USART1_TX
  *         2. 设置内存地址为数据数组首地址
  *         3. 设置DMA方向为内存到外设
  *         4. 设置DMA传输大小
  *         5. 设置外设地址不增、内存地址自增
  *         6. 设置外设数据单位为8位、内存数据单位为8位
  *         7. 设置正常缓存模式（非循环缓存模式）
  *         8. 设置DMA优先级为高
  *         9. 禁止内存到内存的传输
  *         10. DMA1通道4初始化
  *         11. 使能DMA
  *         12. 允许DMA完成中断
  * @param  Array: 发送数据数组指针
  * @param  Length: 发送数据长度
  * @retval None
  */
void Serial_SendArray_DMA(uint8_t *Array, uint16_t Length)
{
	// 复位DMA1通道4
	DMA_DeInit(DMA1_Channel4);
	
	// 配置DMA参数
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;  // 外设地址：USART1数据寄存器
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Array;             // 内存地址：数据数组
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                  // 方向：内存到外设
	DMA_InitStructure.DMA_BufferSize = Length;                          // 传输大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    // 外设地址不增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;             // 内存地址自增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  // 外设数据单位8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;     // 内存数据单位8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                       // 正常模式（非循环）
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;                 // 高优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                       // 禁止内存到内存
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);                        // 初始化DMA1通道4
	
	// 使能DMA传输完成中断
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	
	// 配置DMA1通道4中断
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;           // DMA1通道4中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     // 使能中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;          // 抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;                 // 子优先级2
	NVIC_Init(&NVIC_InitStructure);
	
	// 启动DMA传输
	DMA_Cmd(DMA1_Channel4, ENABLE);
}

/**
  * @brief  串口发送单个字节函数
  * @note   阻塞方式发送一个字节，发送时LED1亮，发送完成LED1灭
  * @param  Byte: 要发送的字节
  * @retval None
  */
void Serial_SendByte(uint8_t Byte)
{
	LED1_ON();                              // 发送时LED1亮
	USART_SendData(USART1, Byte);           // 发送数据
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);  // 等待发送完成                            // 延时5秒
	LED1_OFF();                             // 发送完成LED1灭
}

/**
  * @brief  串口发送数组函数（非DMA方式）
  * @note   通过循环调用Serial_SendByte发送数组
  * @param  Array: 发送数据数组指针
  * @param  Length: 发送数据长度
  * @retval None
  */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

/**
  * @brief  串口发送字符串函数
  * @note   发送以'\0'结尾的字符串
  * @param  String: 字符串指针
  * @retval None
  */
void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

/**
  * @brief  幂运算函数
  * @note   计算X的Y次方
  * @param  X: 底数
  * @param  Y: 指数
  * @retval 计算结果
  */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  串口发送数字函数
  * @note   将数字按指定位数发送
  * @param  Number: 要发送的数字
  * @param  Length: 数字位数
  * @retval None
  */
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  重定向fputc函数
  * @note   使printf函数通过串口输出
  * @param  ch: 要输出的字符
  * @param  f: 文件指针（未使用）
  * @retval 输出的字符
  */
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

/**
  * @brief  串口printf函数
  * @note   支持格式化输出到串口
  * @param  format: 格式化字符串
  * @param  ...: 可变参数
  * @retval None
  */
void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

/**
  * @brief  获取串口接收标志位
  * @note   获取后自动清除标志位
  * @retval 1: 有新数据，0: 无新数据
  */
uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

/**
  * @brief  获取串口接收数据
  * @note   获取最近一次接收到的数据
  * @retval 接收到的字节
  */
uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}

/**
  * @brief  USART1中断服务函数
  * @note   处理串口接收中断
  * @retval None
  */
void USART1_IRQHandler(void)
{
	// 判断是否为接收中断
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		Serial_RxData = USART_ReceiveData(USART1);  // 读取接收数据
		Serial_RxFlag = 1;                           // 设置接收标志
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);  // 清除中断标志
	}
}
