/* 
 * File:   iLCD.h
 * Author: jhagerman
 *
 * Created on March 25, 2015, 5:28 AM
 */

#ifndef ILCD_H
#define	ILCD_H

void LCDInit(void);
void LCDTest(void);
//void I2CInit(void);
void StartI2C2(void);
void StopI2C2(void);
void IdleI2C2(void);
void MasterWriteI2C2(char data);   
void I2C2_SetLocation(char data, int delay);
void I2C2_Command_Write(char data, int delay);
void I2C2_Data_Write(char data, int delay);
void LCDprintstr(const char c[]);
void BacklightON(void);
/*
 * 
 command - RS and RW Low
 * Data - RS high RW Low
 * BF High means Busy - do not write to me
 * 
 */

#endif	/* ILCD_H */

