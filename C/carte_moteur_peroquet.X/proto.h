/* 
 * File:   proto.h
 * Author: table4
 *
 * Created on 21 octobre 2020, 17:45
 */

#ifndef PROTO_H
#define	PROTO_H

void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char msgPayload[]);
void DecodeMessage(unsigned char c);
unsigned char CalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char msgPayload[]);



#ifdef	__cplusplus
}
#endif

#endif	/* PROTO_H */

