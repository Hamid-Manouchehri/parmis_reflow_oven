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



#define  TMR4_REQUIRED_COUNTER_STEPS   1000 // TMR4_REQUIRED_COUNTER_STEPS * 1 (ms) <TODO>
#define  NUM_OF_DIM_VALUE                    3  // <TODO>

#define  ON   true
#define  OFF  false

#define  Z1  -3.9083E-03   /* Datasheet AN709-0 */
#define  Z2  17.58480889E-06
#define  Z3  -23.10E-09    /* For PT100 & PT1000=-2.31E-09 */
#define  Z4  -1.155E-06
// T(r)=(Z1+sqrt(Z2+Z3*r))/Z4


const float R_const = 100.0; // ohm
const float V_cc_const = 5.0; // volt
const float Tolerance_Temp_const = 37.2;  // Celsius
const float HalfCycleACDuration_const = 10.0; // ms
const float TMR2_Timer_Period_const = 100; // us  <TODO>
const float TMR6_Timer_Period_const = 1; // ms  <TODO>
unsigned char Buff_g[20]; // size of the number
uint16_t tmr2_required_counter_steps_g;
uint16_t tmr2_required_counter_steps_Arr_g[NUM_OF_DIM_VALUE];
uint16_t tmr6_required_counter_steps_Arr_g[NUM_OF_DIM_VALUE];
bool DimmerStatusFlag_g = false;
uint16_t required_steps_for_delay_of_dim_g = 0;

float dim_value_Arr_g [NUM_OF_DIM_VALUE] = {10, 20, 50}; // percentage; Defining the amount of dim for each stage. <TODO>
float dim_duration_ms_Arr_g [NUM_OF_DIM_VALUE] = {5000, 5000, 5000}; // ms; Defining the duration of corresponding dim. <TODO>
// Note: dim_duration_ms_Arr_g must have the same Unit as TMR6_Timer_Period_const.

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
void TMR2_Drive_TRIAC_isr(void);
void TMR4_Wroking_Blink_AlarmLED_isr(void);
inline void StartStop_AlarmLED(bool);
inline void StartStop_AlarmLED_Blink(bool);
inline void StartStop_Fan(bool);
inline void StartStop_Buzzer(bool);
inline void StartStop_Dimmer(bool);
inline void StartTouchDetection(void);
inline void StopTouchDetection(void);
void SetDimmer(float);
inline void Config_Heater(void);


/*
 * #############################################################################
 * ################################## main() ###################################
 * #############################################################################
 */


void main(void){
    
    SYSTEM_Initialize();
    Init_Function();
//    StartStop_AlarmLED(ON); // to indicate PCB has power.
    
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
    IOCAF2_SetInterruptHandler(Zero_Detection_isr);
    TMR2_SetInterruptHandler(TMR2_Drive_TRIAC_isr);
    TMR4_SetInterruptHandler(TMR4_Wroking_Blink_AlarmLED_isr);
    
    float Temp_PT100 = 0.0;
    
    Config_Heater();
    
    
    while (1){
        
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
    StartStop_AlarmLED_Blink(OFF);
    StartStop_Fan(OFF);
    StartStop_Buzzer(OFF);
    StartStop_Dimmer(OFF);
    IO_RC5_trig_TRIAC_SetLow();
    
    ADC_SelectChannel(channel_AN10);
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
/* zero-cross happened: (sine signal of 220 v AC approached zero), start timer
 * for measuring the required delay for dimmer.
 */
  
    if (true == DimmerStatusFlag_g){
        
        TMR2_StartTimer();
    }
    
}


void TMR2_Drive_TRIAC_isr(void){
    
    static uint16_t counter_tmr2 = 0;
    
    if (true == DimmerStatusFlag_g){
        
        counter_tmr2 ++;

        if(counter_tmr2 >= tmr2_required_counter_steps_g){

            counter_tmr2 = 0;
            IO_RC5_trig_TRIAC_SetHigh();

        }
        else if (1 == IO_RC5_trig_TRIAC_GetValue()){

            counter_tmr2 = 0;
            TMR2_StopTimer();
            IO_RC5_trig_TRIAC_SetLow();
        }    
    }
    else{
        
        counter_tmr2 = 0;
    }
}


void TMR4_Wroking_Blink_AlarmLED_isr(void){
    
    static uint16_t counter_tmr4 = 0;
    counter_tmr4 ++;
    
    if(counter_tmr4 >= TMR4_REQUIRED_COUNTER_STEPS){
        
        counter_tmr4 = 0;
        IO_RC2_alarm_LED_Toggle();
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


inline void StartStop_AlarmLED_Blink(bool OnOff){
    
    if (ON == OnOff){
        
        TMR4_StartTimer();
    }
    else{
        
        TMR4_StopTimer();
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


inline void StartStop_Dimmer(bool OnOff){
    
    if (ON == OnOff){
        
        DimmerStatusFlag_g = true;
    }
    else{
        
        SetDimmer(3); // calling this function here is an exception, do not touch it.
        DimmerStatusFlag_g = false;
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



void SetDimmer(float dim_percentage){
    /* dim_percentage range: 5% - 95%. 
     * Prerequisite: call StartStop_Dimmer(ON);
     */
 
    float TRIAC_off_duration_ms = 0;
    
    TRIAC_off_duration_ms = HalfCycleACDuration_const - (5.0 * dim_percentage / 50.0); // ms
    tmr2_required_counter_steps_g = (TRIAC_off_duration_ms * 1000) / TMR2_Timer_Period_const; // change delay(ms) into number of required TMR2 steps
   
}


inline void Config_Heater(void){

    StartStop_AlarmLED_Blink(ON);
    StartStop_Dimmer(ON);
    
    // <section #1>
    
    SetDimmer(10);
    __delay_ms(3000);
    
    SetDimmer(11);
    __delay_ms(3000);
    
    SetDimmer(12);
    __delay_ms(3000);
    
    SetDimmer(13);
    __delay_ms(3000);
    
    SetDimmer(14);
    __delay_ms(3000);
    
    SetDimmer(15);
    __delay_ms(3000);
    
    SetDimmer(16);
    __delay_ms(3000);
    
    SetDimmer(17);
    __delay_ms(3000);
    
    SetDimmer(18);
    __delay_ms(3000);
    
    SetDimmer(19);
    __delay_ms(3000);
    
    SetDimmer(20);
    __delay_ms(3000);
    
    SetDimmer(21);
    __delay_ms(3000);
    
    SetDimmer(22);
    __delay_ms(3000);
    
    SetDimmer(23);
    __delay_ms(3000);
    
    SetDimmer(24);
    __delay_ms(3000);
    
    SetDimmer(25);
    __delay_ms(3000);
    
    SetDimmer(26);
    __delay_ms(3000);
    
    SetDimmer(27);
    __delay_ms(3000);
    
    SetDimmer(28);
    __delay_ms(3000);
    
    SetDimmer(29);
    __delay_ms(3000);
    
    SetDimmer(30);
    __delay_ms(3000);
    
    // <section #2>
    
    SetDimmer(30.5);
    __delay_ms(9000);
    
    SetDimmer(31);
    __delay_ms(9000);
    
    SetDimmer(31.5);
    __delay_ms(9000);
    
    SetDimmer(32);
    __delay_ms(9000);
    
    SetDimmer(32.5);
    __delay_ms(9000);
    
    SetDimmer(33);
    __delay_ms(9000);
    
    SetDimmer(33.5);
    __delay_ms(9000);
    
    SetDimmer(34);
    __delay_ms(9000);
    
    SetDimmer(34.5);
    __delay_ms(9000);
    
    SetDimmer(35);
    __delay_ms(9000);
    
    // <section #3>
    
    SetDimmer(35.5);
    __delay_ms(2500);
    
    SetDimmer(36);
    __delay_ms(2500);
    
    SetDimmer(36.5);
    __delay_ms(2500);
    
    SetDimmer(37);
    __delay_ms(2500);
    
    SetDimmer(37.5);
    __delay_ms(2500);
    
    SetDimmer(38);
    __delay_ms(2500);
    
    SetDimmer(38.5);
    __delay_ms(2500);
    
    SetDimmer(39);
    __delay_ms(2500);
    
    SetDimmer(39.5);
    __delay_ms(2500);
    
    SetDimmer(40);
    __delay_ms(2500);
    
    SetDimmer(40.5);
    __delay_ms(2500);
    
    SetDimmer(41);
    __delay_ms(2500);
    
    SetDimmer(41.5);
    __delay_ms(2500);
    
    SetDimmer(42);
    __delay_ms(2500);
    
    SetDimmer(42.5);
    __delay_ms(2500);
    
    SetDimmer(43);
    __delay_ms(2500);
    
    SetDimmer(43.5);
    __delay_ms(2500);
    
    SetDimmer(44);
    __delay_ms(2500);
    
    SetDimmer(44.5);
    __delay_ms(2500);
    
    SetDimmer(45);
    __delay_ms(2500);
    
    // <section #4>

    StartStop_Fan(ON);
    
    StartStop_Buzzer(ON);
    __delay_ms(2000);
    StartStop_Buzzer(OFF);
    
    
    StartStop_Dimmer(OFF);
    StartStop_AlarmLED_Blink(OFF);
}
