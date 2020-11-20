#ifndef UTILITIES_H
#define	UTILITIES_H

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679

double Abs(double value);
double Max(double value, double value2);
double Min(double value,double value2);
double LimitToInterval(double value, double min, double max);
double Modulo2PIAngleRadian(double angleRadian);

float getFloat(unsigned char *p, int index);
double getDouble(unsigned char *p, int index);

void getBytesFromFloat(unsigned char *p, int index, float f);
void getBytesFromDouble(unsigned char *p, int index, double d);
void getBytesFromInt32(unsigned char *p, int index, long in);

void SendPositionData();

#endif	//UTILITIES_H