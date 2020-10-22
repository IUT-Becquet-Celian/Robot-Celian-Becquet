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

enum StateReception
    {
            Waiting,
            FunctionMSB,
            FunctionLSB,
            PayloadLengthMSB,
            PayloadLengthLSB,
            Payload,
            CheckSum
    };
        StateReception rcvState = StateReception.Waiting;
        int msgDecodedFunction = 0;
        int msgDecodedPayloadLength = 0;
        unsigned char msgDecodedPayload[];
        int msgDecodedPayloadIndex = 0;

#ifdef	__cplusplus
}
#endif

#endif	/* PROTO_H */

