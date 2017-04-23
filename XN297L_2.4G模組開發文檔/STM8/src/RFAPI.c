//============================================================================//
//  * @file           RFAPI.c
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
#include "APPSub.h"
#include "LED.h"
#include "Key.h"
#include "stdio.h"

bool  bRFAPI_IRQ = FALSE;
enum  RFAPI_CommuSate  ucRFAPI_STATE;
RF_PAYLOAD  RF_Payload;
uint8_t Channel_Index=19;  //2478
uint8_t const CHANNEL_TABLE[] = {2,6,10,14,18,22,26,30,34,38,42,46,50,54,58,62,66,70,74,78};
uint8_t const CHANNEL_TABLE_CARRIER[] = {2,50,78};


/******************************************************************************/
//            RF_Channel_Next
//                Hop to next channel
/******************************************************************************/
void RFAPI_ChannelNext(void)
{
   
  if((Channel_Index == 0)||(Channel_Index >= sizeof(CHANNEL_TABLE)))
  {
      Channel_Index = sizeof(CHANNEL_TABLE)-1;
  }
  else
  {
      Channel_Index--;
  }
    
  RF_SetChannel(CHANNEL_TABLE[Channel_Index] );
}


/******************************************************************************/
//            RF_PacketData
//                ready for tx data
/******************************************************************************/
void RFAPI_PacketData(void)
{
     uint8_t i; 
    ++RF_Payload.ucPayload[0];
    
    for(i=1;i<sizeof(RF_Payload.ucPayload);i++)
    {
        RF_Payload.ucPayload[i] = 0xC0 | i;
    }
}


/******************************************************************************/
//            RFAPI_TxState
//                In IRFAPI_TxState ,Data will be write into RF if TX enabled
/******************************************************************************/
void RFAPI_TxState(void)
{
    if(!(TX_RQ))                                                               //if don't need to  transmit data
    {
        return;      
    }
    TX_RQ = FALSE; 
    
    RFAPI_PacketData();                                                        //prepare data
    ucRF_TxData(RF_Payload.ucPayload,PAYLOAD_WIDTH);
}


/******************************************************************************/
//            BB_RX_State
//                In RX status ,Judge data has been received and read them
/******************************************************************************/
void  RFAPI_RXState(bool RX_Continue)
{
    if(ucRF_DumpRxData(RF_Payload.ucPayload,PAYLOAD_WIDTH))
    {
        ucRFAPI_STATE = RFAPI_RX;
        RX_READY = TRUE;
        CE_HIGH;       
    }
}


/******************************************************************************/
//            Test_Mode
//                Set RF into TestMode
/******************************************************************************/
void RFAPI_TestMode(void)
{
         
    if(ucTest_Mode == 0)
    {

        return;                                                                 //normal mode
    }
    else if(ucTest_Mode == TEST_CARRIER_MODE)
    {
             RF_Carrier(DEFAULT_CHANNEL);   
             LED_ChangeFont(DEFAULT_CHANNEL,LED_FUNCTION_MODE);   

         while(1)
        {    
             Key_Scan(); 
            if(DATA_READY)                                                                //if Key Event
            {
              DATA_READY = FALSE;
             if(ucKey_RD == KEY3)                                                        //if press Key1 button down
                {
                if((Channel_Index == 0)||(Channel_Index >= sizeof(CHANNEL_TABLE_CARRIER)))
                  {
                  Channel_Index = sizeof(CHANNEL_TABLE_CARRIER)-1;
                  }
                 else
                  {
                   Channel_Index--;
                   }
                       
               RF_Carrier( CHANNEL_TABLE_CARRIER[Channel_Index]);                                //set RF into carry mode
               LED_ChangeFont(CHANNEL_TABLE_CARRIER[Channel_Index],LED_FUNCTION_MODE);   
                }                                                  
                                                     
              }
              LED_Display(); 
          }
    
    }else
      {
        ucTest_Mode = 0;                                                        //normal mode
    
      }
}


/******************************************************************************/
//            BB_StateMachine
//                BB TX/RX Work StateMachine
/******************************************************************************/
void RFAPI_StateMachine(void)
{
    switch(ucRFAPI_STATE)
    {
      case RFAPI_TX:                                                             //RF in Idle
          RFAPI_TxState();
          break;
      case  RFAPI_RX:                                                              //RF in RX
          RFAPI_RXState(TRUE);
          break;
      case  RFAPI_HALT:                                                            //RF in shutdown
          break;
      default:
          break;
    }
}

//********************************end of file*********************************//