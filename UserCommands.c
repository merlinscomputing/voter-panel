#include <xc.h>
#include <libpic30.h> 
#include <stdio.h>
#include <string.h>
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/uart2.h"
#include "mcc_generated_files/uart3.h"
#include "UserCommands.h"
//#include "QuantarCommands.h"
#include "Serial.h"
#include "global.h"
#include "iLCD.h"
#include "delay.h"


extern int Rx1BufferReady;
extern char U1RXOutputBuffer[128];
extern int Rx2BufferReady;
extern char U2RXOutputBuffer[128];
extern int Rx3BufferReady;
extern char U3RXOutputBuffer[128];
extern char U3RXinputBuffer[128];
int RSSMODE = 0;

//http://embedjournal.com/implementing-circular-buffer-embedded-c/
//http://embedjournal.com/arduino-like-serial-uart-library-pic-18-microcontrollers/

//extern static UART_OBJECT uart1_obj;

void UartHello(void) {
    // uint8_t buffer1[30] = "Com1 TEST OK jjh\r\n";
    // uint8_t buffer1[30] = "GET DSP RSSI 1 1 SHORT\r\n";
    // uint8_t buffer2[30] = "Com2 TEST OK\r\n";
    // Show Welcome message on UART @ 9600 N-8-1
    // RSS:                                                                         
    //RSSI = 1 -129.1 -12.9
    //RSS:
    __C30_UART = 1;
    printf("\r\n=========================================================="); // line 01
    printf("\r\n                     JJH board GB202");
    printf("\r\n                  PIC24_Coverage_tester");
    printf("\r\n                          Com 1");
    printf("\r\n=========================================================\r\n"); //line 05


    __C30_UART = 2;
    //__C30_UART=1;
    printf("\r\n=========================================================="); // line 01
    printf("\r\n                     JJH board GB202");
    printf("\r\n                  PIC24_Coverage_tester");
    printf("\r\n                          Com 2");
    printf("\r\n=========================================================\r\n"); //line 05
    
    //__C30_UART=1;
    //printf("\r\n    Com 3 shows up on Com2 test online 1200-n-8-1");
    
    //__C30_UART=1;
    //unsigned char U3Text = "\r\n    Com 3 test online 1200-n-8-1";
     //U2PutS("");

    // U3PutS("TEST QTR Modem Link\r\n");

   
     //UART3_WriteBuffer(U3Text,36);
    
}

/*
void comm_copy_test(void)
{
   // I2C2_SetLocation(0x00, 50); //line 1   0x40, 50  0x26 should be 24 64
   // I2C2_SetLocation(0x40, 50); //line 2 0x26 should be 24 64
   // I2C2_SetLocation(0x14, 50); //line 3 0x26 should be 24 64
   // I2C2_SetLocation(0x54, 50); //line 4 0x26 should be 24 64
    
   char FULL[19] = "RSSI = 1 -129.1 -12";
   char PART[6]; // = (char*) malloc(6); 
   memset(PART, 0, 6); //clear the char
   I2C2_Command_Write(0x01, 50); //clear the LCD
   I2C2_Command_Write(0x02, 50); // go to home position
   LCDprintstr("RSSI = 1 -129.1 -12");
  // char* strn1cpy = strncpy(PART, "RSSI = 1 -129.1 -12.9"+9,6); // this works
   //char* strn1cpy = 
   strncpy(PART,FULL+9,6); // this works
   I2C2_SetLocation(0x40, 50);
   LCDprintstr(PART);
}
*/

void comm_keep_alive(void)
{
  //HeartBeatLED ^=1;
  //send RS232 QtrKeepAlive
}

void SendRSSI1Request(void)
{
  __C30_UART=1;
  if (RSSMODE==0)
  { 
    U1PutS("dorss\r\n");
    RSSMODE=1;
  }
  delay_ms(100); //lets wait a bit for the station to respond.
  
  //we should confirm that the station has returned "RSS:" if we do this properly.
  //printf("GET STN SN\r\n"); // not sure if we need the SN yet.
  // for now just assume and get the RSSI value.
  U1PutS(QtrGetRSSICommand);  
}

void SendRSSI2Request(void)
{
  if (RSSMODE==0)
  { 
    U2PutS("dorss\r\n");
    RSSMODE=1;
  }
  //  __C30_UART=2;
  //printf("dorss");
  //printf(QtrGetRSSICommand);  
  U2PutS(QtrGetRSSICommand);
}

void SendRSSIRequest(void)
{
    #ifdef UserInterface
     U3PutS("RSSIRequest\r\n");
#endif
#ifdef QuantarInterface
   //  U1PutS("GET STN SN\r\n");
     if (RSSMODE==0) //not in RSS Mode.
     {U1PutS("dorss\r\n");
     RSSMODE=1;}
     U1PutS("GET DSP RSSI 1 1 SHORT \r\n");
     //U3PutS("TEST QTR Modem Link\r\n");
#endif
}

void Uart1Handler(void) {
    //char Ustring[12];
    //char Nstring[12];
    if (Rx1BufferReady == TRUE) 
        {
        strip(U1RXOutputBuffer);
        Rx1BufferReady = FALSE;
        //"RSSI = 1 -129.1 -12.9
        
        if (0 == strncmp(U1RXOutputBuffer, "RSSI",4)) //Quantar1 is sending RSSI data
        {
        //I2C2_SetLocation(0x01, 50); // line 2 for testing
        //LCDprintstr("RSSI:");
        RSSMODE=1;
        char RSSI[8]; // = (char*) malloc(6);
       // memset(&RSSI[0], 0, sizeof(RSSI)); //clear the array
        //strncpy(Out1,U1RXOutputBuffer+9,6);
       // strncpy(RSSI, U1RXOutputBuffer+9, 5);
        memcpy (RSSI,U1RXOutputBuffer+9,6);
        I2C2_SetLocation(0x21, 50); // line 4 for testing
        LCDprintstr(RSSI);
        MUTE=0; // Audio Mute to inactive.
         delay_ms(200);
              PTT=1; //make sure PTT/Audio short transistor active.
             //need to install the tuning short and pullup then enable here.
              //if (TUNING_ACTIVE==FALSE) //if pulled low for tuning just send tone
              //{
              delay_ms(1000); // wait for 1/2 second for ptt to stabilize before sending data.
             //sprintf(Ustring, "RSSI1=%s\r",RSSI);
             U3PutS("RSSI1=");
             U3PutS(RSSI);              //
             U3PutS("\r");
            // U3PutS(sprintf(Ustring, "RSSI1=%s\r",RSSI));
              //memmove(Nstring,Nstring,13);
              //stripNULL(Ustring);
              //delay_us(10);
             // U3PutS(Ustring);
              //U3PutS(" \r");
              //PutS("RSSI Test\rLong string\rInVariable\r");
              delay_ms(500); // wait for 1/2 second after sending data.
              MUTE=1; // Audio Mute to active.
        memset(U1RXOutputBuffer, 0, 128); //tclear i, we don't want that
        delay_ms(1000);
        I2C2_SetLocation(0x21, 50); // line 4 for testing
        LCDprintstr("      ");
        }
        
     
        if (0 == strncmp(U1RXOutputBuffer, "RSS:",4))
        {
         RSSMODE=1;
         memset(U1RXOutputBuffer, 0, 128); //clear it, we don't want that
         // return;
        } 
    }
}

void Uart2Handler(void) 
{ //?(0034) wrong number of parameters
  //?(0014) invalid command
  // ]-O 
  

   
     if (Rx2BufferReady == TRUE) 
     {
#ifdef QuantarInterface
        //char Ustring[50];
        strip(U2RXOutputBuffer);
        Rx2BufferReady = FALSE;
        //"RSSI = 1 -129.1 -12.9
        if (0 == strncmp(U2RXOutputBuffer, "RSSI",4))
        {
        RSSMODE=1;
        char RSSI[8]; // = (char*) malloc(6);
        //strncpy(RSSI, U2RXOutputBuffer+9, 6);
        memcpy (RSSI,U2RXOutputBuffer+9,6);
        I2C2_SetLocation(0x61, 50); // line 4 for testing
        LCDprintstr(RSSI);
        MUTE=0; // Audio Mute to inactive.
         delay_ms(100);
              PTT=1; //make sure PTT/Audio short transistor active.
             //need to install the tuning short and pullup then enable here.
              //if (TUNING_ACTIVE==FALSE) //if pulled low for tuning just send tone
              //{
              delay_ms(100); // wait for 1/2 second for ptt to stabilize before sending data.
              U3PutS("RSSI2=");
              U3PutS(RSSI);
              U3PutS("\r");
             // sprintf(Ustring, "RSSI2=%s\n\r ",RSSI);
              //U3PutS(Ustring);
              delay_ms(200); // wait for 1/2 second after sending data.
              MUTE=1; // Audio Mute to active.
                   
        memset(U2RXOutputBuffer, 0, 128); //tclear i, we don't want that
        }
        
        if (0 == strncmp(U2RXOutputBuffer, "RSS:",4))
        {
         RSSMODE=1;
         memset(U2RXOutputBuffer, 0, 128); //clear it, we don't want that
        delay_ms(1000);
        I2C2_SetLocation(0x61, 50); // line 4 for testing
        LCDprintstr("      ");
        // return;
        }
  
#endif //endof quantar Inteface
     
#ifdef UserInterface
     //U2 is not used in User Interface
#endif //endof UserInteface 
  }
}

void Uart3Handler(void) {
    if (Rx3BufferReady == TRUE) {
        Rx3BufferReady = FALSE;
#ifdef QuantarInterface        
        strip(U3RXOutputBuffer);
        if (0 == strncmp(U3RXOutputBuffer, "\0RSSIRequ",10)) //we are recieving RSSI1 Data
        {
        I2C2_SetLocation(0x40,50);
        LCDprintstr("GetRSSI Reqst Recv'd");
        SendRSSI1Request();
        SendRSSI2Request();
        memset(U3RXOutputBuffer, 0, 128); //clear it, we don't want it full
        }
#endif //End of QuantarInterface
#ifdef UserInterface
        char RSSI[6]; // = (char*) malloc(6);
       // stripNULL(U3RXOutputBuffer);
        //"RSSI1 = 1 -129.1 -12.9
        if (0 == strncmp(U3RXinputBuffer+1, "RSSI1",5)) //we are recieving RSSI1 Data
        {
        memcpy(RSSI,U3RXinputBuffer+7,6);
        //strncpy(RSSI, U3RXinputBuffer+7, 7);
        I2C2_SetLocation(0x21, 50); // line 3 for RSSI1
        LCDprintstr(RSSI);
        memset(U3RXinputBuffer, 0, 128); //clear it, we don't want it full
        return;
        }
        
        if (0 == strncmp(U3RXinputBuffer+1, "RSSI2",5)) //we are recieving RSSI2 Data
        {
        RSSMODE=1;
        strncpy(RSSI, U3RXinputBuffer+7, 6);
        I2C2_SetLocation(0x61, 50); // line 4 for RSSI
        LCDprintstr(RSSI);
        memset(U3RXinputBuffer, 0, 128); //tclear i, we don't want that
        return;
        }
               
#endif //end of UserInterface
    }

}


