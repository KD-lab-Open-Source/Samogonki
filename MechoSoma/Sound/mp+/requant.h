#ifndef _requant_h
#define _requant_h_

/* P R O Z E D U R E N */
void initialisiere_Quantisierungstabellen(int, int);
void Skalenfaktoren();
void Band_Limits(int, int);
void Quantisierungsmodes();
void Requantisierungstabelle();
float Requantisieren(int, int);

/* V A R I A B L E N */
float SCF[64];					//tabellierte Skalenfaktoren
unsigned int Q_bit[32];			//Anzahl bits fuer Speicherung der Aufloesung (SV6)
unsigned int Q_res[32][16];		//Index -> Aufloesung (SV6)
float Q_1[ 3],Q_2[  5],Q_3[ 7]; //tabellierte
float Q_4[ 9],Q_5[ 15],Q_6[31]; //Requantisierung bis
float Q_7[63],Q_8[127];         //zum 8ten Quantisierer
int IS_Flag,Max_Band,Min_Band,Bitrate;

/* K O N S T A N T E N */
//bits per sample fuer gewaehlte Aufloesungsstufe
const unsigned int Res_bit[18] = 
{ 0,  0,  0,  0,  0,  0,  0,  0,  7,
  8,  9, 10, 11, 12, 13, 14, 15, 16}; 

//Requantisierungs-Koeffizienten
const float C[18] = 
{    0.0000000000f, 21845.3339843750f, 13107.2001953125f, 9362.2861328125f,
  7281.7778320313f,  4369.0668945313f,  2114.0644531250f, 1040.2540283203f,
   516.0314941406f,   257.0039367676f,   128.2504882813f,   64.0625610352f,
    32.0156326294f,    16.0039081573f,     8.0009765625f,    4.0002441406f,
	 2.0000610352f,     1.0000152588f};

//Requantisierungs-Offset
const int D[18] =
{  0,     1,     2,     3,     4,     7,    15,    31,    63,
 127,   255,   511,  1023,  2047,  4095,  8191, 16383, 32767};
			
#endif