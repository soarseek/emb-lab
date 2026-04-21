#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "CountSensor.h"
#include "OLED.h"

int main(void)
{
	LED_Init();
	OLED_Init();
	CountSensor_Init();
	
	uint16_t lastCount = 0;
	uint16_t currentCount = 0;
	
	OLED_ShowString(1, 1, "Count:");
	while(1)
	{
		currentCount = CountSensor_Get();
		
		// 检查计数值是否变化
		if(currentCount != lastCount)
		{
			// 计数值变化，LED_1闪烁一下
			LED_On(LED_1);
			Delay_ms(100);
			LED_Off(LED_1);
			lastCount = currentCount;
		}
		
		OLED_ShowNum(1, 7, currentCount, 5);
		Delay_ms(10); // 防止过于频繁的更新
	}
}
