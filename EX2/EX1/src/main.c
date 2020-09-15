/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: Lucas Balling $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f30x_conf.h" // STM32 config
#include "30021_io.h" // Input/output library for this course
// Includes need for the GPIOs
//#include "GPIO.h"
//#include "LED_Driver.h"
#include "Timer.h"
// includes needed for the LCD control
#include "lcd.h"
#include "flash.h"
#include "string.h"
///////////////////////////////////////////////////////////////////////
// ------------------------Global Variables--------------------------//
///////////////////////////////////////////////////////////////////////
uint8_t fbuffer[512];
uint8_t number = 100;
char str[15];
uint32_t tempfloat;
uint32_t tempval;
float channel2ADC = 100.1;
float channel1ADC = 100.1;
char str3[20];
char str2[20];
///////////////////////////////////////////////////////////////////////
// -------------------------- functions ----------------------------//
///////////////////////////////////////////////////////////////////////
// Timer interrupt
void TIM2_IRQHandler(void) {
    ADC_measure_PA(1);
    ADC_measure_PA(2);
    printTextDisplay();
//Do whatever you want here, but make sure it doesn’t take too much time
 TIM_ClearITPendingBit(TIM2,TIM_IT_Update); // Clear interrupt bit
}


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
    //ADC_InitStructAll.ADC_ExternalTrigEventEdge =  ;
    //ADC_InitStructAll.ADC_DataAlign =;
    //ADC_InitStructAll.ADC_OverrunMode = ;
    //ADC_InitStructAll.ADC_AutoInjMode = ;
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

void ADC_measure_PA(uint8_t ch){

    // Setup Channel
    // CHannel Config
    if (ch == 1){
        ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_1Cycles5);
        // Read Value
        ADC_StartConversion(ADC1); // Start ADC read
        while ((ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)); // Wait for ADC read
        //for(uint32_t i = 0; i<10000;i++);
        // Read ADC Value
        channel1ADC =  ADC_GetConversionValue(ADC1); // Read the ADC value
    }else{
        ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_1Cycles5);
        // Read Value
        ADC_StartConversion(ADC1); // Start ADC read
        while ((ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)); // Wait for ADC read
        //for(uint32_t i = 0; i<10000;i++);
        // Read ADC Value
        channel2ADC =   ADC_GetConversionValue(ADC1); // Read the ADC value
    }

}

void printTextDisplay(void){
    float bias = (float) (3.3 / 4095.0);
    float temp =(float)  (channel1ADC* bias);
    channel1ADC = temp;

    channel2ADC =  channel2ADC*bias;
    sprintf(str3,"ADC - Ch 1: %f ",temp);
    sprintf(str2,"ADC - Ch 2: %f ",channel2ADC);
    lcd_write_string("Digital Instrumentation", fbuffer, 10, 0);
    lcd_write_string("Group 6", fbuffer, 40, 1);
    lcd_write_string( str3, fbuffer, 15, 2);
    lcd_write_string( str2, fbuffer, 15, 3);
    lcd_push_buffer(fbuffer);
}

void setup(void){
    init_usb_uart( 9600 ); // Initialize USB serial emulation at 9600 baud
    printf("Initialising all hardware components\n");

    // setup LCD
    init_spi_lcd();
    // Initialise tthe buffer
    memset(fbuffer,0x00,512); // Sets each element of the buffer to 0xAA - print nothing to the screen for now.
     // Initilise the display for the first time
     lcd_push_buffer(fbuffer);

     // Setup ADC
     ADC_setup_PA();

     // Setup timer
     initTimer();
}
int main(void)
{
 // Run all setup task before going into the while loop.

 setup();

 // Read ADC from channel 1 // or 2
  ADC_measure_PA(1);

 // Now we are ready to enter the While loop.
     while(1){


        // print to display
        //printTextDisplay();

        /*
        tempval = 200000;
        //tempfloat = read_float_flash(PG31_BASE,0);
        init_page_flash(PG31_BASE);
        FLASH_Unlock();
        for ( int i = 0; i < 10; i++ ){
        write_word_flash(PG31_BASE,i,tempval);
        }
        FLASH_Lock();
        */
        //tempfloat = read_float_flash(PG31_BASE,0);


        //tempval = read_word_flash(PG31_BASE,0);
        //if(tempval!=(uint32_t)0xDEADBEEF){
            //init_page_flash(PG31_BASE);
            //FLASH_Unlock();
            //write_word_flash(PG31_BASE,0,0xDEADBEEF);
            //FLASH_Lock();
        //}
        //tempval = read_hword_flash(PG31_BASE,0);



    }
}
