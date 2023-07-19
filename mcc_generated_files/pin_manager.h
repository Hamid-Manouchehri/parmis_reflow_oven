/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC16F1829
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above
        MPLAB 	          :  MPLAB X 6.00	
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set ZC_RA2 aliases
#define ZC_RA2_TRIS                 TRISAbits.TRISA2
#define ZC_RA2_LAT                  LATAbits.LATA2
#define ZC_RA2_PORT                 PORTAbits.RA2
#define ZC_RA2_WPU                  WPUAbits.WPUA2
#define ZC_RA2_ANS                  ANSELAbits.ANSA2
#define ZC_RA2_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define ZC_RA2_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define ZC_RA2_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define ZC_RA2_GetValue()           PORTAbits.RA2
#define ZC_RA2_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define ZC_RA2_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define ZC_RA2_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define ZC_RA2_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define ZC_RA2_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define ZC_RA2_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set out_CS aliases
#define out_CS_TRIS                 TRISAbits.TRISA4
#define out_CS_LAT                  LATAbits.LATA4
#define out_CS_PORT                 PORTAbits.RA4
#define out_CS_WPU                  WPUAbits.WPUA4
#define out_CS_ANS                  ANSELAbits.ANSA4
#define out_CS_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define out_CS_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define out_CS_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define out_CS_GetValue()           PORTAbits.RA4
#define out_CS_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define out_CS_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define out_CS_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define out_CS_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define out_CS_SetAnalogMode()      do { ANSELAbits.ANSA4 = 1; } while(0)
#define out_CS_SetDigitalMode()     do { ANSELAbits.ANSA4 = 0; } while(0)

// get/set channel_AN10 aliases
#define channel_AN10_TRIS                 TRISBbits.TRISB4
#define channel_AN10_LAT                  LATBbits.LATB4
#define channel_AN10_PORT                 PORTBbits.RB4
#define channel_AN10_WPU                  WPUBbits.WPUB4
#define channel_AN10_ANS                  ANSELBbits.ANSB4
#define channel_AN10_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define channel_AN10_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define channel_AN10_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define channel_AN10_GetValue()           PORTBbits.RB4
#define channel_AN10_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define channel_AN10_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define channel_AN10_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define channel_AN10_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define channel_AN10_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define channel_AN10_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set RB5 procedures
#define RB5_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define RB5_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define RB5_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define RB5_GetValue()              PORTBbits.RB5
#define RB5_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define RB5_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define RB5_SetPullup()             do { WPUBbits.WPUB5 = 1; } while(0)
#define RB5_ResetPullup()           do { WPUBbits.WPUB5 = 0; } while(0)
#define RB5_SetAnalogMode()         do { ANSELBbits.ANSB5 = 1; } while(0)
#define RB5_SetDigitalMode()        do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set RB7 procedures
#define RB7_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define RB7_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define RB7_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define RB7_GetValue()              PORTBbits.RB7
#define RB7_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define RB7_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define RB7_SetPullup()             do { WPUBbits.WPUB7 = 1; } while(0)
#define RB7_ResetPullup()           do { WPUBbits.WPUB7 = 0; } while(0)

// get/set IO_RC2_alarm_LED aliases
#define IO_RC2_alarm_LED_TRIS                 TRISCbits.TRISC2
#define IO_RC2_alarm_LED_LAT                  LATCbits.LATC2
#define IO_RC2_alarm_LED_PORT                 PORTCbits.RC2
#define IO_RC2_alarm_LED_WPU                  WPUCbits.WPUC2
#define IO_RC2_alarm_LED_ANS                  ANSELCbits.ANSC2
#define IO_RC2_alarm_LED_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define IO_RC2_alarm_LED_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define IO_RC2_alarm_LED_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define IO_RC2_alarm_LED_GetValue()           PORTCbits.RC2
#define IO_RC2_alarm_LED_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define IO_RC2_alarm_LED_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define IO_RC2_alarm_LED_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define IO_RC2_alarm_LED_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define IO_RC2_alarm_LED_SetAnalogMode()      do { ANSELCbits.ANSC2 = 1; } while(0)
#define IO_RC2_alarm_LED_SetDigitalMode()     do { ANSELCbits.ANSC2 = 0; } while(0)

// get/set IO_RC3_fan aliases
#define IO_RC3_fan_TRIS                 TRISCbits.TRISC3
#define IO_RC3_fan_LAT                  LATCbits.LATC3
#define IO_RC3_fan_PORT                 PORTCbits.RC3
#define IO_RC3_fan_WPU                  WPUCbits.WPUC3
#define IO_RC3_fan_ANS                  ANSELCbits.ANSC3
#define IO_RC3_fan_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define IO_RC3_fan_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define IO_RC3_fan_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define IO_RC3_fan_GetValue()           PORTCbits.RC3
#define IO_RC3_fan_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define IO_RC3_fan_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define IO_RC3_fan_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define IO_RC3_fan_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define IO_RC3_fan_SetAnalogMode()      do { ANSELCbits.ANSC3 = 1; } while(0)
#define IO_RC3_fan_SetDigitalMode()     do { ANSELCbits.ANSC3 = 0; } while(0)

// get/set IO_RC4_buzzer aliases
#define IO_RC4_buzzer_TRIS                 TRISCbits.TRISC4
#define IO_RC4_buzzer_LAT                  LATCbits.LATC4
#define IO_RC4_buzzer_PORT                 PORTCbits.RC4
#define IO_RC4_buzzer_WPU                  WPUCbits.WPUC4
#define IO_RC4_buzzer_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define IO_RC4_buzzer_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define IO_RC4_buzzer_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define IO_RC4_buzzer_GetValue()           PORTCbits.RC4
#define IO_RC4_buzzer_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define IO_RC4_buzzer_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define IO_RC4_buzzer_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define IO_RC4_buzzer_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)

// get/set IO_RC5_trig_TRIAC aliases
#define IO_RC5_trig_TRIAC_TRIS                 TRISCbits.TRISC5
#define IO_RC5_trig_TRIAC_LAT                  LATCbits.LATC5
#define IO_RC5_trig_TRIAC_PORT                 PORTCbits.RC5
#define IO_RC5_trig_TRIAC_WPU                  WPUCbits.WPUC5
#define IO_RC5_trig_TRIAC_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define IO_RC5_trig_TRIAC_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define IO_RC5_trig_TRIAC_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define IO_RC5_trig_TRIAC_GetValue()           PORTCbits.RC5
#define IO_RC5_trig_TRIAC_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define IO_RC5_trig_TRIAC_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define IO_RC5_trig_TRIAC_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define IO_RC5_trig_TRIAC_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()              PORTCbits.RC6
#define RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetPullup()             do { WPUCbits.WPUC6 = 1; } while(0)
#define RC6_ResetPullup()           do { WPUCbits.WPUC6 = 0; } while(0)
#define RC6_SetAnalogMode()         do { ANSELCbits.ANSC6 = 1; } while(0)
#define RC6_SetDigitalMode()        do { ANSELCbits.ANSC6 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCAF2 pin functionality
 * @Example
    IOCAF2_ISR();
 */
void IOCAF2_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCAF2 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCAF2 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCAF2_SetInterruptHandler(MyInterruptHandler);

*/
void IOCAF2_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCAF2 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCAF2_SetInterruptHandler() method.
    This handler is called every time the IOCAF2 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCAF2_SetInterruptHandler(IOCAF2_InterruptHandler);

*/
extern void (*IOCAF2_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCAF2 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCAF2_SetInterruptHandler() method.
    This handler is called every time the IOCAF2 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCAF2_SetInterruptHandler(IOCAF2_DefaultInterruptHandler);

*/
void IOCAF2_DefaultInterruptHandler(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/