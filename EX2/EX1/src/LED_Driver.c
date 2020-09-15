////////////////////////////////////////////////////////////////////////////////
//***************************** LED Driver ***********************************//
////////////////////////////////////////////////////////////////////////////////
/*
* This file holds all the functionality for controlling the state of the LEDs on
* the STM32f302r8 Mikrocontroller emed board.
* This file is part of the course project for Digital Instrumentation - 30021
* Author: Lucas Balling - s163954
*/
#include "30021_io.h"

void setLed(int8_t Green,int8_t Blue, int8_t Red);

void initLED(void){
    // Init GPIO used for LEDs - Set to output

    // Green - PC7
    // Blue  - PA9
    // Red   - PB4
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE); // Enable clock for GPIO Port C
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE); // Enable clock for GPIO Port B
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE); // Enable clock for GPIO Port A
     GPIO_InitTypeDef GPIO_InitStructAll; // Define typedef struct for setting pins

     GPIO_StructInit(&GPIO_InitStructAll); // Initialize GPIO struct

    // Initialise Green LED
     GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT; // Set as input
     GPIO_InitStructAll.GPIO_OType = GPIO_OType_PP;// Set to push pull
     GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_7; // Set so the configuration is on pin 4
     GPIO_Init(GPIOC, &GPIO_InitStructAll);
    // Things to initialise is
    // PC0 - needed for right movement of the joystick
    // PA4 - Needed for up movement
    // PB5 - Needed for center push
    // PC1 - Needed for left movement
    // PB0 - Needed for down movement

     // Initialise Blue LED
     GPIO_StructInit(&GPIO_InitStructAll); // Initialize GPIO struct

     GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT; // Set as input
     GPIO_InitStructAll.GPIO_OType = GPIO_OType_PP;// Set to push pull
     GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_9; // Set so the configuration is on pin 4
     GPIO_Init(GPIOA, &GPIO_InitStructAll);

     // Initialise RED LED
     GPIO_StructInit(&GPIO_InitStructAll); // Initialize GPIO struct

     GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_OUT; // Set as input
     GPIO_InitStructAll.GPIO_OType = GPIO_OType_PP;// Set to push pull
     GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_4; // Set so the configuration is on pin 4
     GPIO_Init(GPIOB, &GPIO_InitStructAll);

}

void updateLEDValues(uint8_t joyStickStateInput,uint8_t GreenColor, uint8_t BlueColor, uint8_t RedColor ){
        if (joyStickStateInput == 1){
            printf("joyStick is Pulled Up\n");
            GreenColor = 1;
            RedColor = 0;
            BlueColor = 0;
        }else if(joyStickStateInput == 2){
            printf("joyStick is Pulled Down\n");
            GreenColor = 0;
            RedColor = 1;
            BlueColor = 0;
        }else if(joyStickStateInput == 4){
            printf("joyStick is Pressed Left\n");
            GreenColor = 0;
            RedColor = 0;
            BlueColor = 1;
        }else if(joyStickStateInput == 8){
            printf("joyStick is Pressed Right\n");
            GreenColor = 1;
            RedColor = 1;
            BlueColor = 0;
        }else if(joyStickStateInput == 16){
            printf("joyStick is Pressed Center\n");
            GreenColor = 1;
            RedColor = 0;
            BlueColor = 1;
        }
        // Depending on the Joystick direction the LED state will be changed.
        setLed(GreenColor, BlueColor, RedColor);
}
void setLed(int8_t Green,int8_t Blue, int8_t Red){
    // Green - PC7
    // Blue  - PA9
    // Red   - PB4

    // Write to all LEDs
    // Green
    GPIO_WriteBit ( GPIOC, GPIO_Pin_7 , Green );
    // RED
    GPIO_WriteBit ( GPIOB, GPIO_Pin_4 , Red );
    // BLUE
    GPIO_WriteBit ( GPIOA, GPIO_Pin_9 , Blue );
}
