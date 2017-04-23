//============================================================================//
//  * @file           APPSub.c
//  * @author         Shi Zheng
//  * @version        V1.1.0
//  * @date           Aug/12/2013
//  * @brief          Main program body
//  * @modify user:   Shizheng
//  * @modify date:   Aug/12/2013
//============================================================================//
/* Includes ------------------------------------------------------------------*/
#include "APPSub.h"
#include "stm8l15x.h"
#include "APP.H"
#include "Key.h"
#include "LED.h"
#include "stdio.h"



/******************************************************************************/
//            Sleep_Mode
//                set mcu into sleep mode
/******************************************************************************/
void Sleep_Mode(void)
{
#if DEBUG_EN 
    printf("MCU in sleep\r\n");
#endif
    __halt();
#if DEBUG_EN 
    printf("MCU out sleep\r\n");
#endif
}

//*********************************end of file********************************//
