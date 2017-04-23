#include <reg51.h>
#include "delay.h"
#include<intrins.h>
#include"stdio.h"
#include"RF.h"

#define uint8_t unsigned char
sbit KEY_1=P3^0;
sbit KEY_2=P3^1;

/******************************************************************************/
//            main
//            main function body
/******************************************************************************/
void main(void)
{  
    unsigned char ucData[8]={1,2,3,4,5,6,7,8},TransResult = 0;
	RF_Init();
    
    if(KEY_1==0)
	{
		
	    IRQ_STATUS=0;
		Delay2ms();
		RF_TxMode();
		if(KEY_1==0)
		{
			while(1)
		    {
				ucRF_TxData(ucData,  PAYLOAD_WIDTH);
		       	Delay200ms();
				TEST_LED=1;
				Delay200ms();
				TEST_LED=0;
			}
		}
	}	

	if(KEY_2==0)
	{
		Delay2ms();
		RF_RxMode();
		if(KEY_2==0)
		{
			while(1)
		    {
		        
			TransResult = ucRF_DumpRxData(ucData, PAYLOAD_WIDTH);
			if(TransResult)
			{
				OK_LED=0;
				Delay200ms();
				OK_LED=1;
				Delay200ms();
			}
			}
		}
	}
	
}
