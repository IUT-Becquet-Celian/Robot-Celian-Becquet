#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <libpic30.h>
#include "proto.h"

enum StateReception {
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

unsigned char CalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char msgPayload[]) {
    unsigned char checksum = 0;
    checksum ^= 0xFE;
    checksum ^= (unsigned char) (msgFunction >> 8);
    checksum ^= (unsigned char) (msgFunction >> 0);
    checksum ^= (unsigned char) (msgPayloadLength >> 8);
    checksum ^= (unsigned char) (msgPayloadLength >> 0);
    int i;
    for (i = 0; i < msgPayloadLength; i++) {
        checksum ^= msgPayload[i];
    }
    return checksum;
}

void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char msgPayload[]) {

    int j;
    unsigned char trame[6 + msgPayloadLength]; //= new unsigned char[6+msgPayloadLength];

    int pos = 0;

    trame[pos++] = 0xFE;

    trame[pos++] = (unsigned char) (msgFunction >> 8);
    trame[pos++] = (unsigned char) (msgFunction >> 0);

    trame[pos++] = (unsigned char) (msgPayloadLength >> 8);
    trame[pos++] = (unsigned char) (msgPayloadLength >> 0);

    for (j = 0; j < msgPayloadLength; j++) {
        trame[pos++] = msgPayload[j];
    }

    trame[pos++] = (unsigned char) (CalculateChecksum(msgFunction, msgPayloadLength, msgPayload));
    //serialPort1.Write(trame, 0, pos);

}

void DecodeMessage(unsigned char c) {
    switch (rcvState) {
        case StateReception.Waiting:
            if (c == 0xFE)
                rcvState = StateReception.FunctionMSB;
            break;

        case StateReception.FunctionMSB:
            msgDecodedFunction = c << 8;
            rcvState = StateReception.FunctionLSB;
            break;

        case StateReception.FunctionLSB:
            msgDecodedFunction += c << 0;
            rcvState = StateReception.PayloadLengthMSB;
            break;

        case StateReception.PayloadLengthMSB:
            msgDecodedPayloadLength = c << 8;
            rcvState = StateReception.PayloadLengthLSB;
            break;

        case StateReception.PayloadLengthLSB:
            msgDecodedPayloadLength += c << 0;
            unsigned char msgDecodedPayload[msgDecodedPayloadLength];
            rcvState = StateReception.Payload;
            break;

        case StateReception.Payload:
            if (msgDecodedPayloadIndex < msgDecodedPayloadLength) {
                msgDecodedPayload[msgDecodedPayloadIndex] = c;
                msgDecodedPayloadIndex++;
            }
            if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                rcvState = StateReception.CheckSum;
            break;

        case StateReception.CheckSum:
            unsigned char receivedChecksum = c;
            unsigned char calculatedChecksum = CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
            //if(calculatedChecksum == receivedChecksum)
            //{
            //    ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
            //}
            rcvState = StateReception.Waiting;
            break;
        default:
            rcvState = StateReception.Waiting;
            break;
    }
}
