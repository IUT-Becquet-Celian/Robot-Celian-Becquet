//Partie PWM
#include <xc.h> // library xc.h inclut tous les uC
#include "IO.h"
#include "PWM.h"
#include "Robot.h"
#include "ToolBox.h"
#include "Utilities.h"
#include "QEI.h"

#define PWMPER 40.0

void InitPWM(void) {
    PTCON2bits.PCLKDIV = 0b000; //Divide by 1
    PTPER = 100 * PWMPER; //Période en pourcentage

    //Réglage PWM moteur 1 sur hacheur 1
    IOCON1bits.POLH = 1; //High = 1 and active on low =0
    IOCON1bits.POLL = 1; //High = 1
    IOCON1bits.PMOD = 0b01; //Set PWM Mode to Redundant
    FCLCON1 = 0x0003; //Désactive la gestion des faults

    //Reglage PWM moteur 2 sur hacheur 6
    IOCON6bits.POLH = 1; //High = 1
    IOCON6bits.POLL = 1; //High = 1
    IOCON6bits.PMOD = 0b01; //Set PWM Mode to Redundant
    FCLCON6 = 0x0003; //Désactive la gestion des faults

    /* Enable PWM Module */
    PTCONbits.PTEN = 1;
}

float acceleration = 10;

void PWMUpdateSpeed() {
    // Cette fonction est appelée sur timer et permet de suivre des rampes d?accélération
    if (robotState.vitesseDroiteCommandeCourante < robotState.vitesseDroiteConsigne)
        robotState.vitesseDroiteCommandeCourante = Min(
            robotState.vitesseDroiteCommandeCourante + acceleration,
            robotState.vitesseDroiteConsigne);
    if (robotState.vitesseDroiteCommandeCourante > robotState.vitesseDroiteConsigne)
        robotState.vitesseDroiteCommandeCourante = Max(
            robotState.vitesseDroiteCommandeCourante - acceleration,
            robotState.vitesseDroiteConsigne);

    if (robotState.vitesseDroiteCommandeCourante > 0) {
        MOTEUR_DROIT_ENL = 0; //pilotage de la pin en mode IO
        MOTEUR_DROIT_INL = 1; //Mise à 1 de la pin
        MOTEUR_DROIT_ENH = 1; //Pilotage de la pin en mode PWM
    } else {
        MOTEUR_DROIT_ENH = 0; //pilotage de la pin en mode IO
        MOTEUR_DROIT_INH = 1; //Mise à 1 de la pin
        MOTEUR_DROIT_ENL = 1; //Pilotage de la pin en mode PWM
    }
    MOTEUR_DROIT_DUTY_CYCLE = Abs(robotState.vitesseDroiteCommandeCourante) * PWMPER;

    if (robotState.vitesseGaucheCommandeCourante < robotState.vitesseGaucheConsigne)
        robotState.vitesseGaucheCommandeCourante = Min(
            robotState.vitesseGaucheCommandeCourante + acceleration,
            robotState.vitesseGaucheConsigne);
    if (robotState.vitesseGaucheCommandeCourante > robotState.vitesseGaucheConsigne)
        robotState.vitesseGaucheCommandeCourante = Max(robotState.vitesseGaucheCommandeCourante - acceleration, robotState.vitesseGaucheConsigne);

    if (robotState.vitesseGaucheCommandeCourante > 0) {
        MOTEUR_GAUCHE_ENL = 0; //pilotage de la pin en mode IO
        MOTEUR_GAUCHE_INL = 1; //Mise à 1 de la pin
        MOTEUR_GAUCHE_ENH = 1; //Pilotage de la pin en mode PWM
    } else {
        MOTEUR_GAUCHE_ENH = 0; //pilotage de la pin en mode IO
        MOTEUR_GAUCHE_INH = 1; //Mise à 1 de la pin
        MOTEUR_GAUCHE_ENL = 1; //Pilotage de la pin en mode PWM
    }
    MOTEUR_GAUCHE_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante) * PWMPER;
}


void PWMSetSpeedConsigne (float vitesseEnPourcents, char moteur)
{
    
     if(moteur == MOTEUR_GAUCHE)
     {
         robotState.vitesseGaucheConsigne = vitesseEnPourcents;
     }
     else
     {
         robotState.vitesseDroiteConsigne = -vitesseEnPourcents;   
     }
    
}


#define COEFF_VITESSE_LINEAIRE_PERCENT 1/25
#define COEFF_VITESSE_ANGULAIRE_PERCENT 1/50
double IntegralVitesseAngulaire = 0;
double tEch = 0.02;
double corrKpMax = 2;
double corrKiMax = 2;
double corrKdMax = 2;
double Kp = 2;
double Ki = 50;
double Kd = 50;
double erreurVitesseAngulaire_1=0 ; //erreur à t-1

void PWMSetSpeedConsignePolaire()
{
    robotState.vitesseAngulaireConsigne=3;
//    robotState.vitesseLineaireConsigne=1;
    //Correction Angulaire
    double erreurVitesseAngulaire = robotState.vitesseAngulaireConsigne - robotState.vitesseAngulaireFromOdometry;  //Il faut pouvoir visualiser les deux sur l'interface !
    double corrPVitesseAngulaire = erreurVitesseAngulaire * Kp;
    corrPVitesseAngulaire = LimitToInterval(corrPVitesseAngulaire, -corrKpMax, corrKpMax); //On plafonne l'effet Kp
    
    IntegralVitesseAngulaire += erreurVitesseAngulaire*tEch;
    IntegralVitesseAngulaire = LimitToInterval(IntegralVitesseAngulaire, -corrKiMax / Ki, corrKiMax /Ki );    
    double corrIVitesseAngulaire = IntegralVitesseAngulaire * Ki; 
    
    double deriveeVitesseAngulaire = (erreurVitesseAngulaire - erreurVitesseAngulaire_1)/tEch;
    double corrDVitesseAngulaire = LimitToInterval(deriveeVitesseAngulaire * Kd, -corrKdMax, corrKdMax);
    
    double sortieCorrecteurAngulaire = corrPVitesseAngulaire+corrIVitesseAngulaire+corrDVitesseAngulaire;
    //double correctionVitesseAngulaire =
    double correctionVitesseAngulairePourcent = sortieCorrecteurAngulaire * COEFF_VITESSE_ANGULAIRE_PERCENT;
    
    //Correction Lineaire
//    double erreurVitesseLineaire = robotState.vitesseLineaireConsigne - robotState.vitesseLineaireFromOdometry;
//    double sortieCorrecteurLineaire = P * erreurVitesseLineaire;
    double erreurVitesseLineaire = 0;
    double sortieCorrecteurLineaire = 0;
    //double correctionVitesseLineaire =
//    double correctionVitesseLineairePourcent = sortieCorrecteurLineaire * COEFF_VITESSE_LINEAIRE_PERCENT;
    double correctionVitesseLineairePourcent = 0;
    
    //Generation des consignes droite et gauche 
    robotState.vitesseDroiteConsigne = correctionVitesseLineairePourcent + correctionVitesseAngulairePourcent;
    robotState.vitesseDroiteConsigne = LimitToInterval(robotState.vitesseDroiteConsigne, -100, 100);

    robotState.vitesseGaucheConsigne = correctionVitesseLineairePourcent - correctionVitesseAngulairePourcent;
    robotState.vitesseGaucheConsigne = LimitToInterval(robotState.vitesseGaucheConsigne, -100, 100);
}

/*void PWMSetSpeed(float vitesseEnPourcents, int moteur) {
    robotState.vitesseGaucheCommandeCourante = vitesseEnPourcents;
    if(moteur == 1){
        if (vitesseEnPourcents>0)
        {
            MOTEUR_GAUCHE_ENL = 0; //Pilotage de la pin en mode IO
            MOTEUR_GAUCHE_INL = 1; //Mise à 1 de la pin
            MOTEUR_GAUCHE_ENH = 1; //Pilotage de la pin en mode PWM
            MOTEUR_GAUCHE_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante * PWMPER);
        }
        else 
        {
            MOTEUR_GAUCHE_ENH = 0; //Pilotage de la pin en mode IO
            MOTEUR_GAUCHE_INH = 1; //Mise à 1 de la pin
            MOTEUR_GAUCHE_ENL = 1; //Pilotage de la pin en mode PWM
            MOTEUR_GAUCHE_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante * PWMPER);
        } 
       }
    else {
        
       if (vitesseEnPourcents>0)
        {
            MOTEUR_DROIT_ENL = 0; //Pilotage de la pin en mode IO
            MOTEUR_DROIT_INL = 1; //Mise à 1 de la pin
            MOTEUR_DROIT_ENH = 1; //Pilotage de la pin en mode PWM
            MOTEUR_DROIT_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante * PWMPER);
        }
        else 
        {
            MOTEUR_DROIT_ENH = 0; //Pilotage de la pin en mode IO
            MOTEUR_DROIT_INH = 1; //Mise à 1 de la pin
            MOTEUR_DROIT_ENL = 1; //Pilotage de la pin en mode PWM
            MOTEUR_DROIT_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante * PWMPER);
        }
    }
}*/