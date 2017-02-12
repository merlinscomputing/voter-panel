/* 
 * File:   global.h
 * Author: jhagerman
 *
 * Created on May 18, 2015, 20:40
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#define EXT extern

//===========================================================================
//------------- Global defines, variables  ----------------------------------
//===========================================================================
//#define dsPIC33
#define PIC24 
//#define UserInterface
#define QuantarInterface

/*
#define SS              LATBbits.LATB0
#define MOSI            LATBbits.LATB1
#define MISO            LATBbits.LATB2
#define MISO_TRIS       TRISBbits.TRISB2
#define CLK             LATBbits.LATB3
#define I2C_BUS_FRQ     100000
*/

#define HeartBeatLED	LATBbits.LATB15
#define PTT             LATAbits.LATA3
#define MUTE            LATBbits.LATB5 
#define TUNING_ACTIVE   PORTBbits.RB13

#define LCD_E           LATAbits.LATA2
#define LCD_RS          LATAbits.LATA3
#define Input1          PORTAbits.RA4
//#define LCD_RW          LATBbits.LATB4
#define LCD_D3          LATBbits.LATB3
#define LCD_D2          LATBbits.LATB2
#define LCD_D1          LATBbits.LATB1
#define LCD_D0          LATBbits.LATB0

#define	INPUT 	1
#define	OUTPUT	0

#define HIGH = 1
#define LOW = 0

#define true 1
#define false 0

#define TRUE 1
#define FALSE 0

//#define FCY (39613750L)
/* Microcontroller MIPs (FCY) */

#define SYS_FREQ        20000000LL
//#define SYS_FREQ        79257600LL
//#define SYS_FREQ       40550400LL
//#define SYS_FREQ         20275200LL
#define FCY             SYS_FREQ/2
//#define GetInstructionClock()	(SYS_FREQ/2)
#define GetPeripheralClock()	FCY

#define nop() __asm__{volatile(nop)};

//extern char ax25header[1024];
//extern char character;
extern int Rx1BufferReady;
extern int Rx2BufferReady;
extern int Rx3BufferReady;
//extern static UART_OBJECT uart1_obj;
//extern int Index = 0;

#endif  	/* GLOBAL_H */
#undef GLOBAL_H