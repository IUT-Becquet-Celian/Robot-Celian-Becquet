#include "Utilities.h"
#include "math.h"
#include <xc.h>
#include "main.h"
#include "Robot.h"
#include "Timer.h"
#include "UART_Protocol.h"

#define POSITION_DATA 0x0061


double Abs(double value)
{
    if (value>=0)
        return value;
    else return -value;      
}

double Max(double value, double value2)
{
    if(value>value2)
        return value;
    else return value2;
}

double Min(double value,double value2)
{
    if (value < value2)
        return value;
    else
        return value2;    
}

double LimitToInterval(double value, double min, double max)
{
    if(value < min)
        return min;
    else if(value > max)
        return max;
    else
        return value;
}

double Modulo2PIAngleRadian(double angleRadian) {
    double angleTemp = fmod(angleRadian -PI, 2*PI) + PI;
    return fmod(angleTemp + PI, 2*PI) -PI;
}

float getFloat(unsigned char *p, int index)
{
    float *result_ptr = (float*)(p + index);
    float result = *result_ptr;
    return result;
}

double getDouble(unsigned char *p, int index)
{
    double *result_ptr = (double*)(p + index);
    return *result_ptr;
}

void getBytesFromFloat(unsigned char *p, int index, float f)
{
    int i;
    unsigned char *f_ptr = (unsigned char*)&f;
    for (i = 0; i < 4; i++)
        p[index + i] = f_ptr[i];
}

void getBytesFromInt32(unsigned char *p, int index, long in)
{
    int i;
    unsigned char *f_ptr = (unsigned char*)&in;
    for (i = 0; i < 4; i++)
        p[index + i] = f_ptr[3-i];
}

void getBytesFromDouble(unsigned char *p, int index, double d)
{
    int i;
    unsigned char *f_ptr = (unsigned char*)&d;
    for (i = 0; i < 8; i++)
        p[index + i] = f_ptr[i];
}

void SendPositionData()
{
    unsigned char positionPayload[24];
    getBytesFromInt32(positionPayload, 0, timestamp);
    getBytesFromFloat(positionPayload, 4, (float)(robotState.xPosFromOdometry));
    getBytesFromFloat(positionPayload, 8, (float)(robotState.yPosFromOdometry));
//    getBytesFromFloat(positionPayload, 12, (float)(robotState.angleRadianFromOdometry));
//    getBytesFromFloat(positionPayload, 16, (float)(robotState.vitesseLineaireFromOdometry));
//    getBytesFromFloat(positionPayload, 20, (float)(robotState.vitesseAngulaireFromOdometry));
    
//    getBytesFromFloat(positionPayload, 4, (float)(robotState.QEI1RawValue));
//    getBytesFromFloat(positionPayload, 8, (float)(robotState.QEI2RawValue));
    getBytesFromFloat(positionPayload, 12, (float)(robotState.angleRadianFromOdometry));
    getBytesFromFloat(positionPayload, 16, (float)(robotState.vitesseLineaireFromOdometry));
    getBytesFromFloat(positionPayload, 20, (float)(robotState.vitesseAngulaireFromOdometry));
    UartEncodeAndSendMessage(POSITION_DATA, 24, positionPayload);
}
