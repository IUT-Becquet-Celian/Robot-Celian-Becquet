/*
 * File:   IO.c
 */

#include <xc.h>
#include "IO.h"


void InitIO()
 {
    

    // IMPORTANT : désactiver les entrées analogiques, sinon on perd les entrées numériques

    ANSELA = 0; // 0 desactive
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    ANSELF = 0;
    ANSELG = 0;

    //********** Configuration des sorties : _TRISxx = 0 ********************************

    // LED

    _TRISC10 = 0; // LED Orange
    _TRISG6 = 0; //LED Blanche
    _TRISG7 = 0; // LED Bleue
    _TRISA11 = 1;

    // Moteurs 

    _TRISB14=0; //B14
    _TRISB15=0; //B15
    _TRISC6=0; //C6
    _TRISC7=0; //C7

    //********** Configuration des entrées : _TRISxx = 1 ********************************   

    _TRISC0 = 1;
    _TRISC11 = 1;
    _TRISG9 = 1;
    _TRISB1 = 1;
    _TRISE14 = 1;
    
    //UART
    _U1RXR = 24; //Remappe la RP . . . sur l ?entrée Rx1
    _RP36R = 0b00001 ; //Remappe la sortie Tx1 vers RP

    _QEA2R = 97; // assign QEI A to pin RP97
    _QEB2R = 96; // assign QEI A to pin RP96
    _QEA1R = 70; // assign QEI A to pin RP70
    _QEB1R = 69; // assign QEI A to pin RP69

}
