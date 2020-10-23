#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <libpic30.h>
#include "UART_Protocol.h"
#include "CB_TX1.h"

/*enum StateReception {
    Waiting,
    FunctionMSB,
    FunctionLSB,
    PayloadLengthMSB,
    PayloadLengthLSB,
    Payload,
    CheckSum
};

StateReception rcvState = StateReception.Waiting;*/


unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload) {
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

void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload) {

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

    trame[pos++] = (unsigned char) (UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload));
    SendMessage(trame,pos);
    //serialPort1.Write(trame, 0, pos);

}

int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;

void UartDecodeMessage(unsigned char c) {
    //Fonction prenant en entree un octet et servant a reconstituer les trames
    /*
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
    }*/
}

void ProcessDecodedMessage( int msgFunction, int msgPayloadLength, unsigned char* msgPayload)
    //Fonction appelee apres le decodage pour executer l'action correspondant au message recu
        {
    /*
            switch (msgFunction)
            {                
                case 0x30: // IR data
                    flagNewIrData = true;
                    distanceTelemetreGauche = msgPayload[0];
                    distanceTelemetreCentre = msgPayload[1];
                    distanceTelemetreDroit = msgPayload[2];
                    break;
                    
                case 0x40:
                    robot.flagNewVitesseData = true;
                    robot.vitesseMoteurGauche= msgPayload[0];
                    robot.vitesseMoteurDroit = msgPayload[1];
                   break;

                case 0x80: // Text transmission
                    robot.flagNewReceptionData = true;
                    robot.receivedMessage = System.Text.Encoding.UTF8.GetString(msgPayload);
                    break;

                default: // Unknow command
                    break;
            }
      */

        }

