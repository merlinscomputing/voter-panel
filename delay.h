 #ifndef __DELAY_H
 #define __DELAY_H

/*
 delay_us(x) and delay_ms(x) for C30
 */

#define FOSC  32000000LL  // clock-frequecy in Hz with suffix LL (64-bit-long), eg. 32000000LL for 32MHz

#ifndef FCY
#define FCY       (FOSC/2)  // MCU is running at FCY MIPS
#endif
//FCY is already defined in system.h

 #define delay_us(x) __delay32(((x*FCY)/1000000L)) // delays x us
 #define delay_ms(x) __delay32(((x*FCY)/1000L))  // delays x ms
 #include <libpic30.h>
#endif
