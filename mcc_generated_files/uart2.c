/**
  UART2 Generated Driver File 

  @Company
    Microchip Technology Inc.

  @File Name
    uart2.c

  @Summary
    This is the generated source file for the UART2 driver using MPLAB® Code Configurator

  @Description
    This source file provides APIs for driver for UART2. 
    Generation Information : 
        Product Revision  :  MPLAB® Code Configurator - v2.25.2
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

#include "uart2.h"
#include "./../global.h"
#include <stdio.h>
#include <string.h>


char U2RXinputBuffer[128]="";
char U2RXOutputBuffer[128]="";
int in2BufferCount=0;
int Rx2BufferReady;


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

static UART_OBJECT uart2_obj;

/** UART Driver Queue Length

  @Summary
    Defines the length of the Transmit and Receive Buffers

 */

#define UART2_CONFIG_TX_BYTEQ_LENGTH 8
#define UART2_CONFIG_RX_BYTEQ_LENGTH 8


/** UART Driver Queue

  @Summary
    Defines the Transmit and Receive Buffers

 */

static uint8_t uart2_txByteQ[UART2_CONFIG_TX_BYTEQ_LENGTH];
static uint8_t uart2_rxByteQ[UART2_CONFIG_RX_BYTEQ_LENGTH];

/** UART Hardware FIFO Buffer Length

  @Summary
    Defines the length of the Transmit and Receive FIFOs
 
 */

#define UART2_TX_FIFO_LENGTH 1
#define UART2_RX_FIFO_LENGTH 1 

/**
  Section: Driver Interface
 */


void UART2_Initialize(void) {
    // RTSMD enabled; URXINV disabled; BRGH enabled; STSEL 1; UARTEN enabled; PDSEL 8N; LPBACK disabled; WAKE disabled; USIDL disabled; ABAUD disabled; IREN disabled; UEN TX_RX; 
    U2MODE = 0x8808;
    // UTXEN disabled; UTXINV disabled; URXISEL RX_ONE_CHAR; ADDEN disabled; UTXISEL0 TX_ONE_CHAR; UTXBRK COMPLETED; OERR NO_ERROR_cleared; 
    U2STA = 0x0000;
    // U2TXREG disabled; LAST disabled; 
    U2TXREG = 0x0000;
    // U2RXREG disabled; 
    U2RXREG = 0x0000;
    // U2BRG 416; 
    U2BRG = 0x01A0;
    // ADMMASK disabled; ADMADDR disabled; 
    U2ADMD = 0x0000;
    // SCEN disabled; TXRPT Retransmits the error byte once; T0PD 1 ETU; PTRCL T0; CONV Direct; 
    U2SCCON = 0x0000;
    // TXRPTIE disabled; PARIE disabled; TXRPTIF disabled; RXRPTIE disabled; GTCIF disabled; GTCIE disabled; RXRPTIF disabled; WTCIE disabled; WTCIF disabled; 
    U2SCINT = 0x0000;
    // GTC disabled; 
    U2GTC = 0x0000;
    // WTCL disabled; 
    U2WTCL = 0x0000;
    // WTCH disabled; 
    U2WTCH = 0x0000;

    IEC1bits.U2RXIE = 1;

    U2STAbits.UTXEN = 1;

    uart2_obj.txHead = uart2_txByteQ;
    uart2_obj.txTail = uart2_txByteQ;
    uart2_obj.rxHead = uart2_rxByteQ;
    uart2_obj.rxTail = uart2_rxByteQ;
    uart2_obj.rxStatus.s.empty = true;
    uart2_obj.txStatus.s.empty = true;
    uart2_obj.txStatus.s.full = false;
    uart2_obj.rxStatus.s.full = false;
}

/**
void DRV_UART2_Initialize (void)
 */
void DRV_UART2_Initialize(void) {
    UART2_Initialize();
}

/**
    Maintains the driver's transmitter state machine and implements its ISR
 */
void UART2_TasksTransmit(void) {
    if (uart2_obj.txStatus.s.empty) {
        return;
    }

    IFS1bits.U2TXIF = false;

    int count = 0;
    while ((count < UART2_TX_FIFO_LENGTH)&& !(U2STAbits.UTXBF == 1)) {
        count++;

        U2TXREG = *uart2_obj.txHead;

        uart2_obj.txHead++;

        if (uart2_obj.txHead == (uart2_txByteQ + UART2_CONFIG_TX_BYTEQ_LENGTH)) {
            uart2_obj.txHead = uart2_txByteQ;
        }

        uart2_obj.txStatus.s.full = false;

        if (uart2_obj.txHead == uart2_obj.txTail) {
            uart2_obj.txStatus.s.empty = true;
            break;
        }
    }
}
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void) 
{
 while (U2STAbits.URXDA == 1)
   {
    IFS1bits.U2RXIF = false;
    //reset the Interrupt Flag.
    U2RXinputBuffer[in2BufferCount] = U2RXREG;
    //get the Rx register into the Input Buffer
    if(U2RXinputBuffer[in2BufferCount]==13) //if return/enter is pressed
        { 
       // HeartBeatLED ^=1;
       // delay_ms(300);
       // HeartBeatLED ^=1;
        // __C30_UART=1;
        //printf(U1RXinputBuffer);
        strcpy(U2RXOutputBuffer, U2RXinputBuffer); 
           //copy input buffer to output buffer
           //so that the input buffer can be filled again while we deal with output
           //U1RXOutputBuffer=*U1RXinputBuffer;
        memset(U2RXinputBuffer, 0, 128);
        //clear the input buffer. U1RXinputBuffer="";
        Rx2BufferReady=TRUE; 
        //set the flag so that main knows to deal with the output buffer with data
        in2BufferCount=0;
        //reset the input buffer counter
        //IFS0bits.U1RXIF = false;
        break; // we are done here... go home!
}
    in2BufferCount++;    
   // HeartBeatLED ^=1;
   // delay_ms(100);
   // HeartBeatLED ^=1;
  }
}

/*
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void) {
    int count = 0;

    while ((count < UART2_RX_FIFO_LENGTH) && (U2STAbits.URXDA == 1)) {
        count++;

        *uart2_obj.rxTail = U2RXREG;

        uart2_obj.rxTail++;

        if (uart2_obj.rxTail == (uart2_rxByteQ + UART2_CONFIG_RX_BYTEQ_LENGTH)) {
            uart2_obj.rxTail = uart2_rxByteQ;
        }

        uart2_obj.rxStatus.s.empty = false;

        if (uart2_obj.rxTail == uart2_obj.rxHead) {
            //Sets the flag RX full
            uart2_obj.rxStatus.s.full = true;
            break;
        }

    }

    IFS1bits.U2RXIF = false;

}
*/
void __attribute__((interrupt, no_auto_psv)) _U2ErrInterrupt(void) {
    if ((U2STAbits.OERR == 1)) {
        U2STAbits.OERR = 0;
    }

    IFS4bits.U2ERIF = false;
}

/**
  Section: UART Driver Client Routines
 */

uint8_t UART2_Read(void) {
    uint8_t data = 0;

    data = *uart2_obj.rxHead;

    uart2_obj.rxHead++;

    if (uart2_obj.rxHead == (uart2_rxByteQ + UART2_CONFIG_RX_BYTEQ_LENGTH)) {
        uart2_obj.rxHead = uart2_rxByteQ;
    }

    if (uart2_obj.rxHead == uart2_obj.rxTail) {
        uart2_obj.rxStatus.s.empty = true;
    }

    uart2_obj.rxStatus.s.full = false;

    return data;
}

/**
   uint8_t DRV_UART2_Read ( void )
 */
uint8_t DRV_UART2_Read(void) {
    return (UART2_Read());
}

unsigned int UART2_ReadBuffer(uint8_t *buffer, const unsigned int bufLen) {
    unsigned int numBytesRead = 0;
    while (numBytesRead < (bufLen)) {
        if (uart2_obj.rxStatus.s.empty) {
            break;
        } else {
            buffer[numBytesRead++] = UART2_Read();
        }
    }

    return numBytesRead;
}

/**
   unsigned int DRV_UART2_ReadBuffer( uint8_t *buffer, const unsigned int bufLen)
 */
unsigned int DRV_UART2_ReadBuffer(uint8_t *buffer, const unsigned int bufLen) {
    return (UART2_ReadBuffer(buffer, bufLen));
}

void UART2_Write(const uint8_t byte) {
    *uart2_obj.txTail = byte;

    uart2_obj.txTail++;

    if (uart2_obj.txTail == (uart2_txByteQ + UART2_CONFIG_TX_BYTEQ_LENGTH)) {
        uart2_obj.txTail = uart2_txByteQ;
    }

    uart2_obj.txStatus.s.empty = false;

    if (uart2_obj.txHead == uart2_obj.txTail) {
        uart2_obj.txStatus.s.full = true;
    }


}

/**
   void DRV_UART2_Write( const uint8_t byte)
 */
void DRV_UART2_Write(const uint8_t byte) {
    UART2_Write(byte);
}

unsigned int UART2_WriteBuffer(const uint8_t *buffer, const unsigned int bufLen) {
    unsigned int numBytesWritten = 0;

    while (numBytesWritten < (bufLen)) {
        if ((uart2_obj.txStatus.s.full)) {
            break;
        } else {
            UART2_Write(buffer[numBytesWritten++]);
        }
    }

    return numBytesWritten;

}

/**
   unsigned int DRV_UART2_WriteBuffer( const uint8_t *buffer , const unsigned int bufLen )
 */
unsigned int DRV_UART2_WriteBuffer(const uint8_t *buffer, const unsigned int bufLen) {
    return (UART2_WriteBuffer(buffer, bufLen));
}

UART2_TRANSFER_STATUS UART2_TransferStatusGet(void) {
    UART2_TRANSFER_STATUS status = 0;

    if (uart2_obj.txStatus.s.full) {
        status |= UART2_TRANSFER_STATUS_TX_FULL;
    }

    if (uart2_obj.txStatus.s.empty) {
        status |= UART2_TRANSFER_STATUS_TX_EMPTY;
    }

    if (uart2_obj.rxStatus.s.full) {
        status |= UART2_TRANSFER_STATUS_RX_FULL;
    }

    if (uart2_obj.rxStatus.s.empty) {
        status |= UART2_TRANSFER_STATUS_RX_EMPTY;
    } else {
        status |= UART2_TRANSFER_STATUS_RX_DATA_PRESENT;
    }
    return status;
}

/**
   DRV_UART2_TRANSFER_STATUS UART2_TransferStatusGet (void )
 */
DRV_UART2_TRANSFER_STATUS DRV_UART2_TransferStatusGet(void) {
    return (UART2_TransferStatusGet());
}

uint8_t UART2_Peek(uint16_t offset) {
    if ((uart2_obj.rxHead + offset) > (uart2_rxByteQ + UART2_CONFIG_RX_BYTEQ_LENGTH)) {
        return uart2_rxByteQ[offset - (uart2_rxByteQ + UART2_CONFIG_RX_BYTEQ_LENGTH - uart2_obj.rxHead)];
    } else {
        return *(uart2_obj.rxHead + offset);
    }
}

/**
  uint8_t DRV_UART2_Peek(uint16_t offset)
 */
uint8_t DRV_UART2_Peek(uint16_t offset) {
    return (UART2_Peek(offset));
}

unsigned int UART2_ReceiveBufferSizeGet(void) {
    if (!uart2_obj.rxStatus.s.full) {
        if (uart2_obj.rxHead > uart2_obj.rxTail) {
            return (uart2_obj.rxHead - uart2_obj.rxTail);
        } else {
            return (UART2_CONFIG_RX_BYTEQ_LENGTH - (uart2_obj.rxTail - uart2_obj.rxHead));
        }
    }
    return 0;
}

/**
  unsigned int DRV_UART2_ReceiveBufferSizeGet(void)
 */
unsigned int DRV_UART2_ReceiveBufferSizeGet(void) {
    return (UART2_ReceiveBufferSizeGet());
}

unsigned int UART2_TransmitBufferSizeGet(void) {
    if (!uart2_obj.txStatus.s.full) {
        if (uart2_obj.txHead > uart2_obj.txTail) {
            return (uart2_obj.txHead - uart2_obj.txTail);
        } else {
            return (UART2_CONFIG_TX_BYTEQ_LENGTH - (uart2_obj.txTail - uart2_obj.txHead));
        }
    }
    return 0;
}

/**
  unsigned int DRV_UART2_TransmitBufferSizeGet(void)
 */
unsigned int DRV_UART2_TransmitBufferSizeGet(void) {
    return (UART2_TransmitBufferSizeGet());
}

bool UART2_ReceiveBufferIsEmpty(void) {
    return (uart2_obj.rxStatus.s.empty);
}

/**
  bool DRV_UART2_ReceiveBufferIsEmpty(void)
 */
bool DRV_UART2_ReceiveBufferIsEmpty(void) {
    return (UART2_ReceiveBufferIsEmpty());
}

bool UART2_TransmitBufferIsFull(void) {
    return (uart2_obj.txStatus.s.full);
}

/**
  bool DRV_UART2_TransmitBufferIsFull(void)
 */
bool DRV_UART2_TransmitBufferIsFull(void) {
    return (UART2_TransmitBufferIsFull());
}

UART2_STATUS UART2_StatusGet(void) {
    return U2STA;
}

/**
  DRV_UART2_STATUS DRV_UART2_StatusGet (void)
 */
DRV_UART2_STATUS DRV_UART2_StatusGet(void) {
    return (UART2_StatusGet());
}


/**
  End of File
 */
