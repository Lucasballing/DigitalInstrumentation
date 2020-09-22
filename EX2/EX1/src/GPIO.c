////////////////////////////////////////////////////////////////////////////////
//****************************** GPIO ****************************************//
////////////////////////////////////////////////////////////////////////////////
/*
* This file holds all the functionality for controlling the state of the GPIO on
* the STM32f302r8 Mikrocontroller.
* This file is part of the course project for Digital Instrumentation - 30021
* Author: Lucas Balling - s163954
*/
#include "30021_io.h"

// init for the Joystick
// This function initialises all the GPIO's needed to control the Joystick on the
void initJoystick(void){
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE); // Enable clock for GPIO Port C
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE); // Enable clock for GPIO Port B
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE); // Enable clock for GPIO Port A
     GPIO_InitTypeDef GPIO_InitStructAll; // Define typedef struct for setting pins

     GPIO_StructInit(&GPIO_InitStructAll); // Initialize GPIO struct

     GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_IN; // Set as input
     GPIO_InitStructAll.GPIO_PuPd = GPIO_PuPd_DOWN;// Set as pull down
     GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_4; // Set so the configuration is on pin 4
     GPIO_Init(GPIOA, &GPIO_InitStructAll);
    // Things to initialise is
    // PC0 - needed for right movement of the joystick
    // PA4 - Needed for up movement
    // PB5 - Needed for center push
    // PC1 - Needed for left movement
    // PB0 - Needed for down movement

     // PC0
     GPIO_StructInit(&GPIO_InitStructAll); // Initialize GPIO struct

     GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_IN; // Set as input
     GPIO_InitStructAll.GPIO_PuPd = GPIO_PuPd_DOWN;// Set as pull down
     GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_0; // Set so the configuration is on pin 4
     GPIO_Init(GPIOC, &GPIO_InitStructAll);

     // PB5
     GPIO_StructInit(&GPIO_InitStructAll); // Initialize GPIO struct

     GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_IN; // Set as input
     GPIO_InitStructAll.GPIO_PuPd = GPIO_PuPd_DOWN;// Set as pull down
     GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_5; // Set so the configuration is on pin 4
     GPIO_Init(GPIOB, &GPIO_InitStructAll);

     // PC1
     GPIO_StructInit(&GPIO_InitStructAll); // Initialize GPIO struct

     GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_IN; // Set as input
     GPIO_InitStructAll.GPIO_PuPd = GPIO_PuPd_DOWN;// Set as pull down
     GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_1; // Set so the configuration is on pin 4
     GPIO_Init(GPIOC, &GPIO_InitStructAll);

     // PB0
     GPIO_StructInit(&GPIO_InitStructAll); // Initialize GPIO struct

     GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_IN; // Set as input
     GPIO_InitStructAll.GPIO_PuPd = GPIO_PuPd_DOWN;// Set as pull down
     GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_0; // Set so the configuration is on pin 4
     GPIO_Init(GPIOB, &GPIO_InitStructAll);


}

void GPIO_set_AF1_PA6(){


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



uint8_t readJoystick(void){
    uint8_t up, down, center, left, right = 0;
    // Read all the GPIO states
    up = GPIO_ReadInputDataBit ( GPIOA, GPIO_Pin_4);
    down = GPIO_ReadInputDataBit ( GPIOB, GPIO_Pin_0);
    center = GPIO_ReadInputDataBit ( GPIOB, GPIO_Pin_5);
    left = GPIO_ReadInputDataBit ( GPIOC, GPIO_Pin_1);
    right = GPIO_ReadInputDataBit ( GPIOC, GPIO_Pin_0);
    // Return the states
    uint8_t returnValue = 0;
    returnValue = returnValue | up; // Set the 0 bit to up
    returnValue = returnValue | (down << 1); // Set the 1 bit to down
    returnValue = returnValue | (left << 2); // Set the 2 bit to left
    returnValue = returnValue | (right << 3); // Set the 3 bit to right
    returnValue = returnValue | (center << 4); // Set the 4 bit to center

    return returnValue;




}
