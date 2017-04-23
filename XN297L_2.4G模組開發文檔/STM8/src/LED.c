//============================================================================//
//  * @file           Sensor.c
//  * @author         Shi Zheng
//  * @version        V1.1.0
//  * @date           Aug/12/2013
//  * @brief          Main program body
//  * @modify user:   Shizheng
//  * @modify date:   Aug/12/2013
//============================================================================//
#include "stm8l15x.h"
#include "LED.h"
#include "App.h"
#include "stdio.h"


static uint8_t ucLED_DIS[4] = {EIGHT,EIGHT,EIGHT,EIGHT};
bool bLED_Enable=TRUE;
/******************************************************************************/
//            LED_TAB
//                LED character Table
/******************************************************************************/
const uint8_t LED_TAB[] = 
{
    0xC0,   //'0'  
    0xF9,   //'1' 
    0xA4,   //'2' 
    0xB0,   //'3' 
    0x99,   //'4' 
    0x92,   //'5' 
    0x82,   //'6' 
    0xF8,   //'7' 
    0x80,   //'8' 
    0x90,   //'9' 
    0xBF,   //'-' 
    0xFF,   //' ' 
    0x86,   //'E' 
    0x88,   //'R' 
    0x46,   //'c.' 
    0x00,   //'8.' 
    0x8E,   //'F'
    0x88,   //'A'
    0x83,   //'b'
    0xc6,   //'C'
    0x0B    //'h'
};
/******************************************************************************/
//            LED_Initial
//                Initial LED Enable pin and CH595 pin
/******************************************************************************/
void LED_Init(void)
{
    unsigned char i;
    for(i=0;i<sizeof(ucLED_DIS);i++)
    {
        ucLED_DIS[i] = 8;
    }
    
    GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);  
    GPIO_Init(GPIOC, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Fast);  
    GPIO_Init(GPIOC, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOA, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOB, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOD, GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Fast);
}

/******************************************************************************/
//            HC595_Write
//                write ucData into HC595
/******************************************************************************/
void HC595_Write(unsigned char ucData)
{
    unsigned char i;
    
    CH595_CP_RESET;
    for(i=0;i<8;i++)
    {
        CH595_CLK_RESET;
        if(ucData&0x80)
        {
            CH595_DAT_SET;        
        }
        else
        {
            CH595_DAT_RESET;        
        }
        ucData<<=1;
        CH595_CLK_SET;
    }
}



/******************************************************************************/
//            LED_Display
//                LED display handler
/******************************************************************************/
void LED_Display(void)
{
    static unsigned char ucLedDis_Bit_Index = 0;

    if(bLED_Enable)                                                             //if Enable LED display
    {
        LED_BIT2_OFF;
        LED_BIT0_OFF;
        LED_BIT1_OFF;
        LED_BIT3_OFF;
        HC595_Write(LED_TAB[ucLED_DIS[ucLedDis_Bit_Index]&0x1F]);               //write data into HC595
        CH595_CP_SET; 
/*******************switch next led**********************/
        if(ucLedDis_Bit_Index == 0)
        {
            LED_BIT0_ON;
            ucLedDis_Bit_Index = 1;
        }
        else if(ucLedDis_Bit_Index == 1)
        {
            LED_BIT1_ON;
            ucLedDis_Bit_Index = 2;
        }
        else if(ucLedDis_Bit_Index == 2)
        {
            LED_BIT2_ON;
            ucLedDis_Bit_Index = 3;
        }
        else if(ucLedDis_Bit_Index == 3)
        {
            LED_BIT3_ON;
            ucLedDis_Bit_Index = 0;
        }
        else
        {
            ucLedDis_Bit_Index = 0;
        }
    }
    else                                                                        //if not Enable LED 
    {
        LED_BIT2_OFF;
        LED_BIT0_OFF;
        LED_BIT1_OFF;
        LED_BIT3_OFF;
        ucLedDis_Bit_Index = 0;      
    }
}

/******************************************************************************/
//            LED_ChangeFont
//                change LED display mode
//                change LED display font
/******************************************************************************/
void LED_ChangeFont(unsigned short usLED_Data,unsigned char ucLED_Mode)
{
  
  
  
  /******************************CARRIER MODE****************************/
     if(ucTest_Mode == TEST_CARRIER_MODE)
      {
        
                  ucLED_DIS[3] = LED_C;
                  ucLED_DIS[2] = LED_h;
                  ucLED_DIS[1] = usLED_Data%100/10;
                  ucLED_DIS[0] = usLED_Data%10;

      }
/**************RX FUNCTION*****************/  
     else if(Work_State == FUNCTION_RX)
      {
          switch(ucLED_Mode)
          {
            case LED_CHANNEL_MODE:
            
                ucLED_DIS[3] = LED_F;
                ucLED_DIS[2] = LED_C;
                ucLED_DIS[1] = usLED_Data%100/10;
                ucLED_DIS[0] = usLED_Data%10;      
                break;
            case LED_FUNCTION_MODE:                                             //function mode:FC. 0 - FC. 9)
                ucLED_DIS[3] = LED_F;
                ucLED_DIS[2] = LED_CP;
                ucLED_DIS[1] = LED_NULL;
                ucLED_DIS[0] = usLED_Data%10;      
                break;
            case LED_CNT_MODE:                                                  //CNT  mode:(0000 - 9999)
                ucLED_DIS[3] = usLED_Data%10000/1000;
                ucLED_DIS[2] = usLED_Data%1000/100;
                ucLED_DIS[1] = usLED_Data%100/10;
                ucLED_DIS[0] = usLED_Data%10;
                break;
            default:
                break;
       
          }
      }
      else if(Work_State == FUNCTION_TX)
      {
/**************TX FUNCTION*****************/  
          switch(ucLED_Mode)
          {
            case LED_CHANNEL_MODE:
                  ucLED_DIS[3] = LED_F;
                  ucLED_DIS[2] = LED_C;
                  ucLED_DIS[1] = usLED_Data%100/10;
                  ucLED_DIS[0] = usLED_Data%10;
                  break;
            case LED_FUNCTION_MODE:                                             //function mode:FC. 0 - FC. 9)
                ucLED_DIS[3] = LED_F;
                ucLED_DIS[2] = LED_CP;
                ucLED_DIS[1] = LED_NULL;
                if(usLED_Data == 1)
                {
                    ucLED_DIS[0] = LED_A;              
                }
                else if(usLED_Data == 2)
                {
                    ucLED_DIS[0] = LED_b;               
                }
                else if(usLED_Data == 3)
                {
                    ucLED_DIS[0] = LED_C;              
                }            
                break;
            case LED_CNT_MODE:                                                  //CNT mode
              if(ucTXMode == MODE_TX_SINGLE)                                    //Single tx:(A000 - A999)
                {
                    ucLED_DIS[3] = LED_A;                  
                }
                else
                {
                    ucLED_DIS[3] = usLED_Data%10000/1000;                       //else:(0000-9999)                
                }
                ucLED_DIS[2] = usLED_Data%1000/100;
                ucLED_DIS[1] = usLED_Data%100/10;
                ucLED_DIS[0] = usLED_Data%10;
                break;
          default:
                break;
         }        
    }
     
}
//*******************************end of file*****************************//