/* ******** LCD Menu List ********** */ 
/* ******** Merlins Computing  ********** */
/* ********   HHTRC  ********** */

#include "iLCD.h"
//#include "delay.h"
#include "global.h"

void UserHello(void)
{
    //I2C2_Command_Write(0x02, 50);
    I2C2_SetLocation(0x00, 50); // 0x26 should be 24 6
    LCDprintstr("Coverage Tester-USER");
    I2C2_SetLocation(0x40, 50); // 0x26 should be 24 64
    LCDprintstr("                    ");
    I2C2_SetLocation(0x14, 50); // 0x26 should be 24 64
    LCDprintstr("RSSI1:              "); //rssi1 value goes at position 21
    I2C2_SetLocation(0x54, 50); // 0x26 should be 24 64
    LCDprintstr("RSSI2:              "); //rssi1 value goes at position 61
    //delay_ms(500);
}

void QuantarHello(void)
{
    //I2C2_Command_Write(0x02, 50);
    I2C2_SetLocation(0x00, 50); // 0x26 should be 24 6
    LCDprintstr("Coverage Tester-QTR");
    I2C2_SetLocation(0x40, 50); // 0x26 should be 24 64
    LCDprintstr("                   ");
    I2C2_SetLocation(0x14, 50); // 0x26 should be 24 64
    LCDprintstr("RSSI1:             "); //rssi1 value goes at position 21
    I2C2_SetLocation(0x54, 50); // 0x26 should be 24 64
    LCDprintstr("RSSI2:             "); //rssi1 value goes at position 61
    //delay_ms(500);
}

void BootWelcome(void)
{  char* verstring = "     Ver HHT1       ";
   //char* verstring =   '     Ver HHT1 '+HHTRC_VERSION; // not working yet
   //I2C2_Command_Write(0x02, 50); // go home
   I2C2_SetLocation(0x00,50);
   LCDprintstr(" Merlins Computing  ");
   I2C2_SetLocation(0x40,50);
   LCDprintstr("       HHTRC        ");
   I2C2_SetLocation(0x14,50);
   LCDprintstr("                    ");
   I2C2_SetLocation(0x54,50);
   LCDprintstr(verstring);
  // delay_ms(1000);
}
