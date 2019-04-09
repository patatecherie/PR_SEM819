#include "FCT_03A.h"

//global variable


unsigned char xdata c1[15] = "D 1";





void delay (void){
	unsigned int k;
	for(k=0;k<3000;k++){
	}
	return;
} 
void arriver(void){
	SBUF0='A';
	delay();
	SBUF0='S';
	delay();
	SBUF0=' ';
	delay();
	SBUF0='H';
}
void compare_servo_H(void){
	unsigned char xdata c2[8] = "CS H A:";
	unsigned char xdata subBuffer[8];
	char xdata subBufferNum[3];
	memcpy(subBuffer,&Buffer,7);
	subBuffer[7]='\0';
	memcpy(subBufferNum,&Buffer[7],3);
	if ((strcmp(Buffer,c1))==0){
		
	}
	else if(strcmp(subBuffer,c2)==0){
		float timetomove = CDE_Servo_H(subBufferNum);
		temps =150.0*timetomove;
		running=1;
	}
	else{ SBUF0='#';}
	memset(Buffer, 0, sizeof(Buffer));
}
		
		

