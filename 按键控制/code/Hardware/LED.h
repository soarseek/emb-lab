#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"

#define LED_0 GPIO_Pin_0
#define LED_1 GPIO_Pin_1
#define LED_2 GPIO_Pin_2
#define LED_3 GPIO_Pin_3

void LED_Init(void);
void LED_On(uint8_t led);
void LED_Off(uint8_t led);

#endif
