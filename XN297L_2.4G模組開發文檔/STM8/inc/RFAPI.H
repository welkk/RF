//============================================================================//
//  * @file           PN006_APP.H
//  * @author         Shi Zheng 
//  * @version        V1.0
//  * @date           24/4/2015
//  * @brief          RF PN006 communication interface
//  * @modify user:   Shizheng
//  * @modify date:   24/4/2015
//============================================================================//
#ifndef   _RF_API_H_
#define   _RF_API_H_
#include "RF.h"

typedef union 
{
    uint8_t ucPayload[PAYLOAD_WIDTH];

}RF_PAYLOAD;

enum RFAPI_WorkMode
{
    TEST_DEFAULT_MODE = 0,
    TEST_CARRIER_MODE ,
    TEST_TX_MODE,
    TEST_NoHOP_MODE
};

enum  RFAPI_CommuSate
{
  RFAPI_RX,
  RFAPI_TX,
  RFAPI_HALT  
};

extern    bool  bRFAPI_IRQ;
extern    RF_PAYLOAD RF_Payload;
extern    uint8_t Channel_Index;
extern    const uint8_t TX_ADDRESS_DEF[]; 
extern    uint8_t const CHANNEL_TABLE[];
extern    enum  RFAPI_CommuSate  ucRFAPI_STATE;
void      RFAPI_RXState(bool RX_Continue);
void      RFAPI_TXState(void);
void      RFAPI_StateMachine(void);
void      RFAPI_PacketData(void);
void      RFAPI_ChannelNext(void);
void      RFAPI_TestMode(void);
#endif

/****************************************end of file********************************************/

