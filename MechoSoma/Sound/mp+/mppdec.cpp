#include <windows.h>
#define DECODER
#define VERSION 1
#define MAX_SV  7
#define BUILD   "5b"

#include "requant.c"
#include "huff_old.c"
#include "huff_new.c"
#include "synth_filter.h"
#include "bitstream_decode.c"

#include "mppdec.h"

/*********************** Typen ***************************/
typedef struct quantisierte_Samples {unsigned int L[36]; unsigned int R[36];} QuantTyp;

/******************** Funktionen *************************/
void Requantisierung();
void Calculate_New_V_R(float*,float*);
void Synthese_Filter_opt();
void Intensity_Stereo_Decode();
void Lese_Bitstrom_SV6();
void Lese_Bitstrom_SV7();

/****************** Globale Variablen ********************/
static float V_L[1024];
static float V_R[1024];
static float Y_L[36][32];
static float Y_R[36][32];
static short Stream[2304];
 
static unsigned int SCF_Index_L[3][32],SCF_Index_R[3][32];	//Skalenfaktor-Indizes fuer Speicherung
static QuantTyp Q[32];					//quantisierte Samples
static unsigned int Res_L[32],Res_R[32];		//Aufloesungsstufen der Teilbaender
static int DSCF_Flag_L[32],DSCF_Flag_R[32];	//Flag bei Benutzung von differential-SCF
static unsigned int SCFI_L[32],SCFI_R[32];		//Uebertragungsreihenfolge der SCF

static int DSCF_Reference_L[32],DSCF_Reference_R[32];

static int MS_used = 0;	//globaler Flag fuer M/S-Signalfuehrung
static int MS_Flag[32];	//subbandweiser Flag fuer M/S-Signalfuehrung
static int Max_used_Band = 0;

static int StreamVersion;

static int Blockgroesse;

/***********************/

static HANDLE inputFile = INVALID_HANDLE_VALUE;
static unsigned int NumOfSample,NumOfSampleFrames;


bool MpegOpen(const char* file)
{
	int Coding_Mode = -1;
	int Max_Band_desired = 0;
	unsigned int OverallFrames = 0;
	unsigned int DecodedFrames = 0;

	Max_used_Band=0;
	MS_used=0;
	Zaehler=0;
	dword=0;
	pos=0;
	BITS=0;
	BitsRead=0;

	MpegClose();

	inputFile = CreateFile(file,GENERIC_READ,FILE_SHARE_READ,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
 
	if(inputFile==INVALID_HANDLE_VALUE)
		return false;

	//fread((void*)&Speicher[0], 4, 65536, inputFile);
	DWORD NumberOfBytesRead=0;
	BOOL is_read=ReadFile(
		inputFile,
		(void*)&Speicher[0],
		4*65536,
		&NumberOfBytesRead,NULL);
	if(!is_read)// || NumberOfBytesRead!=4*65536)
	{
		MpegClose();
		return false;
	}
 
	//Suche nach Tag
	{
		char Tag[4] = {0,0,0,0};
		memcpy(Tag, Speicher, 3);
		if (!strcmp(Tag,"MP+")) StreamVersion = Speicher[0]>>24;
	}
	if (StreamVersion>=7)
	{
		unsigned int dummy;

		//Initialisiere Bitstromdecodierung
		dword = Speicher[Zaehler+=1];
		
		//Auslesen des Fileheaders
		Bitrate = 0; Blockgroesse = 1;
		OverallFrames = Bitstream_read(32);
		IS_Flag  = Bitstream_read(1);
		MS_used  = Bitstream_read(1);
		Max_Band_desired = Bitstream_read(6);

		//future purpose data arrays
		dummy = Bitstream_read(32);
		dummy = Bitstream_read(32);
		dummy = Bitstream_read(32);
		dummy = Bitstream_read(32);
	}
	else
	{
		//Initialisiere Bitstromdecodierung
		dword = Speicher[Zaehler];

		//Auslesen des Fileheaders
		Bitrate  = Bitstream_read(9);
		IS_Flag  = Bitstream_read(1);
		MS_used  = Bitstream_read(1);
		StreamVersion = Bitstream_read(10);
		Max_Band_desired = Bitstream_read(5);
		Blockgroesse = Bitstream_read(6);
		if (StreamVersion<5) OverallFrames = Bitstream_read(16);
		else                 OverallFrames = Bitstream_read(32);
	}
	if (StreamVersion<4 || StreamVersion>7)
	{
		//("\n invalid MPEGplus Bitstream or unknown StreamVersion (%i)!\n\n",StreamVersion);
		return false;
	}
	
	//Bugfix: Last Frame was invalid for all StreamVersions < 6
	if (StreamVersion<6) OverallFrames-=1;

	//Initialisierung
	Huffman_SV6_Decoder();
	Huffman_SV7_Decoder();
	initialisiere_Quantisierungstabellen(Max_Band_desired,1);
	memset(V_L, 0, 1024*4);
	memset(V_R, 0, 1024*4);


	NumOfSample=0;
	NumOfSampleFrames=OverallFrames;

	return true;
}

void MpegClose()
{
	if(inputFile!=INVALID_HANDLE_VALUE)
		CloseHandle(inputFile);
	inputFile=INVALID_HANDLE_VALUE;
}

bool MpegGetNextSample(short*& buffer,int& len)
{
	buffer=Stream;
	len=0;

	if(NumOfSample>NumOfSampleFrames)
		return false;

	len=1152*2;

	if(NumOfSample<NumOfSampleFrames)
	{
		int RING;
		++NumOfSample;
		RING = Zaehler/32768;

		// dummy-read Sprung-Info
		if ((NumOfSample-1)%Blockgroesse==0) int dummy=Bitstream_read(20);
		
		if		(StreamVersion<=6) Lese_Bitstrom_SV6();
		else if (StreamVersion>=7) Lese_Bitstrom_SV7();

		// Nachladen des Ringpuffers
		if (RING!=Zaehler/32768)
		{
			//fread((void*)&Speicher[RING*32768], 4, 32768, inputFile);
			DWORD NumberOfBytesRead=0;
			BOOL is_read=ReadFile(
				inputFile,
				(void*)&Speicher[RING*32768],
				4*32768,
				&NumberOfBytesRead,NULL);
			if(!is_read)// || NumberOfBytesRead!=4*32768)
				return false;

		}
		
		// Decoder
		Requantisierung();
		if (IS_Flag)Intensity_Stereo_Decode();
		Synthese_Filter_opt();

		// write all frames except the last frame
		if (NumOfSample<NumOfSampleFrames)
		{
			if(NumOfSample==1)
			{
				buffer=Stream+962;
				len=1342;
			}
			return true;
		}
	}

	NumOfSample=NumOfSampleFrames+1;
	return true;
}

/**************************** Hauptprogramm *****************************/

void Calculate_New_V(float *Sample,float *V)
{

  //Calculating of new V-buffer values for left channel
  //Berechnen neuer V-Werte (s. ISO-11172-3 S. 39)
  //basierend auf dem Algorithmus von Byeong Gi Lee
  
  int i,k,n;
  float tmp;
  const float *C=&Cos64[0];
  static float Ai[16],Bi[16];

  Ai[i=0] = Sample[k=0] + Sample[n=31];
  Ai[++i] = Sample[++k] + Sample[--n];
  Ai[++i] = Sample[++k] + Sample[--n];
  Ai[++i] = Sample[++k] + Sample[--n];
  Ai[++i] = Sample[++k] + Sample[--n];
  Ai[++i] = Sample[++k] + Sample[--n];
  Ai[++i] = Sample[++k] + Sample[--n];
  Ai[++i] = Sample[++k] + Sample[--n];
  Ai[++i] = Sample[++k] + Sample[--n];
  Ai[++i] = Sample[++k] + Sample[--n];
  Ai[++i] = Sample[++k] + Sample[--n];
  Ai[++i] = Sample[++k] + Sample[--n];
  Ai[++i] = Sample[++k] + Sample[--n];
  Ai[++i] = Sample[++k] + Sample[--n];
  Ai[++i] = Sample[++k] + Sample[--n];
  Ai[++i] = Sample[++k] + Sample[--n];
  

  Bi[i=0] =  Ai[k=0] + Ai[n=15];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] = (Ai[k=0] - Ai[n=15])* C[2];
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[6];
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[10];
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[14];
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[18];
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[22];
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[26];
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[30];
 
  Ai[i=0] =  Bi[k=0] + Bi[n=7];
  Ai[++i] =  Bi[++k] + Bi[--n];
  Ai[++i] =  Bi[++k] + Bi[--n];
  Ai[++i] =  Bi[++k] + Bi[--n];
  Ai[++i] = (Bi[k=0] - Bi[n=7]) * C[4];
  Ai[++i] = (Bi[++k] - Bi[--n]) * C[12];
  Ai[++i] = (Bi[++k] - Bi[--n]) * C[20];
  Ai[++i] = (Bi[++k] - Bi[--n]) * C[28];
  Ai[++i] =  Bi[k=8] + Bi[n=15];
  Ai[++i] =  Bi[++k] + Bi[--n];
  Ai[++i] =  Bi[++k] + Bi[--n];
  Ai[++i] =  Bi[++k] + Bi[--n];
  Ai[++i] = (Bi[k=8] - Bi[n=15])* C[4];
  Ai[++i] = (Bi[++k] - Bi[--n]) * C[12];
  Ai[++i] = (Bi[++k] - Bi[--n]) * C[20];
  Ai[++i] = (Bi[++k] - Bi[--n]) * C[28];

  tmp = C[8];
  Bi[i=0] =  Ai[k=0] + Ai[n=3];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] = (Ai[--k] - Ai[++n]) * tmp;
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[24];
  Bi[++i] =  Ai[k=4] + Ai[n=7];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] = (Ai[--k] - Ai[++n]) * tmp;
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[24];
  Bi[++i] =  Ai[k=8] + Ai[n=11];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] = (Ai[--k] - Ai[++n]) * tmp;
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[24];
  Bi[++i] =  Ai[k=12] + Ai[n=15];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] = (Ai[--k] - Ai[++n]) * tmp;
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[24];

  tmp = C[16];
  Ai[i=0] =  Bi[k=0] + Bi[n=1];
  Ai[++i] = (Bi[k++] - Bi[n++]) * tmp;
  Ai[++i] =  Bi[++k] + Bi[++n];
  Ai[++i] = (Bi[k++] - Bi[n++]) * tmp;
  Ai[++i] =  Bi[++k] + Bi[++n];
  Ai[++i] = (Bi[k++] - Bi[n++]) * tmp;
  Ai[++i] =  Bi[++k] + Bi[++n];
  Ai[++i] = (Bi[k++] - Bi[n++]) * tmp;
  Ai[++i] =  Bi[++k] + Bi[++n];
  Ai[++i] = (Bi[k++] - Bi[n++]) * tmp;
  Ai[++i] =  Bi[++k] + Bi[++n];
  Ai[++i] = (Bi[k++] - Bi[n++]) * tmp;
  Ai[++i] =  Bi[++k] + Bi[++n];
  Ai[++i] = (Bi[k++] - Bi[n++]) * tmp;
  Ai[++i] =  Bi[++k] + Bi[++n];
  Ai[++i] = (Bi[k++] - Bi[n++]) * tmp;

  V[12]=   Ai[7];
  tmp = Ai[5] + Ai[7];
  V[4] =   tmp;
  V[36]= -(tmp + Ai[6]);
  V[44]= - Ai[4] - Ai[6] - Ai[7];
  V[14]=   Ai[15];
  tmp = Ai[11] + Ai[15];
  V[10]=   tmp;
  V[6] =   tmp + Ai[13];
  tmp = Ai[9] + Ai[13] + Ai[15];
  V[2] =   tmp;
  V[34]= -(tmp + Ai[14]);
  V[38]= -(tmp - Ai[9] + Ai[10] + Ai[11] + Ai[14]);
  tmp = Ai[12] + Ai[14] + Ai[15];
  V[46]= -(tmp + Ai[8]);
  V[42]= -(tmp + Ai[10] + Ai[11]);
  V[48]= - Ai[0];
  V[0] =   Ai[1];
  V[8] =   Ai[3];
  V[40]= - Ai[2] - Ai[3];
 
  Ai[i=0] = (Sample[k=0] - Sample[n=31])* C[1];
  Ai[++i] = (Sample[++k] - Sample[--n]) * C[3];
  Ai[++i] = (Sample[++k] - Sample[--n]) * C[5];
  Ai[++i] = (Sample[++k] - Sample[--n]) * C[7];
  Ai[++i] = (Sample[++k] - Sample[--n]) * C[9];
  Ai[++i] = (Sample[++k] - Sample[--n]) * C[11];
  Ai[++i] = (Sample[++k] - Sample[--n]) * C[13];
  Ai[++i] = (Sample[++k] - Sample[--n]) * C[15];
  Ai[++i] = (Sample[++k] - Sample[--n]) * C[17];
  Ai[++i] = (Sample[++k] - Sample[--n]) * C[19];
  Ai[++i] = (Sample[++k] - Sample[--n]) * C[21];
  Ai[++i] = (Sample[++k] - Sample[--n]) * C[23];
  Ai[++i] = (Sample[++k] - Sample[--n]) * C[25];
  Ai[++i] = (Sample[++k] - Sample[--n]) * C[27];
  Ai[++i] = (Sample[++k] - Sample[--n]) * C[29];
  Ai[++i] = (Sample[++k] - Sample[--n]) * C[31];
 
  Bi[i=0] =  Ai[k=0] + Ai[n=15];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] = (Ai[k=0] - Ai[n=15])* C[2];
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[6];
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[10];
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[14];
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[18];
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[22];
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[26];
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[30];

  Ai[i=0] =  Bi[k=0] + Bi[n=7];
  Ai[++i] =  Bi[++k] + Bi[--n];
  Ai[++i] =  Bi[++k] + Bi[--n];
  Ai[++i] =  Bi[++k] + Bi[--n];
  Ai[++i] = (Bi[k=0] - Bi[n=7]) * C[4];
  Ai[++i] = (Bi[++k] - Bi[--n]) * C[12];
  Ai[++i] = (Bi[++k] - Bi[--n]) * C[20];
  Ai[++i] = (Bi[++k] - Bi[--n]) * C[28];
  Ai[++i] =  Bi[k=8] + Bi[n=15];
  Ai[++i] =  Bi[++k] + Bi[--n];
  Ai[++i] =  Bi[++k] + Bi[--n];
  Ai[++i] =  Bi[++k] + Bi[--n];
  Ai[++i] = (Bi[k=8] - Bi[n=15])* C[4];
  Ai[++i] = (Bi[++k] - Bi[--n]) * C[12];
  Ai[++i] = (Bi[++k] - Bi[--n]) * C[20];
  Ai[++i] = (Bi[++k] - Bi[--n]) * C[28];

  tmp = C[8];
  Bi[i=0] =  Ai[k=0] + Ai[n=3];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] = (Ai[--k] - Ai[++n]) * tmp;
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[24];
  Bi[++i] =  Ai[k=4] + Ai[n=7];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] = (Ai[--k] - Ai[++n]) * tmp;
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[24];
  Bi[++i] =  Ai[k=8] + Ai[n=11];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] = (Ai[--k] - Ai[++n]) * tmp;
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[24];
  Bi[++i] =  Ai[k=12] + Ai[n=15];
  Bi[++i] =  Ai[++k] + Ai[--n];
  Bi[++i] = (Ai[--k] - Ai[++n]) * tmp;
  Bi[++i] = (Ai[++k] - Ai[--n]) * C[24];

  tmp = C[16];
  Ai[i=0] =  Bi[k=0] + Bi[n=1];
  Ai[++i] = (Bi[k++] - Bi[n++]) * tmp;
  Ai[++i] =  Bi[++k] + Bi[++n];
  Ai[++i] = (Bi[k++] - Bi[n++]) * tmp;
  Ai[++i] =  Bi[++k] + Bi[++n];
  Ai[++i] = (Bi[k++] - Bi[n++]) * tmp;
  Ai[++i] =  Bi[++k] + Bi[++n];
  Ai[++i] = (Bi[k++] - Bi[n++]) * tmp;
  Ai[++i] =  Bi[++k] + Bi[++n];
  Ai[++i] = (Bi[k++] - Bi[n++]) * tmp;
  Ai[++i] =  Bi[++k] + Bi[++n];
  Ai[++i] = (Bi[k++] - Bi[n++]) * tmp;
  Ai[++i] =  Bi[++k] + Bi[++n];
  Ai[++i] = (Bi[k++] - Bi[n++]) * tmp;
  Ai[++i] =  Bi[++k] + Bi[++n];
  Ai[++i] = (Bi[k++] - Bi[n++]) * tmp;

  V[15]= Ai[15];
  tmp = Ai[7] + Ai[15];
  V[13]= tmp;
  tmp += Ai[11];
  V[5] = tmp + Ai[5] + Ai[13];
  V[11]= tmp;
  tmp = Ai[3] + Ai[11] + Ai[15];
  V[7] = tmp + Ai[13];
  V[9] = tmp;
  tmp = Ai[1] + Ai[9] + Ai[13] + Ai[15];
  V[1] = tmp;
  V[33]= -(tmp + Ai[14]);
  tmp = Ai[5] + Ai[7] + Ai[9] + Ai[13] + Ai[15];
  V[3] = tmp;
  V[35]= -(tmp + Ai[6] +Ai[14]);
  tmp = Ai[10] + Ai[11] + Ai[13] + Ai[14] + Ai[15];
  V[37]= -(tmp + Ai[5] + Ai[6] + Ai[7]);
  V[39]= -(tmp + Ai[2] + Ai[3]);
  tmp = tmp + Ai[12] - Ai[13];;
  V[41]= -(tmp + Ai[2] + Ai[3]);
  V[43]= -(tmp + Ai[4] + Ai[6] + Ai[7]);

  tmp = Ai[8] + Ai[12] + Ai[14] + Ai[15];
  V[47]= -(tmp + Ai[0]);
  V[45]= -(tmp + Ai[4] + Ai[6] + Ai[7]);

    V[32] = -V[0];
	V[31] = -V[1];
	V[30] = -V[2];
	V[29] = -V[3];
	V[28] = -V[4];
	V[27] = -V[5];
	V[26] = -V[6];
	V[25] = -V[7];
	V[24] = -V[8];
	V[23] = -V[9];
	V[22] = -V[10];
	V[21] = -V[11];
	V[20] = -V[12];
	V[19] = -V[13];
	V[18] = -V[14];
	V[17] = -V[15];

	V[63] = V[33];
	V[62] = V[34];
	V[61] = V[35];
	V[60] = V[36];
	V[59] = V[37];
	V[58] = V[38];
	V[57] = V[39];
	V[56] = V[40];
	V[55] = V[41];
	V[54] = V[42];
	V[53] = V[43];
	V[52] = V[44];
	V[51] = V[45];
	V[50] = V[46];
	V[49] = V[47];
}

void Synthese_Filter_opt()
{
	int n,k,Index1;
	int Sum;

	for (n=0; n<36; ++n)
	{
		/* shifting um 64 Indices aufwaerts */
		memmove(V_L+64,V_L,3840);

		Calculate_New_V(&Y_L[n][0],&V_L[0]);

		/* vectoring & windowing & calculating PCM-Output */
		for (k=0; k<32; ++k)
		{
			Index1 = k*16;
			Sum=(int)(V_L[k    ]*Di_opt[Index1  ]
					+ V_L[k+ 96]*Di_opt[Index1+ 1]
					+ V_L[k+128]*Di_opt[Index1+ 2]
					+ V_L[k+224]*Di_opt[Index1+ 3]
					+ V_L[k+256]*Di_opt[Index1+ 4]
					+ V_L[k+352]*Di_opt[Index1+ 5]
					+ V_L[k+384]*Di_opt[Index1+ 6]
					+ V_L[k+480]*Di_opt[Index1+ 7]
					+ V_L[k+512]*Di_opt[Index1+ 8]
					+ V_L[k+608]*Di_opt[Index1+ 9]
					+ V_L[k+640]*Di_opt[Index1+10]
					+ V_L[k+736]*Di_opt[Index1+11]
					+ V_L[k+768]*Di_opt[Index1+12]
					+ V_L[k+864]*Di_opt[Index1+13]
					+ V_L[k+896]*Di_opt[Index1+14]
					+ V_L[k+992]*Di_opt[Index1+15]);

			// copy to PCM
			Stream[n*64+k*2] = Sum;
			// prevent from overflow
			if      (Sum> 32767) Stream[n*64+k*2] = 32767;
			else if (Sum<-32768) Stream[n*64+k*2] =-32768;
		}
	}
	for (n=0; n<36; ++n)
	{
		/* shifting um 64 Indices aufwaerts */
		memmove(V_R+64,V_R,3840);

		Calculate_New_V(&Y_R[n][0],&V_R[0]);

		/* vectoring & windowing & calculating PCM-Output */
		for (k=0; k<32; ++k)
		{
			Index1 = k*16;
			Sum=(int)(V_R[k    ]*Di_opt[Index1   ]
					+ V_R[k+ 96]*Di_opt[Index1+ 1]
					+ V_R[k+128]*Di_opt[Index1+ 2]
					+ V_R[k+224]*Di_opt[Index1+ 3]
					+ V_R[k+256]*Di_opt[Index1+ 4]
					+ V_R[k+352]*Di_opt[Index1+ 5]
					+ V_R[k+384]*Di_opt[Index1+ 6]
					+ V_R[k+480]*Di_opt[Index1+ 7]
					+ V_R[k+512]*Di_opt[Index1+ 8]
					+ V_R[k+608]*Di_opt[Index1+ 9]
					+ V_R[k+640]*Di_opt[Index1+10]
					+ V_R[k+736]*Di_opt[Index1+11]
					+ V_R[k+768]*Di_opt[Index1+12]
					+ V_R[k+864]*Di_opt[Index1+13]
					+ V_R[k+896]*Di_opt[Index1+14]
					+ V_R[k+992]*Di_opt[Index1+15]);
					
			// copy to PCM
			Stream[n*64+k*2+1] = Sum;
			// prevent from overflow
			if      (Sum> 32767) Stream[n*64+k*2+1] = 32767;
			else if (Sum<-32768) Stream[n*64+k*2+1] =-32768;
		}
	}
}

void Requantisierung()
{
	int Band,Last_Band,Subframe,n;

	//Requantisierung und Skalierung der Subband- und Subframesamples
	if (!IS_Flag)	Last_Band = Max_Band;
	else			Last_Band = Min_Band-1;

	for (Band=0; Band<=Last_Band; ++Band)
	{
		float templ,tempr;

		if (MS_Flag[Band]==1)
		{
			if      (Res_L[Band]!=0 && Res_R[Band]==0) for (n=0; n<36; ++n)
			{
				Y_R[n][Band] = Y_L[n][Band] = Requantisieren(Q[Band].L[n],Res_L[Band])*SCF[SCF_Index_L[n/12][Band]];
			}
			else if (Res_L[Band]!=0 && Res_R[Band]!=0) for (n=0; n<36; ++n)
			{
				Subframe = n/12;
				templ = Requantisieren(Q[Band].L[n],Res_L[Band])*SCF[SCF_Index_L[Subframe][Band]];
				tempr = Requantisieren(Q[Band].R[n],Res_R[Band])*SCF[SCF_Index_R[Subframe][Band]];
				Y_L[n][Band] = (templ+tempr);
				Y_R[n][Band] = (templ-tempr);
			}
			else if (Res_L[Band]==0 && Res_R[Band]!=0) for (n=0; n<36; ++n)
			{
				Y_L[n][Band] = Requantisieren(Q[Band].R[n],Res_R[Band])*SCF[SCF_Index_R[n/12][Band]];
				Y_R[n][Band] = -Y_L[n][Band];
			}
			else for (n=0; n<36; ++n) Y_L[n][Band] = Y_R[n][Band] = (float)(0);
		}
		else /*if (MS_Flag[Band]==0)*/
		{
			if      (Res_L[Band]!=0 && Res_R[Band]!=0) for (n=0; n<36; ++n)
			{
				Subframe = n/12;
				Y_L[n][Band] = Requantisieren(Q[Band].L[n],Res_L[Band])*SCF[SCF_Index_L[Subframe][Band]];
				Y_R[n][Band] = Requantisieren(Q[Band].R[n],Res_R[Band])*SCF[SCF_Index_R[Subframe][Band]];
			}
			else if (Res_L[Band]==0 && Res_R[Band]!=0) for (n=0; n<36; ++n)
			{
				Y_L[n][Band] = (float)(0);
				Y_R[n][Band] = Requantisieren(Q[Band].R[n],Res_R[Band])*SCF[SCF_Index_R[n/12][Band]];
			}
			else if (Res_L[Band]!=0 && Res_R[Band]==0) for (n=0; n<36; ++n)
			{
				Y_L[n][Band] = Requantisieren(Q[Band].L[n],Res_L[Band])*SCF[SCF_Index_L[n/12][Band]];
				Y_R[n][Band] = (float)(0);
			}
			else  for (n=0; n<36; ++n) Y_L[n][Band] = Y_R[n][Band] = (float)(0);
		}
	}
}

void Intensity_Stereo_Decode()
{
	int Band,n,k;
	float norm;

	//Requantisierung und Wiederherstellung des Stereo-Images
	norm = float(sqrt(sqrt(2)));
	for (Band=Min_Band; Band<=Max_Band; ++Band)
	{
		for (n=0; n<36; ++n)
		{
			if (Res_L[Band]!=0)
			{
				k=n/12;
				
				Y_L[n][Band] = Requantisieren(Q[Band].L[n],Res_L[Band])/norm;

				Y_R[n][Band] = Y_L[n][Band]*SCF[SCF_Index_R[k][Band]];
				Y_L[n][Band] = Y_L[n][Band]*SCF[SCF_Index_L[k][Band]];
			}
			else
			{
				Y_L[n][Band] = Y_R[n][Band] = (float)1.0E-30;
			}
		}
	}
}

/****************************************** SV 6 ******************************************/
void Lese_Bitstrom_SV6()
{
	int n,k;
	HuffmanTyp *Table;
	Max_used_Band=0;

	//reading Frameheader
	for (n=0; n<=Max_Band; ++n)
	{
		if      (n<11)           Table = &Region_A[0];
		else if (n>=11 && n<=22) Table = &Region_B[0];
		else /*if (n>=23)*/      Table = &Region_C[0];

		if (Bitrate<=128) Res_L[n] = Q_res[n][Huffman_Decode(Table)];
		else			  Res_L[n] = Q_res[n][Bitstream_read(Q_bit[n])];

		//nur Lesen, falls kein IS
		if (!(IS_Flag==1 && n>=Min_Band))
		{
			if (MS_used)	  MS_Flag[n] = Bitstream_read(1);
			if (Bitrate<=128) Res_R[n] = Q_res[n][Huffman_Decode(Table)];
			else			  Res_R[n] = Q_res[n][Bitstream_read(Q_bit[n])];
		}
		else
		{
			Res_R[n]=0;
		}
		//fuehre nachfolgende Operationen nur bis zum maximal enthaltenen Subband aus
		if (Res_L[n]!=0 || Res_R[n]!=0) Max_used_Band = n;
	}

	//reading SCFI-Bundle
	for (n=0; n<=Max_used_Band; ++n)
	{
		if (Res_L[n]>0)
		{
			BundleTyp Word;
			Word = SCFI_Bundle_read();
			SCFI_L[n] = Word.SCFI;
			DSCF_Flag_L[n] = Word.DSCF;
		}
		if ((Res_R[n]>0) || (Res_L[n]>0 && IS_Flag==1 && n>=Min_Band))
		{
			BundleTyp Word;
			Word = SCFI_Bundle_read();
			SCFI_R[n] = Word.SCFI;
			DSCF_Flag_R[n] = Word.DSCF;
		}
	}
	
	//reading SCF
	Table = &DSCF_Entropie[0];
	for (n=0; n<=Max_used_Band; ++n)
	{
		if (Res_L[n]>0)
		{   
			//mit DSCF
			if (DSCF_Flag_L[n]==1)
			{
				if      (SCFI_L[n]==3)
				{
					SCF_Index_L[0][n] = DSCF_Reference_L[n] + Huffman_Decode_fast(Table) -6;
					SCF_Index_L[1][n] = SCF_Index_L[0][n];
					SCF_Index_L[2][n] = SCF_Index_L[1][n];
					DSCF_Reference_L[n] = SCF_Index_L[2][n];
				}
				else if (SCFI_L[n]==1)
				{
					SCF_Index_L[0][n] = DSCF_Reference_L[n] + Huffman_Decode_fast(Table) -6;
					SCF_Index_L[1][n] = SCF_Index_L[0][n]   + Huffman_Decode_fast(Table) -6;
					SCF_Index_L[2][n] = SCF_Index_L[1][n];
					DSCF_Reference_L[n] = SCF_Index_L[2][n];
				}
				else if (SCFI_L[n]==2)
				{
					SCF_Index_L[0][n] = DSCF_Reference_L[n] + Huffman_Decode_fast(Table) -6;
					SCF_Index_L[1][n] = SCF_Index_L[0][n];
					SCF_Index_L[2][n] = SCF_Index_L[1][n]   + Huffman_Decode_fast(Table) -6;
					DSCF_Reference_L[n] = SCF_Index_L[2][n];
				}
				else /*if      (SCFI_L[n]==0)*/
				{
					SCF_Index_L[0][n] = DSCF_Reference_L[n] + Huffman_Decode_fast(Table) -6;
					SCF_Index_L[1][n] = SCF_Index_L[0][n]   + Huffman_Decode_fast(Table) -6;
					SCF_Index_L[2][n] = SCF_Index_L[1][n]   + Huffman_Decode_fast(Table) -6;
					DSCF_Reference_L[n] = SCF_Index_L[2][n];
				}
			}
			//ohne DSCF
			else /*if (DSCF_Flag_L[n]==0)*/
			{
				if      (SCFI_L[n]==3)
				{
					SCF_Index_L[0][n] = Bitstream_read(6);
					SCF_Index_L[1][n] = SCF_Index_L[0][n];
					SCF_Index_L[2][n] = SCF_Index_L[1][n];
					DSCF_Reference_L[n] = SCF_Index_L[2][n];
				}
				else if (SCFI_L[n]==1)
				{
					SCF_Index_L[0][n] = Bitstream_read(6);
					SCF_Index_L[1][n] = Bitstream_read(6);
					SCF_Index_L[2][n] = SCF_Index_L[1][n];
					DSCF_Reference_L[n] = SCF_Index_L[2][n];
				}
				else if (SCFI_L[n]==2)
				{
					SCF_Index_L[0][n] = Bitstream_read(6);
					SCF_Index_L[1][n] = SCF_Index_L[0][n];
					SCF_Index_L[2][n] = Bitstream_read(6);
					DSCF_Reference_L[n] = SCF_Index_L[2][n];
				}
				else /*if      (SCFI_L[n]==0)*/
				{
					SCF_Index_L[0][n] = Bitstream_read(6);
					SCF_Index_L[1][n] = Bitstream_read(6);
					SCF_Index_L[2][n] = Bitstream_read(6);
					DSCF_Reference_L[n] = SCF_Index_L[2][n];
				}
			}
		}
		if ((Res_R[n]>0) || (Res_L[n]>0 && IS_Flag==1 && n>=Min_Band))
		{
			//mit DSCF
			if (DSCF_Flag_R[n]==1)
			{
				if      (SCFI_R[n]==3)
				{
					SCF_Index_R[0][n] = DSCF_Reference_R[n] + Huffman_Decode_fast(Table) -6;
					SCF_Index_R[1][n] = SCF_Index_R[0][n];
					SCF_Index_R[2][n] = SCF_Index_R[1][n];
					DSCF_Reference_R[n] = SCF_Index_R[2][n];
				}
				else if (SCFI_R[n]==1)
				{
					SCF_Index_R[0][n] = DSCF_Reference_R[n] + Huffman_Decode_fast(Table) -6;
					SCF_Index_R[1][n] = SCF_Index_R[0][n]   + Huffman_Decode_fast(Table) -6;
					SCF_Index_R[2][n] = SCF_Index_R[1][n];
					DSCF_Reference_R[n] = SCF_Index_R[2][n];
				}
				else if (SCFI_R[n]==2)
				{
					SCF_Index_R[0][n] = DSCF_Reference_R[n] + Huffman_Decode_fast(Table) -6;
					SCF_Index_R[1][n] = SCF_Index_R[0][n];
					SCF_Index_R[2][n] = SCF_Index_R[1][n]   + Huffman_Decode_fast(Table) -6;
					DSCF_Reference_R[n] = SCF_Index_R[2][n];
				}
				else /*if (SCFI_R[n]==0)*/
				{
					SCF_Index_R[0][n] = DSCF_Reference_R[n] + Huffman_Decode_fast(Table) -6;
					SCF_Index_R[1][n] = SCF_Index_R[0][n]   + Huffman_Decode_fast(Table) -6;
					SCF_Index_R[2][n] = SCF_Index_R[1][n]   + Huffman_Decode_fast(Table) -6;
					DSCF_Reference_R[n] = SCF_Index_R[2][n];
				}
			}
			//ohne DSCF
			else /*if (DSCF_Flag_R[n]==0)*/
			{
				if      (SCFI_R[n]==3)
				{
					SCF_Index_R[0][n] = Bitstream_read(6);
					SCF_Index_R[1][n] = SCF_Index_R[0][n];
					SCF_Index_R[2][n] = SCF_Index_R[1][n];
					DSCF_Reference_R[n] = SCF_Index_R[2][n];
				}
				else if (SCFI_R[n]==1)
				{
					SCF_Index_R[0][n] = Bitstream_read(6);
					SCF_Index_R[1][n] = Bitstream_read(6);
					SCF_Index_R[2][n] = SCF_Index_R[1][n];
					DSCF_Reference_R[n] = SCF_Index_R[2][n];
				}
				else if (SCFI_R[n]==2)
				{
					SCF_Index_R[0][n] = Bitstream_read(6);
					SCF_Index_R[1][n] = SCF_Index_R[0][n];
					SCF_Index_R[2][n] = Bitstream_read(6);
					DSCF_Reference_R[n] = SCF_Index_R[2][n];
				}
				else /*if      (SCFI_R[n]==0)*/
				{
					SCF_Index_R[0][n] = Bitstream_read(6);
					SCF_Index_R[1][n] = Bitstream_read(6);
					SCF_Index_R[2][n] = Bitstream_read(6);
					DSCF_Reference_R[n] = SCF_Index_R[2][n];
				}
			}
		}
	}

	//reading Samples
	for (n=0; n<=Max_used_Band; ++n)
	{
		HuffmanTyp *x1,*x2;
		
		if      (Res_L[n] == 1) {x1 = &Entropie_1[0];}
		else if (Res_L[n] == 2) {x1 = &Entropie_2[0];}
		else if (Res_L[n] == 3) {x1 = &Entropie_3[0];}
		else if (Res_L[n] == 4) {x1 = &Entropie_4[0];}
		else if (Res_L[n] == 5) {x1 = &Entropie_5[0];}
		else if (Res_L[n] == 6) {x1 = &Entropie_6[0];}
		else if (Res_L[n] == 7) {x1 = &Entropie_7[0];}
		else					{x1 = NULL;}

		if      (Res_R[n] == 1) {x2 = &Entropie_1[0];}
		else if (Res_R[n] == 2) {x2 = &Entropie_2[0];}
		else if (Res_R[n] == 3) {x2 = &Entropie_3[0];}
		else if (Res_R[n] == 4) {x2 = &Entropie_4[0];}
		else if (Res_R[n] == 5) {x2 = &Entropie_5[0];}
		else if (Res_R[n] == 6) {x2 = &Entropie_6[0];}
		else if (Res_R[n] == 7) {x2 = &Entropie_7[0];}
		else					{x2 = NULL;}

		for (k=0; k<36; ++k)
		{
			if (x1!=NULL) Q[n].L[k] = Huffman_Decode_fast(x1);
			if (x2!=NULL) Q[n].R[k] = Huffman_Decode_fast(x2);
		}

		if (!(Res_L[n]<=7 && Res_R[n]<=7))
		for (k=0; k<36; ++k)
		{
			if (Res_L[n]>7) {Q[n].L[k] = Bitstream_read(Res_bit[Res_L[n]]);}
			if (Res_R[n]>7) {Q[n].R[k] = Bitstream_read(Res_bit[Res_R[n]]);}
		}
	}
}

/****************************************** SV 7 ******************************************/
void Lese_Bitstrom_SV7()
{
	int n,k;
	HuffmanTyp *Table;
	Max_used_Band=0;

	/***************************** Header *****************************/
	Table = HuffHdr;
	//first subband
	Res_L[0] = Bitstream_read(4);
	if (!(IS_Flag==1 && 0>=Min_Band))
	{
		Res_R[0] = Bitstream_read(4);
		if (MS_used && !(Res_L[0]==0 && Res_R[0]==0)) MS_Flag[0] = Bitstream_read(1);
	}
	//consecutive subbands
	for (n=1; n<=Max_Band; ++n)
	{
		unsigned int diff;

		diff = Huffman_Decode(Table);
		if (diff!=9) Res_L[n] = Res_L[n-1] + diff - 5;
		else         Res_L[n] = Bitstream_read(4);

		//nur Lesen, falls kein IS
		if (!(IS_Flag==1 && n>=Min_Band))
		{
			diff = Huffman_Decode(Table);
			if (diff!=9) Res_R[n] = Res_R[n-1] + diff - 5;
			else         Res_R[n] = Bitstream_read(4);
			if (MS_used && !(Res_L[n]==0 && Res_R[n]==0)) MS_Flag[n] = Bitstream_read(1);
		}
		else
		{
			Res_R[n]=0;
		}
		//fuehre nachfolgende Operationen nur bis zum maximal enthaltenen Subband aus
		if (Res_L[n]!=0 || Res_R[n]!=0) Max_used_Band = n;
	}	
	
	/****************************** SCFI ******************************/
	Table = HuffSCFI;
	for (n=0; n<=Max_used_Band; ++n)
	{
		if (Res_L[n]>0)
		{
			SCFI_L[n] = Huffman_Decode_fast(Table);
		}
		if ((Res_R[n]>0) || (Res_L[n]>0 && IS_Flag==1 && n>=Min_Band))
		{
			SCFI_R[n] = Huffman_Decode_fast(Table);
		}
	}
	
	/**************************** SCF/DSCF ****************************/
	Table = HuffDSCF;
	for (n=0; n<=Max_used_Band; ++n)
	{
		unsigned int diff;
		if (Res_L[n]>0)
		{   
			if      (SCFI_L[n]==1)
			{
				diff = Huffman_Decode_fast(Table);
				if (diff!=15) SCF_Index_L[0][n] = DSCF_Reference_L[n] + diff - 7;
				else		  SCF_Index_L[0][n] = Bitstream_read(6);
				diff = Huffman_Decode_fast(Table);
				if (diff!=15) SCF_Index_L[1][n] = SCF_Index_L[0][n] + diff - 7;
				else		  SCF_Index_L[1][n] = Bitstream_read(6);
				SCF_Index_L[2][n] = SCF_Index_L[1][n];
			}
			else if (SCFI_L[n]==3)
			{
				diff = Huffman_Decode_fast(Table);
				if (diff!=15) SCF_Index_L[0][n] = DSCF_Reference_L[n] + diff - 7;
				else		  SCF_Index_L[0][n] = Bitstream_read(6);
				SCF_Index_L[1][n] = SCF_Index_L[0][n];
				SCF_Index_L[2][n] = SCF_Index_L[1][n];
			}
			else if (SCFI_L[n]==2)
			{
				diff = Huffman_Decode_fast(Table);
				if (diff!=15) SCF_Index_L[0][n] = DSCF_Reference_L[n] + diff - 7;
				else		  SCF_Index_L[0][n] = Bitstream_read(6);
				SCF_Index_L[1][n] = SCF_Index_L[0][n];
				diff = Huffman_Decode_fast(Table);
				if (diff!=15) SCF_Index_L[2][n] = SCF_Index_L[1][n] + diff - 7;
				else		  SCF_Index_L[2][n] = Bitstream_read(6);
			}
			else /*if      (SCFI_L[n]==0)*/
			{
				diff = Huffman_Decode_fast(Table);
				if (diff!=15) SCF_Index_L[0][n] = DSCF_Reference_L[n] + diff - 7;
				else		  SCF_Index_L[0][n] = Bitstream_read(6);
				diff = Huffman_Decode_fast(Table);
				if (diff!=15) SCF_Index_L[1][n] = SCF_Index_L[0][n] + diff - 7;
				else		  SCF_Index_L[1][n] = Bitstream_read(6);
				diff = Huffman_Decode_fast(Table);
				if (diff!=15) SCF_Index_L[2][n] = SCF_Index_L[1][n] + diff - 7;
				else		  SCF_Index_L[2][n] = Bitstream_read(6);
			}
			DSCF_Reference_L[n] = SCF_Index_L[2][n];
		}
		if ((Res_R[n]>0) || (Res_L[n]>0 && IS_Flag==1 && n>=Min_Band))
		{
			if      (SCFI_R[n]==1)
			{
				diff = Huffman_Decode_fast(Table);
				if (diff!=15) SCF_Index_R[0][n] = DSCF_Reference_R[n] + diff - 7;
				else		  SCF_Index_R[0][n] = Bitstream_read(6);
				diff = Huffman_Decode_fast(Table);
				if (diff!=15) SCF_Index_R[1][n] = SCF_Index_R[0][n] + diff - 7;
				else		  SCF_Index_R[1][n] = Bitstream_read(6);
				SCF_Index_R[2][n] = SCF_Index_R[1][n];
			}
			else if (SCFI_R[n]==3)
			{
				diff = Huffman_Decode_fast(Table);
				if (diff!=15) SCF_Index_R[0][n] = DSCF_Reference_R[n] + diff - 7;
				else		  SCF_Index_R[0][n] = Bitstream_read(6);
				SCF_Index_R[1][n] = SCF_Index_R[0][n];
				SCF_Index_R[2][n] = SCF_Index_R[1][n];
			}
			else if (SCFI_R[n]==2)
			{
				diff = Huffman_Decode_fast(Table);
				if (diff!=15) SCF_Index_R[0][n] = DSCF_Reference_R[n] + diff - 7;
				else		  SCF_Index_R[0][n] = Bitstream_read(6);
				SCF_Index_R[1][n] = SCF_Index_R[0][n];
				diff = Huffman_Decode_fast(Table);
				if (diff!=15) SCF_Index_R[2][n] = SCF_Index_R[1][n] + diff - 7;
				else		  SCF_Index_R[2][n] = Bitstream_read(6);
			}
			else /*if      (SCFI_R[n]==0)*/
			{
				diff = Huffman_Decode_fast(Table);
				if (diff!=15) SCF_Index_R[0][n] = DSCF_Reference_R[n] + diff - 7;
				else		  SCF_Index_R[0][n] = Bitstream_read(6);
				diff = Huffman_Decode_fast(Table);
				if (diff!=15) SCF_Index_R[1][n] = SCF_Index_R[0][n] + diff - 7;
				else		  SCF_Index_R[1][n] = Bitstream_read(6);
				diff = Huffman_Decode_fast(Table);
				if (diff!=15) SCF_Index_R[2][n] = SCF_Index_R[1][n] + diff - 7;
				else		  SCF_Index_R[2][n] = Bitstream_read(6);
			}
			DSCF_Reference_R[n] = SCF_Index_R[2][n];
		}
	}
	/***************************** Samples ****************************/
	for (n=0; n<=Max_used_Band; ++n)
	{
		HuffmanTyp *Table;
	
		if      (Res_L[n]==1)
		{
			Table = HuffQ1[Bitstream_read(1)];
			for (k=0; k<36; k+=3)
			{
				unsigned int idx = Huffman_Decode(Table);
				Q[n].L[k+2] = idx/9;
				Q[n].L[k+1] = (idx-9*Q[n].L[k+2])/3;
				Q[n].L[k  ] = idx%3;
			}
		}
		else if (Res_L[n]==2)
		{
			Table = HuffQ2[Bitstream_read(1)];
			for (k=0; k<36; k+=2)
			{
				unsigned int idx = Huffman_Decode(Table);
				Q[n].L[k+1] = idx/5;
				Q[n].L[k  ] = idx%5;
			}
		}
		else if (Res_L[n]==3)
		{
			Table = HuffQ3[Bitstream_read(1)];
			for (k=0; k<36; ++k) Q[n].L[k] = Huffman_Decode_fast(Table);
		}
		else if (Res_L[n]==4)
		{
			Table = HuffQ4[Bitstream_read(1)];
			for (k=0; k<36; ++k) Q[n].L[k] = Huffman_Decode_fast(Table);
		}
		else if (Res_L[n]==5)
		{
			Table = HuffQ5[Bitstream_read(1)];
			for (k=0; k<36; ++k) Q[n].L[k] = Huffman_Decode_fast(Table);
		}
		else if (Res_L[n]==6)
		{
			Table = HuffQ6[Bitstream_read(1)];
			for (k=0; k<36; ++k) Q[n].L[k] = Huffman_Decode(Table);
		}
		else if (Res_L[n]==7)
		{
			Table = HuffQ7[Bitstream_read(1)];//Entropie_7;
			for (k=0; k<36; ++k) Q[n].L[k] = Huffman_Decode(Table);
		}
		else if (Res_L[n]>=8)
		{
			for (k=0; k<36; ++k) Q[n].L[k] = Bitstream_read(Res_bit[Res_L[n]]);
		}

		if      (Res_R[n]==1)
		{
			Table = HuffQ1[Bitstream_read(1)];
			for (k=0; k<36; k+=3)
			{
				unsigned int idx = Huffman_Decode(Table);
				Q[n].R[k+2] = idx/9;
				Q[n].R[k+1] = (idx-9*Q[n].R[k+2])/3;
				Q[n].R[k  ] = idx%3;
			} 
		}
		else if (Res_R[n]==2)
		{
			Table = HuffQ2[Bitstream_read(1)];
			for (k=0; k<36; k+=2)
			{
				unsigned int idx = Huffman_Decode(Table);
				Q[n].R[k+1] = idx/5;
				Q[n].R[k  ] = idx%5;
			}
		}
		else if (Res_R[n]==3)
		{
			Table = HuffQ3[Bitstream_read(1)];
			for (k=0; k<36; ++k) Q[n].R[k] = Huffman_Decode_fast(Table);
		}
		else if (Res_R[n]==4)
		{
			Table = HuffQ4[Bitstream_read(1)];
			for (k=0; k<36; ++k) Q[n].R[k] = Huffman_Decode_fast(Table);
		}
		else if (Res_R[n]==5)
		{
			Table = HuffQ5[Bitstream_read(1)];
			for (k=0; k<36; ++k) Q[n].R[k] = Huffman_Decode_fast(Table);
		}
		else if (Res_R[n]==6)
		{
			Table = HuffQ6[Bitstream_read(1)];
			for (k=0; k<36; ++k) Q[n].R[k] = Huffman_Decode(Table);
		}
		else if (Res_R[n]==7)
		{
			Table = HuffQ7[Bitstream_read(1)];//Entropie_7;
			for (k=0; k<36; ++k) Q[n].R[k] = Huffman_Decode(Table);
		}
		else if (Res_R[n]>=8)
		{
			for (k=0; k<36; ++k) Q[n].R[k] = Bitstream_read(Res_bit[Res_R[n]]);
		}		
	}
}