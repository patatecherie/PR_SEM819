#ifndef FO-M4_H
#define FO-M4_H
#include "c8051F020.h"
#include "FO-M3.h"
#include <stdio.h>
#include <stdlib.h>

#define Pulse_10us 0xFFF0
#define CLK 22100000


//global variable
 extern int distance_ultra;

//define function
void Timer2_pulse(void);
void Count_or_Capt(void);
void Launch_Timer2(void);
unsigned int Ultrason_Dist(void);
void init_FO_M4();
#endif