/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: Christoffer Johansen, Edgars Kipans $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/

#include "stm32f30x_conf.h" // STM32 config
#include "30021_io.h" // Input/output library for this course
#include "Timer.h"
// Includes need for the GPIOs
#include "GPIO.h"
// includes needed for the LCD control
//#include "lcd.h"
//#include "flash.h"
//#include "string.h"
#include "Stepper.h"

// Include ADC header file
//#include "ADC.h"

///////////////////////////////////////////////////////////////////////
// ------------------------Global Variables--------------------------//
///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
// -------------------------- functions ----------------------------//
///////////////////////////////////////////////////////////////////////


void setup(void){
    init_usb_uart( 9600 ); // Initialize USB serial emulation at 9600 baud
    printf("\n\n Initialising all hardware components\n");

    timer16_clock_init();
}


int main(void)
{
//Run all setup task before going into the while loop.
setup();


//Stops the code ontil the something is pressed
serialcom();




 // Now we are ready to enter the While loop.
while(1){


    for(uint32_t i = 0; i < 2000000; i++)
    {
        //delay
    }

    printf("Running Main \n");


        }
}
