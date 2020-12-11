/* 
 * File:   PWM.h
 * Author: TABLE 6
 *
 * Created on 31 janvier 2020, 15:28
 */

#ifndef PWM_H
#define	PWM_H

#define MOTEUR_GAUCHE 0
#define MOTEUR_DROIT 1



#define P 0

void InitPWM();
void PWMUpdateSpeed();
void PWMSetSpeedConsigne (float vitesseEnPourcents, char moteur);
void PWMSetSpeed(float vitesseEnPourcents, int moteur);
void PWMSetSpeedConsignePolaire();

#endif	/* PWM_H */

