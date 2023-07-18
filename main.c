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
#include <stdlib.h>
#include <math.h>
#include "mcc_generated_files/mcc.h"


#define  Z1  -3.9083E-03   /* Datasheet AN709-0 */
#define  Z2  17.58480889E-06
#define  Z3  -23.10E-09    /* For PT100 & PT1000=-2.31E-09 */
#define  Z4  -1.155E-06
// T(r)=(Z1+sqrt(Z2+Z3*r))/Z4


unsigned char Buff[20]; // size of the number

inline void TX_Command(uint8_t);
inline void TX_Whole_String(char*);
inline void ReadTemperature(uint16_t);
inline uint16_t ReadVoltage_ADC(void);


/*
 * #############################################################################
 * ################################## main() ###################################
 * #############################################################################
 */


void main(void)
{
    SYSTEM_Initialize();
    IO_RC2_alarm_LED_SetHigh(); // to indicate PCB has power.
    
    uint16_t V_ADC = 0;
    
    ADC_SelectChannel(channel_AN10);
    
    while (1){
       
        ADC_StartConversion();
        V_ADC = V_ADC = ReadVoltage_ADC();
        sprintf(Buff, "%d \n", V_ADC);
        TX_Whole_String(Buff);
        __delay_ms(500);
        
        
    } // end of while(1)
} // end of main()


/*
 * #############################################################################
 * ######################## Function definition ################################
 * #############################################################################
 */

inline void TX_Whole_String(char *input_string){
    
    uint16_t size_of_string = strlen(input_string);
    
    for (uint8_t i = 0; i < size_of_string; i++){
        
        TX_Command(input_string[i]);
        __delay_ms(5); // necessary to send data
        
    }
    
    TX_Command(10); // New Line
    
}


inline void TX_Command(uint8_t txData) {

    if (EUSART_is_tx_ready()) { // Second send data value:

        EUSART_Write(txData);
    }
}


inline void ReadTemperature(uint16_t R_PT100){
    
    uint16_t Temperature;
    
    Temperature = (Z1 + sqrt(Z2 + (Z3 * R_PT100))) / Z4;
      
}


inline uint16_t ReadVoltage_ADC(void){
    
    uint16_t convertedValue;
    
    while(!ADC_IsConversionDone());
    convertedValue = ADC_GetConversionResult();
            
    return convertedValue;
    
}