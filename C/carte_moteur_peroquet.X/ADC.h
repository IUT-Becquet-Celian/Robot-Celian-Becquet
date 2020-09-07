/* 
 * File:   ADC.h
 * Author: Table2
 *
 * Created on 5 février 2020, 16:31
 */

#ifndef ADC_H
#define	ADC_H

void InitADC1(void);
void ADC1StartConversionSequence();
unsigned int * ADCGetResult(void);
unsigned char ADCIsConversionFinished(void);
void ADCClearConversionFinishedFlag(void);

#endif	/* ADC_H */

