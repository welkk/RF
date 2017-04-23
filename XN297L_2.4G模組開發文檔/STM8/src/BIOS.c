#include "stm8l15x.h"

#include "LED.h"
#include "Key.h"
#include "App.h"
#include "appsub.h"
#include "stm8l15x_tim2.h"
#include "stm8l15x_tim3.h"
#include "stm8l15x_dma.h"
#include "stm8l15x_usart.h"
#include "stdio.h"
#include "CPU.H"
#include "VCPU.H"
//#include "CONFIG.H"
#include "BIOS.H"


unsigned char Work_State;
uint8_t   ucTest_Mode ;

/******************************************************************************/
//            Cpu_initial
//            Initial cpu main clock: 16MHz HIRC
/******************************************************************************/

void Oscillator_Init(void)
{
    CLK_DeInit();
    CLK_HSICmd(ENABLE);
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
    CLK_ClockSecuritySystemEnable();
}

/******************************************************************************/
//            IO_Port_Initial
//            Initial all gpio: set RESET_PIN to Output
/******************************************************************************/
void  IO_Init(void)
{
    //CLK_PeripheralClockConfig(CLK_Peripheral_TypeDef CLK_Peripheral, FunctionalState NewState);
    GPIO_DeInit(GPIOA);  
    GPIO_DeInit(GPIOB);   
    GPIO_DeInit(GPIOC);   
    GPIO_DeInit(GPIOD);
    GPIO_Init( GPIOA, GPIO_Pin_4, GPIO_Mode_In_PU_No_IT);                       //TX/RX PIN input pulling high
}

/******************************************************************************/
//            Timer_Iniital
//            Initial all timers: 
//                Timer2: 8ms AutoReload with Interrupt
//                Timer4: 80ms AutoReload with Interrupt
/******************************************************************************/
void Timer_Init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);  
    TIM2_Cmd(DISABLE);
    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_Prescaler_16, TIM2_CounterMode_Up, 8000);
    TIM2_ARRPreloadConfig(ENABLE);
    TIM2_ClearFlag(TIM2_FLAG_Update);
    TIM2_ITConfig(TIM2_IT_Update, ENABLE);
    TIM2_SetCounter(0);

/*    
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);  
    TIM3_Cmd(DISABLE);
    TIM3_DeInit();
    TIM3_TimeBaseInit(TIM3_Prescaler_128, TIM3_CounterMode_Up, 10000);
    TIM3_ARRPreloadConfig(ENABLE);
    TIM3_ClearFlag(TIM3_FLAG_Update);
    TIM3_ITConfig(TIM3_IT_Update, ENABLE);
    TIM3_SetCounter(0);
*/
}


/******************************************************************************/
//            Usart_Initial
//            Initial UART1: 
//                UART1: Enable for Debug,without Interrupt
/******************************************************************************/
void Usart_Init(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE); 
    USART_Init(USART1,UART_BOUNDRATE , USART_WordLength_8b, USART_StopBits_1,USART_Parity_No, USART_Mode_Tx);  
    USART_Cmd(USART1, ENABLE);
}


/******************************************************************************/
//            Status_initial
//            Initial Work status: 
//                Judge work as TX or RX in normal mode
 //               Judge work in carry mode or TRX mode
/******************************************************************************/
void Status_Init(void)
{
    if(TRX_PIN_MODE)
    {
        Work_State = FUNCTION_RX;
        ucRFAPI_STATE = RFAPI_RX;  
        RF_RxMode();                                                              //work as rx 
#ifdef  DEBUG_EN 
        printf("Initial RX!\r\n");
#endif 
        LED_ChangeFont(CHANNEL_TABLE[Channel_Index],LED_CHANNEL_MODE);
        DISP_Status = LED_CHANNEL_MODE;
    }
    else
    {
        Work_State = FUNCTION_TX;
        ucRFAPI_STATE = RFAPI_TX;
        ucTXMode = MODE_TX_SINGLE;
        RF_TxMode();                                                              //work as tx 
#ifdef  DEBUG_EN 
        printf("Initial TX!\r\n");
#endif 
        LED_ChangeFont(ucTXMode,LED_FUNCTION_MODE);
        DISP_Status = LED_FUNCTION_MODE;
    }
   
    if(Key_Rd() == 0x01)
    {
        ucTest_Mode = TEST_CARRIER_MODE;     
    }
    else
    {
        ucTest_Mode = TEST_DEFAULT_MODE;      
    }
}

/******************************************************************************/
//            Sys_Initial
//            Initial All periaral device and IO for System 
/******************************************************************************/
void Sys_Init(void)
{
    IO_Init();
#ifdef  DEBUG_EN
    Usart_Init();
#endif
    Key_Init();
    LED_Init();
    RF_Init();
    Timer_Init();
    Status_Init();
}

/******************************************************************************/
//            Sys_Start
//            prepare to start system:
//                Enable Timer,Enable ExtInt
//                Enable Interrupt
/******************************************************************************/
void Sys_Start(void)
{
    TIM2_Cmd(ENABLE);
    EXTI_ClearITPendingBit(EXTI_IT_PortD);
    
}
/***********************************************************************/
void BIOS_Init(void)
{
  
    __disable_interrupt();
 
 /****cpu  int*********/ 
    Oscillator_Init();
    IO_Init();
#ifdef  DEBUG_EN
    Usart_Init();
#endif
    Key_Init();
    LED_Init();
    Timer_Init();
   __enable_interrupt();    
/*****rf init***********/    
    RF_Init();
    Status_Init();
/*****clock run********/   
    Sys_Start();
}