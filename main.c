/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using MPLAB® Code Configurator

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB® Code Configurator - v2.25.2
        Device            :  PIC24FJ128GB202
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.24
        MPLAB             :  MPLAB X v2.35 or v3.00
 */

/*
Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 */

#include "mcc_generated_files/mcc.h"
#include "global.h"
#include "UserCommands.h"
//#include "QuantarCommands.h"
#include "delay.h"
#include "Serial.h"
#include <stdio.h>
#include <string.h>
#include <libpic30.h>
#include "iLCD.h" 
#include "LCDMenu.h"

int TimerAccum = 0;
bool Button1Active = FALSE;
/*
                         Main application
 */
int main(void) {
    // initialize the device
    SYSTEM_Initialize();
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 0;

    
LCDInit();
delay_ms(10);
//LCDprintstr("  Merlins-Computing  "); //for LCD testing
//delay_ms(2000); //display MC Boot Welcome for 2 seconds

BootWelcome();
delay_ms(1000); //display MC Boot Welcome for 2 seconds
//LCDprintstr("  Merlins-Computing  ");
#ifdef UserInterface
    UserHello();
    UartHello();
     while (1) {
        if (Input1==0) //handle the button press
        {
           delay_us(100);
           if (Input1==0) // if it's still pressed
           {  
             
            if (Button1Active==FALSE) // so that we only send the command once per press
             {
             
              PTT=1; //pull PTT/Audio short transistor active.
               delay_ms(500);
              MUTE=0; // Audio Mute to inactive.
              delay_ms(500);
               I2C2_SetLocation(0x40, 50); //0x40, 50  0x26 should be 24 64
               LCDprintstr("Sending RSSI Request"); // 20 char
              //if (TUNING_ACTIVE==FALSE) //if pulled low for tuning just send tone
              //{
             // delay_ms(100); // wait for 1/2 second for ptt to stabilize before sending data.
              U3PutS("RSSIRequest\r");
              //SendRSSIRequest();
              delay_ms(500); // wait for 1-2 second for ptt to stabilize before sending data.
                if (TUNING_ACTIVE==TRUE) //if pulled low for tuning just keep sending tone
                   {
                   MUTE=1; // Audio Mute to active.
                   }
              Button1Active=TRUE;
              //}
            }
           }
        }
        
        if (Input1==1) //handle the button release
        {
           delay_us(100);
           if (Input1==1) // if it's still open
           {
           if (Button1Active==TRUE)
               {
               Button1Active=FALSE;
                MUTE=1; // Make sure audio mute is active.
               // delay_ms(5000); //5 seconds TX to let both radios get rssi back 
               PTT=0; // PTT/Audio short transistor back to inactive.
              
               I2C2_SetLocation(0x40, 50); //0x40, 50  0x26 should be 24 64
               LCDprintstr("                    "); // 20 char
               }
           }
        }
        Uart1Handler();
        Uart3Handler();
       // delay_ms(500);
        //OpenXLCD ( FOUR_BIT & LINES_5X7);
        // delay_ms(100);
        // lprintf("TEST");
        // putsXLCD(msg1);

        // RssiTest_Radio3();
        // Repeater11();  
        // Repeater12(); 
        //Repeater21();
        //if (TimerAccum==60){comm_keep_alive();} //quantar.c
        // if (Input1==0){RssiTest();} //quantar.c
        // Quantar1RSSI();

    }
    return -1;
}
#endif

#ifdef QuantarInterface
   
    //UartHello();
    QuantarHello();
    
      while (1) {
        if (Input1==0) //handle the button press
        {
           delay_us(100);
           if (Input1==0) // if it's still pressed
           {
           if (Button1Active==FALSE) // so that we only send the command once per press
             {
              MUTE=0; // Audio Mute to inactive.
              delay_ms(100);
              PTT=1; //make sure PTT/Audio short transistor active.
             //need to install the tuning short and pullup then enable here.
              //if (TUNING_ACTIVE==FALSE) //if pulled low for tuning just send tone
              //{
              delay_ms(100); // wait for 1/2 second for ptt to stabilize before sending data.
              SendRSSI1Request();
              SendRSSI2Request();
              //delay_ms(200); // wait for 1/2 second after sending data.
               if (TUNING_ACTIVE==TRUE) //if pulled low for tuning just keep sending tone
                   {
                   MUTE=1; // Audio Mute to active.
                   }
              //MUTE=1; // Audio Mute to active.
              Button1Active=TRUE;
              //} 
             } 
            I2C2_SetLocation(0x40, 50); //line 2 0x40, 50
            LCDprintstr("RSSI Request        "); // 20 char
           }
        }
        
        if (Input1==1) //handle the button release
        {
           delay_us(100);
           if (Input1==1) // if it's still open
           {
           //if (Button1Active==TRUE) we really don't care.. just shut it up..
           //    {
               Button1Active=FALSE;
               PTT=0; // PTT/Audio short transistor back to inactive.
               MUTE=1; // Make sure audio mute is active.
               I2C2_SetLocation(0x40, 50); //0x40, 50  0x26 should be 24 64
               LCDprintstr("                    "); // 20 char
           //    }
           }
        }
        Uart1Handler();
        Uart2Handler();
        Uart3Handler();
        //if (TimerAccum==60){comm_keep_alive();} //quantar.c
        // if (Input1==0){RssiTest();} //quantar.c
        // Quantar1RSSI();

    }
    return -1;
}

#endif

  
/**
 End of File
 */
