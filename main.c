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
#define  SMALL_PCB                     10
#define  LARGE_PCB                     11

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
inline void StartStop_AlarmLED(bool);
inline void StartStop_AlarmLED_Blink(bool);
inline void StartStop_Fan(bool);
inline void StartStop_Buzzer(bool);
inline void StartStop_Dimmer(bool);
void StartTouchDetection(void);
void StopTouchDetection(void);
void SetDimmer(float);
inline void StartHeater(uint8_t);


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
    
    while(1){ // do not delete while(1)
        
        StartTouchDetection();
        StopTouchDetection();
       
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


void StartTouchDetection(void){
    /* To use; just call this function where ever you want to detect touch*/
   
    if (true == MTOUCH_Service_Mainloop()) {

        if (true == MTOUCH_Button_isPressed(T_start)){ // touch pressed: Start the Procedure
            
            while (true == MTOUCH_Button_isPressed(T_start)) {
                MTOUCH_Service_Mainloop();
            }
                
            StartStop_Buzzer(ON);
            __delay_ms(100);
            StartStop_Buzzer(OFF);
            
            StartHeater(SMALL_PCB);

            while (true == MTOUCH_Button_isPressed(T_start)) {
                MTOUCH_Service_Mainloop();
            }
        }
    }    
}


void StopTouchDetection(void){
    /* To use; just call this function where ever you want to detect touch*/
    
    if (true == MTOUCH_Service_Mainloop()) {

        if (true == MTOUCH_Button_isPressed(T_stop)){ // touch pressed: Emergency Stop

            while (true == MTOUCH_Button_isPressed(T_stop)) {
                MTOUCH_Service_Mainloop();
            }
                
            StartStop_Buzzer(ON);
            __delay_ms(100);
            StartStop_Buzzer(OFF);
            
            StartHeater(LARGE_PCB);

            while (true == MTOUCH_Button_isPressed(T_stop)) {
                MTOUCH_Service_Mainloop();
            }
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


inline void StartHeater(uint8_t status_flag){
    
    StartStop_AlarmLED_Blink(ON);
    StartStop_Dimmer(ON);
    
    if(SMALL_PCB == status_flag){
        
        SetDimmer(10);
        __delay_ms(1000);

        SetDimmer(11);
        __delay_ms(1000);

        SetDimmer(12);
        __delay_ms(1000);

        SetDimmer(13);
        __delay_ms(1000);

        SetDimmer(14);
        __delay_ms(1000);

        SetDimmer(15);
        __delay_ms(1000);

        SetDimmer(16);
        __delay_ms(1000);

        SetDimmer(17);
        __delay_ms(1000);

        SetDimmer(18);
        __delay_ms(1000);

        SetDimmer(19);
        __delay_ms(1000);

        SetDimmer(20);
        __delay_ms(1000);

        SetDimmer(21);
        __delay_ms(1000);

        SetDimmer(22);
        __delay_ms(1000);

        SetDimmer(23);
        __delay_ms(1000);

        SetDimmer(24);
        __delay_ms(1000);

        SetDimmer(25);
        __delay_ms(1000);

        SetDimmer(26);
        __delay_ms(1000);

        SetDimmer(27);
        __delay_ms(1000);

        SetDimmer(28);
        __delay_ms(1000);

        SetDimmer(29);
        __delay_ms(1000);

        SetDimmer(30);
        __delay_ms(1000);


        // <section #2>    
        SetDimmer(30.5);
        __delay_ms(4500);

        SetDimmer(31);
        __delay_ms(4500);

        SetDimmer(31.5);
        __delay_ms(4500);

        SetDimmer(32);
        __delay_ms(4500);

        SetDimmer(32.5);
        __delay_ms(4500);

        SetDimmer(33);
        __delay_ms(4500);

        SetDimmer(33.5);
        __delay_ms(4500);

        SetDimmer(34);
        __delay_ms(4500);

        SetDimmer(34.5);
        __delay_ms(4500);

        SetDimmer(35);
        __delay_ms(4500);


        // <section #3>
        SetDimmer(37);
        __delay_ms(3000);

        SetDimmer(39);
        __delay_ms(3000);

        SetDimmer(41);
        __delay_ms(3000);

        SetDimmer(43);
        __delay_ms(3000);

        SetDimmer(45);
        __delay_ms(3000);

        SetDimmer(47);
        __delay_ms(3000);

        SetDimmer(49);
        __delay_ms(3000);

        SetDimmer(51);
        __delay_ms(3000);

        SetDimmer(53);
        __delay_ms(3000);

        SetDimmer(55);
        __delay_ms(3000);

        SetDimmer(57);
        __delay_ms(3000);

        SetDimmer(59);
        __delay_ms(3000);

        SetDimmer(61);
        __delay_ms(3000);

        SetDimmer(63);
        __delay_ms(3000);

        SetDimmer(65);
        __delay_ms(3000);

        SetDimmer(67);
        __delay_ms(3000);

        SetDimmer(69);
        __delay_ms(3000);

        SetDimmer(71);
        __delay_ms(3000);
    }
    else if(LARGE_PCB == status_flag){
        
        SetDimmer(10);
        __delay_ms(1000);

        SetDimmer(11);
        __delay_ms(1000);

        SetDimmer(12);
        __delay_ms(1000);

        SetDimmer(13);
        __delay_ms(1000);

        SetDimmer(14);
        __delay_ms(1000);

        SetDimmer(15);
        __delay_ms(1000);

        SetDimmer(16);
        __delay_ms(1000);

        SetDimmer(17);
        __delay_ms(1000);

        SetDimmer(18);
        __delay_ms(1000);

        SetDimmer(19);
        __delay_ms(1000);

        SetDimmer(20);
        __delay_ms(1000);

        SetDimmer(21);
        __delay_ms(1000);

        SetDimmer(22);
        __delay_ms(1000);

        SetDimmer(23);
        __delay_ms(1000);

        SetDimmer(24);
        __delay_ms(1000);

        SetDimmer(25);
        __delay_ms(1000);

        SetDimmer(26);
        __delay_ms(1000);

        SetDimmer(27);
        __delay_ms(1000);

        SetDimmer(28);
        __delay_ms(1000);

        SetDimmer(29);
        __delay_ms(1000);

        SetDimmer(30);
        __delay_ms(1000);


        // <section #2>    
        SetDimmer(30.5);
        __delay_ms(5500);

        SetDimmer(31);
        __delay_ms(5500);

        SetDimmer(31.5);
        __delay_ms(5500);

        SetDimmer(32);
        __delay_ms(5500);

        SetDimmer(32.5);
        __delay_ms(5500);

        SetDimmer(33);
        __delay_ms(5500);

        SetDimmer(33.5);
        __delay_ms(5500);

        SetDimmer(34);
        __delay_ms(5500);

        SetDimmer(34.5);
        __delay_ms(5500);

        SetDimmer(35);
        __delay_ms(5500);


        // <section #3>
        SetDimmer(37);
        __delay_ms(4500);

        SetDimmer(40);
        __delay_ms(4500);

        SetDimmer(42);
        __delay_ms(4500);

        SetDimmer(44);
        __delay_ms(4500);

        SetDimmer(46);
        __delay_ms(4500);

        SetDimmer(48);
        __delay_ms(4500);

        SetDimmer(50);
        __delay_ms(4500);

        SetDimmer(52);
        __delay_ms(4500);

        SetDimmer(54);
        __delay_ms(4500);

        SetDimmer(56);
        __delay_ms(4500);

        SetDimmer(58);
        __delay_ms(5000);

        SetDimmer(60);
        __delay_ms(5000);

        SetDimmer(63);
        __delay_ms(5000);

        SetDimmer(65);
        __delay_ms(5000);

        SetDimmer(68);
        __delay_ms(5000);

        SetDimmer(72);
        __delay_ms(5000);

        SetDimmer(75);
        __delay_ms(5000);

        SetDimmer(79);
        __delay_ms(5000);
    }


    // <section #4>       
    StartStop_Fan(ON);
    StartStop_Dimmer(OFF);
    for (uint8_t i = 0; i < 36; i++){ // generate 3 min delay
        
        __delay_ms(5000);
    }
    StartStop_Fan(OFF);
    StartStop_Buzzer(ON);
    __delay_ms(1000);
    StartStop_Buzzer(OFF);
    StartStop_AlarmLED_Blink(OFF);
}
