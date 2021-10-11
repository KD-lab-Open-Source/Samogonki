#include <math.h>
#include "requant.h"

void initialisiere_Quantisierungstabellen(int maximum_Band, int Blockgroesse)
{
	Band_Limits(maximum_Band, Blockgroesse);
	Requantisierungstabelle();
	Quantisierungsmodes();
	Skalenfaktoren();
}

void Skalenfaktoren()
{
	int n;
	//Abdeckung von +1.58...-98.41 dB, wobei scf[n]/scf[n-1] = 1.200508058
	for (n=0; n<64; ++n) SCF[n] = (float)( pow(10,-0.1*(n-1)/1.26) );
}


void Band_Limits(int Max_Band_desired, int Blockgroesse)
{
	// default
	if (Max_Band_desired==0)
	{
		if (!IS_Flag)
		{
			if (Bitrate> 384) {Min_Band=32; Max_Band=31;}
			if (Bitrate<=384) {Min_Band=30; Max_Band=29;}
			if (Bitrate<=160) {Min_Band=27; Max_Band=26;}
			if (Bitrate<= 64) {Min_Band=21; Max_Band=20;}
			if (Bitrate==  0) {Min_Band=32; Max_Band=31;}
		}
		else if (IS_Flag)
		{
			if (Bitrate<=384) {Min_Band=16; Max_Band=29;}
			if (Bitrate<=160) {Min_Band=12; Max_Band=26;}
			if (Bitrate<=112) {Min_Band= 8; Max_Band=26;}
			if (Bitrate<= 64) {Min_Band= 4; Max_Band=20;}
		}
	}

	// Bandbreite vom user ausgewaehlt
	else
	{
		if (!IS_Flag)
		{
			Max_Band = Max_Band_desired;
			Min_Band = Max_Band +1;
		}
		else if (IS_Flag)
		{
			if (Bitrate<=384) {Min_Band=16; Max_Band=Max_Band_desired;}
			if (Bitrate<=160) {Min_Band=12; Max_Band=Max_Band_desired;}
			if (Bitrate<=112) {Min_Band= 8; Max_Band=Max_Band_desired;}
			if (Bitrate<= 64) {Min_Band= 4; Max_Band=Max_Band_desired;}

			if (Min_Band>=Max_Band) Min_Band = Max_Band;
		}
	}
}

void Quantisierungsmodes()
{
	int Band;
	//Zuordnung Index->Quantisierungsstufe (Bitstrom lesen)
	//Zuordnung Quantisierungsstufe->Index (Bitstrom schreiben)
	for (Band=0; Band<=10; ++Band)
	{
		Q_bit[Band]=4;
		Q_res[Band][0]=0;
		Q_res[Band][1]=1;
		Q_res[Band][2]=2;
		Q_res[Band][3]=3;
		Q_res[Band][4]=4;
		Q_res[Band][5]=5;
		Q_res[Band][6]=6;
		Q_res[Band][7]=7;
		Q_res[Band][8]=8;
		Q_res[Band][9]=9;
		Q_res[Band][10]=10;
		Q_res[Band][11]=11;
		Q_res[Band][12]=12;
		Q_res[Band][13]=13;
		Q_res[Band][14]=14;
		Q_res[Band][15]=17;
	}
	for (Band=11; Band<=22; ++Band)
	{
		Q_bit[Band]=3;
		Q_res[Band][0]=0;
		Q_res[Band][1]=1;
		Q_res[Band][2]=2;
		Q_res[Band][3]=3;
		Q_res[Band][4]=4;
		Q_res[Band][5]=5;
		Q_res[Band][6]=6;
		Q_res[Band][7]=17;
	}
	for (Band=23; Band<=31; ++Band)
	{
		Q_bit[Band]=2;
		Q_res[Band][0]=0;
		Q_res[Band][1]=1;
		Q_res[Band][2]=2;
		Q_res[Band][3]=17;
	}
}

float Requantisieren(int In, int Stufe)
{
	if      (Stufe==1) return Q_1[In];
	else if (Stufe==2) return Q_2[In];
	else if (Stufe==3) return Q_3[In];
	else if (Stufe==4) return Q_4[In];
	else if (Stufe==5) return Q_5[In];
	else if (Stufe==6) return Q_6[In];
	else if (Stufe==7) return Q_7[In];
	else if (Stufe==8) return Q_8[In];
	else               return C[Stufe]*(In-D[Stufe]);
}

void Requantisierungstabelle()
{
	int n;
	//erster Quantisierer
	for (n=0; n<3  ; ++n) {Q_1[n] = C[1]*(n-D[1]);}
	//zweiter Quantisierer
	for (n=0; n<5  ; ++n) {Q_2[n] = C[2]*(n-D[2]);}
	//dritter Quantisierer
	for (n=0; n<7  ; ++n) {Q_3[n] = C[3]*(n-D[3]);}
	//vierter Quantisierer
	for (n=0; n<9  ; ++n) {Q_4[n] = C[4]*(n-D[4]);}
	//fuenfter Quantisierer
	for (n=0; n<15 ; ++n) {Q_5[n] = C[5]*(n-D[5]);}
	//sechster Quantisierer
	for (n=0; n<31 ; ++n) {Q_6[n] = C[6]*(n-D[6]);}
	//siebter Quantisierer
	for (n=0; n<63 ; ++n) {Q_7[n] = C[7]*(n-D[7]);}
	//achter Quantisierer
	for (n=0; n<127; ++n) {Q_8[n] = C[8]*(n-D[8]);}
}


