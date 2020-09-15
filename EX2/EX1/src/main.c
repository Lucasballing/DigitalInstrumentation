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
//#include "Timer.h"
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
///////////////////////////////////////////////////////////////////////
// -------------------------- functions ----------------------------//
///////////////////////////////////////////////////////////////////////
void printTextDisplay(void){
    sprintf(str,"Number  %d",number);
    lcd_write_string("Digital Instrumentation", fbuffer, 10, 0);
    lcd_write_string("Group 6", fbuffer, 40, 1);
    lcd_write_string("The display is easy", fbuffer, 10, 2);
    lcd_write_string( str, fbuffer, 25, 3);
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
}
int main(void)
{
 // Run all setup task before going into the while loop.
 setup();

 // Now we are ready to enter the While loop.
     while(1){
        // print to display
        printTextDisplay();
        tempval = 200000;
        //tempfloat = read_float_flash(PG31_BASE,0);
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
