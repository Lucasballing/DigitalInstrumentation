////////////////////////////////////////////////////////////////////////////////
//****************************** ADC ****************************************//
////////////////////////////////////////////////////////////////////////////////
/*
* This file holds the diver for the ADC
* the STM32f302r8 Mikrocontroller.
* This file is part of the course project for Digital Instrumentation - 30021
* Author: Lucas Balling
*/
#include "stm32f30x_conf.h"
#include "30021_io.h"


void ADC_setup_PA(void);

void ADC_Cal(void);

void ADC_measure_PA(uint8_t ch);
