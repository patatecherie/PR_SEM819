#include "FCT_05A.h"





void compare_detect_dist(void){
	unsigned char xdata c1[15] = "D 1";
	unsigned char xdata c2[4] = "MOU";
	unsigned char xdata c3[6] = "MOU D";
	unsigned char xdata c4[7] = "MOB A:";
	unsigned char xdata c5[9] = "MOB D A:";
	unsigned char xdata c6[7] = "MOS A:";
	unsigned char xdata c7[9] = "MOS D A:";
	unsigned char xdata subBuffer4[7];
	unsigned char xdata subBuffer5[9];
	unsigned char xdata subBuffer6[7];
	unsigned char xdata subBuffer7[9];
	
	char xdata subBufferNum[3];
	char distance[10];
	memcpy(subBuffer4,&Buffer,5);
	subBuffer4[5]='\0';
	memcpy(subBuffer5,&Buffer,8);
	subBuffer5[8]='\0';
	memcpy(subBuffer6,&Buffer,5);
	subBuffer6[5]='\0';
	memcpy(subBuffer7,&Buffer,8);
	subBuffer7[8]='\0';
	
	if ((strcmp(Buffer,c1))==0){
		 
		 sprintf(distance,"%d",Ultrason_Dist());
	}
	else if(strcmp(Buffer,c2)==0){
		
		sprintf(distance,"%d",Ultrason_Dist());
	}
	else if(strcmp(Buffer,c3)==0){
		sprintf(distance,"%d",Ultrason_Dist());
	}
	else if(strcmp(subBuffer4,c4)==0){
		sprintf(distance,"%d",Ultrason_Dist());
	}
	else if(strcmp(subBuffer5,c5)==0){
		sprintf(distance,"%d",Ultrason_Dist());
	}
	else if(strcmp(subBuffer6,c6)==0){
		sprintf(distance,"%d",Ultrason_Dist());
	}
	else if(strcmp(subBuffer7,c7)==0){
		sprintf(distance,"%d",Ultrason_Dist());
	}
	else{ SBUF0='#';}
	memset(Buffer, 0, sizeof(Buffer));
}