#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"

uint8_t RandomBit(void)
{
	static uint32_t seed = 12345;
	seed = (seed * 1103515245 + 12345) % 0x7fffffff;
	return seed % 2;
}

int main(void)
{
	OLED_Init();
	
	while(1)
	{
		char buffer[17];
		// 生成并显示第1行
		for(uint8_t i = 0; i < 16; i++)
		{
			buffer[i] = RandomBit() ? '1' : '0';
		}
		buffer[16] = '\0';
		OLED_ShowString(1, 1, buffer);
		
		// 生成并显示第2行
		for(uint8_t i = 0; i < 16; i++)
		{
			buffer[i] = RandomBit() ? '1' : '0';
		}
		buffer[16] = '\0';
		OLED_ShowString(2, 1, buffer);
		
		// 生成并显示第3行
		for(uint8_t i = 0; i < 16; i++)
		{
			buffer[i] = RandomBit() ? '1' : '0';
		}
		buffer[16] = '\0';
		OLED_ShowString(3, 1, buffer);
		
		// 生成并显示第4行
		for(uint8_t i = 0; i < 16; i++)
		{
			buffer[i] = RandomBit() ? '1' : '0';
		}
		buffer[16] = '\0';
		OLED_ShowString(4, 1, buffer);
		
		Delay_ms(20);
	}
}
