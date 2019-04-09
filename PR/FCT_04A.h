//#include "c8051F020.h"
#include "FO-M3.h"
//#include <stdlib.h>
//#include <string.h>

int index;
unsigned char xdata Buffer[15];
unsigned char xdata c1[15] = "D 1";

int temps;
float temps2;
int running;
void initialisation(void);
void config_clock(void);
void config_timer1(void);
void config_UART(void);
void serial0();
void delay (void);
