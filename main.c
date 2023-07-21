/**
  Author: Hamid Manouchehri
  Date: July 14, 2023
  
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "mcc_generated_files/mcc.h"


#define  ON              true
#define  OFF             false

#define  DIM_PERCENT     25 // range: 5-95 percent <TODO>

#define  Z1  -3.9083E-03   /* Datasheet AN709-0 */
#define  Z2  17.58480889E-06
#define  Z3  -23.10E-09    /* For PT100 & PT1000=-2.31E-09 */
#define  Z4  -1.155E-06
// T(r)=(Z1+sqrt(Z2+Z3*r))/Z4


const float R_const = 100.0; // ohm
const float V_cc_const = 5.0; // volt
const float Tolerance_Temp_const = 37.2;  // Celsius
const float HalfCycleACDuration_const = 10.0; // ms
const float TMR2_Period_const = 100; // us  <TODO>
unsigned char Buff_g[20]; // size of the number
uint16_t required_delay_for_dim_ms_g = 0;


inline void Init_Function(void);
inline void TX_Command(uint8_t);
inline void TX_Whole_String(char*);
inline void ReadTemperature(uint16_t);
inline uint16_t Read_ADC_Num(void);
inline float ChangeADCtoVoltage(uint16_t);
inline float Measure_R_PT100_2Wire(float);
inline float Measure_Temp_PT100_2Wire(float);
inline float Read_PT100_Temp(void);
void Zero_Detection_isr(void);
 void TMR2_Int_isr(void);
inline void StartStop_AlarmLED(bool);
inline void StartStop_Fan(bool);
inline void StartStop_Buzzer(bool);
inline void StartTouchDetection(void);
inline void StopTouchDetection(void);
inline uint16_t CalcRequiredDelayForTrigTRIAC(float);


/*
 * #############################################################################
 * ################################## main() ###################################
 * #############################################################################
 */


void main(void){
    
    SYSTEM_Initialize();
    Init_Function();
    StartStop_AlarmLED(ON); // to indicate PCB has power.
    
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
//    IOCAF2_SetInterruptHandler(Zero_Detection_isr); // internal pull-up of the RA2 must be enabled.
//    TMR2_SetInterruptHandler(TMR2_Int_isr); // For measuring the required delay for dim.
    
    float Temp_PT100 = 0.0;
    
    while (1){
       
        StopTouchDetection();
//        Temp_PT100 = Read_PT100_Temp();
//        sprintf(Buff_g, "%f Celsius\n", Temp_PT100);
//        TX_Whole_String(Buff_g);
//        __delay_ms(500);
        
    } // end of while(1)
} // end of main()


/*
 * #############################################################################
 * ######################## Function definition ################################
 * #############################################################################
 */

inline void Init_Function(void){
    
    StartStop_AlarmLED(OFF);
    StartStop_Fan(OFF);
    StartStop_Buzzer(OFF);
    IO_RC5_trig_TRIAC_SetLow();
    
    ADC_SelectChannel(channel_AN10);
    
    required_delay_for_dim_ms_g = CalcRequiredDelayForTrigTRIAC(DIM_PERCENT);
    
}


inline void TX_Command(uint8_t txData) {

    if (EUSART_is_tx_ready()) { // Second send data value:

        EUSART_Write(txData);
    }
}


inline void TX_Whole_String(char *input_string){
    
    uint16_t size_of_string = strlen(input_string);
    
    for (uint8_t i = 0; i < size_of_string; i++){
        
        TX_Command(input_string[i]);
        __delay_ms(5); // necessary to send data
        
    }
    
//    TX_Command(10); // New Line
    
}


inline void ReadTemperature(uint16_t R_PT100){
    
    uint16_t Temperature;
    
    Temperature = (Z1 + sqrt(Z2 + (Z3 * R_PT100))) / Z4;
      
}


inline uint16_t Read_ADC_Num(void){
    
    uint16_t convertedValue;
    
    while(!ADC_IsConversionDone());
    convertedValue = ADC_GetConversionResult();
            
    return convertedValue;
    
}


inline float ChangeADCtoVoltage(uint16_t ADC_number){
    
    return (float)(5.0 * ADC_number / 1023);
    
}


inline float Measure_R_PT100_2Wire(float ADC_volt){
    
    float R_PT100 = 0;
    
    R_PT100 = R_const * ADC_volt / (V_cc_const - ADC_volt);
    
    return R_PT100;
    
}


inline float Measure_Temp_PT100_2Wire(float R_PT100){
    
    float Temp_PT100;
    
    Temp_PT100 = ((Z1 + sqrt(Z2 + Z3 * R_PT100)) / Z4) - Tolerance_Temp_const;
    
    return Temp_PT100;
    
}


inline float Read_PT100_Temp(void){
    
    uint16_t ADC_num = 0;
    float ADC_volt = 0;
    float R_PT100 = 0;
    float Temp_PT100 = 0;
    
    ADC_StartConversion();

    ADC_num = Read_ADC_Num();
//    sprintf(Buff_g, "ADC number: %d \n", ADC_num);
    ADC_volt = ChangeADCtoVoltage(ADC_num);
//    sprintf(Buff_g, "V-adc: %f v\n", ADC_volt);
    R_PT100 = Measure_R_PT100_2Wire(ADC_volt);
//    sprintf(Buff_g, "R PT100: %f ohm\n", R_PT100);
    Temp_PT100 = Measure_Temp_PT100_2Wire(R_PT100);
    
    return Temp_PT100;    
}


void Zero_Detection_isr(void){  
/* zero-cross happened: (sine signal of 220 v AC approached zero) */
        
    TMR2_StartTimer();
    
}


void TMR2_Int_isr(void){
    
    static uint16_t counter = 0;
    counter ++;
    
    if(counter >= required_delay_for_dim_ms_g){
        
        counter = 0;
        IO_RC5_trig_TRIAC_SetHigh();
        
    }
    else if (1 == IO_RC5_trig_TRIAC_GetValue()){
        
        counter = 0;
        TMR2_StopTimer();
        IO_RC5_trig_TRIAC_SetLow();
    }    
}


inline void StartStop_AlarmLED(bool OnOff){
    
    if (ON == OnOff){
        
        IO_RC2_alarm_LED_SetHigh();
    }
    else{
        
        IO_RC2_alarm_LED_SetLow();
    }
    
}


inline void StartStop_Fan(bool OnOff){
    
    if (ON == OnOff){
        
        IO_RC3_fan_SetHigh();
    }
    else{
        
        IO_RC3_fan_SetLow();
    }
    
}


inline void StartStop_Buzzer(bool OnOff){
    
    if (ON == OnOff){
        
        IO_RC4_buzzer_SetHigh();
    }
    else{
        
        IO_RC4_buzzer_SetLow();
    }
    
}


inline void StartTouchDetection(void){
    /* To use; just call this function where ever you want to detect touch*/
   
    if (true == MTOUCH_Service_Mainloop()) {

        if (true == MTOUCH_Button_isPressed(T_start)){ // touch pressed:

            StartStop_AlarmLED(ON);

        }            
        else{

            StartStop_AlarmLED(OFF);

        }
    }    
}


inline void StopTouchDetection(void){
    /* To use; just call this function where ever you want to detect touch*/
    
    if (true == MTOUCH_Service_Mainloop()) {

        if (true == MTOUCH_Button_isPressed(T_stop)){ // touch pressed:

            StartStop_AlarmLED(ON);

        }            
        else{

            StartStop_AlarmLED(OFF);

        }
    }
    
}


inline uint16_t CalcRequiredDelayForTrigTRIAC(float dim_percentage){
 
    float delay;
    uint16_t ret;
    
    delay = HalfCycleACDuration_const - (5.0 * dim_percentage / 50.0); // ms
    ret = (delay * 1000) / TMR2_Period_const; // change delay(ms) into number of required TMR2 steps
    
    return ret;
}