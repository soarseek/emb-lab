#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "Key.h"

uint8_t KeyNum;

int main(void)
{
	LED_Init();
	Key_Init();

	LED_On(LED_0);
	Delay_ms(100);
	LED_Off(LED_0);
	Delay_ms(100);
	while(1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
			LED_Off(LED_0);
		}
		else if(KeyNum == 2)
		{
			LED_On(LED_0);
		}
		Delay_ms(10);
	}
}
