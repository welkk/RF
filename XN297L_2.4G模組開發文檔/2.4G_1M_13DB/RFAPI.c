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



#include "stdio.h"
#include "RFAPI.h"
#define uint8_t unsigned char
  

enum  RFAPI_CommuSate  ucRFAPI_STATE;
union RF_PAYLOAD  RF_Payload;
uint8_t Channel_Index=19;  //2478
uint8_t const CHANNEL_TABLE[] = {2,6,10,14,18,22,26,30,34,38,42,46,50,54,58,62,66,70,74,78};
uint8_t const CHANNEL_TABLE_CARRIER[] = {2,50,78};
//uint8_t ucTest_Mode=0,DATA_READY=0,RX_READY=0,ucKey_RD=0;
//, TIME_8MS, RX_READY,DATA_READY_ADC;
//uint8_t RX_READY=0;
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



//********************************end of file*********************************//