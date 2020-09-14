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
#include "Timer.h"
///////////////////////////////////////////////////////////////////////
// ------------------------Global Variables--------------------------//
///////////////////////////////////////////////////////////////////////
uint8_t joyStickState = 0;
uint8_t lastJoystickstate = 1;
uint8_t GreenColor, BlueColor, RedColor = 0;
uint8_t stopState = 0;
uint8_t joyStickStateLocal = 0;
uint8_t temp =0;
///////////////////////////////////////////////////////////////////////
// -------------------------- functions ----------------------------//
///////////////////////////////////////////////////////////////////////
void EXTI0_IRQHandler(void){
 // Simply make a light show: - Change colors every 2 second.
    printf("The Interupt is called!\n Now we make a real crazy light show for you guys ;) \n");
    for (int8_t i = 0;i<10; i++){
            if( i < 3){
                GreenColor = 1;
                RedColor = 0;
                BlueColor = 1;
            }else if(i < 6){
                GreenColor = 1;
                RedColor = 0;
                BlueColor = 0;
            } else {
                GreenColor = 0;
                RedColor = 0;
                BlueColor = 1;
            }

    // Depending on the Joystick direction the LED state will be changed.
    setLed(GreenColor, BlueColor, RedColor);
    // Use a for loop to
    for (int j = 0;j<1000000; j++){
     // Do nothing - Just count.
    }
    }
 //Clear the interrupt pending bit
EXTI_ClearITPendingBit(EXTI_Line0);
}

void  init_interrupt(void){
    // Setup of interrupt rutines:
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource0); // sets port B pin 5 to the interrupts

    // define and set setting for EXTI
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line0; // line 5 see [RM p. 215]
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_Init(&EXTI_InitStructure);

    // setup NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}

/*
void init_PulseWidthMeas(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM2,ENABLE);
    // Set the time base unit
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    TIM_TimeBaseInitTypeDef TIM_ICInitStructure;
    TIM_TimeBaseStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = 0;
    TIM_ICInitStructure.TIM_ICPolarity = 0;
    TIM_ICInitStructure.TIM_ICFilter = 0;
    TIM_ICInitStructure..TIM_ICPrescaler = 0;
    TIM_PWMIConfig(TIM2, &TIM_ICInitStructure) ;
    TIM_ITConfig(TIM2, TIM_IT_CC2);
    TIM_Cmd(ENABLE);


// Use this function to read the value
TIM_GetCapturex(TIMx);
}
*/
void GPIO_set_AF_PAx(void){
     // Init GPIO - Here we use GPIOA pin 0 for the
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE); // Enable clock for GPIO Port A
     GPIO_InitTypeDef GPIO_InitStructAll; // Define typedef struct for setting pins

     GPIO_StructInit(&GPIO_InitStructAll); // Initialize GPIO struct

     GPIO_InitStructAll.GPIO_Mode = GPIO_Mode_AF; // Set as input
     GPIO_InitStructAll.GPIO_PuPd = GPIO_PuPd_DOWN;// Set as pull down
     GPIO_InitStructAll.GPIO_Pin = GPIO_Pin_0; // Set so the configuration is on pin 4
     GPIO_Init(GPIOA, &GPIO_InitStructAll);
}

void setup(void){
    init_usb_uart( 9600 ); // Initialize USB serial emulation at 9600 baud
    printf("Initialising all hardware components\n");

    // Initialise the Joystick
    initJoystick();

    // Init LED GPIO
    initLED();

    // init interrupts
    init_interrupt();

    //init timer
    initTimer();
    initstopwatch();

    // Setup exericee 1-6 - This exercise wwas not completed
    // init_PulseWidthMeas();
}
int main(void)
{
 // Run all setup task before going into the while loop.
 setup();

 // Now we are ready to enter the While loop.
 while(1){
    // Read Joystick state
    joyStickState = readJoystick();

    // Debounce Logic
    if ( (joyStickState >> 4) == 1) {
        // Check if the buttom is still pressed after a short loop
        for (int i=0; i<10000; i++) {
            // Do nothing - This part is used as an delay function
        }
        temp = readJoystick();
        if ( (temp >> 4) == 1) {
            // Do nothing
        }else{
            joyStickState &= 1+ (1 << 1) + (1 << 2) + (1 << 3);
        }
    }

    if(joyStickState != lastJoystickstate){
        PrintStopwatch(joyStickState);
    }
    // Logic for changing color depending on how the joystick
    if((joyStickState != lastJoystickstate) && joyStickState){
        //Print the state of the Joystick to the console
        updateLEDValues( joyStickState, GreenColor, BlueColor, RedColor );
        // Save last Joystick state to ensure that it doesn't print to much (Only change state if the state has changed)
        lastJoystickstate = joyStickState;
    }
 }
}
