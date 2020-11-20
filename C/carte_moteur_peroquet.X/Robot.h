#ifndef ROBOT_H
#define ROBOT_H


typedef struct robotStateBITS {

    union {

        struct {
            unsigned char taskEnCours;
            unsigned char stateRobot;  
            unsigned char isInAutomaticMode;
            
            float distanceTelemetreExterieurDroit;
            float distanceTelemetreInterieurDroit;
            float distanceTelemetreCentre;
            float distanceTelemetreInterieurGauche;
            float distanceTelemetreExterieurGauche;
            float vitesseGaucheConsigne;
            float vitesseGaucheCommandeCourante;
            float vitesseDroiteConsigne;
            float vitesseDroiteCommandeCourante;
            
            float vitesseDroitFromOdometry;
            float vitesseGaucheFromOdometry; 
            float vitesseLineaireFromOdometry; 
            float vitesseAngulaireFromOdometry; 
            
            float xPosFromOdometry;
            float yPosFromOdometry;
            float xPosFromOdometry_1;
            float yPosFromOdometry_1;
            float angleRadianFromOdometry_1;
            float angleRadianFromOdometry;
            
        };
    };
} ROBOT_STATE_BITS;
extern volatile ROBOT_STATE_BITS robotState;
#endif /* ROBOT_H */
