////////////////////////////////////////////////////////////////////////////////
//****************************** GPIO ****************************************//
////////////////////////////////////////////////////////////////////////////////
/*
* This file holds all the functionality for controlling the state of the GPIO on
* the STM32f302r8 Microcontroller.
* This file is part of the course project for Digital Instrumentation - 30021
* Authors:  Christoffer Johansen - s183813
            Edgar Kipans - s144780
*/

#include "30021_io.h"


// This function initialises all the GPIO's needed for TIM16 CH1
void GPIO_set_AF1_PA6(){

    //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE); // Enable clock for GPIO Port A
    GPIO_InitTypeDef GPIO_InitStructAll;
    GPIO_StructInit(&GPIO_InitStructAll);
     // Then set things that are not default.
    GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructAll.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructAll.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructAll);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6,GPIO_AF_1);



}


