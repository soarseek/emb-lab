/*
 * File Name: main.c
 * Description: STM32F10x LED running light control program with buzzer sound effect
 * 
 * Hardware Pin Description:
 * - LED Pins: PA0, PA1, PA2, PA3 (Active Low)
 * - Buzzer Pin: PB12
 * 
 * GPIO Configuration:
 * - Mode: Push-Pull Output
 * - Speed: 50MHz
 */

#include "stm32f10x.h"

void delay_ms(uint32_t ms)
{
    uint32_t i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 7200; j++);
}

void beep(uint32_t ms)
{
    uint32_t t = ms * 10;
    while(t--)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
        delay_ms(1);
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
        delay_ms(1);
    }
}

int main(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);

    while(1)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
        beep(20);
        delay_ms(60);

        GPIO_SetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);
        beep(22);
        delay_ms(60);

        GPIO_SetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
        GPIO_ResetBits(GPIOA, GPIO_Pin_2);
        beep(24);
        delay_ms(60);

        GPIO_SetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
        GPIO_ResetBits(GPIOA, GPIO_Pin_3);
        beep(26);
        delay_ms(60);
    }
}