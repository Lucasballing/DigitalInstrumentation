////////////////////////////////////////////////////////////////////////////////
//****************************** Timer ****************************************//
////////////////////////////////////////////////////////////////////////////////
/*
* This file holds the diver for the timer
* the STM32f302r8 Mikrocontroller.
* This file is part of the course project for Digital Instrumentation - 30021
* Author: Christoffer Frost- s183813
*/
#include "stm32f30x_conf.h"
#include "30021_io.h"
#include "GPIO.h"
#include "Timer.h"
extern uint8_t joyStickStateLocal;
//Timer set to 100Hz at highest priority
void initTimer(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    TIM_TimeBaseStructInit(&TIM_InitStructure);
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStructure.TIM_Period = 6300000; // Double check this
    TIM_InitStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInit(TIM2,&TIM_InitStructure);
    // NVIC for timer
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2,ENABLE);
}
/*
void TIM2_IRQHandler(void) {
 TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

 ICValue1 = TIM_GetCapture1(TIM2); // Period
 ICValue2 = TIM_GetCapture2(TIM2); // Duty/Width
 ICValid = 1;
}
*/
// - Uncomment old Timer IRQ for the stop watch.


 void initstopwatch(){
    time1.hs = 0;
    time1.s = 0;
    time1.m = 0;
    time1.h = 0;
}

void PrintWatch(){
        printf("\n%d",time1.h);
        printf(":%d",time1.m);
        printf(":%d",time1.s);
        printf(":%d",time1.hs);
}

void PrintStopwatch(uint8_t joyStickStateLocal){

    if(joyStickStateLocal == 16){
        printf("joyStick is Pressed Center\n");
        PrintWatch();
        if(stopState == 1){
            stopState = 0;
        }
        else if(stopState == 0){
            stopState = 1;
        }
        printf("joyStick is Pressed Center\n");
    }

    else if(joyStickStateLocal == 4){
        if(stopState == 0){
            PrintWatch();
        }
    }
    else if(joyStickStateLocal == 8){
        if(stopState == 0){
            PrintWatch();
        }
    }
    else if(joyStickStateLocal == 2){
        //printf("%c[%d%c", 0x1B,2, 0x4a);
        initstopwatch();
        stopState = 1;
    }
    else{
        //printf("%c[%d;%d%c", 0x1B, 1,  1, 0x66);
        if(stopState == 0){
            PrintWatch();
        }

    }
}
