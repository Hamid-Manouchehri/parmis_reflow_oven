/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F1829
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "mcc_generated_files/mcc.h"


inline void TX_Command(uint8_t);
inline float _power(uint16_t, uint16_t);
inline void TX_Whole_String(char *);


void reverse(char* str, int len) {
  int i = 0, j = len - 1;
  char temp;
  while (i < j) {
    temp = str[i];
    str[i] = str[j];
    str[j] = temp;
    i++;
    j--;
  }
}

int intToStr(int x, char str[], int d) {
  int i = 0;
  while (x) {
    str[i++] = (x % 10) + '0';
    x = x / 10;
  }
  while (i < d)
    str[i++] = '0';
  reverse(str, i);
  str[i] = '\0';
  return i;
}

void ftoa(float n, char* res, int afterpoint) {
  int ipart = (int)n;
  float fpart = n - (float)ipart;
  int i = intToStr(ipart, res, 0);
  if (afterpoint != 0) {
    res[i] = '.';
    fpart = fpart * _power(10, afterpoint);
    intToStr((int)fpart, res + i + 1, afterpoint);
  }
}



void main(void)
{
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    
    IO_RC2_SetHigh(); // to indicate PCB has power.
    
    uint8_t SPI1_read;
    float aNumber_int = 321.98;
    char aNumber_str[20]; // size of the number
    ftoa(aNumber_int, aNumber_str, 2); // maximum precision to 2.
    
    while (1)
    {
        // select client by setting its pin to low.
//        SPI1_read = SPI1_ReadByte();
//        TX_Command(SPI1_read);
        TX_Whole_String(aNumber_str);
        __delay_ms(1000);
        
    }
}


inline void TX_Whole_String(char *input_string){
    
    uint8_t size_of_string = strlen(input_string);
    
    for (uint8_t i = 0; i < size_of_string; i++){
        
        TX_Command(input_string[i]);
        __delay_ms(5);
        
    }
    
    TX_Command(10); // New Line
    
}


inline void TX_Command(uint8_t txData) {

    if (EUSART_is_tx_ready()) { // Second send data value:

        EUSART_Write(txData);
    }
}


inline float _power(uint16_t base_num, uint16_t exp_num){
    
    uint8_t result = 1; // Initialize the result
    
    while (exp_num > 0) { // Loop until the exponent is zero
        result *= base_num; // Multiply the result by the base
        exp_num--; // Decrease the exponent by one
    }
    
    return result; // Return the final result
  
}