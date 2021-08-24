#include <stdio.h> 
#include <stdlib.h>
#include <math.h>

//#define MANUAL_ENTER uncomment this for manual enter instead of CLI arg parsing 

int main (int argc, char** argv) {

#if defined MANUAL_ENTER
printf("================================================\n");
printf("#  This tool allows you to calculate\n"); 
printf("#  drag function for various types of\n"); 
printf("#  projectiles\n");
printf("================================================\n");
printf("#  It's a C-version of MCDRAG.BAS of R.L.MACCOY\n");
printf("================================================\n\n");
#endif

#ifndef MANUAL_ENTER
if(argc == 1 && argc < 11) {
	puts("Wrong quantity of args!!!");
	puts("Usage: ./app + following args:\n\tcaliber,\n\tlength total,"); 
	puts("\tRT/R,\n\tboat-tail lenght,\n\tbase diam.,\n\tmeplat diam.,");
	puts("\tband diam.,\n\tCOG distance from nose,\n\tK=3 where: 1 - LL, 2 - LT, 3 - TT");
	puts("Exiting to OS...");
	exit(1);
}
#endif

/* EXAMPLE
	D1=7.823, // diam
	L1=3.945, // length total
	L2=2.240, // nose length
	R1=0.90, // RT/R
	L3=0.503, // boat-tail lenght
	D2=0.786, // base diam.
	D3=0.211, // meplat diam.
	D4=1.0, // band diam.
	X1=0; // COG distance from nose
	K=3; // 1 - LL, 2 - LT, 3 - TT
*/

const double M[] =   {0.5, 0.6, 0.7, 0.8, 0.85, 0.9, 0.925, 0.95, 
                    0.975, 1.0, 1.1, 1.3, 1.4, 1.5, 1.6, 1.7,
                    1.8, 2.0, 2.2, 2.5, 3.0};

int ARR_SIZE = sizeof(M)/sizeof(double);

double C1[ARR_SIZE], C2[ARR_SIZE], C3[ARR_SIZE], 
C4[ARR_SIZE], C5[ARR_SIZE], C6[ARR_SIZE], P1[ARR_SIZE];

double D1;
double L1; 
double L2; 
double R1; 
double L3; 
double D2; 
double D3; 
double D4; 
double X1;
int K;

#ifndef MANUAL_ENTER
	D1 = atof(argv[1]);
	L1 = atof(argv[2]); 
	L2 = atof(argv[3]); 
	R1 = atof(argv[4]); 
	L3 = atof(argv[5]); 
	D2 = atof(argv[6]); 
	D3 = atof(argv[7]); 
	D4 = atof(argv[8]); 
	X1 = atof(argv[9]);
	K = atoi(argv[10]);
#endif

char K1[255] = {};

#if defined MANUAL_ENTER
printf("ENTER THE MCDRAG INPUTS, ONE QUANTITY AT A TIME.\n");
printf("================================================\n\n");

printf("ENTER PROJECTILE REFERENCE DIAMTER (MM):");
scanf("%f", &D1);

printf("ENTER TOTAL PROJECTILE LENGTH (CALIBERS):");
scanf("%f", &L1);

printf("ENTER NOSE LENGTH (CALIBERS):");
scanf("%f", &L2);

printf("ENTER RT/R (HEADSHAPE PARAMETER):");
scanf("%f", &R1);

printf("ENTER BOATTAIL LENGTH (CALIBERS):");
scanf("%f", &L3);

printf("ENTER BASE DIAMTER (CALIBERS):");
scanf("%f", &D2);

printf("ENTER MEPLAT DIAMETER (CALIBERS):");
scanf("%f", &D3);

printf("ENTER ROTATING BAND DIAMTER (CALIBERS):");
scanf("%f", &D4);

printf("ENTER CENTER OF GRAVITY LOCATION (CALIBERS FROM NOSE):\n");
printf("[NOTE: COG LOCATION IS OPTIONAL, IF UNKNOWN, ENTER 0]");
scanf("%f", &X1);

printf("ENTER THE BOUNDARY LAYER CODE [1 - LL, 2 - LT, 3 - TT]:");
scanf("%d", &K);

printf("ENTER PROJECTILE IDENTIFICATION:");
scanf("%s", K1);
#endif

/* Additional variables */
double T1, M2, R2, R3, C7, C8, D5, S1, S2, S3, C9, C10;   
double C15, P5, C16, C18, P2, P4, T2, T3, E1, B4;
double B2, B, Z, S4, C11, C12, C13, C14, R4, X2, C17;
double B3, __A12, __A11, __A1, E2, X3, R5, __A2, E3;

/* Main loop for CD vs Mach determination */

	for (int i = 0; i < ARR_SIZE; ++i) {
		T1=(1.0-D3)/L2;
		M2=pow(M[i],2);
		R2=23296.3*M[i]*L1*D1;
		R3=0.4343*(log(R2));
		C7=(1.328/(sqrt(R2)))*(pow(1.0+0.12*M2,-0.12));
		C8=(0.455/pow(R3,2.58))*(pow(1.0+0.21*M2,-0.32));
		D5=1.0+((0.333+(0.02/(L2*L2)))*R1);
		S1=1.5708*L2*D5*(1.0+(1.0/(8.0*L2*L2)));
		S2=3.1416*(L1-L2);
		S3=S1+S2;

			switch(K) {
				case 1: {C9=1.2732*S3*C7; C10=C9; break;}
				case 2: {C9=1.2732*S3*C7; C10=1.2732*S3*C8; break;}
				case 3: {C9=1.2732*S3*C8; C10=C9; break;}
			}

		C3[i]=(C9*S1+C10*S2)/S3;
		C15=(M2-1.0)/(2.4*M2);

			if(M[i]<=1.0) {
			P5=pow(1.0+0.2*M2,3.5);
			}
			else {
			P5=pow(1.2*M2,3.5)*pow((6.0/(7.0*M2- 1)),2.5);
			}

		C16=(1.122*(P5-1.0)*pow(D3,2))/M2;

			if(M[i]<=0.91) {C18=0;}
			else if(M[i]>=1.41) {C18=0.85*C16;}
			else {C18=(0.254+(2.88*C15))*C16;}
	
			if(M[i]<1.0) {
			P2=1.0/(1.0+.1875*M2+0.0531*M2*M2);
			}
			else {
			P2=1.0/(1.0+0.2477*M2+0.0345*M2*M2);
			}
		P4=(1.0+0.09000001*M2*(1.0-exp(L2-L1)))*(1.0+0.25*M2*(1.0-D2));
		P1[i]=P2*P4;

		if(P1[i]<0) {P1[i]=0;}
		C6[i]=(1.4286*(1.0-P1[i])*(D2*D2))/M2;

		if(M[i]<0.95) {C4[i]= pow(M[i],12.5)*(D4-1.0);}
		else if(M[i]>=0.95) {C4[i]=(0.21+0.28/M2)*(D4-1.0);}

		/* Subsonic and transsonic speeds */
		if(M[i]<=1) {
			if(L3<=0) {C5[i]=0;}
			else if(L3>0) {
				
				if(M[i]<=0.85) {C5[i]=0;}
				else {
					T2=(1.0-D2)/(2.0*L3);
					T3=(2.0*T2*T2)+pow(T2,3);
					E1=exp((-2)*L3);
					B4=1.0-E1+2.0*T2*((E1*(L3+0.5))-0.5);
					C5[i]=2.0*T3*B4*(1.0/(0.564+1250*C15*C15));	
				}
			}	
		X2=pow((1.0+0.552*pow(T1,0.8)),-0.5);
		
		if(M[i]<=X2) {C17=0;}
		else {C17=0.368*pow(T1,1.8)+(1.6*T1*C15);}

		C2[i]=C17+C18;
		}/* End of subsonic and transsonic speeds */

		/* Supersonic speeds */
		else if(M[i]>1) {
			B2=M2-1.0;
			B=sqrt(B2);
			S4=1.0+0.368*pow(T1,1.85);
			if(M[i]<S4) {Z=sqrt(pow(S4,2)-1);}
			else if (M[i]>=S4) {Z=B;}

 			C11=0.7156-(0.5313*R1)+(0.595*pow(R1,2));
			C12=0.0796+(0.0779*R1);
			C13=1.587+(0.049*R1);
			C14=0.1122+(0.1658*R1);
			R4=1.0/pow(Z,2);			

			C17=(C11-C12*pow(T1,2))*R4*pow((T1*Z),(C13+C14*T1));
			C2[i]=C17+C18;

			if(L3<=0) {C5[i]=0;}
			else if(L3>0) {
				T2=(1.0-D2)/(2.0*L3);
				if(M[i]<=1.2) {
					T3=2.0*pow(T2,2)+pow(T2,3);
					E1=exp((-2)*L3);
					B4=1.0-E1+2.0*T2*((E1*(L3+0.5))-0.5);
					C5[i]=2.0*T3*B4*(1.774-9.3*C15);
				}
				else if(M[i]>1.2) {
					B3=0.85/B;
					__A12=(5.0*T1)/(6.0*B)+pow(0.5*T1,2)-(0.7435/M2)*pow(T1*M[i],1.6);
					__A11=(1.0-((0.6*R1)/M[i]))*__A12;
					E2=exp((-1.1952/M[i])*(L1-L2-L3));
					X3=((2.4*pow(M2,2)-4*B2)*pow(T2,2))/(2.0*pow(B2,2));
					__A1=__A11*E2-X3+((2.0*T2)/B);
					R5=1.0/B3;
					E3=exp((-B3)*L3);
					__A2=1.0-E3+(2.0*T2*(E3*(L3+R5)-R5));
					C5[i]=4.0*__A1*T2*__A2*R5;	
				}		

			}			
		} /* End of supersonic speeds */

	C1[i]=C2[i]+C3[i]+C4[i]+C5[i]+C6[i];
	} /* End of main loop */

	/* Calculation output */
	//printf("Mach=\t CD0=\t CDH=\t CDSF=\t CDBND=\tCDBT=\t CDB=\t PB/PINF=\t\n");

	for(int i=0; i < ARR_SIZE ; ++i) {
		printf("%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t\n", 
		M[i], C1[i], C2[i], C3[i], C4[i], C5[i], C6[i], P1[i]);
	}

return 0;
} /* end of main(void) */