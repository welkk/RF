#include "RF.H"
#include "delay.h"
#include <reg51.h>
#define uint8_t unsigned char
uint8_t idata TX_ADDRESS_DEF[5] = {0xCC,0xCC,0xCC,0xCC,0xCC};    		//RF ��ַ�����ն˺ͷ��Ͷ���һ��
unsigned char ucCurrent_Channel;
/******************************************************************************/
//            SPI_init
//               init spi pin and IRQ  CE input/out mode
/******************************************************************************/
void SPI_init(void)
{
	IRQ_STATUS =1;
	CE  =0;
	CSN =1;
	SCK =0;
	
}

/******************************************************************************/
//            SPI_RW
//                SPI Write/Read Data
//            SPIд��һ��BYTE��ͬʱ������һ��BYTE����
/******************************************************************************/
unsigned char SPI_RW(uint8_t R_REG)
{
    uint8_t	  i;
    for(i = 0; i < 8; i++)
    {
        SCK=0;
        if(R_REG & 0x80)
        {
            MOSI=1;
        }
        else
        {
            MOSI=0;
        }		
        R_REG = R_REG << 1;
        SCK=1;
        if(MISO==1)
        {
          R_REG = R_REG | 0x01;
        }
    }
    SCK=0;
    return R_REG;
}

/******************************************************************************/
//            RF_WriteReg
//                Write Data(1 Byte Address ,1 byte data)
/******************************************************************************/
void RF_WriteReg( uint8_t reg,  uint8_t wdata)
{
    CSN=0;
    SPI_RW(reg);
    SPI_RW(wdata);
    CSN=1;
}


/******************************************************************************/
//            RF_ReadReg
//                Read Data(1 Byte Address ,1 byte data return)
/******************************************************************************/
 uint8_t ucRF_ReadReg( uint8_t reg)
{
     uint8_t tmp;
    
    CSN=0;
    SPI_RW(reg);
    tmp = SPI_RW(0);
    CSN=1;
    
    return tmp;
}

/******************************************************************************/
//            RF_WriteBuf
//                Write Buffer
/******************************************************************************/
void RF_WriteBuf( uint8_t reg, uint8_t *pBuf, uint8_t length)
{
     uint8_t j;
    CSN=0;
    j = 0;
    SPI_RW(reg);
    for(j = 0;j < length; j++)
    {
        SPI_RW(pBuf[j]);
    }
    j = 0;
    CSN=1;
}

/******************************************************************************/
//            RF_ReadBuf
//                Read Data(1 Byte Address ,length byte data read)
/******************************************************************************/
void RF_ReadBuf( uint8_t reg, unsigned char *pBuf,  uint8_t length)
{
    uint8_t byte_ctr;

    CSN=0;                    		                               			
    SPI_RW(reg);       		                                                		
    for(byte_ctr=0;byte_ctr<length;byte_ctr++)
    	pBuf[byte_ctr] = SPI_RW(0);                                                 		
    CSN=1;                                                                   		
}



/******************************************************************************/
//            RF_TxMode
//                Set RF into TX mode
/******************************************************************************/
void RF_TxMode(void)
{
    CE=0;
    RF_WriteReg(W_REGISTER + CONFIG,  0X8E);							// ��RF���ó�TXģʽ
    Delay10us();
}


/******************************************************************************/
//            RF_RxMode
//            ��RF���ó�RXģʽ��׼����������
/******************************************************************************/
void RF_RxMode(void)
{
    CE=0;
    RF_WriteReg(W_REGISTER + CONFIG,  0X8F );							// ��RF���ó�RXģʽ
    CE=1;											// Set CE pin high ��ʼ��������
    Delay2ms();
}

/******************************************************************************/
//            RF_GetStatus
//            read RF IRQ status,3bits return
/******************************************************************************/
unsigned char ucRF_GetStatus(void)
{
   uint8_t state;
   state=ucRF_ReadReg(STATUS)&0x70;
   return state;								//��ȡRF��״̬ 
}

/******************************************************************************/
//            RF_ClearStatus
//                clear RF IRQ
/******************************************************************************/
void RF_ClearStatus(void)
{
    RF_WriteReg(W_REGISTER + STATUS,0x70);							//���RF��IRQ��־ 
}

/******************************************************************************/
//            RF_ClearFIFO
//                clear RF TX/RX FIFO
/******************************************************************************/
void RF_ClearFIFO(void)
{
    RF_WriteReg(FLUSH_TX, 0);			                                		//���RF �� TX FIFO		
    RF_WriteReg(FLUSH_RX, 0);                                                   		//���RF �� RX FIFO	
}

/******************************************************************************/
//            RF_SetChannel
//                Set RF TX/RX channel:Channel
/******************************************************************************/
void RF_SetChannel( uint8_t Channel)
{    
    CE=0;
	ucCurrent_Channel = Channel;
    RF_WriteReg(W_REGISTER + RF_CH, Channel);
}

/******************************************************************************/
//            �������ݣ�
//            ������
//              1. ucPayload����Ҫ���͵������׵�ַ
//              2. length:  ��Ҫ���͵����ݳ���
//              Return:
//              1. MAX_RT: TX Failure  (Enhance mode)
//              2. TX_DS:  TX Successful (Enhance mode)
//              note: Only use in Tx Mode
//              length ͨ������ PAYLOAD_WIDTH
/******************************************************************************/

void ucRF_TxData( uint8_t *ucPayload,  uint8_t length)
{
    /*if(0==ucRF_GetStatus())                                                                        // rf free status                                                                                                                                                                   
   {
    RF_WriteBuf(W_TX_PAYLOAD, ucPayload, length); 
    CE=1;                                                                    		//rf entery tx mode start send data 
    Delay20us();                                                              		//keep ce high at least 600us
    CE=0;                                                                                     //rf entery stb3                                                        			
   } */
   uint8_t   Status_Temp;
    
    CE=1;                                                                    		//rf entery tx mode start send data 
    Delay20us();                                                             		//keep ce high at least 16us
    CE=0;
	RF_WriteBuf(W_TX_PAYLOAD, ucPayload, length);                               		//write data to txfifo        
  
	CE=1;                                                                    		//rf entery tx mode start send data 
    Delay20us();                                                             		//keep ce high at least 16us
    CE=0;
			
				                                                                   		//rf entery stb3
    while(IRQ_STATUS);                                                          		//waite irq pin low 
    Status_Temp = ucRF_ReadReg(STATUS) & 0x70;                                                  //��ȡ������ɺ��status
    RF_WriteReg(W_REGISTER + STATUS, Status_Temp);                                 		//���Status
    RF_WriteReg(FLUSH_TX,0); 
	/* if(ucRF_ReadReg(OBSERVE_TX) > 0xCF)						
    {
	RF_SetChannel(ucCurrent_Channel);					//��0 OBSERVE_TX
    } */                                                  			//�� FIFO
   
    //return Status_Temp;
   
   	
}	


/******************************************************************************/
//            ucRF_DumpRxData
//            �������յ������ݣ�
//            ������
//              1. ucPayload���洢��ȡ�������ݵ�Buffer
//              2. length:    ��ȡ�����ݳ���
//              Return:
//              1. 0: û�н��յ�����
//              2. 1: ��ȡ���յ������ݳɹ�
//              note: Only use in Rx Mode
//              length ͨ������ PAYLOAD_WIDTH
/******************************************************************************/
unsigned char ucRF_DumpRxData( uint8_t *ucPayload,  uint8_t length)
{
    if(IRQ_STATUS==1)
    {
      return 0;                                                                 		//��IRQ PINΪ�ߣ���û�н��յ�����
    }

    CE=0;
    RF_ReadBuf(R_RX_PAYLOAD, ucPayload, length);                                		//�����յ������ݶ�����ucPayload�������rxfifo
    RF_WriteReg(FLUSH_RX, 0);	
    RF_WriteReg(W_REGISTER + STATUS, 0x70);                                     		//���Status
    CE=1;                                                                    		//������ʼ����
    
    return 1;
}


////////////////////////////////////////////////////////////////////////////////

//          ���²�����RFͨ����أ��������޸�
////////////////////////////////////////////////////////////////////////////////
/******************************************************************************/
//            PN006_Initial
//                Initial RF
/******************************************************************************/
void RF_Init(void)
{
    uint8_t  BB_cal_data[]    = {0x0A,0x6D,0x67,0x9C,0x46}; 
    uint8_t  RF_cal_data[]    = {0xF6,0x37,0x5D};
    uint8_t  RF_cal2_data[]   = {0x45,0x21,0xef,0x2C,0x5A,0x50};
    uint8_t  Dem_cal_data[]   = {0x01};  
    uint8_t  Dem_cal2_data[]  = {0x0b,0xDF,0x02};   
	//IRQ_STATUS=0;
    SPI_init();
    CE=0;
	CSN=1;
    SCK=0;
    ucCurrent_Channel = DEFAULT_CHANNEL;                
    RF_WriteReg(RST_FSPI, 0x5A);								//Software Reset    			
    RF_WriteReg(RST_FSPI, 0XA5);
    
    RF_WriteReg(FLUSH_TX, 0);									// CLEAR TXFIFO		    			 
    RF_WriteReg(FLUSH_RX, 0);									// CLEAR  RXFIFO
    RF_WriteReg(W_REGISTER + STATUS, 0x70);							// CLEAR  STATUS	
    RF_WriteReg(W_REGISTER + EN_RXADDR, 0x01);							// Enable Pipe0
    RF_WriteReg(W_REGISTER + SETUP_AW,  0x03);							// address witdth is 5 bytes
    RF_WriteReg(W_REGISTER + RF_CH,     DEFAULT_CHANNEL);                                       // 2478M HZ
    RF_WriteReg(W_REGISTER + RX_PW_P0,  PAYLOAD_WIDTH);						// 8 bytes
    RF_WriteBuf(W_REGISTER + TX_ADDR,   ( uint8_t*)TX_ADDRESS_DEF, sizeof(TX_ADDRESS_DEF));	// Writes TX_Address to PN006
    RF_WriteBuf(W_REGISTER + RX_ADDR_P0,( uint8_t*)TX_ADDRESS_DEF, sizeof(TX_ADDRESS_DEF));	// RX_Addr0 same as TX_Adr for Auto.Ack   
    RF_WriteBuf(W_REGISTER + BB_CAL,    BB_cal_data,  sizeof(BB_cal_data));
    RF_WriteBuf(W_REGISTER + RF_CAL2,   RF_cal2_data, sizeof(RF_cal2_data));
    RF_WriteBuf(W_REGISTER + DEM_CAL,   Dem_cal_data, sizeof(Dem_cal_data));
    RF_WriteBuf(W_REGISTER + RF_CAL,    RF_cal_data,  sizeof(RF_cal_data));
    RF_WriteBuf(W_REGISTER + DEM_CAL2,  Dem_cal2_data,sizeof(Dem_cal2_data));
    RF_WriteReg(W_REGISTER + DYNPD, 0x00);					
    RF_WriteReg(W_REGISTER + FEATURE, 0x00);
    RF_WriteReg(W_REGISTER + RF_SETUP,  RF_POWER);						//DBM  		
  
    
#if(TRANSMIT_TYPE == TRANS_ENHANCE_MODE)      
    RF_WriteReg(W_REGISTER + SETUP_RETR,0x03);							//  3 retrans... 	
    RF_WriteReg(W_REGISTER + EN_AA,     0x01);							// Enable Auto.Ack:Pipe0  	
#elif(TRANSMIT_TYPE == TRANS_BURST_MODE)                                                                
    RF_WriteReg(W_REGISTER + SETUP_RETR,0x00);							// Disable retrans... 	
    RF_WriteReg(W_REGISTER + EN_AA,     0x00);							// Disable AutoAck 
#endif

if(PAYLOAD_WIDTH <33)											
{
	RF_WriteReg(W_REGISTER +FEATURE, 0x00);							//�л���32byteģʽ
}
else
{
	RF_WriteReg(W_REGISTER +FEATURE, 0x18);							//�л���64byteģʽ	   
}

}


/******************************************************************************/
//            		�����ز�ģʽ
/******************************************************************************/
void RF_Carrier( uint8_t ucChannel_Set)
{
    uint8_t BB_cal_data[]    = {0x0A,0x6D,0x67,0x9C,0x46}; 
    uint8_t RF_cal_data[]    = {0xF6,0x37,0x5D};
    uint8_t RF_cal2_data[]   = {0x45,0x21,0xEF,0xAC,0x5A,0x50};
    uint8_t Dem_cal_data[]   = {0xE1}; 								
    uint8_t Dem_cal2_data[]  = {0x0B,0xDF,0x02};      
    
    CE=0;
    RF_WriteReg(RST_FSPI, 0x5A);								//Software Reset    			
    RF_WriteReg(RST_FSPI, 0XA5);
    Delay200ms();
    RF_WriteReg(W_REGISTER + RF_CH, ucChannel_Set);						//���ز�Ƶ��	   
    RF_WriteReg(W_REGISTER + RF_SETUP, RF_POWER);      						//dbm
    RF_WriteBuf(W_REGISTER + BB_CAL,    BB_cal_data,  sizeof(BB_cal_data));
    RF_WriteBuf(W_REGISTER + RF_CAL2,   RF_cal2_data, sizeof(RF_cal2_data));
    RF_WriteBuf(W_REGISTER + DEM_CAL,   Dem_cal_data, sizeof(Dem_cal_data));
    RF_WriteBuf(W_REGISTER + RF_CAL,    RF_cal_data,  sizeof(RF_cal_data));
    RF_WriteBuf(W_REGISTER + DEM_CAL2,  Dem_cal2_data,sizeof(Dem_cal2_data));
    Delay5000us();	
}

/***************************************end of file ************************************/
