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
// Includes need for the GPIOs
#include "GPIO.h"
// includes needed for the LCD control
#include "lcd.h"
#include "flash.h"
#include "string.h"
#include "Stepper.h"

// Include ADC header file
#include "ADC.h"

///////////////////////////////////////////////////////////////////////
// ------------------------Global Variables--------------------------//
///////////////////////////////////////////////////////////////////////
uint8_t fbuffer[512];
uint32_t tempfloat;
uint32_t tempval;
float channel2ADC = 0;
float channel1ADC = 0;
float VREFINT_DATA = 3.30;
volatile uint8_t CalibFlag = 0;
volatile uint8_t PrintFlag = 0;
float CALFACT1 = 1.0; //will be loaded from flash, initialized for testing only
float CALFACT2 = 1.0;
uint16_t ADC_CH1_RAW = 0;
uint16_t ADC_CH2_RAW = 0;

///////////////////////////////////////////////////////////////////////
// -------------------------- functions ----------------------------//
///////////////////////////////////////////////////////////////////////
// Timer interrupt
void TIM2_IRQHandler(void) {

    PrintFlag = 1;
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update); // Clear interrupt bit
}

void EXTI0_IRQHandler(void)
{
    printf("Joystick down interrupt called! \n");  // SET Calibration Flag - Checked by main
    CalibFlag = 1;
    EXTI_ClearITPendingBit(EXTI_Line0);  //Clear the interrupt pending bit
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
    printf("voltage on Ch.1: %fV \n", channel1ADC);

    channel2ADC = channel2ADC * bias;
    printf("voltage on Ch.2: %fV \n", channel2ADC);

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

void init_interrupt(void){
    // Setup of interrupt rutines:
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,EXTI_PinSource0); // sets port B pin 0 to the interrupts

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

void setup(void){
    init_usb_uart( 9600 ); // Initialize USB serial emulation at 9600 baud
    printf("\n\n Initialising all hardware components\n");

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

void LoadCorrectionFactor(void)
{

        //  READ CALFACT1 from FLASH:
        CALFACT1 = read_float_flash(PG31_BASE, 0);

         //  READ CALFACT2 from FLASH:
        CALFACT2 = read_float_flash(PG31_BASE, 1); //offset of 4 bytes for a float?


/* Checking channel 1 correction factor validity */
	if( (0.5 < CALFACT1) && (CALFACT1 < 1.5) )
	{
		printf("Correction Factor for Ch.1: -> VALID RANGE (%f) \n", CALFACT1);
	}
	else
	{
		printf("Correction Factor for Ch.1: -> INVALID RANGE (%f), Auto-set to 1.0 \n", CALFACT1);
        CALFACT1 = 1.0;
	}

/* Checking channel 2 correction factor validity */
	if( (0.5 < CALFACT2) && (CALFACT2 < 1.5) )
	{
		printf("Correction Factor for Ch.2: -> VALID RANGE (%f) \n", CALFACT2);
	}
	else
	{
		printf("Correction Factor for Ch.2: -> INVALID RANGE (%f), Auto-set to 1.0 \n", CALFACT2);
        CALFACT2 = 1.0;
	}

}

void BurstMeasurement(uint32_t *Accum1, uint32_t *Accum2)
{
	uint32_t temp1 = 0;
	uint32_t temp2 = 0;

/* 16 consecutive ADC measurements for channel 1 */
//ADC_measure_PA(1); //dummy ADC conversion - first conversion in a 'burst' on CH1 defaults to VDDA (unknown why)
for(uint8_t i = 0; i <= 15; i++)
{
	ADC_measure_PA(1);
	temp1 = ADC_CH1_RAW + temp1;
	//printf("Raw Value of ADC_CH1: %u @ iteration i = %u \n", ADC_CH1_RAW, i);
}
*Accum1 = temp1;

/* 16 consecutive ADC measurements for channel 2 */
for(uint8_t i = 0; i <= 15; i++)
{
	ADC_measure_PA(2);
	temp2 = ADC_CH2_RAW + temp2;
    //printf("Raw Value of ADC_CH2: %u @ iteration i = %u \n", ADC_CH2_RAW, i);
}
*Accum2 = temp2;

}

void BenchCalibration(void)
{
float TargetVoltage1 = 3.2;
float TargetVoltage2 = 3.2;
uint32_t Accum1 = 0;
uint32_t Accum2 = 0;
float bias = (3.3 / 4095.0);

printf("Used Target Voltages for Ch.1: %fV and Ch.2: %fV \n", TargetVoltage1, TargetVoltage2);

BurstMeasurement(&Accum1, &Accum2);

/* Converting average average ADC readings to voltage */
float ave1 = ( ( (float) Accum1 ) / 16.0 ) * bias;
float ave2 = ( ( (float) Accum2 ) / 16.0 ) * bias;

printf("Average voltages on Ch.1: %fV and Ch.2: %fV \n", ave1, ave2);

/*Computing Correction Factors */
CALFACT1 = TargetVoltage1 / ave1;
CALFACT2 = TargetVoltage2 / ave2;

printf("Correction Factors Computed as: %f and %f \n", CALFACT1, CALFACT2);

}

void CalibrationCompare(void)
{
    float bias_ideal =      (float) (3.3 / 4095.0);
    float bias_real =       (float) (3.3172 / 4095.0);
    float bias_ideal_corr1 = (float) ( (3.3 / 4095.0) *  CALFACT1);
    float bias_ideal_corr2 = (float) ( (3.3 / 4095.0) *  CALFACT2);

    channel1ADC = ( (float) (ADC_CH1_RAW) ) * bias_ideal;
    channel2ADC = ( (float) (ADC_CH2_RAW) ) * bias_ideal;
    printf("ADC voltage using IDEAL VDDA - NO correction: Ch1 = %f, Ch2 = %f \n", channel1ADC, channel2ADC );

    channel1ADC = ( (float) (ADC_CH1_RAW) ) * bias_ideal_corr1;
    channel2ADC = ( (float) (ADC_CH2_RAW) ) * bias_ideal_corr2;
    printf("ADC voltage using IDEAL VDDA - WITH correction: Ch1 = %f, Ch2 = %f\n", channel1ADC, channel2ADC );

    channel1ADC = ( (float) (ADC_CH1_RAW) ) * bias_real;
    channel2ADC = ( (float) (ADC_CH2_RAW) ) * bias_real;
    printf("ADC voltage using MEASURED VDDA - NO correction: Ch1 = %f, Ch2 = %f\n", channel1ADC, channel2ADC );

}


int main(void)
{
//Run all setup task before going into the while loop.
setup();

//Run GPIO Setup
initJoystick();
init_interrupt();

//Stops the code ontil the something is pressed
serialcom();


printf("hej");
//LOAD CALFACT1 and CALFACT2 from FLASH <--- help chrissssssss

// Check correction factors - valid range or not
LoadCorrectionFactor();

//Flag for enabling usage of correction factor

 // Now we are ready to enter the While loop.
     while(1){

/* Exercise 2.4 */
    /*if(CalibFlag == 1)
    {
        BenchCalibration();
/* STORE Correction Factors in FLASH
        init_page_flash(PG31_BASE);
        FLASH_Unlock();
        write_float_flash(PG31_BASE, 0, CALFACT1);
        write_float_flash(PG31_BASE, 1, CALFACT2);
        FLASH_Lock();

        CalibrationCompare();

        CalibFlag = 0;
    }*/

/* Exercise 2.3 */
/*
    if(PrintFlag == 1)
    {

        ADC_measure_PA(1);
        ADC_measure_PA(2);
        printTextDisplay();
        PrintFlag = 0;
    }

*/



/*
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

*/

    }
}
