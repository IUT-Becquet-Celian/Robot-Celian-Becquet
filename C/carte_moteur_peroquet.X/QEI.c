#include "QEI.h"
#include "Robot.h"
#include "Utilities.h"
#include <xc.h>
#include <math.h>
//#define DISTROUES 0.221

float QeiDroitPosition_T_1 = 0;
float QeiGauchePosition_T_1 = 0;

float delta_d = 0;
float delta_g = 0;

int POS1HDL;
int POS2HDL;

float QeiDroitPosition;
float QeiGauchePosition;

float QeiDroitPosition_T_1;
float QeiGauchePosition_T_1;

float delta_theta;
float dx;

void InitQEI1() {
    QEI1IOCbits.SWPAB = 1; //QEAx and QEBx are swapped
    QEI1GECL = 0xFFFF;
    QEI1GECH = 0xFFFF;
    QEI1CONbits.QEIEN = 1; //Enable QEI Module
}

void InitQEI2() {
    QEI2IOCbits.SWPAB = 1; //QEAx and QEBx are not swapped
    QEI2GECL = 0xFFFF;
    QEI2GECH = 0xFFFF;
    QEI2CONbits.QEIEN = 1; //Enable QEI Module
}

void QEIUpdateData() {
    //On sauvegarde les anciennes valeurs
    QeiDroitPosition_T_1 = QeiDroitPosition;
    QeiGauchePosition_T_1 = QeiGauchePosition;

    //On éractualise les valeurs des positions
    robotState.QEI1RawValue = POS1CNTL;
    robotState.QEI1RawValue += ((long) POS1HLD << 16);

    robotState.QEI2RawValue = POS2CNTL;
    robotState.QEI2RawValue += ((long) POS2HLD << 16);

    //Conversion en mm (r\'egl\'e pour la taille des roues codeuses)
    QeiDroitPosition = (0.01629854 / 1000) * robotState.QEI1RawValue;
    QeiGauchePosition = (-0.01629854 / 1000) * robotState.QEI2RawValue;

    //Calcul des deltas de position 
    delta_d = QeiDroitPosition - QeiDroitPosition_T_1;
    delta_g = QeiGauchePosition - QeiGauchePosition_T_1;
    //delta_theta = atan ((delta_d - delta_g) / DISTROUES);
    delta_theta = atan((delta_d - delta_g) / DISTROUES);
    dx = (delta_d + delta_g) / 2;

    //Calcul des vitesses
    //attention à remultiplier par la fréquence d'échantillonnage
    robotState.vitesseDroitFromOdometry = delta_d*FREQ_ECH_QEI;
    robotState.vitesseGaucheFromOdometry = delta_g*FREQ_ECH_QEI;
    robotState.vitesseLineaireFromOdometry = (robotState.vitesseDroitFromOdometry + robotState.vitesseGaucheFromOdometry) / 2;
    robotState.vitesseAngulaireFromOdometry = delta_theta*FREQ_ECH_QEI;
    unsigned char payload[] = {robotState.vitesseGaucheFromOdometry, robotState.vitesseDroitFromOdometry};
    UartEncodeAndSendMessage(0x0040, 2, payload);

    //Mise à jour du positionnement terrain à t-1
    robotState.xPosFromOdometry_1 = robotState.xPosFromOdometry;
    robotState.yPosFromOdometry_1 = robotState.yPosFromOdometry;
    robotState.angleRadianFromOdometry_1 = robotState.angleRadianFromOdometry;

    //Calcul des positions dans le referentiel du terrain
    robotState.xPosFromOdometry = robotState.xPosFromOdometry_1 + dx * cos(robotState.angleRadianFromOdometry_1);
    robotState.yPosFromOdometry = robotState.yPosFromOdometry_1 + dx * sin(robotState.angleRadianFromOdometry_1);
    robotState.angleRadianFromOdometry = robotState.angleRadianFromOdometry_1 + delta_theta;
    if (robotState.angleRadianFromOdometry > PI)
        robotState.angleRadianFromOdometry -= 2 * PI;
    if (robotState.angleRadianFromOdometry < -PI)
        robotState.angleRadianFromOdometry += 2 * PI;
}
