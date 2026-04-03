/*
 * File Name: main.c
 * Description: STM32F10x Morse code telegraph program with buzzer and LED output
 * 
 * Hardware Pin Description:
 * - Buzzer Pin: PB12 (Active Low)
 * - LED Pin: PA0 (Active Low)
 * 
 * GPIO Configuration:
 * - Mode: Push-Pull Output
 * - Speed: 50MHz
 * 
 * Morse Code Timing:
 * - Dot: 120ms
 * - Dash: 360ms
 * - Gap: 120ms
 * - Character Gap: 360ms
 * - Word Gap: 720ms
 */

#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"

//#define CONTENT "HELLO YOU WILL BE A HAPPY DAY FOR ALL"
#define CONTENT "SOS"

//=============================
// Buzzer: PB12, LED: PA0
//=============================
#define BEEP_PIN GPIO_Pin_12
#define BEEP_PORT GPIOB
#define BEEP_RCC RCC_APB2Periph_GPIOB

#define LED_PIN GPIO_Pin_0
#define LED_PORT GPIOA
#define LED_RCC RCC_APB2Periph_GPIOA

// Morse code timing
#define DOT     120
#define DASH    360
#define GAP     120
#define CHAR_GAP  360
#define WORD_GAP  720

//=============================
// Morse code table A~Z
//=============================
const char *morse_table[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
    ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
    "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."
};

//=============================
// Initialization
//=============================
void Beep_Init(void)
{
    GPIO_InitTypeDef gpio;
    RCC_APB2PeriphClockCmd(BEEP_RCC | LED_RCC, ENABLE);
    
    // Buzzer
    gpio.GPIO_Pin = BEEP_PIN;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BEEP_PORT, &gpio);
    GPIO_SetBits(BEEP_PORT, BEEP_PIN); // Buzzer off
    
    // LED PA0
    gpio.GPIO_Pin = LED_PIN;
    GPIO_Init(LED_PORT, &gpio);
    GPIO_SetBits(LED_PORT, LED_PIN); // LED off
}

void delay_ms(u32 ms)
{
    u32 i,j;
    for(i=0;i<ms;i++)
        for(j=0;j<7200;j++);
}

void Beep_On(void)  { GPIO_ResetBits(BEEP_PORT, BEEP_PIN); GPIO_ResetBits(LED_PORT, LED_PIN); }
void Beep_Off(void) { GPIO_SetBits(BEEP_PORT, BEEP_PIN);   GPIO_SetBits(LED_PORT, LED_PIN);   }

void play_dot(void)
{
    Beep_On(); delay_ms(DOT);
    Beep_Off(); delay_ms(GAP);
}

void play_dash(void)
{
    Beep_On(); delay_ms(DASH);
    Beep_Off(); delay_ms(GAP);
}

//=============================
// Play single character
//=============================
void play_char(char c)
{
    const char *code;
    u8 idx;

    if(c >= 'A' && c <= 'Z') c += 32;
    if(c >= 'a' && c <= 'z') {
        idx = c - 'a';
        code = morse_table[idx];
        while(*code) {
            if(*code == '.') play_dot();
            else if(*code == '-') play_dash();
            code++;
        }
        delay_ms(CHAR_GAP);
    }
    else if(c == ' ') {
        delay_ms(WORD_GAP);
    }
}

//=============================
// Play string
//=============================
void play_string(char *str)
{
    while(*str) {
        play_char(*str);
        str++;
    }
}

void play_from_content(void)
{
    play_string(CONTENT);
}

//=============================
// Main function
//=============================
int main(void)
{
    Beep_Init();
    // SD card initialization would go here if needed

    while(1)
    {
        play_from_content();
        delay_ms(2000);
    }
}