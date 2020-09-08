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

// Function for initialising the GPIOs for the LEDs
void initLED(void);

// Function for changing the LED state depending on the Joystick
void updateLEDValues(uint8_t joyStickStateInput,uint8_t GreenColor, uint8_t BlueColor, uint8_t RedColor );

// Function for setting the LED color
void setLed(int8_t Green,int8_t Blue, int8_t Red);
