/* 
 * File:   UserCommands.h
 * Author: jhagerman
 *
 * Created on August 5, 2015, 5:14 AM
 */

#ifndef USERCOMMANDS_H
#define	USERCOMMANDS_H

#ifdef	__cplusplus
extern "C" {
#endif

   
    
#define  XMTBUFSIZE  32                         // output buffer size
#define  RCVBUFSIZE  64                         // input buffer size
#define  DELTA       17                         // minimum free rcv buffer ..
                                                // .. space (PC UARTFiFo + 1)
#define QtrDoRSSCommand   "dorss \r\n"
#define QtrGetRSSICommand "GET DSP RSSI 1 1 SHORT \r\n" //RSSI = 1 -128.7 -12.5
#define QtrGetVerCommand  "getVer  \r\n"  
#define QtrGetSNCommand   "GET STN SN  \r\n"  //STN SN = <station_serial_number>
#define QtrPTTCommand     "RC SET PTT XMIT  \r\n"
#define QtrPTTOffCommand  "RC SET PTT OFF \r\n"
    

void comm_keep_alive(void);
void SendRSSI1Request(void);
void SendRSSI2Request(void);
void SendRSSIRequest(void);
void UartHello(void);
void Uart1Handler(void);
void Uart2Handler(void);
void Uart3Handler(void);
 
#ifdef	__cplusplus
}
#endif

#endif	/* USERCOMMANDS_H */

