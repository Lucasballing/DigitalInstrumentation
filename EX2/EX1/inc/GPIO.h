//////////////////////////////////////////////////////
/*
* This file holds all the functionality for controlling the state of the GPIO on
* the STM32f302r8 Mikrocontroller.
* This file is part of the course project for Digital Instrumentation - 30021
* Author: Lucas Balling - s163954
* File type: HEADER File
*/
#ifndef _GPIO_H_
#define _GPIO_H_


/////////////////// Private Functions //////////////////////









/////////////////////////////////////////////////////////////
///////////////// function declaration //////////////////////
/////////////////////////////////////////////////////////////

/* initJoystick
* Used to initialise all the GPIOs on the board
* This function has no input parameters
*/
void initJoystick(void);

/* readJoystick
* Used to read the state of the GPIOs on the board used for the Joystick
* This function has no input parameters
* Output- the state of the GPIO in a special format defined in the exercise.
*/
uint8_t readJoystick(void);
#endif
