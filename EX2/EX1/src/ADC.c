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
#include "ADC.h"


// Forward declaration of global Variables
extern float channel2ADC;
extern float channel1ADC;
extern uint16_t ADC_CH1_RAW;
extern uint16_t ADC_CH2_RAW;
extern float VREFINT_DATA;


void ADC_setup_PA(void){
    // Set ADC CLock
    //uint32_t clock_Division = RCC_ADC12PLLCLK_Div8;
    RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div8);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12,ENABLE);


// USe PA the number will be selected later
    // PA0
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE); // Enable clock for GPIO Port A
     GPIO_InitTypeDef GPIO_InitStructAll; // Define typedef struct for setting pins

     GPIO_StructInit(&GPIO_InitStructAll); // Initialize GPIO struct

     GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_AN; // Set as input - analog mode
     //GPIO_InitStructAll.GPIO_PuPd = GPIO_PuPd_DOWN;// Set as pull down
     GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_0; // Set so the configuration is on pin 0
     GPIO_Init(GPIOA, &GPIO_InitStructAll);

     // PA1

     GPIO_StructInit(&GPIO_InitStructAll); // Initialize GPIO struct

     GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_AN; // Set as input - analog mode
     //GPIO_InitStructAll.GPIO_PuPd = GPIO_PuPd_DOWN;// Set as pull down
     GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_1; // Set so the configuration is on pin 0
     GPIO_Init(GPIOA, &GPIO_InitStructAll);
         // Init ADC
    ADC_InitTypeDef ADC_InitStructAll;
    ADC_StructInit(&ADC_InitStructAll);
    ADC_InitStructAll.ADC_ContinuousConvMode =  DISABLE;
    ADC_InitStructAll.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructAll.ADC_ExternalTrigConvEvent = DISABLE;

    ADC_InitStructAll.ADC_NbrOfRegChannel = 2;
     ADC_Init(ADC1, &ADC_InitStructAll);
     ADC_Cmd(ADC1, ENABLE);

     // Wait while it initliases
     while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_RDY)){}

     // Automatic Calibration
     // set internal reference voltage source and wait
    ADC_VoltageRegulatorCmd(ADC1,ENABLE);
    //Wait for at least 10uS before continuing...
    for(uint32_t i = 0; i<10000;i++);
    //Calibrate
    ADC_Cmd(ADC1,DISABLE);
    while(ADC_GetDisableCmdStatus(ADC1)){} // wait for disable of ADC
    ADC_SelectCalibrationMode(ADC1,ADC_CalibrationMode_Single);
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1)){}
    for(uint32_t i = 0; i<1000;i++);
    // Enable ADC
    ADC_Cmd(ADC1,ENABLE);
    while(!(ADC_GetFlagStatus(ADC1,ADC_FLAG_RDY)))
    {
    }

}


void ADC_Cal(void){
    ADC_VrefintCmd(ADC1,ENABLE); // setup ref voltage to channel 18
    for(uint32_t i = 0; i<10000;i++); // I think this is needed...
    // turn on ADC - 8Mhz  - 2,2us
    ADC_RegularChannelConfig(ADC1, ADC_Channel_18, 1, ADC_SampleTime_4Cycles5); // need to be 2,2us
    // Measure the internal Voltage
    ADC_StartConversion(ADC1); // Start ADC read
    while ((ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)); // Wait for ADC read
    for(uint32_t i = 0; i<10000;i++);
    // Read ADC Value
    VREFINT_DATA = ADC_GetConversionValue(ADC1); // Read the ADC value

}

void ADC_measure_PA(uint8_t ch){

    // Setup Channel
    // Channel Config
    if (ch == 1){
        ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5); //configure ADC conversion
        ADC_StartConversion(ADC1); // Start ADC conversion
        while ((ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)); // Wait for ADC read
        for(uint32_t i = 0; i<10000;i++); //ABOVE WHILE STATEMENT IS INSUFFICENT WHEN TESTED ON HARDWARE

        ADC_CH1_RAW = ADC_GetConversionValue(ADC1);
        printf("Raw Value of ADC_CH1: %u \n", ADC_CH1_RAW);
        channel1ADC =  ADC_GetConversionValue(ADC1); // Read the ADC value

    }else{
        ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_1Cycles5); //configure ADC conversion
        ADC_StartConversion(ADC1); // Start ADC conversion
        while ((ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)); // Wait for ADC read
        for(uint32_t i = 0; i<10000;i++); //ABOVE WHILE STATEMENT IS INSUFFICENT WHEN TESTED ON HARDWARE

        ADC_CH2_RAW = ADC_GetConversionValue(ADC1);
        printf("Raw Value of ADC_CH2: %u \n", ADC_CH2_RAW);
        channel2ADC =   ADC_GetConversionValue(ADC1); // Read the ADC value
    }

}
