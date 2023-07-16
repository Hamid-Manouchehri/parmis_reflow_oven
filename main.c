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

typedef struct
{
    unsigned filter :1;
    unsigned fault :1;
    unsigned f2 :1;
    unsigned f1 :1;
    unsigned wire :1;
    unsigned shot :1;
    unsigned mode :1;
    unsigned vbias :1;
}conf_t;

#define  VBIAS              (0<<7)   /* D7: V_BIAS Enabled */
#define  CONVERSION_MODE    (0<<6)   /* D6: Auto */
#define  SHOT               (0<<5)   /* D5: Disabled */
#define  WIRE_RTD           (1<<4)   /* D4: 1=3_Wire Enabled & 0=2or4_Wire Enabled */
#define  FAULT_DETECTION_D3 (0<<3)   /* D3: No Action _Table 3 */
#define  FAULT_DETECTION_D2 (0<<2)   /* D2: No Action _Table 3 */
#define  FAULT_STATUS       (1<<1)   /* D1: Auto_Clear */
#define  FILTER             (1<<0)   /* D0: 50Hz */

#define  High_Threshold     0x7FFF
#define  Low_Threshold      0x0000
#define  RTD_RESISTANCE     100      /* PT100=100 & PT1000=1000 Ohm */
#define  RREF               430      /* Res Ref Module Max31865 = 430 Ohm */
//
//#define  Reg_Configuration   0x00
//#define  Reg_RTD_MSBs        0x01
//#define  Reg_RTD_LSBs        0x02
//#define  Reg_High_Fault_MSB  0x03
//#define  Reg_High_Fault_LSB  0x04
//#define  Reg_Low_Fault_MSB   0x05
//#define  Reg_Low_Fault_LSB   0x06
//#define  Reg_Fault_Status    0x07
//
#define  Z1  -3.9083E-03   /* Datasheet AN709-0 */
#define  Z2  17.58480889E-06
#define  Z3  -23.10E-09    /* For PT100 & PT1000=-2.31E-09 */
#define  Z4  -1.155E-06
// T(r)=(Z1+sqrt(Z2+Z3*r))/Z4


struct {
    volatile unsigned char Configuration;
    volatile float Resistance;
    volatile float Temperature;
    volatile unsigned int Value;
    volatile unsigned int HighThreshold;
    volatile unsigned int LowThreshold;
    volatile unsigned char FaultStatus;
} RTD;

unsigned char Buff[20]; // size of the number

inline void TX_Command(uint8_t);
//inline float _power(uint16_t, uint16_t);
inline void TX_Whole_String(char*);
//inline void reverse(char*, int);
//inline uint16_t intToStr(int, char [], int);
//inline void ftoa(float, char*, int);
inline void RTD_SetConfiguration (conf_t* ); // write on configuration register
inline void RTD_SetFaultThreshold(void); // write on Threshold registers
inline void RTD_ReadAll(void);
void RTD_ReadTemperature(void);
uint16_t read_rtd_register (void);

void main(void)
{
    SYSTEM_Initialize();
    IO_RC2_alarm_LED_SetHigh(); // to indicate PCB has power.
    
    SPI1_Open(SPI1_DEFAULT);
    RTD_SetConfiguration(NULL);
    __delay_ms(100);
//    RTD_SetFaultThreshold();
//    __delay_ms(100);

    conf_t conf;
    conf.f1 = 0;
    conf.f2 = 0;
    conf.fault = 1;
    conf.filter = 1;
    conf.mode = 0;
    conf.shot = 0;
    conf.wire = 1;
    
    uint16_t Data = 0;
    
    while (1){
        

    conf.vbias = 1;
    RTD_SetConfiguration(&conf);
    __delay_ms (100);
    conf.shot = 1;
    RTD_SetConfiguration(&conf);
    __delay_ms (100);
    conf.shot = 0;
    conf.vbias = 0;
    RTD_SetConfiguration(&conf);
    sprintf (Buff, "rtd: %d\n", read_rtd_register());
    TX_Whole_String(Buff);
    __delay_ms(1000);
//        RTD_ReadAll();        
        

//        if(RTD.FaultStatus == 0){
            
//             RTD_ReadTemperature();
             
//             sprintf(Buff,"RTD: %d Ohm ", RTD.Resistance);
//             TX_Whole_String(Buff);
//             sprintf(Buff,"Temp: %0.1f \xDF\x43   ", RTD.Temperature);
//             TX_Whole_String(Buff);
//             __delay_ms(1000);
//        }
//        else{
//            
//             sprintf(Buff,"Err: %0.1f       ", RTD.FaultStatus);
//             TX_Whole_String(Buff);
//             __delay_ms(1000);
//        }
        
    } // end of while(1)
} // end of main()


inline void TX_Whole_String(char *input_string){
    
    uint16_t size_of_string = strlen(input_string);
    
    for (uint8_t i = 0; i < size_of_string; i++){
        
        TX_Command(input_string[i]);
        __delay_ms(5); // necessary to send data
        
    }
    
//    for(char* p = input_string; p; p++){
//        TX_Command(*p);
//    }
    
    TX_Command(10); // New Line
    
}


inline void TX_Command(uint8_t txData) {

    if (EUSART_is_tx_ready()) { // Second send data value:

        EUSART_Write(txData);
    }
}


inline void RTD_SetConfiguration (conf_t* conf){
    
    unsigned char ControlBits = 0;
    unsigned char X[20]; // size of the number
    
    if (NULL == conf)
    {
    ControlBits = 0;
    ControlBits |= VBIAS;
    ControlBits |= CONVERSION_MODE;
    ControlBits |= SHOT;
    ControlBits |= WIRE_RTD;
    ControlBits |= FAULT_DETECTION_D3;
    ControlBits |= FAULT_DETECTION_D2;
    ControlBits |= FAULT_STATUS;
    ControlBits |= FILTER;
    }
    else
    {
        
        ControlBits = *(char*)conf;
    }
    
//    sprintf(X,"control bits: %d  ", ControlBits);
//    TX_Whole_String(X); 
    
    out_CS_SetLow();
    SPI1_ExchangeByte(0x80);
    SPI1_ExchangeByte(ControlBits);
    out_CS_SetHigh();
    
}


inline void RTD_SetFaultThreshold(void){

       out_CS_SetLow();
       SPI1_ExchangeByte(0x83);
       SPI1_ExchangeByte((High_Threshold >> 8) & 0xFF);
       SPI1_ExchangeByte(High_Threshold & 0xFF);
       SPI1_ExchangeByte((Low_Threshold >> 8) & 0xFF);
       SPI1_ExchangeByte(Low_Threshold & 0xFF);
       out_CS_SetHigh();

   }


inline void RTD_ReadAll(void){

       static uint16_t Data = 0;
       static float Ratio = 0;
       
       out_CS_SetLow();
       SPI1_ExchangeByte(0x00);  // read address of configuration register
       RTD.Configuration = SPI1_ExchangeByte(0x00);

       Data = ((uint16_t)SPI1_ExchangeByte(0x00) << 8);
       Data |= SPI1_ExchangeByte(0x00);
       RTD.Value = (Data>>1);

       Data = ((uint16_t)SPI1_ExchangeByte(0x00) << 8);
       Data |= SPI1_ExchangeByte(0x00);
       RTD.HighThreshold = Data;

       Data = ((uint16_t)SPI1_ExchangeByte(0x00) << 8);
       Data |= SPI1_ExchangeByte(0x00);
       RTD.LowThreshold = Data;

       RTD.FaultStatus = SPI1_ExchangeByte(0x00);
       out_CS_SetHigh();

       Ratio = RTD.Value;
       RTD.Resistance = (float)((Ratio*RREF)/32768);
       
       sprintf(Buff,"res: %f ohm", RTD.Resistance);
       TX_Whole_String(Buff);

       if(RTD.Resistance == 0 || RTD.FaultStatus != 0){
           
            RTD_SetConfiguration(NULL);
            RTD_SetFaultThreshold();
            
        }
   }


void RTD_ReadTemperature(void){
    
      RTD.Temperature = (Z1 + sqrt(Z2 + (Z3 * RTD.Resistance))) / Z4;
      
   }

uint16_t read_rtd_register (void)
{
    
    struct 
    {
        uint8_t address;
        uint8_t msb;
        uint8_t lsb;
    }buf;
    
    buf.address = 0x01;
    
    out_CS_SetLow();
    SPI1_ExchangeBlock(&buf, 3);
    return ((uint16_t)buf.msb << 8) | buf.lsb;
}