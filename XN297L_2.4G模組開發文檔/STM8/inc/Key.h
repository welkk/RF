#ifndef _KEY_SCAN_H_
#define _KEY_SCAN_H_

#define   KEY1_STATUS         GPIOA->IDR&GPIO_Pin_2
#define   KEY2_STATUS         GPIOA->IDR&GPIO_Pin_3
#define   KEY3_STATUS         GPIOB->IDR&GPIO_Pin_0

enum
{
  KEY1 = 0x01,
  KEY2 = 0x02,
  KEY3 = 0x04  
};

#define   KEY_DEBOUNCE_TIME       1000

extern uint8_t ucKey_RD;
extern uint8_t ucADC_Level;

void Key_Init(void);
uint8_t Key_Rd(void);
void Key_Scan(void);
void ADC_Check(void);

#endif

//****************************************end of file**********************************************//