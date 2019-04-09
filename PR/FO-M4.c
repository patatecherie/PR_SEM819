#include "FO-M4.h"
int distance_ultra=0;

//WARNING change this!!
sbit Signal_Trig=P3^7;
sbit Echo =P0^1;



	void init_FO_M4(){
		WDTCN = 0xDE;   							// Disable watchdog 
		WDTCN = 0xAD;
		OSCXCN = 0x67;  								// use external clock
		OSCICN |= 0x08;  								//System clock = extern clock
		XBR0 |= 0x08;
		XBR1 |= 0x08;
		XBR2 |= 0x40;
		P0MDOUT |= 0x01; 							// WARNING push-pull P0.0
		P3MDOUT |=  0x80; 						// WARNING push-pull P3.7
		IE |= 0x02;
		EA=1;
		
	}

void Timer2_pulse(void)
{
    ET2=1;
    TL2=Pulse_10us;         //initial value
    TH2=(Pulse_10us>>8);
    RCAP2L=TL2;                    //reload value
    RCAP2H=TH2;
    EXEN2=0;                     //disable External trigger
    CT2=0;                       //Mode Timer
    CPRL2=0;                     //mode Reload
    Signal_Trig=1;              //pulse on
    TR2=1;                       //enable timer2
}
void Count_or_Capt(void) interrupt 5{
			
    int time=0;
    if(TF2){
        TF2=0;
        TR2=0;                       //disable timer
        Signal_Trig=0;              //pulse off
        EXEN2=1;                     //enable External trigger
        CPRL2=1;                     //mode capture
        
    }
    else{
        EXF2=0;
        time =(10*RCAP2H + RCAP2L)/CLK;
        distance_ultra=time/58;
        TR2=0;
    }
}
void Launch_Timer2(void) interrupt 2{
	TR2=1;
}
unsigned int Ultrason_Dist(void){
	Timer2_pulse();
	return distance_ultra;
}
