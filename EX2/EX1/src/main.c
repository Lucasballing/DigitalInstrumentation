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
#define VREFINT_CAL *((uint16_t*) ((uint32_t) 0x1FFFF7BA)) //calibrated at 3.3V@ 30C



#include "stm32f30x_conf.h" // STM32 config
#include "30021_io.h" // Input/output library for this course
#include "Timer.h"
// includes needed for the LCD control
#include "lcd.h"
#include "flash.h"
#include "string.h"

// Include ADC header file
#include "ADC.h"

///////////////////////////////////////////////////////////////////////
// ------------------------Global Variables--------------------------//
///////////////////////////////////////////////////////////////////////
uint8_t fbuffer[512];
uint32_t tempfloat;
uint32_t tempval;
float channel2ADC = 100.1;
float channel1ADC = 100.1;
float VREFINT_DATA = 3.30;

///////////////////////////////////////////////////////////////////////
// -------------------------- functions ----------------------------//
///////////////////////////////////////////////////////////////////////
// Timer interrupt
void TIM2_IRQHandler(void) {
    for(uint32_t i = 0; i < 10000; i++);

    // Measure on the ADCs different channels
    ADC_measure_PA(1);
    ADC_measure_PA(2);

    // Update Display
    printTextDisplay();

    //Do whatever you want here, but make sure it doesn’t take too much time
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update); // Clear interrupt bit
}

void printTextDisplay(void){

    // Initialise and declare Char array for string storage
    char str5[20];
    char str4[20];
    char str3[20];
    char str2[20];
    // Run function for measuring internal voltage.
    ADC_Cal();

    // Print ADC step size
    float VDDA = (float) (3.3 *( (float) (VREFINT_CAL) / VREFINT_DATA) );
    float VCH1 = (float) ( (VDDA / 4095.0) * channel1ADC ); // Print the step size and do it for the other channel too
    float VCH2 = (float) ( (VDDA / 4095.0) * channel2ADC );

    // DCalculate the bias
    float bias = (float) (3.3 / 4095.0);

    // Convert the ADC values to voltages
    channel1ADC = channel1ADC * bias;
    channel2ADC = channel2ADC * bias;

    // Convert numbers to string Formatting
    sprintf(str5,"ADC STEP 1: %1.3f ",VCH1);
    sprintf(str4,"ADC STEP 2: %1.3f ",VCH2);
    sprintf(str3,"ADC - Ch 1: %1.3f ",channel1ADC);
    sprintf(str2,"ADC - Ch 2: %1.3f ",channel2ADC);

    // Send bytes to the display
    lcd_write_string( str5, fbuffer, 15, 0);
    lcd_write_string( str4, fbuffer, 15, 1);
    lcd_write_string( str3, fbuffer, 15, 2);
    lcd_write_string( str2, fbuffer, 15, 3);
    // Push buffer to the display
    lcd_push_buffer(fbuffer);
}

void setup(void){
    init_usb_uart( 9600 ); // Initialize USB serial emulation at 9600 baud
    printf("Initialising all hardware components\n");

    // setup LCD
    init_spi_lcd();

    // Initialise the buffer
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

        //  Read and Write from flash
        tempval = 200000;
        tempfloat = read_float_flash(PG31_BASE,0);
        init_page_flash(PG31_BASE);
        FLASH_Unlock();
        for ( int i = 0; i < 10; i++ ){
        write_word_flash(PG31_BASE,i,tempval);
        }
        FLASH_Lock();

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
