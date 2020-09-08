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
#include "GPIO.h"
#include "LED_Driver.h"

///////////////////////////////////////////////////////////////////////
// ------------------------Global Variables--------------------------//
///////////////////////////////////////////////////////////////////////
uint8_t joyStickState = 0;
uint8_t lastJoystickstate = 0;
uint8_t GreenColor, BlueColor, RedColor = 0;

///////////////////////////////////////////////////////////////////////
// -------------------------- functions ----------------------------//
///////////////////////////////////////////////////////////////////////
int main(void)
{
 init_usb_uart( 9600 ); // Initialize USB serial emulation at 9600 baud
 initJoystick();
 // Init LED GPIO
 initLED();
 printf("Initialising all hardware components\n"); // Show the world you are alive!
 while(1){
    // Do things in the while loop
    // Read Joystick state
    joyStickState = readJoystick();


    if(joyStickState != lastJoystickstate){
        //Print the state of the Joystick to the consol
        if (joyStickState == 1){
            printf("joyStick is Pulled Up\n");
            GreenColor = 1;
            RedColor = 0;
            BlueColor = 0;
        }else if(joyStickState == 2){
            printf("joyStick is Pulled Down\n");
            GreenColor = 0;
            RedColor = 1;
            BlueColor = 0;
        }else if(joyStickState == 4){
            printf("joyStick is Pressed Left\n");
            GreenColor = 0;
            RedColor = 0;
            BlueColor = 1;
        }else if(joyStickState == 8){
            printf("joyStick is Pressed Right\n");
            GreenColor = 1;
            RedColor = 1;
            BlueColor = 0;
        }else if(joyStickState == 16){
            printf("joyStick is Pressed Center\n");
            GreenColor = 1;
            RedColor = 0;
            BlueColor = 1;
        }
    }
    // Save last Joystick state to ensure that it doesnt print to much
    lastJoystickstate = joyStickState;

    // Depending on the Joystick direction the LED state will be changed.
    setLed(GreenColor, BlueColor, RedColor);


 }
}
