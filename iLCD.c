/* ******** I2C Intefaced LCD Module ********** */ 
/* ******** Merlins Computing  **************** */

#include <xc.h>
//#include "mcc_generated_files/i2c2.h"
#include "iLCD.h"
//#include "global.h"
#include "delay.h"
#include <string.h>

char iLCD_Read_ADDR=0x7F;//0x41;
char iLCD_Write_ADDR=0x7E; //0x4E = 0x27 for Old LCD 0x7E = 0x3F for new LCD's
//#define iLCD_Write_ADDR2 "0x7E"; //0x3F
#define BUS_FRQ 100000
//char * buffer;
char data ;

#define ClearDisplay 0x01;
#define ReturnHome 0x02;
#define Function01 0x30;
#define Function02 0x26;
#define DisplayON 0x0F; 
#define EntryMode 0x04; // or 0x06

void StartI2C2(void)
{
    I2C2CONLbits.SEN=1; // set START bit
    while(I2C2CONLbits.SEN); // wait for start to finish
}

void StopI2C2(void)
{
   I2C2CONLbits.PEN=1; // send STOP - cleared by hardware when complete
   while(I2C2CONLbits.PEN); // wait for stop to finish
}

void IdleI2C2(void)
{
     //while(I2C2STATbits.ACKSTAT == 1);
     delay_us(5);
}

void MasterWriteI2C2(char data)
{ 
    //while(I2C2STATbits.ACKSTAT == 1);
    IFS3bits.MI2C2IF = 0; //clear the interrupt flag
    I2C2TRN = data; // send data to the i2c Transmit Register.
    while(I2C2STATbits.TRSTAT==1){};
    I2C1STATbits.ACKSTAT = 1;
    //delay_us(5);
   
}


void I2C2_Data_Write(char data, int delay) //RS needs to be 1
{   MasterWriteI2C2((data & 0xF0) | 0x0D);
    MasterWriteI2C2((data & 0xF0) | 0x09);
    MasterWriteI2C2(((data & 0x0F) << 4) | 0x0D); //was 0x05
    MasterWriteI2C2(((data & 0x0F) << 4) | 0x09);
   // MasterWriteI2C2(0x00);
    IdleI2C2();		//Wait to complete
    delay_us(delay);
}
void I2C2_Command_Write2(char data, int delay) // 0x26 should be 24 64
{ // command write is byte broken into high and low bits and strobed with E
    StartI2C2();	//set the start bit
    delay_us(50);
    IdleI2C2();	// 5us Wait to complete   
    MasterWriteI2C2(iLCD_Write_ADDR); //address
    MasterWriteI2C2((data & 0xF0) | 0x0C); //was 04
    IdleI2C2();		//Wait to complete  
    StopI2C2();	
    IdleI2C2();	//Wait to complete 

    StartI2C2();	//set the start bit
    //IdleI2C2();	// 5us Wait to complete   
    MasterWriteI2C2(iLCD_Write_ADDR); //address
    MasterWriteI2C2((data & 0xF0) | 0x08);
    IdleI2C2();		//Wait to complete  
    StopI2C2();	
    IdleI2C2();	//Wait to complete 
}

void I2C2_Command_Write(char data, int delay) // 0x26 should be 24 64
{ // command write is byte broken into high and low bits and strobed with E
    StartI2C2();	//set the start bit
    delay_us(50);
    IdleI2C2();	// 5us Wait to complete   
    MasterWriteI2C2(iLCD_Write_ADDR); //address
    MasterWriteI2C2((data & 0xF0) | 0x04); //was 04
    IdleI2C2();		//Wait to complete  
    StopI2C2();	
    IdleI2C2();	//Wait to complete 

    StartI2C2();	//set the start bit
    //IdleI2C2();	// 5us Wait to complete   
    MasterWriteI2C2(iLCD_Write_ADDR); //address
    MasterWriteI2C2((data & 0xF0));
    IdleI2C2();		//Wait to complete  
    StopI2C2();	
    IdleI2C2();	//Wait to complete 
/*    
    StartI2C2();	//set the start bit
    IdleI2C2();	// 5us Wait to complete   
    MasterWriteI2C2(iLCD_Write_ADDR); //address
    MasterWriteI2C2(((data & 0x0F) << 4) | 0x04);
    IdleI2C2();		//Wait to complete  
    StopI2C2();	
    IdleI2C2();	//Wait to complete 

    StartI2C2();	//set the start bit
    IdleI2C2();	// 5us Wait to complete   
    MasterWriteI2C2(iLCD_Write_ADDR); //address
    MasterWriteI2C2((data & 0x0F) << 4);
    IdleI2C2();		//Wait to complete  
    StopI2C2();	
    IdleI2C2();	//Wait to complete 
*/
    delay_us(delay);
}

void I2C2_OLD_Command_Write(char data, int delay) // 0x26 should be 24 64
{ // command write is byte broken into high and low bits and strobed with E
    StartI2C2();	//set the start bit
    delay_us(50);
    IdleI2C2();	// 5us Wait to complete   
    MasterWriteI2C2(iLCD_Write_ADDR); //address
    MasterWriteI2C2((data & 0xF0) | 0x04);
    IdleI2C2();		//Wait to complete  
    StopI2C2();	
    IdleI2C2();	//Wait to complete 

    StartI2C2();	//set the start bit
    //IdleI2C2();	// 5us Wait to complete   
    MasterWriteI2C2(iLCD_Write_ADDR); //address
    MasterWriteI2C2(data & 0xF0);
    IdleI2C2();		//Wait to complete  
    StopI2C2();	
    IdleI2C2();	//Wait to complete 
    
    StartI2C2();	//set the start bit
    IdleI2C2();	// 5us Wait to complete   
    MasterWriteI2C2(iLCD_Write_ADDR); //address
    MasterWriteI2C2(((data & 0x0F) << 4) | 0x04);
    IdleI2C2();		//Wait to complete  
    StopI2C2();	
    IdleI2C2();	//Wait to complete 

    StartI2C2();	//set the start bit
    IdleI2C2();	// 5us Wait to complete   
    MasterWriteI2C2(iLCD_Write_ADDR); //address
    MasterWriteI2C2((data & 0x0F) << 4);
    IdleI2C2();		//Wait to complete  
    StopI2C2();	
    IdleI2C2();	//Wait to complete 

    delay_us(delay);
}

void I2C2_SetLocation(char data, int delay) // 0x26 should be 24 64
{ // command write is 0b1xxx xxxx data byte broken into high and low bits and strobed with E
    StartI2C2();	
    IdleI2C2();	//Wait to complete   
    MasterWriteI2C2(iLCD_Write_ADDR); //address 4e
   // MasterWriteI2C2(0x00);  
    MasterWriteI2C2(((0x80 | data)& 0xF0)  | 0x0c); //E and backlight ON was 0x04
    MasterWriteI2C2((0x80 | data) & 0xF0);
    MasterWriteI2C2((((0x80 | data)& 0x0F) << 4) | 0x0c); //E and backlight ON
    MasterWriteI2C2(((0x80 | data)& 0x0F) << 4);
  //  MasterWriteI2C2(0x00);
    IdleI2C2();		//Wait to complete
    StopI2C2();	
    IdleI2C2();	//Wait to complete   
    delay_us(delay);
}

/*
void I2CInit(void) 
{   // SCL = RB8  SDA = RB9  
    I2CCONbits.I2CEN = 0;
    I2CCONbits.I2CSIDL= 0;  //run in Idle mode.
    I2CCONbits.SMEN=1; //signal conditioning
    I2CCONbits.STREN = 1; // Clock Stretching Enabled
    I2CCONbits.A10M = 0; //7 bit address.
    I2C1BRG = (FCY/BUS_FRQ - FCY/1111111) - 1; //196; // or 358?
    I2C1CONbits.I2CEN = 1;
    
    TRISBbits.TRISB8=0; // make SCL an output
    LATBbits.LATB8=0; //drive SCL low
    delay_ms(1);
    LATBbits.LATB8=1; //drive SCL high  
    TRISBbits.TRISB8=1; // make SCL an input
    
    I2C1CONbits.I2CEN = 1; //re-enable I2C1.
}
*/

void BacklightON(void)
{
     StartI2C2();	//set the start bit
    IdleI2C2();	// 5us Wait to complete   
    MasterWriteI2C2(iLCD_Write_ADDR); //address
    MasterWriteI2C2((0x00));
    IdleI2C2();		//Wait to complete  
    StopI2C2();	
    IdleI2C2();	//Wait to complete
    
     StartI2C2();	//set the start bit
    IdleI2C2();	// 5us Wait to complete   
    MasterWriteI2C2(iLCD_Write_ADDR); //address
    MasterWriteI2C2((0x08));
    IdleI2C2();		//Wait to complete  
    StopI2C2();	
    IdleI2C2();	//Wait to complete 
}

void LCDInit(void)
{
    // db7, db6, db5, db4, *, E, RS, RW
/*1. Display clear
2. Function set:
DL = 1; 8-bit interface data
N = 0; 1-line display
F = 0; 5 × 8 dot character font
3. Display on/off control:
D = 0; Display off
C = 0; Cursor off
B = 0; Blinking off
4. Entry mode set: 0x000001IS
I/D = 1; Increment by 1
S = 0; No shift
 */
   // StartI2C2();	
   // IdleI2C2();	//Wait to complete   
	I2C2_Command_Write(0x30, 5000); //function1 8 bit 4.5 ms delay
    delay_ms(100);
    //  MasterWriteI2C2(0x4E); //address
	I2C2_Command_Write(0x30, 5000); //function1 8 bit 4.5 ms delay
    I2C2_Command_Write(0x30, 150); //function1 8 bit
    I2C2_Command_Write(0x30, 50); //function1 8 bit
    I2C2_Command_Write(0x20, 50); //function1 - 4 Bit 1 line
    //I2C2_Command_Write(0x2C, 50); //function1 - 4 Bit 2 line 5x11
    //I2C2_Command_Write(0x28, 50); //function1 - 4 Bit 2 line 5x8
    //I2C2_Command_Write(0x24, 50); //function1 - 4 Bit 1 line 5x11
    //I2C2_Command_Write(0x20, 50); //function1 - 4 Bit 1 line 5x8 
    //I2C2_Command_Write(0x20, 50);//function set: 4 bit, 2 line 5x8 char 0x28 - bottom line missing to row
    I2C2_Command_Write(0x20, 50);//function set: 4 bit, 2 line 5x8 char 0x28 - bottom line missing to row
    I2C2_Command_Write(0x80, 50);//function set: 4 bit, 2 line 5x8 char 0x28 - bottom line missing to row
    I2C2_Command_Write(0x00, 50);//function set: 4 bit, 2 line 5x8 char 0x28 - bottom line missing to row
    I2C2_Command_Write(0xC0, 50);//function set: 4 bit, 2 line 5x8 char 0x28 - bottom line missing to row
    I2C2_Command_Write(0x00, 50);//function set: 4 bit, 2 line 5x8 char 0x28 - bottom line missing to row
    I2C2_Command_Write(0x10, 50);//function set: 4 bit, 2 line 5x8 char 0x28 - bottom line missing to row
    delay_ms(2); 
   
    I2C2_Command_Write(0x00, 50); //Display OFF
    I2C2_Command_Write(0x60, 50); //Display ON , no cursor, no blink
    BacklightON();
    I2C2_Command_Write2(0x00, 50); //Display ON , no cursor, blinking
    I2C2_Command_Write2(0x20, 50); //Display ON , cursor on , no blink 
    //I2C2_Command_Write(0x0F, 50); //Display ON , cursor on, blinking
    
    //I2C2_Command_Write(0x0C, 50);//display on/off off=0x08 on=0x0C 0x0D 0x0E blink cursor=0x0F 
    // C=  D= E=Block Blinking  F= Block Blinking
    //I2C2_Command_Write(0x01, 1800);//Display Clear
    //I2C2_Command_Write(0x06, 180);//entry mode set
    //I2C2_Command_Write(0x80, 180); //set location 1,1
  //  StopI2C2();	//Send the Stop condition
//	IdleI2C2();	//Wait to complete   
    
}

//void LCDInit(void)
//{
    // db7, db6, db5, db4, *, E, RS, RW
/*1. Display clear
2. Function set:
DL = 1; 8-bit interface data
N = 0; 1-line display
F = 0; 5 × 8 dot character font
3. Display on/off control:
D = 0; Display off
C = 0; Cursor off
B = 0; Blinking off
4. Entry mode set: 0x000001IS
I/D = 1; Increment by 1
S = 0; No shift
 */
  /* // StartI2C2();	
   // IdleI2C2();	//Wait to complete   
	I2C2_Command_Write(0x30, 5000); //function1 8 bit 4.5 ms delay
    delay_ms(100);
    //  MasterWriteI2C2(0x4E); //address
	I2C2_Command_Write(0x30, 5000); //function1 8 bit 4.5 ms delay
    I2C2_Command_Write(0x30, 150); //function1 8 bit
    I2C2_Command_Write(0x30, 50); //function1 8 bit
    I2C2_Command_Write(0x28, 50); //function1 - 4 Bit 1 line
    //I2C2_Command_Write(0x2C, 50); //function1 - 4 Bit 2 line 5x11
    //I2C2_Command_Write(0x28, 50); //function1 - 4 Bit 2 line 5x8
    //I2C2_Command_Write(0x24, 50); //function1 - 4 Bit 1 line 5x11
    //I2C2_Command_Write(0x20, 50); //function1 - 4 Bit 1 line 5x8 
    I2C2_Command_Write(0x28, 50);//function set: 4 bit, 2 line 5x8 char 0x28 - bottom line missing to row
    //I2C2_Command_Write(0x08, 50); //Display OFF
    //I2C2_Command_Write(0x0C, 50); //Display ON , no cursor, no blink
    //I2C2_Command_Write(0x0D, 50); //Display ON , no cursor, blinking
    //I2C2_Command_Write(0x0E), 50); //Display ON , cursor on , no blink 
    //I2C2_Command_Write(0x0F), 50); //Display ON , cursor on, blinking
    I2C2_Command_Write(0x0C, 50);//display on/off off=0x08 on=0x0C 0x0D 0x0E blink cursor=0x0F 
    // C=  D= E=Block Blinking  F= Block Blinking
    I2C2_Command_Write(0x01, 1800);//Display Clear
    I2C2_Command_Write(0x06, 180);//entry mode set
    I2C2_Command_Write(0x80, 180); //set location 1,1
  //  StopI2C2();	//Send the Stop condition
//	IdleI2C2();	//Wait to complete   
}
*/
void  LCDprintstr(const char c[])
{
  int len;
  StartI2C2();
  IdleI2C2(); //idle the bus
	while (*c)
		{
        len = strlen(c);
		MasterWriteI2C2(0x7E); //address
		while (len--)
	        I2C2_Data_Write( *c++,2 );
		if (*c)
		   {
            delay_us(50);	 
           }// More to send.  Wait a bit
		}
    StopI2C2();	//Send the Stop condition
  	IdleI2C2();	//Wait to complete 
}

void LCDTest(void)
{  
   I2C2_Command_Write(0x01, 1800); //clear display
   I2C2_Command_Write(0x02, 1800); // go home
   LCDprintstr("  Merlins-Computing  ");
   I2C2_SetLocation(0x14,50); // line 3
   LCDprintstr("       HHTRC         ");
   I2C2_SetLocation(0x4D,50); // line 3
   StartI2C2();
   IdleI2C2(); //idle the bus
   MasterWriteI2C2(0x4E); //address
   I2C2_Data_Write(0xF8,50);
   StopI2C2();	//Send the Stop condition
   IdleI2C2();	//Wait to complete 
   
   I2C2_SetLocation(0x4F,50); // line 3
   StartI2C2();
   IdleI2C2(); //idle the bus
   MasterWriteI2C2(0x4E); //address
   I2C2_Data_Write(0xFF,50);
   StopI2C2();	//Send the Stop condition
   IdleI2C2();	//Wait to complete 
   
   I2C2_SetLocation(0x52,50); // line 3
   StartI2C2();
   IdleI2C2(); //idle the bus
   MasterWriteI2C2(0x4E); //address
   I2C2_Data_Write(0x32,50);
   StopI2C2();	//Send the Stop condition
   IdleI2C2();	//Wait to complete 
   
   I2C2_SetLocation(0x61,50); // line 3
   StartI2C2();
   IdleI2C2(); //idle the bus
   MasterWriteI2C2(0x4E); //address
   I2C2_Data_Write(0xF8,50);
   StopI2C2();	//Send the Stop condition
   IdleI2C2();	//Wait to complete 
   
   I2C2_SetLocation(0x63,50); // line 3
   StartI2C2();
   IdleI2C2(); //idle the bus
   MasterWriteI2C2(0x4E); //address
   I2C2_Data_Write(0xF8,50);
   StopI2C2();	//Send the Stop condition
   IdleI2C2();	//Wait to complete 
   
   I2C2_SetLocation(0x65,50); // line 3
   StartI2C2();
   IdleI2C2(); //idle the bus
   MasterWriteI2C2(0x4E); //address
   I2C2_Data_Write(0x32,50);
   StopI2C2();	//Send the Stop condition
   IdleI2C2();	//Wait to complete 
   
   StartI2C2();
   IdleI2C2(); //idle the bus
   MasterWriteI2C2(0x4E); //address
   I2C2_Data_Write(0xFF,50);
   StopI2C2();	//Send the Stop condition
   IdleI2C2();	//Wait to complete 
   
   /*delay_ms(2000);
   I2C2_Command_Write(0x02, 50);
   LCDprintstr("Merlins HHTRC");
   I2C2_SetLocation(0x40,50); // 0x26 should be 24 64
   LCDprintstr("Line 2             ");
   I2C2_SetLocation(0x14,50); // 0x26 should be 24 64
   LCDprintstr("Line 3             ");
   I2C2_SetLocation(0x54,50); // 0x26 should be 24 64
   LCDprintstr("Line 4             ");
   delay_ms(2000);*/
  }
