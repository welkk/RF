//===================================================================================//
//  * @file           Key_scan.c
//  * @author         Shi Zheng
//  * @version        V1.1.0
//  * @date           Aug/12/2013
//  * @brief          Main program body
//  * @modify user:   Shizheng
//  * @modify date:   Aug/12/2013
//===================================================================================//
/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "Key.H"
#include "APP.H"

uint8_t ucKey,ucKey_Temp,ucKey_RD;
uint16_t ucKey_Debounce = 0;
uint8_t ucADC_Level;

/******************************************************************************/
//            Key_IO_Initial
//                Set key det pin as input pulling high no interrupt
//                Initial variable
/******************************************************************************/
void Key_Init(void)
{
    GPIO_Init( GPIOB, GPIO_Pin_0, GPIO_Mode_In_PU_No_IT);                       // KEY1 Input pull high 
    GPIO_Init( GPIOA, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT);                       // KEY3
    GPIO_Init( GPIOA, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT);                       // KEY2 Input pull high 

    delay_10us(50);    
    ucKey = 0;
    ucKey_Debounce = ucKey_Debounce+1;
    ucKey = Key_Rd();
    ucKey_Temp =  ucKey;
    ucKey_RD = ucKey_Temp;
}

/******************************************************************************/
//            Key_Rd
//                Read pin status
/******************************************************************************/
unsigned char Key_Rd(void)
{
    unsigned char ucKey_D;
     
    ucKey_D = 0;
    if(~KEY1_STATUS)
    {
        ucKey_D |= 0x01;      
    }
    if(~KEY2_STATUS )
    {
        ucKey_D |= 0x02;      
    }  
    if(~KEY3_STATUS )
    {
        ucKey_D |= 0x04;      
    } 
    return ucKey_D;
}



/******************************************************************************/
//            Key_Scan
//                scan key function
//                when key status changed ,set DATA_READY as TRUE
/******************************************************************************/
void Key_Scan(void)
{
    ucKey = Key_Rd();
    
    if(ucKey != ucKey_Temp)
    {
        ucKey_Temp =  ucKey;
        ucKey_Debounce = 0;
    }
    else if(ucKey_Debounce < KEY_DEBOUNCE_TIME )
    {
        ucKey_Debounce++;      
    }
    else if(ucKey_Debounce == KEY_DEBOUNCE_TIME)
    {
        ucKey_RD = ucKey_Temp;      
        DATA_READY = TRUE; 
        ucKey_Debounce++;  
    }
}
//******************************end of file************************************/