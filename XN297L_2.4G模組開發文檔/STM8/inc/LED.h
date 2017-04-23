#ifndef   _SENSOR_H_
#define   _SENSOR_H_

#include "stm8l15x.h"


#define        LED_BIT1_ON             GPIOC->ODR &= (~GPIO_Pin_4)
#define        LED_BIT1_OFF              GPIOC->ODR |= GPIO_Pin_4
#define        LED_BIT2_ON             GPIOC->ODR &= (~GPIO_Pin_5)
#define        LED_BIT2_OFF              GPIOC->ODR |= GPIO_Pin_5
#define        LED_BIT3_ON             GPIOC->ODR &= (~GPIO_Pin_6)
#define        LED_BIT3_OFF              GPIOC->ODR |= GPIO_Pin_6
#define        LED_BIT0_ON             GPIOD->ODR &= (~GPIO_Pin_1)
#define        LED_BIT0_OFF              GPIOD->ODR |= GPIO_Pin_1

#define       CH595_DAT_SET             GPIOD->ODR |= GPIO_Pin_4
#define       CH595_DAT_RESET           GPIOD->ODR &= (~GPIO_Pin_4)
#define       CH595_CLK_SET             GPIOA->ODR |= GPIO_Pin_5
#define       CH595_CLK_RESET           GPIOA->ODR &= (~GPIO_Pin_5)
#define       CH595_CP_SET              GPIOB->ODR |= GPIO_Pin_3
#define       CH595_CP_RESET            GPIOB->ODR &= (~GPIO_Pin_3)
  

enum
{
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    SUB,
    LED_NULL,
    LED_E,
    LED_R,
    LED_CP,
    LED_8p,
    LED_F,
    LED_A,
    LED_b,
    LED_C,
    LED_h
    
};
enum
{
    LED_CHANNEL_MODE,
    LED_FUNCTION_MODE,
    LED_CNT_MODE  
};


extern unsigned char ucLED_FlashCnt;
extern bool bLED_Enable;

void LED_Init(void);
void LED_Display(void);
void LED_ChangeFont(unsigned short usLED_Data,unsigned char ucLED_Mode);

#endif


//*********************************end of file***********************************//