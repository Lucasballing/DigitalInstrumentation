////////////////////////////////////////////////////////////////////////////////
//****************************** Timer ****************************************//
////////////////////////////////////////////////////////////////////////////////
/*
* This file holds the driver for the timer(s)
* the STM32f302r8 Microcontroller.
* This file is part of the course project for Digital Instrumentation - 30021
* Authors:  Christoffer Johansen - s183813
            Edgar Kipans - s144780
*/
#include "stm32f30x_conf.h"
#include "30021_io.h"
#include "GPIO.h"
#include "Timer.h"

void timer16_clock_init(){

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16,ENABLE);

    GPIO_set_AF1_PA6();

    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    TIM_TimeBaseStructInit(&TIM_InitStructure);
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStructure.TIM_Period = 255;
    TIM_InitStructure.TIM_Prescaler = 24;
    TIM_TimeBaseInit(TIM16,&TIM_InitStructure);
    // NVIC for timer
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
    TIM_OC1Init(TIM16,&TIM_OCInitStruct);



    TIM_OC1PreloadConfig(TIM16,TIM_OCPreload_Enable);
    TIM_SetCompare1(TIM16,200);
    TIM_CtrlPWMOutputs(TIM16, ENABLE);
    TIM_Cmd(TIM16,ENABLE);

}

