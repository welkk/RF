//============================================================================//
//  * @file           app.c
//  * @author         Shi Zheng
//  * @version        V1.1.0
//  * @date           Aug/12/2013
//  * @brief          Main program body
//  * @modify user:   Shizheng
//  * @modify date:   Aug/12/2013
//============================================================================//
/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "App.h"
#include "appsub.h"
#include "LED.h"
#include "Key.h"
#include "stdio.h"


unsigned char DISP_Status;
unsigned long int ulState_Timer = 0;
bool DATA_READY = FALSE, TIME_8MS = FALSE;
bool  RX_READY = FALSE,DATA_READY_ADC = FALSE;
bool TX_TRIGGER = FALSE,TX_RQ = FALSE,TX_TIME=FALSE;

unsigned char ucLED_FlashCnt = 0;
unsigned short TRX_Cnt = 0;
unsigned char ucTXMode = MODE_TX_SINGLE;

/******************************************************************************/
//            APP_StateMachine
//                TX/RX Work Application
/******************************************************************************/
void  APP_StateMachine(void)
{
  if(Work_State == FUNCTION_RX)
  {
      APP_RX_Normal();    
  }
  else if(Work_State == FUNCTION_TX)
  {
     APP_TX_Normal();    
  }
}

/******************************************************************************/
//            APP_RX_Event
//                RX Event handler
/******************************************************************************/
void APP_RX_Event(void)
{
  if(RX_READY)                                                                  //RF RX data prepare
  {
#ifdef  DEBUG_EN
      printf("RX_Event:%x\n",TRX_Cnt);    
#endif
      RX_READY = FALSE;
      TRX_Cnt++;                                                                //RX Count add 1
      if(DISP_Status == LED_CNT_MODE)                                           //if LED has been in count mode
      {
          LED_ChangeFont(TRX_Cnt,LED_CNT_MODE);
          DISP_Status = LED_CNT_MODE;
      }
  }
  
  if(DATA_READY)                                                                //if Key Event
  {
//    printf("key_rd:%x\n",ucKey_RD);    
    DATA_READY = FALSE;
    if(ucKey_RD == KEY1)                                                        //if press Key1 button down
    {
          if(DISP_Status == LED_CNT_MODE)                                       //if LED has been in count mode
          {
#ifdef  DEBUG_EN
            printf("Clear cnt\n");    
#endif
              TRX_Cnt = 0;                                                      // clear rx count
          }
          LED_ChangeFont(TRX_Cnt,LED_CNT_MODE);                                 //set LED in count mode
          DISP_Status = LED_CNT_MODE;
      }
      else if(ucKey_RD == KEY2)                                                 //if press Key2 button down
      {
#ifdef  DEBUG_EN
      printf("key 2\n");    
#endif
    
      }
      else if(ucKey_RD == KEY3)                                                 //if press Key3 button down
      {
          if(DISP_Status == LED_CHANNEL_MODE)                                   //if LED in Channel Mode
          {
                RFAPI_ChannelNext();                                              //Set RF work in Next Channel
                RF_RxMode();                                                      //Set RF work in RX mode
          }
          LED_ChangeFont(CHANNEL_TABLE[Channel_Index],LED_CHANNEL_MODE);    
          DISP_Status = LED_CHANNEL_MODE;                                       //set LED in Channel mode
#ifdef  DEBUG_EN
      printf("Hop channel:%d\n",(int)CHANNEL_TABLE[Channel_Index]);    
#endif
      }
  }
}
/******************************************************************************/
//            APP_RX_Normal
//                RX StateMachine
/******************************************************************************/

void APP_RX_Normal(void)
{
    APP_RX_Event();                               //Event handler
    Key_Scan();                                   //scan key status
    LED_Display();                                //LED handler
}

/******************************************************************************/
//            APP_TXMODE_Swtich
//                Switch between:Function A,B and C
//                              A:  single TX
//                              B:  1000packet TX
//                              C:  continues TX
/******************************************************************************/
void APP_TXMODE_Swtich(void)
{
    if(ucTXMode == MODE_TX_SINGLE)
    {
        ucTXMode = MODE_TX_1000PACK;                                            //Switch into B
        TRX_Cnt = 1000;
    }
    else if(ucTXMode == MODE_TX_1000PACK)
    {
        ucTXMode = MODE_TX_CONTINUE;                                            //Switch into C
        TRX_Cnt = 0;
    }
    else
    {
        ucTXMode = MODE_TX_SINGLE;                                              //Switch into A
        TRX_Cnt = 0;
    }
}

/******************************************************************************/
//            APP_TX_Event
//                TX Event handler
/******************************************************************************/
void  APP_TX_Event(void)
{
  if(DATA_READY)                                                                //Key Event occur
  {
    DATA_READY = FALSE;
    if(ucKey_RD == KEY1)                                                        //if press Key1 button down
    {
        TX_TRIGGER = FALSE;
        if(DISP_Status == LED_FUNCTION_MODE)                                    //if LED in Function mode
        {
            APP_TXMODE_Swtich();                                                //function switch
#ifdef  DEBUG_EN
            printf("function switch:%x\n",ucTXMode);    
#endif
        }
        else
        {
            if(ucTXMode == MODE_TX_SINGLE)
            {
                TRX_Cnt = 0;
            }
            else if(ucTXMode == MODE_TX_1000PACK)
            {
                TRX_Cnt = 1000;
            }
            else
            {
                TRX_Cnt = 0;
            }          
        }
        DISP_Status = LED_FUNCTION_MODE;                                        //set LED in function switch and clear tx count
        LED_ChangeFont(ucTXMode,LED_FUNCTION_MODE);
    }
    else if(ucKey_RD == KEY2)                                                   //if press Key2 button down
    {
        if(TX_TRIGGER)                                                          // start or stop tx    
        {
            TX_TRIGGER = FALSE;           
        }
        else
        {
            TX_TRIGGER = TRUE;          
        }

#ifdef  DEBUG_EN
        printf("start trigger\n");    
#endif
    }
    else if(ucKey_RD == KEY3)                                                   //if press Key3 button down
    {
        if(DISP_Status == LED_CHANNEL_MODE)                                     //if LED in Channel mode
        {
            RFAPI_ChannelNext();                                                  //RF hop to hext channel
#ifdef  DEBUG_EN
            printf("Hop channel:%d\n",(int)CHANNEL_TABLE[Channel_Index]);    
#endif
        }
        TX_TRIGGER = FALSE;
        DISP_Status = LED_CHANNEL_MODE;                                         //set LED in channel mode
        LED_ChangeFont(CHANNEL_TABLE[Channel_Index],LED_CHANNEL_MODE);
    }
  }
  if((TX_TRIGGER)&&(TX_TIME)&&(ucRFAPI_STATE == RFAPI_TX))                          //if triggered and 8msEvent and TX idle
  {
      TX_TIME = FALSE;
      TX_RQ = TRUE;                                                             //enable to tx
      
      if(ucTXMode == MODE_TX_SINGLE)                                            //if in single mode
      {
          TRX_Cnt++;                                    
          TX_TRIGGER = FALSE;
#ifdef  DEBUG_EN
          printf("trigger stop\n");    
#endif
      }
      else if(ucTXMode == MODE_TX_CONTINUE)                                     //if in continues mode
      {
          TRX_Cnt++;
          if(TRX_Cnt >= 9999)                                                   //if LED overflow
          {
            TRX_Cnt = 0;
          }
      }
      else                                                                      //if in single mode
      {
          if(TRX_Cnt>0)                               
          {
              TRX_Cnt--;
          }
          else                                                                  //if tx over 
          {
              TRX_Cnt = 1000;
              TX_RQ = FALSE;                          
              TX_TRIGGER = FALSE;                                               //stop tx
#ifdef  DEBUG_EN
              printf("trigger stop\n");    
#endif
          }
      }
      LED_ChangeFont(TRX_Cnt,LED_CNT_MODE);
      DISP_Status = LED_CNT_MODE;
  }
}

/******************************************************************************/
//            APP_TX_Normal
//                TX StateMachine
/******************************************************************************/
void APP_TX_Normal(void)
{
      APP_TX_Event();
      LED_Display();
      Key_Scan();
}

//*********************************end of file********************************//