#include "stm32f10x.h"                  // 设备头文件
#include "LED.h"
#include "Serial.h"

#define DATA_SIZE 15000                    // DMA发送数据大小：15000字节

uint8_t TxData[DATA_SIZE];                // 发送数据缓冲区

/**
  * @brief  主函数
  * @note   程序入口，实现上电LED1亮、DMA发送数据、发送完成后LED0和LED2亮
  * @retval None
  */
int main(void)
{
	uint16_t i;
	// 初始化发送数据缓冲区，填充0-255循环数据
	for (i = 0; i < DATA_SIZE; i++)
	{
		TxData[i] = i & 0xFF;
	}
	
	// 初始化LED和串口
	LED_Init();
	Serial_Init();
	
	// 上电时点亮中间蓝色指示灯LED1（PA1）
	LED1_ON();
	
	// 启动DMA通过串口1发送15000字节数据
	Serial_SendArray_DMA(TxData, DATA_SIZE);
	
	// 主循环等待
	while (1)
	{
	}
}

/**
  * @brief  DMA1通道4中断服务函数
  * @note   处理USART1 TX DMA传输完成中断
  * @retval None
  */
void DMA1_Channel4_IRQHandler(void)
{
	// 判断是否为DMA传输完成中断
	if (DMA_GetITStatus(DMA1_IT_TC4) == SET)
	{
		// 发送完成后点亮边上蓝色LED0（PA0）和橙色LED2（PA2）
		LED0_ON();
		LED2_ON();
		LED1_OFF();
		// 清除中断标志位
		DMA_ClearITPendingBit(DMA1_IT_TC4);
	}
}