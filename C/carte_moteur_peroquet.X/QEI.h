#ifndef QEI_H
#define	QEI_H

    void InitQEI1();
    void InitQEI2();
    void QEIUpdateDate();

    float QEIDroitPosition_T_1 = 0;
    float QEIGauchePosition_T_1 = 0; 
    
    float QEIGauchePostion = 0;
    float QEIDroitPostion = 0;
    
    float QEI1RawValue = 0;
    float QEI2RawValue = 0;
    
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
    float FREQ_ECH_QEI=250;
    
    
#endif	/* QEI_H */

