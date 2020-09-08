//////////////////////////////////////////////////////
/*
* This file holds all the functionality for controlling the state of the GPIO on
* the STM32f302r8 Mikrocontroller.
* This file is part of the course project for Digital Instrumentation - 30021
* Author: Lucas Balling - s163954
* File type: HEADER File
*/
#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f30x_conf.h"
#include "30021_io.h"
extern uint8_t stopState;
/////////////////// Exported Struct ////////////////////
struct time {
    uint8_t h;
    uint16_t m;
    uint16_t s;
    uint16_t hs;
};
struct time time1;
/////////////////////////////////////////////////////////////
///////////////// function declaration //////////////////////
/////////////////////////////////////////////////////////////
void initTimer();
void initstopwatch();
void PrintStopwatch();
#endif


