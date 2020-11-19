#include "QEI.h"
#include "Robot.h"
#include <xc.h>

void InitQEI1()
{
    QEI1IOCbits.SWPAB = 1 ; //QEAx and QEBx are swapped
    QEI1GECL = 0xFFFF;
    QEI1GECH = 0xFFFF;
    QEI1CONbits.QEIEN = 1 ; //Enable QEI Module
}

void InitQEI2()
{
    QEI2IOCbits.SWPAB = 1 ; //QEAx and QEBx are not swapped
    QEI2GECL = 0xFFFF;
    QEI2GECH = 0xFFFF;
    QEI2CONbits.QEIEN = 1 ; //Enable QEI Module
}

void QEIUpdateData()
{
    //On sauvegarde les anciennes valeurs
    QEIDroitPosition_T_1 = QEIDroitPostion;
    QEIGauchePosition_T_1 = QEIGauchePostion;

    //On éractualise les valeurs des positions
    long QEIRawValue = POS1CNTL;
    QEI1RawValue += ((long)POS1HDL<<16);

    long QEIRawValue = POS2CNTL;
    QEI2RawValue += ((long)POS2HDL<<16);

    //Conversion en mm (r\'egl\'e pour la taille des roues codeuses)
    QeiDroitPosition = 0.01620*QEI1RawValue;
    QeiGauchePosition = -0.01620*QEI1RawValue;
    
    //Calcul des deltas de position 
    delta_d = QeiDroitPosition - QeiDroitPosition_T_1;
    delta_g = QeiGauchePosition - QeiGauchePosition_T_1;
    
    //delta_theta = atan ((delta_d - delta_g) / DISTROUES);
    delta_theta = atan ((delta_d - delta_g) / DISTROUES);
    dx = (delta_d + delta_g) /2;

    //Calcul des vitesses
    //attention à remultiplier par la fréquence d'échantillonnage
    robotState.vitesseDroitFromOdometry = delta_d*FREQ_ECH_QEI;
    robotState.vitesseGaucheFromOdometry = delta_g*FREQ_ECH_QEI;
    robotState.vitesseLineaireFromOdometry = (robotState.vitesseDroitFromOdometry + robotState.vitesseGaucheFromOdometry)/2;
    robotState.vitesseAngulaireFromOdometry = delta_theta*FREQ_ECH_QEI;
    
    //Mise à jour du positionnement terrain à t-1
    robotState.xPosFromOdometry_1 = robotState.xPosFromOdometry;
    robotState.yPosFromOdometry_1 = robotState.yPosFromOdometry;
    robotState.angleRadianFromOdometry = robotState.angleRadianFromOdometry;

    //Calcul des positions dans le referentiel du terrain
    robotState.xPosFromOdometry = . . .
    robotState.yPosFromOdometry = . . .
    robotState.angleRadianFromOdometry = . . .
    if(robotState.angleRadianFromOdometry > PI)
        robotState.angleRadianFromOdometry ?= 2?PI;
    if (robotState.angleRadianFromOdometry < ?PI)
        robotState.angleRadianFromOdometry += 2?PI;
}
