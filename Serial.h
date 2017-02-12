/* 
 * File:   Serial.h
 * Author: hagermanj
 *
 * Created on August 14, 2015, 3:59 PM
 */

#ifndef SERIAL_H
#define	SERIAL_H

#ifdef	__cplusplus
extern "C" {
#endif



void U1PutC(char c);
void U1PutS(char *s);

void U2PutC(char c);
void U2PutS(char *s);

void U3PutC(char c);
void U3PutS(char *s);

int U1_getchar(void);
int U2_getchar(void);
char UART1GetChar(void);
bool get_Char();
void strip(char *s);
void stripNULL(char *s);

#ifdef	__cplusplus
}
#endif

#endif	/* SERIAL_H */

