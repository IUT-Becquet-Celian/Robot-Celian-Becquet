#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "IO.h"
#include "CB_TX1.h"

#define CBTX1_BUFFER_SIZE 128

int cbTX1Head;
int cbTX1Tail;
unsigned char cbTX1Buffer [CBTX1_BUFFER_SIZE ] ;
unsigned char isTransmitting = 0 ;

void SendMessage (unsigned char* message , int length)
{
    unsigned char i =0;
    if (CB_TX1_RemainingSize() >1)//length)
    {
//On peut écrire le message
        for (i=0;i<length;i++)
            CB_TX1_Add(message[i]);
        if (!CB_TX1_IsTranmitting())
            SendOne();
    }
}

void CB_TX1_Add(unsigned char value)
{
    cbTX1Buffer[cbTX1Head]++ = value;
    if (cbTX1Head == CBTX1_BUFFER_SIZE)
        cbTX1Head = 0;
}

unsigned char CB_TX1_Get(void)
{
    unsigned char mot = cbTX1Buffer[cbTX1Tail++];
    if (cbTX1Tail == CBTX1_BUFFER_SIZE)
        cbTX1Tail = 0;
    return mot;
}

void __attribute__ ((interrupt, no_auto_psv )) _U1TXInterrupt(void) {
    IFS0bits.U1TXIF = 0; // c l e a r TX i n t e r r u p t f l a g
    if(cbTX1Tail!=cbTX1Head)
        SendOne();
    else
        isTransmitting = 0;
}

void SendOne()
{
    isTransmitting = 1 ;
    unsigned char value=CB_TX1_Get ( ) ;
    U1TXREG = value ; // Transmit one c h a r a c t e r
}

unsigned char CB_TX1_IsTranmitting (void)
{
    return isTransmitting;
}

int CB_TX1_RemainingSize(void)
{
    int rSize;
    if (cbTX1Head > cbTX1Tail)
        return CBTX1_BUFFER_SIZE - (cbTX1Head - cbTX1Tail);
    else
        return CBTX1_BUFFER_SIZE - (cbTX1Head - cbTX1Tail);
    return rSize;
}

