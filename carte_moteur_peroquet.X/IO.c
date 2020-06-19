
/*

 * File:   IO.c

 */

#include <xc.h>

#include "IO.h"

void InitIO()
 {

    // IMPORTANT : d�sactiver les entr�es analogiques, sinon on perd les entr�es num�riques

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

    //********** Configuration des entr�es : _TRISxx = 1 ********************************   

    _TRISC0 = 1;
    _TRISC11 = 1;
    _TRISG9 = 1;
    _TRISB1 = 1;
    _TRISE14 = 1;
}