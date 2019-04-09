#ifndef FO-M3_H
#define FO-M3_H
#include "c8051F020.h"
#include <stdio.h>
#include <stdlib.h>

#define Speed 0.5
#define PWM_START 0x0ACC


//global variable
extern int PWM;
extern float Old_Angle;

//define function
void init_FO_M3(void);
void init_PCA();
void PCA_ISR(void);
int AngleConvert(char* Angle);
extern float CDE_Servo_H(char* Angle);
#endif