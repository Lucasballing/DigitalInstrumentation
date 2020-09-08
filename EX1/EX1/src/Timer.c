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
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler(void) {
        if(stopState == 0){
        time1.hs++;
        if (time1.hs == 100){
            time1.s++;
            time1.hs = 0;

        }
        if (time1.s == 60){
            time1.m++;
            time1.s = 0;

        }
        if (time1.m == 60){
            time1.h++;
            time1.m = 0;
        }
        }


//Do whatever you want here, but make sure it doesn’t take too much time
 TIM_ClearITPendingBit(TIM2,TIM_IT_Update); // Clear interrupt bit
}

 void initstopwatch(){

    time1.hs = 0;
    time1.s = 0;
    time1.m = 0;
    time1.h = 0;
}


void PrintStopwatch(){

    uint8_t joyStickStateLocal = readJoystick();



    if(joyStickStateLocal == 16){
        printf("joyStick is Pressed Center\n");
        printf("%d",time1.hs);
        printf(":");
        printf("%d",time1.s);
        printf(":");
        printf("%d",time1.m);
        printf(":");
        printf("%d",time1.h);
        if(stopState==1){
            stopState=0;
        }
        else{
          stopState = 1;
        }
    }

    else if(joyStickStateLocal == 4){

        printf("\n%d",time1.hs);
        printf(":%d",time1.s);
        printf(":%d",time1.m);
        printf(":%d",time1.h);
    }
    else if(joyStickStateLocal == 8){
        printf("\n");
        printf("%d",time1.h);
        printf(":%d",time1.m);
        printf(":%d",time1.s);
        printf(":%d",time1.hs);
    }
    else if(joyStickStateLocal == 2){
        printf("%c[%d%c", 0x1B,2, 0x4a);
        initstopwatch();
        stopState = 1;
    }
    else{
        printf("%c[%d;%d%c", 0x1B, 1,  1, 0x66);
        printf("%d",time1.hs);
        printf(":%d",time1.s);
        printf(":%d",time1.m);
        printf(":%d",time1.h);

    }
}
