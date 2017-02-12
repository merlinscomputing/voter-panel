/**
  UART1 Generated Driver File 

  @Company
    Microchip Technology Inc.

  @File Name
    uart1.c

  @Summary
    This is the generated source file for the UART1 driver using MPLAB� Code Configurator

  @Description
    This source file provides APIs for driver for UART1. 
    Generation Information : 
        Product Revision  :  MPLAB� Code Configurator - v2.25.2
        Device            :  PIC24FJ128GB202
        Driver Version    :  0.5
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.24
        MPLAB 	          :  MPLAB X v2.35 or v3.00
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

/**
  Section: Included Files
 */

#include "uart1.h"
//#include "./../delay.h"
#include "./../global.h"
#include <stdio.h>
#include <string.h>
//#include "./../UserCommands.h"
//#include <libpic30.h> 

char U1RXinputBuffer[128]="";
char U1RXOutputBuffer[128]="";
int in1BufferCount=0;
int Rx1BufferReady;
/**
  Section: Data Type Definitions
 */

/** UART Driver Queue Status

  @Summary
    Defines the object required for the status of the queue.
 */

typedef union {

    struct {
        uint8_t full : 1;
        uint8_t empty : 1;
        uint8_t reserved : 6;
    } s;
    uint8_t status;
}

UART_BYTEQ_STATUS;

/** UART Driver Hardware Instance Object

  @Summary
    Defines the object required for the maintenance of the hardware instance.

 */
typedef struct {
    /* RX Byte Q */
    uint8_t *rxTail;

    uint8_t *rxHead;

    /* TX Byte Q */
    uint8_t *txTail;

    uint8_t *txHead;

    UART_BYTEQ_STATUS rxStatus;

    UART_BYTEQ_STATUS txStatus;

} UART_OBJECT;

static UART_OBJECT uart1_obj;

/** UART Driver Queue Length

  @Summary
    Defines the length of the Transmit and Receive Buffers

 */

#define UART1_CONFIG_TX_BYTEQ_LENGTH 8
#define UART1_CONFIG_RX_BYTEQ_LENGTH 8 //32


/** UART Driver Queue

  @Summary
    Defines the Transmit and Receive Buffers

 */

static uint8_t uart1_txByteQ[UART1_CONFIG_TX_BYTEQ_LENGTH];
static uint8_t uart1_rxByteQ[UART1_CONFIG_RX_BYTEQ_LENGTH];

/** UART Hardware FIFO Buffer Length

  @Summary
    Defines the length of the Transmit and Receive FIFOs
 
 */

#define UART1_TX_FIFO_LENGTH 1
#define UART1_RX_FIFO_LENGTH 1 

/**
  Section: Driver Interface
 */


void UART1_Initialize(void) {
    // RTSMD enabled; URXINV disabled; BRGH enabled; STSEL 1; UARTEN enabled; PDSEL 8N; LPBACK disabled; WAKE disabled; USIDL disabled; ABAUD disabled; IREN disabled; UEN TX_RX; 
    U1MODE = 0x8808;
    // UTXEN disabled; UTXINV disabled; URXISEL RX_ONE_CHAR; ADDEN disabled; UTXISEL0 TX_ONE_CHAR; UTXBRK COMPLETED; OERR NO_ERROR_cleared; 
    U1STA = 0x0000;
    // U1TXREG disabled; LAST disabled; 
    U1TXREG = 0x0000;
    // U1RXREG disabled; 
    U1RXREG = 0x0000;
    // U1BRG 416; 
    U1BRG = 0x01A0;
    // ADMMASK disabled; ADMADDR disabled; 
    U1ADMD = 0x0000;
    // SCEN disabled; TXRPT Retransmits the error byte once; T0PD 1 ETU; PTRCL T0; CONV Direct; 
    U1SCCON = 0x0000;
    // TXRPTIE disabled; PARIE disabled; TXRPTIF disabled; RXRPTIE disabled; GTCIF disabled; GTCIE disabled; RXRPTIF disabled; WTCIE disabled; WTCIF disabled; 
    U1SCINT = 0x0000;
    // GTC disabled; 
    U1GTC = 0x0000;
    // WTCL disabled; 
    U1WTCL = 0x0000;
    // WTCH disabled; 
    U1WTCH = 0x0000;

    IEC0bits.U1RXIE = 1;

    U1STAbits.UTXEN = 1;

    uart1_obj.txHead = uart1_txByteQ;
    uart1_obj.txTail = uart1_txByteQ;
    uart1_obj.rxHead = uart1_rxByteQ;
    uart1_obj.rxTail = uart1_rxByteQ;
    uart1_obj.rxStatus.s.empty = true;
    uart1_obj.txStatus.s.empty = true;
    uart1_obj.txStatus.s.full = false;
    uart1_obj.rxStatus.s.full = false;
}

/**
void DRV_UART1_Initialize (void)
 */
void DRV_UART1_Initialize(void) {
    UART1_Initialize();
}

/**
    Maintains the driver's transmitter state machine and implements its ISR
 */
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void) {
    if (uart1_obj.txStatus.s.empty) {
        IEC0bits.U1TXIE = false;
        return;
    }

    IFS0bits.U1TXIF = false;

    int count = 0;
    while ((count < UART1_TX_FIFO_LENGTH)&& !(U1STAbits.UTXBF == 1)) {
        count++;

        U1TXREG = *uart1_obj.txHead;

        uart1_obj.txHead++;

        if (uart1_obj.txHead == (uart1_txByteQ + UART1_CONFIG_TX_BYTEQ_LENGTH)) {
            uart1_obj.txHead = uart1_txByteQ;
        }

        uart1_obj.txStatus.s.full = false;

        if (uart1_obj.txHead == uart1_obj.txTail) {
            uart1_obj.txStatus.s.empty = true;
            break;
        }
    }
}

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void) 
{
 while (U1STAbits.URXDA == 1)
   {
    IFS0bits.U1RXIF = false;
    //reset the Interrupt Flag.
    U1RXinputBuffer[in1BufferCount] = U1RXREG;
    //get the Rx register into the Input Buffer
    if(U1RXinputBuffer[in1BufferCount]==13) //if return/enter is pressed
        { 
       // HeartBeatLED ^=1;
       // delay_ms(300);
       // HeartBeatLED ^=1;
        // __C30_UART=1;
        //printf(U1RXinputBuffer);
        strcpy(U1RXOutputBuffer, U1RXinputBuffer); 
           //copy input buffer to output buffer
           //so that the input buffer can be filled again while we deal with output
           //U1RXOutputBuffer=*U1RXinputBuffer;
        memset(U1RXinputBuffer, 0, 128);
        //clear the input buffer. U1RXinputBuffer="";
        Rx1BufferReady=TRUE; 
        //set the flag so that main knows to deal with the output buffer with data
        in1BufferCount=0;
        //reset the input buffer counter
        //IFS0bits.U1RXIF = false;
        break; // we are done here... go home!
        }
    in1BufferCount++;    
   // HeartBeatLED ^=1;
   // delay_ms(100);
   // HeartBeatLED ^=1;
  }
}

/*
 //original U1RX Interrupt
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void) {
    int count = 0;

    while ((count < UART1_RX_FIFO_LENGTH) && (U1STAbits.URXDA == 1)) {
        count++; 
              
        *uart1_obj.rxTail = U1RXREG; // put the uart reg char to the ring buffer
       
        // U1TXREG= *uart1_obj.rxTail; //echo the char to the screen
        
        uart1_obj.rxTail++; //increase the ring buffer pointer

        if (uart1_obj.rxTail == (uart1_rxByteQ + UART1_CONFIG_RX_BYTEQ_LENGTH)) {
            uart1_obj.rxTail = uart1_rxByteQ;
        }
     
        uart1_obj.rxStatus.s.empty = false;

        if (uart1_obj.rxTail == uart1_obj.rxHead) {
            //Sets the flag RX full
            uart1_obj.rxStatus.s.full = true;
            break;
        }

    } //end of While (count &&)
    
    IFS0bits.U1RXIF = false;
}
*/

void __attribute__((interrupt, no_auto_psv)) _U1ErrInterrupt(void) {
    if ((U1STAbits.OERR == 1)) {
        U1STAbits.OERR = 0;
    }

    IFS4bits.U1ERIF = false;
}

/**
  Section: UART Driver Client Routines
 */

uint8_t UART1_Read(void) {
    uint8_t data = 0;

    data = *uart1_obj.rxHead;

    uart1_obj.rxHead++;

    if (uart1_obj.rxHead == (uart1_rxByteQ + UART1_CONFIG_RX_BYTEQ_LENGTH)) {
        uart1_obj.rxHead = uart1_rxByteQ;
    }

    if (uart1_obj.rxHead == uart1_obj.rxTail) {
        uart1_obj.rxStatus.s.empty = true;
    }

    uart1_obj.rxStatus.s.full = false;

    return data;
}

/**
   uint8_t DRV_UART1_Read ( void )
 */
uint8_t DRV_UART1_Read(void) {
    return (UART1_Read());
}

unsigned int UART1_ReadBuffer(uint8_t *buffer, const unsigned int bufLen) {
    unsigned int numBytesRead = 0;
    while (numBytesRead < (bufLen)) {
        if (uart1_obj.rxStatus.s.empty) {
            break;
        } else {
            buffer[numBytesRead++] = UART1_Read();
        }
    }

    return numBytesRead;
}

/**
   unsigned int DRV_UART1_ReadBuffer( uint8_t *buffer, const unsigned int bufLen)
 */
unsigned int DRV_UART1_ReadBuffer(uint8_t *buffer, const unsigned int bufLen) {
    return (UART1_ReadBuffer(buffer, bufLen));
}

void UART1_Write(const uint8_t byte) {
    *uart1_obj.txTail = byte;

    uart1_obj.txTail++;

    if (uart1_obj.txTail == (uart1_txByteQ + UART1_CONFIG_TX_BYTEQ_LENGTH)) {
        uart1_obj.txTail = uart1_txByteQ;
    }

    uart1_obj.txStatus.s.empty = false;

    if (uart1_obj.txHead == uart1_obj.txTail) {
        uart1_obj.txStatus.s.full = true;
    }

    if (IEC0bits.U1TXIE == false) {
        IEC0bits.U1TXIE = true;
    }

}

/**
   void DRV_UART1_Write( const uint8_t byte)
 */
void DRV_UART1_Write(const uint8_t byte) {
    UART1_Write(byte);
}

unsigned int UART1_WriteBuffer(const uint8_t *buffer, const unsigned int bufLen) {
    unsigned int numBytesWritten = 0;

    while (numBytesWritten < (bufLen)) {
        if ((uart1_obj.txStatus.s.full)) {
            break;
        } else {
            UART1_Write(buffer[numBytesWritten++]);
        }
    }

    return numBytesWritten;

}

/**
   unsigned int DRV_UART1_WriteBuffer( const uint8_t *buffer , const unsigned int bufLen )
 */
unsigned int DRV_UART1_WriteBuffer(const uint8_t *buffer, const unsigned int bufLen) {
    return (UART1_WriteBuffer(buffer, bufLen));
}

UART1_TRANSFER_STATUS UART1_TransferStatusGet(void) {
    UART1_TRANSFER_STATUS status = 0;

    if (uart1_obj.txStatus.s.full) {
        status |= UART1_TRANSFER_STATUS_TX_FULL;
    }

    if (uart1_obj.txStatus.s.empty) {
        status |= UART1_TRANSFER_STATUS_TX_EMPTY;
    }

    if (uart1_obj.rxStatus.s.full) {
        status |= UART1_TRANSFER_STATUS_RX_FULL;
    }

    if (uart1_obj.rxStatus.s.empty) {
        status |= UART1_TRANSFER_STATUS_RX_EMPTY;
    } else {
        status |= UART1_TRANSFER_STATUS_RX_DATA_PRESENT;
    }
    return status;
}

/**
   DRV_UART1_TRANSFER_STATUS UART1_TransferStatusGet (void )
 */
DRV_UART1_TRANSFER_STATUS DRV_UART1_TransferStatusGet(void) {
    return (UART1_TransferStatusGet());
}

uint8_t UART1_Peek(uint16_t offset) {
    if ((uart1_obj.rxHead + offset) > (uart1_rxByteQ + UART1_CONFIG_RX_BYTEQ_LENGTH)) {
        return uart1_rxByteQ[offset - (uart1_rxByteQ + UART1_CONFIG_RX_BYTEQ_LENGTH - uart1_obj.rxHead)];
    } else {
        return *(uart1_obj.rxHead + offset);
    }
}

/**
  uint8_t DRV_UART1_Peek(uint16_t offset)
 */
uint8_t DRV_UART1_Peek(uint16_t offset) {
    return (UART1_Peek(offset));
}

unsigned int UART1_ReceiveBufferSizeGet(void) {
    if (!uart1_obj.rxStatus.s.full) {
        if (uart1_obj.rxHead > uart1_obj.rxTail) {
            return (uart1_obj.rxHead - uart1_obj.rxTail);
        } else {
            return (UART1_CONFIG_RX_BYTEQ_LENGTH - (uart1_obj.rxTail - uart1_obj.rxHead));
        }
    }
    return 0;
}

/**
  unsigned int DRV_UART1_ReceiveBufferSizeGet(void)
 */
unsigned int DRV_UART1_ReceiveBufferSizeGet(void) {
    return (UART1_ReceiveBufferSizeGet());
}

unsigned int UART1_TransmitBufferSizeGet(void) {
    if (!uart1_obj.txStatus.s.full) {
        if (uart1_obj.txHead > uart1_obj.txTail) {
            return (uart1_obj.txHead - uart1_obj.txTail);
        } else {
            return (UART1_CONFIG_TX_BYTEQ_LENGTH - (uart1_obj.txTail - uart1_obj.txHead));
        }
    }
    return 0;
}

/**
  unsigned int DRV_UART1_TransmitBufferSizeGet(void)
 */
unsigned int DRV_UART1_TransmitBufferSizeGet(void) {
    return (UART1_TransmitBufferSizeGet());
}

bool UART1_ReceiveBufferIsEmpty(void) {
    return (uart1_obj.rxStatus.s.empty);
}

/**
  bool DRV_UART1_ReceiveBufferIsEmpty(void)
 */
bool DRV_UART1_ReceiveBufferIsEmpty(void) {
    return (UART1_ReceiveBufferIsEmpty());
}

bool UART1_TransmitBufferIsFull(void) {
    return (uart1_obj.txStatus.s.full);
}

/**
  bool DRV_UART1_TransmitBufferIsFull(void)
 */
bool DRV_UART1_TransmitBufferIsFull(void) {
    return (UART1_TransmitBufferIsFull());
}

UART1_STATUS UART1_StatusGet(void) {
    return U1STA;
}

/**
  DRV_UART1_STATUS DRV_UART1_StatusGet (void)
 */
DRV_UART1_STATUS DRV_UART1_StatusGet(void) {
    return (UART1_StatusGet());
}
/**
  End of File
 */
