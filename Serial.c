#include <xc.h>
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/uart2.h"
#include "mcc_generated_files/uart3.h"
#include "UserCommands.h"
#include "QuantarCommands.h"
#include <stdio.h>
#include <string.h>
#include "global.h"
#include "delay.h"

void U1PutC(char c)
{
while(U1STAbits.UTXBF == 1);
U1TXREG=c;
}
   
void U1PutS(char *s)
{
    while(*s)
    {
        U1PutC(*s);
        s++;
    }
}
void U2PutC(char c)
{
while(U2STAbits.UTXBF == 1);
U2TXREG=c;
}

void U2PutS(char *s)
{
    while(*s)
    {
        U2PutC(*s);
        s++;
    }
}

void U3PutC(char c)
{
while(U3STAbits.UTXBF == 1);
U3TXREG=c;   
}

void U3PutS(char *s)
{
    while(*s)
    {
        U3PutC(*s);
        s++;
    }
}

char character = 0;
       
bool get_Char()
{
    if(U1STAbits.URXDA == 1)
    {
        character = U1RXREG;
        return TRUE;
    }
    else
        return FALSE; 
}

void strip(char *s)
{
    char *p2 = s;
    while(*s != '\0') {
    	if(*s != '\r' && *s != '\n') {
    		*p2++ = *s++;
    	} else {
    		++s;
    	}
    }
    *p2 = '\0';
}

void stripNULL(char *s)
{
    char *p2 = s;
    while(*s < strlen(s)) {
    	if(*s != '\0') {
    		*p2++ = *s++;
    	} else {
    		++s;
    	}
    }
   // *p2 = '\0';
}