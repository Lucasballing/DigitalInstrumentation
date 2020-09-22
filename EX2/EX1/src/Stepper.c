#include "Stepper.h"



void serialcom(){
    int8_t A[2]={0}; // Defines an array of two zeros
    char temp;
    // Initiate a loop, until break by while statement
    do {
        temp = uart_getc(); // Saves the last character input from keyboard. 0 if nothing is pressed.

        if(temp !=0 ){
                A[0]=temp;}// Saves the last input from keyboard at the first position in A.
        else {}
    }
    while ((A[0]!=49) && (A[0]!=50) && (A[0]!=51) && (A[0]!=57) && A[0]!=27);
}


void StepperControle(){
    int8_t A[2]={0}; // Defines an array of two zeros
    char temp;
    uint32_t POSITIONS[4] = { 0b0101, 0b1001, 0b1010, 0b0110 };
    // Initiate a loop, until break by while statement
    do {
        temp = uart_getc(); // Saves the last character input from keyboard. 0 if nothing is pressed.

        if(temp !=0 ){
                A[0]=temp;}// Saves the last input from keyboard at the first position in A.
        else {}
    }
    while ((A[0]!=49) && (A[0]!=50) && (A[0]!=51) && (A[0]!=57) && A[0]!=27);

    if(A[0]==49){
        for(int i = 0; i < 201; i++ ){
                POSITIONS[i&3];
        }



    }





}



