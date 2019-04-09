#include "FO-M3.h"
float Old_Angle = 0;
int PWM = PWM_START;
//WARNING change this!!
sbit PWM_OUT = P0^2;


//needed to be launch at the start of the robot 
	void init_FO_M3(){
		WDTCN = 0xDE;   							// Disable watchdog 
		WDTCN = 0xAD;
		OSCXCN = 0x67;  								// use external clock
		OSCICN |= 0x08;  								//System clock = extern clock
		XBR0 |= 0x08;
		XBR2 |= 0x40;
		P3MDOUT |= 0x20; 							// WARNING push-pull P0.0
		P1MDOUT |=  0x20; 						// WARNING push-pull P1.6			
		PWM_OUT=!PWM_OUT;
		init_PCA();		
	}
	
	//needed to be launch at the start of the robot 
void init_PCA()
{
    PCA0MD = 0x00;                //disable CF interrupt & SysClk/12
    PCA0CPL0 = (0xFF & PWM); //initialize PCA compare value
    PCA0CPH0 = (0xFF & (PWM>>8));
    PCA0CPM0 = 0x4D;              //CCM0 in High Speed outpout mode
    EIE1 |= 0x08;                 //enable PCA interrupt
    EA = 1;
    PCA0CN = 0x40;
}


void PCA_ISR(void)interrupt 9 {
    
    if (CCF0){
        CCF0=0;                       //clear compare indicator
        if (PWM_OUT){                 //process rising edge
            PCA0CPL0 = (0xff & PWM); //set next match to PWM
            PCA0CPH0=(0xff & (PWM >> 8));
        }
        else{                         //process falling edge
            PCA0CPL0 = 0;             //set next match to 0
            PCA0CPH0 = 0;
        }
                                      //handle other PCA interrupt
    }
    else if (CCF1){
        CCF1 = 0;
    }
    else if (CCF2){
        CCF2 = 0;
    }
    else if (CCF3){
        CCF3 = 0;
    }
    else if (CCF4){
        CCF4 = 0;
    }
    else if (CF){
        CF = 0;
    }
}
	//convert char Angle to int between 0 and 180 deg
int AngleConvert(char* Angle){
    float AngleInt;
   if ( Angle[0] == '-' ){             // negative angle
			if (Angle[2]=='\0'){
				AngleInt=90-( Angle[1] - '0' );
			}
			else{
        AngleInt = 90 -(( Angle[1] - '0' ) * 10 + ( Angle[2] - '0' )) ;
			}
    }
    
    else{                              // positive angle
			if (Angle[1]=='\0'){
				AngleInt=90+( Angle[0] - '0' );
			}
			else{
        AngleInt = 90 + ( Angle[0] - '0' ) * 10 + ( Angle[1] - '0' );
			}
    }
    return AngleInt;
}

//Change servo angle and return an estimated time to do it, (will be call in FO-M1)
float CDE_Servo_H(char* Angle){
    float Time_To_Move;
    float AngleFloat = AngleConvert(Angle);        // [0;180]
    float dutyCycle = AngleFloat / 35.6 + 1.69;    //convert angle to a pourcentage of the period [1.69%;6.74%]
    float test;
    PWM = (dutyCycle/100)*65536;                   //change PWM value
    test=( AngleFloat - Old_Angle );
    Time_To_Move = Speed * abs( AngleFloat - Old_Angle ) / 60;//time to move calcul
    Old_Angle = AngleFloat;                 //keep the actual angle
    return Time_To_Move;
}

