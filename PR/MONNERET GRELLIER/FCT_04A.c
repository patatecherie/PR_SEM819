#include "FCT_04A.h"


bit TI0_flag,ok;
sfr Reg    =	 0xFF; 


void initialisation(void){
	//Devalidation du WatchDog
	Reg = 0xDE;
	Reg = 0xAD;
	
	//Crossbar!
  XBR2 |= 0x40;// Enable crossbar
	XBR1 |=0x80; // Enable SYSCLK
	XBR0 |= 0x04;// TX routed P0.0 , RX routed P0.1;

	//Gestion des sorties
	P0MDOUT |=0x01; //Port 0.0 Push pull
	P0MDOUT |=0x04; //Port 0.2 Push pull
	P1MDOUT |=0x40; //Port 1.6 Push Pull	
	
	//Interrupt
	EA=1; //Enable all interrupt
	EIE2 |= 0x20; // Enable interrupt 19 (Bouton 3.7)
	ES0 = 1; // Enable interrupt 4 (UART0)

	//Appel fonctions de configuration
	config_clock();
	config_timer1();
	config_UART();
	//definition_param_test();
	return;
}
//

void delay (void){
	unsigned int k;
	for(k=0;k<3000;k++){
	}
	return;
}
void wait(int time){
	unsigned int k,j;
	for(k=0;k<3000;k++){
		for(j=0;j<time;j++){
		}
	}
	return;
}

void config_clock(void){
	unsigned int n;
	OSCXCN = 0x67;// Config Extern Oscil
	for(n=0;n<2;n++){
		delay();
	}
	OSCICN |= 0x08; //Enable Extern Clock
	return;
}

void config_timer1(void){
   CKCON |= 0x10;  // On ne divise pas par 12
   TH1 = 0xB8; // 184 debut timer1 --> 19200 baudrate
   TMOD &= 0xaf; // T1M1
   TMOD |= 0x20; //T1M0
   TR1 = 1; // enable timer 1
}

void config_UART(void){      //19200
	SM00 = 0; // UART0 mode 1    
	SM10 = 1;    // UART0 mode 1    
	PCON |= 0x80; // baud rate divided by 2 disabled                    
	REN0 = 1; // receive enable
	RI0 = 0; // RECEIVE INTERRUPT FLAG
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
void compare(void){
	unsigned char xdata c2[8] = "CS H A:";
	unsigned char xdata subBuffer[8];
	char xdata subBufferNum[3];
	memcpy(subBuffer,&Buffer,7);
	subBuffer[7]='\0';
	memcpy(subBufferNum,&Buffer[7],3);
	if ((strcmp(Buffer,c1))==0){
		
	}//do something}
	else if(strcmp(subBuffer,c2)==0){
		float timetomove = CDE_Servo_H(subBufferNum);
		temps =150.0*timetomove;
		running=1;
	}
	else{ SBUF0='#';}
	memset(Buffer, 0, sizeof(Buffer));
}
		
		
void serial0() interrupt 4{
		unsigned char ch;
		if(RI0){
			ch=SBUF0;
			if(ch!=0x0D){
				Buffer[index++]=ch;
			}
			else{
				compare();
			ok=1;
			}
			RI0=0;
		}
		else{
			TI0=0;
			TI0_flag=1;
		}
}

void main(void){
	initialisation();
	init_FO_M3();
	index=0;
	TI0_flag=0;
	ok=0;
	temps = CDE_Servo_H("-00");
	while(1){
		if(ok==1){
			ok=0;
			index=0;
			if (running==1){
				wait(temps);
				arriver();
				running=0;
			}
		}
	}
}