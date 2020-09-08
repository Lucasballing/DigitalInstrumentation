////////////////////////////////////////////////////////////////////////////////
//****************************** Timer ****************************************//
////////////////////////////////////////////////////////////////////////////////
/*
* This file holds the diver for the timer
* the STM32f302r8 Mikrocontroller.
* This file is part of the course project for Digital Instrumentation - 30021
* Author: Christoffer Frost- s183813
*/

#include "30021_io.h"
#include "GPIO.h"

//Timer set to 100Hz at highest priority
void initTimer(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    TIM_TimeBaseStructInit(&TIM_InitStructure);
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStructure.TIM_Period = 63;
    TIM_InitStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInit(TIM2,&TIM_InitStructure);
    // NVIC for timer
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler(void) {

        if(stopState=0){
        time.hs++;
        if (time.hs == 100){
            time.s++;
            time.hs = 0;

        }
        if (time.s == 60){
            time.m++;
            time.s = 0;

        }
        if (time.m == 60){
            time.h++;
            time.m = 0;
        }
        }


//Do whatever you want here, but make sure it doesn’t take too much time
 TIM_ClearITPendingBit(TIM2,TIM_IT_Update); // Clear interrupt bit
}

 void initstopwatch(){

    time.hs = 0;
    time.s = 0;
    time.m = 0;
    time.h = 0;
    time.s2 = 0;
    time.hs2 = 0;
}

void PrintStopwatch(){

    joyStickState = readJoystick();



    if(joyStickState == 16){
        printf("joyStick is Pressed Center\n");
        printf("%d",time.hs);
        printf(":");
        printf("%d",time.s);
        printf(":");
        printf("%d",time.m);
        printf(":")
        printf("%d",time.h);
        if(stopState==1){
            stopState=0;
        }
        else{
          stopState = 1;
        }

    else if(joyStickState == 4){

        printf("\n%d",time.hs);
        printf(":%d",time.s);
        printf(":%d",time.m);
        printf(":%d",time.h);
    }
    else if(joyStickState == 8){
        printf("\n")
        printf("%d",time.hs);
        printf(":%d",time.s);
        printf(":%d",time.m);
        printf(":%d",time.h);
    }
    else if(joyStickState == 2){
        clrscr();
        initstopwatch();
        stopState = 1;
    }
    else{
        printf("%d",time.hs);
        printf(":%d",time.s);
        printf(":%d",time.m);
        printf(":%d",time.h);
    }
}
