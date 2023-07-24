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

#define  ON   true
#define  OFF  false

const float HalfCycleACDuration_const = 10.0; // ms
const float TMR2_Timer_Period_const = 100; // us  <TODO>
uint16_t tmr2_required_counter_steps_g;
bool DimmerStatusFlag_g = false;

inline void Init_Function(void);
void Zero_Detection_isr(void);
void TMR2_Drive_TRIAC_isr(void);
void TMR4_Wroking_Blink_AlarmLED_isr(void);
void TMR6_Check_Touch_Keys_Status_isr(void);
inline void StartStop_AlarmLED(bool);
inline void StartStop_AlarmLED_Blink(bool);
inline void StartStop_Fan(bool);
inline void StartStop_Buzzer(bool);
inline void StartStop_Dimmer(bool);
inline void StartTouchDetection(void);
void StopTouchDetection(void);
void SetDimmer(float);
inline void StartHeater(void);


/*
 * #############################################################################
 * ################################## main() ###################################
 * #############################################################################
 */


void main(void){
    
    SYSTEM_Initialize();
    Init_Function();
    
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
    IOCAF2_SetInterruptHandler(Zero_Detection_isr);
    TMR2_SetInterruptHandler(TMR2_Drive_TRIAC_isr);
    TMR4_SetInterruptHandler(TMR4_Wroking_Blink_AlarmLED_isr);
//    TMR6_SetInterruptHandler(TMR6_Check_Touch_Keys_Status_isr);
    
    while(1){
        
        StartTouchDetection();
       
    }
    
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


void TMR6_Check_Touch_Keys_Status_isr(void){
    
//    StartTouchDetection();
    StopTouchDetection();
    
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

        if (true == MTOUCH_Button_isPressed(T_start)){ // touch pressed: Start the Procedure

            StartStop_Buzzer(ON);
            __delay_ms(100);
            StartStop_Buzzer(OFF);
            
            StartHeater();

        }            
        else{

//            StartStop_AlarmLED(OFF);
            // Do nothing.

        }
    }    
}


void StopTouchDetection(void){
    /* To use; just call this function where ever you want to detect touch*/
    
    if (true == MTOUCH_Service_Mainloop()) {

        if (true == MTOUCH_Button_isPressed(T_stop)){ // touch pressed: Emergency Stop

            StartStop_Buzzer(ON);
            __delay_ms(1000);
            StartStop_Buzzer(OFF);
            
            Init_Function();

        }            
        else{

            // Do nothing.

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


inline void StartHeater(void){
    
    StartStop_AlarmLED_Blink(ON);
    StartStop_Dimmer(ON);
    
    SetDimmer(10);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(11);
    __delay_ms(2000);
    StopTouchDetection();
    
    SetDimmer(12);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(13);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(14);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(15);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(16);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(17);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(18);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(19);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(20);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(21);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(22);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(23);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(24);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(25);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(26);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(27);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(28);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(29);
    __delay_ms(2000);
    StopTouchDetection();

    SetDimmer(30);
    __delay_ms(2000);
    StopTouchDetection();


    // <section #2>    
    SetDimmer(30.5);
    __delay_ms(8000);
    StopTouchDetection();

    SetDimmer(31);
    __delay_ms(8000);
    StopTouchDetection();

    SetDimmer(31.5);
    __delay_ms(8000);
    StopTouchDetection();

    SetDimmer(32);
    __delay_ms(8000);
    StopTouchDetection();

    SetDimmer(32.5);
    __delay_ms(8000);
    StopTouchDetection();

    SetDimmer(33);
    __delay_ms(8000);
    StopTouchDetection();

    SetDimmer(33.5);
    __delay_ms(8000);
    StopTouchDetection();

    SetDimmer(34);
    __delay_ms(8000);
    StopTouchDetection();

    SetDimmer(34.5);
    __delay_ms(8000);
    StopTouchDetection();

    SetDimmer(35);
    __delay_ms(8000);
    StopTouchDetection();


    // <section #3>
    SetDimmer(37);
    __delay_ms(4500);
    StopTouchDetection();

    SetDimmer(39);
    __delay_ms(4500);
    StopTouchDetection();

    SetDimmer(41);
    __delay_ms(4500);
    StopTouchDetection();

    SetDimmer(43);
    __delay_ms(4500);
    StopTouchDetection();

    SetDimmer(45);
    __delay_ms(4500);
    StopTouchDetection();

    SetDimmer(47);
    __delay_ms(4500);
    StopTouchDetection();

    SetDimmer(49);
    __delay_ms(4500);
    StopTouchDetection();

    SetDimmer(51);
    __delay_ms(4500);
    StopTouchDetection();

    SetDimmer(53);
    __delay_ms(4500);
    StopTouchDetection();

    SetDimmer(55);
    __delay_ms(4500);
    StopTouchDetection();

    SetDimmer(57);
    __delay_ms(4500);
    StopTouchDetection();

    SetDimmer(59);
    __delay_ms(4500);
    StopTouchDetection();

    SetDimmer(61);
    __delay_ms(4500);
    StopTouchDetection();

    SetDimmer(63);
    __delay_ms(4500);
    StopTouchDetection();

    SetDimmer(65);
    __delay_ms(4500);
    StopTouchDetection();

    SetDimmer(67);
    __delay_ms(4500);
    StopTouchDetection();

    SetDimmer(69);
    __delay_ms(4500);
    StopTouchDetection();

    SetDimmer(71);
    __delay_ms(4500);
    StopTouchDetection();


    // <section #4>       
    StartStop_AlarmLED_Blink(OFF);
    StartStop_Dimmer(OFF);
    StartStop_Fan(ON);
    for (uint8_t i = 0; i < 18; i++){ // generate 3 min delay
        
        __delay_ms(10000);
    }
    StartStop_Fan(OFF);
    StartStop_Buzzer(ON);
    __delay_ms(1000);
    StartStop_Buzzer(OFF);
}
