#include "QEI.h"
#include "Robot.h"
#include "Utilities.h"
#include <xc.h>
#include <math.h>
#define DISTROUES 281.2

 

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

    QEIRawValue = POS2CNTL;
    QEI2RawValue += ((long)POS2HDL<<16);

    //Conversion en mm (r\'egl\'e pour la taille des roues codeuses)
    QeiDroitPosition = (0.01629854/1000)*QEI1RawValue;
    QeiGauchePosition = (-0.01629854/1000)*QEI2RawValue;
    
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
    robotState.angleRadianFromOdometry_1 = robotState.angleRadianFromOdometry;

    //Calcul des positions dans le referentiel du terrain
    robotState.xPosFromOdometry = robotState.xPosFromOdometry_1 + dx*cos(robotState.angleRadianFromOdometry);
    robotState.yPosFromOdometry = robotState.yPosFromOdometry_1 + dx*sin(robotState.angleRadianFromOdometry);
    robotState.angleRadianFromOdometry = robotState.angleRadianFromOdometry_1 + delta_theta;
    if(robotState.angleRadianFromOdometry > PI)
        robotState.angleRadianFromOdometry -= 2*PI;
    if (robotState.angleRadianFromOdometry < -PI)
        robotState.angleRadianFromOdometry += 2*PI;
}
