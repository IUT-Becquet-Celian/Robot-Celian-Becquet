#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <libpic30.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "robot.h"
#include "ADC.h"
#include "main.h"
#include "time.h"
#include "UART.h"
#include "CB_TX1.h"
#include "CB_RX1.h"
#include "UART_Protocol.h"


int main(void) {
    /***************************************************************************************************/
    //Initialisation de l'oscillateur
    /****************************************************************************************************/
    InitOscillator();
    robotState.vitesseGaucheCommandeCourante=0;
    robotState.vitesseDroiteCommandeCourante=0;
    /****************************************************************************************************/
    // Configuration des entrées sorties
    /****************************************************************************************************/
    InitIO();
    InitPWM();
    /*LED_BLANCHE = 1;
    LED_BLEUE = 1;
    LED_ORANGE = 1;*/
    InitTimer23();
    InitTimer1();
    InitTimer4();
    InitADC1();
    InitUART();
   // int ADCValue0, ADCValue1, ADCValue2;
   
   
    
    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/
    while (1) {
//        PWMSetSpeedConsigne(-20,MOTEUR_GAUCHE);
 //       PWMSetSpeedConsigne(20,MOTEUR_DROIT);
        if (ADCIsConversionFinished()==1)
        {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult ( ) ;
            float volts = ((float) result[0]) * 3.3 / 4096 * 3.2 ;
            robotState.distanceTelemetreExterieurDroit = 34/volts-5;
            volts = ((float) result[1]) * 3.3 / 4096 * 3.2 ;
            robotState.distanceTelemetreInterieurDroit = 34/volts-5;
            volts = ((float) result[2]) * 3.3 / 4096 * 3.2 ;
            robotState.distanceTelemetreCentre = 34/volts-5;
            volts = ((float) result[4]) * 3.3 / 4096 * 3.2 ;
            robotState.distanceTelemetreInterieurGauche = 34/volts-5;
            volts = ((float) result[3]) * 3.3 / 4096 * 3.2 ;
            robotState.distanceTelemetreExterieurGauche = 34/volts-5;
            int i;
        unsigned char payload[] = {robotState.distanceTelemetreInterieurGauche, robotState.distanceTelemetreCentre, robotState.distanceTelemetreInterieurDroit } ;
        UartEncodeAndSendMessage(0x0030, 3, payload);
        
        for(i=0; i<CB_RX1_GetDataSize(); i++)
        {
            unsigned char c = CB_RX1_Get();
            SendMessage(&c,1);
        }
            
        }     
        
//        int i;
//        unsigned char payload[] = {3,0x00,0x00,0x00,0x01} ;
//        UartEncodeAndSendMessage(0x0050, 5, payload);
//        
//        for(i=0; i<CB_RX1_GetDataSize(); i++)
//        {
//            unsigned char c = CB_RX1_Get();
//            SendMessage(&c,1);
//        }
//        __delay32(40000000);
//        SendMessage((unsigned char *)"Bonjour", 7);     
////        SendMessageDirect((unsigned char *)"Bonjour", 7);     
//        __delay32(4000000); 
    } 
}

unsigned char stateRobot;
 
void OperatingSystemLoop(void) {
    if (JACK)
    {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            LED_BLANCHE = 0;
            LED_BLEUE = 0;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
        case STATE_AVANCE_FOND:
            PWMSetSpeedConsigne(40, MOTEUR_DROIT);
            PWMSetSpeedConsigne(40, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_FOND_EN_COURS;
            LED_BLANCHE = 0;
            LED_BLEUE = 1;
            break;
        case STATE_AVANCE_FOND_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            LED_BLANCHE = 0;
            LED_BLEUE = 0;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            LED_BLANCHE = 0;
            LED_BLEUE = 0;
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
     
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
        case STATE_TOURNE_DROITE_FAIBLE:
            PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_FAIBLE_EN_COURS;
            LED_BLANCHE = 0;
            LED_BLEUE = 0;
            break;
        
        case STATE_TOURNE_DROITE_FAIBLE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
         case STATE_TOURNE_GAUCHE_FAIBLE:
            PWMSetSpeedConsigne(20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-10, MOTEUR_GAUCHE);
            LED_BLANCHE = 0;
            LED_BLEUE = 0;
            stateRobot = STATE_TOURNE_DROITE_FAIBLE_EN_COURS;
            break;
        
        case STATE_TOURNE_GAUCHE_FAIBLE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-20, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            LED_BLANCHE = 0;
            LED_BLEUE = 0;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            LED_BLANCHE = 0;
            LED_BLEUE = 0;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
    }
        else
        {
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE;
        }
    
    }

unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    unsigned char positionObstacle = PAS_D_OBSTACLE;
    int distExtGauche = 22;
    int distIntGauche = 32;
    int distCentre = 37;
    int distIntDroit = 32;
    int distExtDroit = 22;
    //Détermination de la position des obstacles en fonction des télémètres
    if (robotState.distanceTelemetreExterieurGauche > distExtGauche &&
        robotState.distanceTelemetreInterieurGauche > distIntGauche &&
        robotState.distanceTelemetreCentre < distCentre &&
        robotState.distanceTelemetreInterieurDroit > distIntDroit &&
        robotState.distanceTelemetreExterieurDroit > distExtDroit) 
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreInterieurGauche < 15 &&
        robotState.distanceTelemetreInterieurDroit < 15) 
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreExterieurGauche > distExtGauche &&
        robotState.distanceTelemetreInterieurGauche > distIntGauche &&
        robotState.distanceTelemetreCentre > distCentre &&
        robotState.distanceTelemetreInterieurDroit > distIntDroit &&
        robotState.distanceTelemetreExterieurDroit > distExtDroit) 
        positionObstacle = PAS_D_OBSTACLE_FOND;
    else if (robotState.distanceTelemetreExterieurGauche > distExtGauche &&
        robotState.distanceTelemetreInterieurGauche > distIntGauche &&
        robotState.distanceTelemetreCentre > distCentre &&
        robotState.distanceTelemetreInterieurDroit > distIntDroit &&
        robotState.distanceTelemetreExterieurDroit < distExtDroit) 
        positionObstacle = OBSTACLE_EXT_DROIT;
    else if (robotState.distanceTelemetreExterieurGauche > distExtGauche &&
        robotState.distanceTelemetreInterieurGauche > distIntGauche &&
        robotState.distanceTelemetreCentre > distCentre &&
        robotState.distanceTelemetreInterieurDroit < distIntDroit &&
        robotState.distanceTelemetreExterieurDroit < distExtDroit) 
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreExterieurGauche > distExtGauche &&
        robotState.distanceTelemetreInterieurGauche > distIntGauche &&
        robotState.distanceTelemetreCentre > distCentre &&
        robotState.distanceTelemetreInterieurDroit < distIntDroit &&
        robotState.distanceTelemetreExterieurDroit > distExtDroit) 
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreExterieurGauche < distExtGauche &&
        robotState.distanceTelemetreInterieurGauche > distIntGauche &&
        robotState.distanceTelemetreCentre > distCentre &&
        robotState.distanceTelemetreInterieurDroit > distIntDroit &&
        robotState.distanceTelemetreExterieurDroit > distExtDroit) 
        positionObstacle = OBSTACLE_EXT_GAUCHE;
    else if (robotState.distanceTelemetreExterieurGauche < distExtGauche &&
        robotState.distanceTelemetreInterieurGauche < distIntGauche &&
        robotState.distanceTelemetreCentre > distCentre &&
        robotState.distanceTelemetreInterieurDroit > distIntDroit &&
        robotState.distanceTelemetreExterieurDroit > distExtDroit) 
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreExterieurGauche > distExtGauche &&
        robotState.distanceTelemetreInterieurGauche < distIntGauche &&
        robotState.distanceTelemetreCentre > distCentre &&
        robotState.distanceTelemetreInterieurDroit > distIntDroit &&
        robotState.distanceTelemetreExterieurDroit > distExtDroit) 
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreExterieurGauche > distExtGauche &&
        robotState.distanceTelemetreInterieurGauche > distIntGauche &&
        robotState.distanceTelemetreCentre < distCentre &&
        robotState.distanceTelemetreInterieurDroit < distIntDroit &&
        robotState.distanceTelemetreExterieurDroit > distExtDroit) 
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreExterieurGauche > distExtGauche &&
        robotState.distanceTelemetreInterieurGauche < distIntGauche &&
        robotState.distanceTelemetreCentre < distCentre &&
        robotState.distanceTelemetreInterieurDroit > distIntDroit &&
        robotState.distanceTelemetreExterieurDroit > distExtDroit) 
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreExterieurGauche > distExtGauche &&
        robotState.distanceTelemetreInterieurGauche < distIntGauche &&
        robotState.distanceTelemetreCentre < distCentre &&
        robotState.distanceTelemetreInterieurDroit < distIntDroit &&
        robotState.distanceTelemetreExterieurDroit > distExtDroit) 
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreExterieurGauche > distExtGauche &&
        robotState.distanceTelemetreInterieurGauche > distIntGauche &&
        robotState.distanceTelemetreCentre < distCentre &&
        robotState.distanceTelemetreInterieurDroit < distIntDroit &&
        robotState.distanceTelemetreExterieurDroit < distExtDroit) 
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreExterieurGauche < distExtGauche &&
        robotState.distanceTelemetreInterieurGauche < distIntGauche &&
        robotState.distanceTelemetreCentre < distCentre &&
        robotState.distanceTelemetreInterieurDroit < distIntDroit &&
        robotState.distanceTelemetreExterieurDroit > distExtDroit) 
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreExterieurGauche < distExtGauche &&
        robotState.distanceTelemetreInterieurGauche > distIntGauche &&
        robotState.distanceTelemetreCentre > distCentre &&
        robotState.distanceTelemetreInterieurDroit > distIntDroit &&
        robotState.distanceTelemetreExterieurDroit < distExtDroit) 
        positionObstacle = PAS_D_OBSTACLE;
    else if (robotState.distanceTelemetreCentre < distCentre) 
        positionObstacle = OBSTACLE_EN_FACE;
   
    else
        positionObstacle = PAS_D_OBSTACLE;

    //Détermination de l?état à venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == PAS_D_OBSTACLE_FOND)
        nextStateRobot = STATE_AVANCE_FOND;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_EXT_DROIT)
        nextStateRobot = STATE_TOURNE_GAUCHE_FAIBLE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
     else if (positionObstacle == OBSTACLE_EXT_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE_FAIBLE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
   
    
    //Si l?on n?est pas dans la transition de l?étape en cours
    if (nextStateRobot != stateRobot - 1)
    {
        stateRobot = nextStateRobot;
        
        unsigned char payload[] = {stateRobot, (unsigned char)(timestamp>>24),(unsigned char)(timestamp>>16),(unsigned char)(timestamp>>8),(unsigned char)(timestamp>>0)} ;
        UartEncodeAndSendMessage(0x0050, 5, payload);
    }
}

