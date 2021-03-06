/* 
 * File:   proto.h
 * Author: table4
 *
 * Created on 21 octobre 2020, 17:45
 */

#ifndef PROTO_H
#define	PROTO_H
#define SET_ROBOT_STATE 0x0051
#define SET_ROBOT_MANUAL_CONTROL 0x0052
#define SET_CONSIGNE 0x0041



void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char msgPayload[]);
void UartDecodeMessage(unsigned char c);
void UartProcessDecodedMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);
unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char msgPayload[]);
void SetRobotState (unsigned char state);
void SetRobotAutoControlState (unsigned char state);

//float vitessePolaireLineaireConsigne=0;

#ifdef	__cplusplus
}
#endif

#endif	/* PROTO_H */

