 //------------------------------------------------------------------------------------
// FTC_08B.c
//------------------------------------------------------------------------------------
// DATE: 28/02/2019 -
//
// Target: C8051F02x
// Tool chain: KEIL Microvision 4
//
//  NOMS:APPRIOU Dimitri - BARD Christophe - BURGEVIN Valentin - GRELLIER Hugo
//	HONEGGER Joachim - LEMOINE Thomas - MONNERET Mathieu - MOUSSA Pierre
//
//------------------------------------------------------------------------------------
#include "c8051F020.h"
//------------------------------------------------------------------------------------
// Attribution des variables de sortie et initialisation de resigstres
//------------------------------------------------------------------------------------
sfr Reg    =	 0xFF; 
sbit CDE = P1^6; // LED de test, affiche le signal de sortie CDE
sbit BP = P3^7; // Bouton 3.7 du 8051
//sbit CDE = PX^X; // Pin de sortie, signal carre 0-3.3V rapport cyclique variable
 #define temps_clock 0x1000							// 27800 => 20ms //Definition de la variable 20ms

// Definition variables
unsigned int compteur_cycle=0;
unsigned int led_on=0;
unsigned int led_off=0;
unsigned int cycle_haut=0;
unsigned int cycle_bas=0;
//unsigned int duree_allum;
//unsigned int duree_ext;
//unsigned int nb_allumage;
//unsigned int intensite;
unsigned int GO=0; // variable de déclenchement de la séquence

//variables de commandes
unsigned int I = 100; //intensite
unsigned int D = 100;	//duree
unsigned int E = 90;	//extinction
unsigned int N = 4; //nombre de repet

// Prototypes des fonctions
void main(void);
void initialisation(void);
void config_clock(void);
void config_timer4(void);
void config_timer1(void);
void config_UART(void);
//void definition_param_test(void);
void delay(void);
void cycle(unsigned int,unsigned int,unsigned int,unsigned int);
void allumage(unsigned int);
int conversion_ASCII_UINT(char);

void super_extinction(void); //interrupt 19
void timer_interrupt(void); //interrupt 16
void interrupt_reception(void); //interrupt 4

//------------------------------------------------------------------------------------
// MAIN Routine
//------------------------------------------------------------------------------------

void main(){	
	initialisation();
	while(1){
		if (GO == 1){
			//T4CON |= 0x04; //Lancement du timer
			cycle(I,D,E,N);
			GO = 0;// RAZ
		}
	}
}

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
	EIE2 |= 0x04; // Enable interrupt 16 (Timer 4)
	EIE2 |= 0x20; // Enable interrupt 19 (Bouton 3.7)
	ES0 = 1; // Enable interrupt 4 (UART0)

	//Appel fonctions de configuration
	config_clock();
	config_timer4();
	config_timer1();
	config_UART();
	//definition_param_test();
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

void config_timer4(void){
	T4CON |= 0x02; //Configuration Timer 4
	T4CON &= 0xFD; //Incremente par la clock
  RCAP4L = 0xFF; //LB rechargement
	RCAP4H = 0xEF; //HB rechargement
	CKCON |= 0x40; //Timer 4 uses system clock
	T4CON &=0x7F; //Flag = 0
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


//void definition_param_test(void){
//	intensite = 90;
//	duree_allum = 20;
//	duree_ext = 10;
//	nb_allumage = 4;
//	return;
//}

void delay (void){
	unsigned int k;
	for(k=0;k<3000;k++){
	}
	return;
}

void cycle(unsigned int intensite_cycle , unsigned int t_duree_allum , unsigned int t_duree_ext , unsigned int nb_allumage_cycle){
	if(compteur_cycle<=nb_allumage_cycle-1){
		if(led_on<=t_duree_allum*500){
			led_on++;
			allumage(intensite_cycle);
		}
		else{
			if(led_off<=t_duree_ext*500){
				led_off++;
				CDE=0;
			}
			else{
				led_on=0;
				led_off=0;
				compteur_cycle++;
			}	
		}
	}
	else {
		T4CON &= 0xFB;
		}
}

void allumage(unsigned int rapport_cyclique){
	if(cycle_haut*20<rapport_cyclique){
		cycle_haut++;
		CDE=1;
		cycle_bas=cycle_haut;
	}
	else{
		if(cycle_bas*20<100){
			CDE=0;
			cycle_bas++;
		}
		else{
			cycle_haut=0;
			cycle_bas=0;
		}
	}
}

//Fonctions d'interruption

// interruption timer 4 overflow
//void timer_interrupt() interrupt 16{
//		//cycle(intensite,duree_allum , duree_ext , nb_allumage);
//		cycle(I,D,E,N);
//		T4CON &=0x7F; // On baisse la flag a la fin de l'allumage
//}

//interruption manuelle on force la LED a 0
// interrupt 19 pour le test, si falling edge sur le bouton P3.7
void super_extinction(void) interrupt 19 {
	CDE=0;
	T4CON &= 0xFB; //On coupe le timer
}


//interruption 4 (reception caractere)
void interrupt_reception(void) interrupt 4 {
    int value = conversion_ASCII_UINT(SBUF0);
    if (RI0 == 1){// caractere recu
        RI0=0;// RECEIVE INTERRUPT FLAG CLEARED    
			
// premier carctere                    
				if (SBUF0 == 0x49){ // si je reçois "I" de PUTTY (0x49) ;  
					I = -1; // permet d'avoir une condition pour rentrer dans la boucle pour les valeurs de I saisies par la suite
				}
				else if (SBUF0 == 0x44){ // si je reçois "D" de PUTTY
					D = -1;
				}
				else if (SBUF0 == 0x45){ // si je reçois "E" de PUTTY{  
					E = -1;
				}
				else if (SBUF0 == 0x4E){ // si je reçois "N" de PUTTY 
					N = -1;
				}							 
//      ========================DEUXIEME CARACTERE DU PARAMETRE======================================                  
				
				else if ((value != -1) && (I == -1)){ // quand je reçois la valeur de "I" de PUTTY
					I = value;
				} 
				else if ((value != -1) && (D == -1)){ // quand je reçois la valeur de "D" de PUTTY
					D = value;
				}  
				else if ((value != -1) && (E == -1)){ // quand je reçois la valeur de "E" de PUTTY
					E = value; 
				}
				else if ((value != -1) && (N == -1)){ // quand je reçois la valeur de "N" de PUTTY  
					N = value;
				}
//========================DERNIER CARACTERE======================================           
				else if (SBUF0 == 0x21){ // quand je reçois le "!" pour lancer la séquence
					GO = 1;
				}																													
//========================CARACTERE D'ERREUR======================================        
				else{
					SBUF0 = 0x23; // Envoi caractère # pour signifier que le caractère n'est pas recevable.
				}
		}
		else{ // Trans
					TI0 = 0;
		}
		value = -1;
}

int conversion_ASCII_UINT(char SBUF0){
	switch(SBUF0){
		 case '0':
				 return 0;
				 break;
		 case '1':
				 return 1;
				 break;         

		 case '2':
				 return 2;
				 break;

		 case '3':
				 return 3;
				 break;
		 case '4':
				 return 4;
				 break;
		 case '5':
				 return 5;
			   break;
		 case '6':
			   return 6;
				 break;
		 case '7':
				 return 7;
				 break;
		 case '8':
				 return 8;
				 break;
		 case '9':
				 return 9;
				 break;
		 default:
				 return -1;
	}
}