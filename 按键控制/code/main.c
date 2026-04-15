#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "Key.h"

uint8_t KeyNum;
uint8_t Key1_Count = 0;
uint8_t Key2_Count = 0;

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
			Key1_Count++;
			if(Key1_Count % 2 == 1)
			{
				LED_On(LED_0);
			}
			else
			{
				LED_Off(LED_0);
			}
		}
		else if(KeyNum == 2)
		{
			Key2_Count++;
			if(Key2_Count % 2 == 1)
			{
				LED_On(LED_3);
			}
			else
			{
				LED_Off(LED_3);
			}
		}
		Delay_ms(10);
	}
}
