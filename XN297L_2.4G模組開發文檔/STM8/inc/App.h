#ifndef _APP_H_
#define _APP_H_
#include "global.h"
#include "RFAPI.h"
#define     UART_BOUNDRATE            115200
#define     TRX_PIN_MODE            GPIOA->IDR&GPIO_Pin_4

enum  APP_STATE
{
     FUNCTION_TX,  
     FUNCTION_RX,
};

enum  APP_TXB_Status
{
    MODE_NULL,
    MODE_TX_SINGLE,
    MODE_TX_1000PACK,
    MODE_TX_CONTINUE
};



extern unsigned char Work_State;
extern unsigned char DISP_Status;
extern unsigned char ucTXMode;
extern bool DATA_READY, TIME_8MS, RX_READY,DATA_READY_ADC;
extern bool TX_RQ,TX_TIME;
extern uint8_t   ucTest_Mode;

void  APP_StateMachine(void);
int   putchar(int c); 
void APP_RX_Normal(void);
void APP_TX_Normal(void);
#endif
