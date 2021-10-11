#include "huff_old.h"

#ifndef RESORT_PROC
#define RESORT_PROC

void Huffman_SV6_Decoder()
{
	Huffman_SV6_Encoder();
	Resort_HuffTables( 16, Region_A);
	Resort_HuffTables(  8, Region_B);
	Resort_HuffTables(  4, Region_C);
	Resort_HuffTables(  8, SCFI_Bundle);
	Resort_HuffTables( 13, DSCF_Entropie);
	Resort_HuffTables(  3, Entropie_1);
	Resort_HuffTables(  5, Entropie_2);
	Resort_HuffTables(  7, Entropie_3);
	Resort_HuffTables(  9, Entropie_4);
	Resort_HuffTables( 15, Entropie_5);
	Resort_HuffTables( 31, Entropie_6);
	Resort_HuffTables( 63, Entropie_7);
}

void Resort_HuffTables(unsigned int elements, HuffmanTyp *Table)
{
	unsigned int c[LARGEST_TABLE];
	unsigned int v[LARGEST_TABLE];
	unsigned int l[LARGEST_TABLE];
	unsigned int i;
	int sorted=0;

	for (i=0; i<elements; ++i)
	{
		Table[i].Value = i;
		c[i] = Table[i].Code<<(32-Table[i].Length);
		v[i] = Table[i].Value;
		l[i] = Table[i].Length;
	}
	
	//bubble sort
	while (!sorted)
	{
		sorted = 1;
		for (i=1; i<elements; ++i)
		{
			if (c[i]>c[i-1])
			{
				unsigned int tmp;
				tmp = v[i]; v[i] = v[i-1]; v[i-1] = tmp;
				tmp = c[i]; c[i] = c[i-1]; c[i-1] = tmp;
				tmp = l[i];	l[i] = l[i-1]; l[i-1] = tmp;
				sorted = 0;
			}
		}
	}
	for (i=0; i<elements; ++i)
	{
		Table[i].Code =   c[i];
		Table[i].Length = l[i];
		Table[i].Value =  v[i];
	}
}
#endif

void Huffman_SV6_Encoder()
{
	//SCFI-Bundle
	SCFI_Bundle[7].Code=  1; SCFI_Bundle[7].Length= 1;
	SCFI_Bundle[3].Code=  1; SCFI_Bundle[3].Length= 2;
	SCFI_Bundle[5].Code=  0; SCFI_Bundle[5].Length= 3;
	SCFI_Bundle[1].Code=  7; SCFI_Bundle[1].Length= 5;
	SCFI_Bundle[2].Code=  6; SCFI_Bundle[2].Length= 5;
	SCFI_Bundle[4].Code=  4; SCFI_Bundle[4].Length= 5;
	SCFI_Bundle[0].Code= 11; SCFI_Bundle[0].Length= 6;
	SCFI_Bundle[6].Code= 10; SCFI_Bundle[6].Length= 6;

	//Region A (Subbands  0..10)
	Region_A[ 1].Code=    1; Region_A[ 1].Length=  1;
	Region_A[ 2].Code=    0; Region_A[ 2].Length=  2;
	Region_A[ 0].Code=    2; Region_A[ 0].Length=  3;
	Region_A[ 3].Code=   15; Region_A[ 3].Length=  5;
	Region_A[ 5].Code=   13; Region_A[ 5].Length=  5;
	Region_A[ 6].Code=   12; Region_A[ 6].Length=  5;
	Region_A[ 4].Code=   29; Region_A[ 4].Length=  6;
	Region_A[ 7].Code=   57; Region_A[ 7].Length=  7;
	Region_A[ 8].Code=  113; Region_A[ 8].Length=  8;
	Region_A[ 9].Code=  225; Region_A[ 9].Length=  9;
	Region_A[10].Code=  449; Region_A[10].Length= 10;
	Region_A[11].Code=  897; Region_A[11].Length= 11;
	Region_A[12].Code= 1793; Region_A[12].Length= 12;
	Region_A[13].Code= 3585; Region_A[13].Length= 13;
	Region_A[14].Code= 7169; Region_A[14].Length= 14;
	Region_A[15].Code= 7168; Region_A[15].Length= 14;

	//Region B (Subbands 11..22)
	Region_B[1].Code= 1; Region_B[1].Length= 1;
	Region_B[0].Code= 1; Region_B[0].Length= 2;
	Region_B[2].Code= 1; Region_B[2].Length= 3;
	Region_B[3].Code= 1; Region_B[3].Length= 4;
	Region_B[4].Code= 1; Region_B[4].Length= 5;
	Region_B[5].Code= 1; Region_B[5].Length= 6;
	Region_B[6].Code= 1; Region_B[6].Length= 7;
	Region_B[7].Code= 0; Region_B[7].Length= 7;

	//Region C (Subbands 23..31)
	Region_C[0].Code= 1; Region_C[0].Length= 1;
	Region_C[1].Code= 1; Region_C[1].Length= 2;
	Region_C[2].Code= 1; Region_C[2].Length= 3;
	Region_C[3].Code= 0; Region_C[3].Length= 3;

	//DSCF
	DSCF_Entropie[ 6].Code=  0; DSCF_Entropie[ 6].Length= 2;
	DSCF_Entropie[ 7].Code=  7; DSCF_Entropie[ 7].Length= 3;
	DSCF_Entropie[ 5].Code=  4; DSCF_Entropie[ 5].Length= 3;
	DSCF_Entropie[ 8].Code=  3; DSCF_Entropie[ 8].Length= 3;
	DSCF_Entropie[ 9].Code= 13; DSCF_Entropie[ 9].Length= 4;
	DSCF_Entropie[ 4].Code= 11; DSCF_Entropie[ 4].Length= 4;
	DSCF_Entropie[10].Code= 10; DSCF_Entropie[10].Length= 4;
	DSCF_Entropie[ 2].Code=  4; DSCF_Entropie[ 2].Length= 4;
	DSCF_Entropie[11].Code= 25; DSCF_Entropie[11].Length= 5;
	DSCF_Entropie[ 3].Code= 24; DSCF_Entropie[ 3].Length= 5;
	DSCF_Entropie[ 1].Code= 11; DSCF_Entropie[ 1].Length= 5;
	DSCF_Entropie[12].Code= 21; DSCF_Entropie[12].Length= 6;
	DSCF_Entropie[ 0].Code= 20; DSCF_Entropie[ 0].Length= 6;

	//erster Quantisierer
	Entropie_1[1].Code= 1; Entropie_1[1].Length= 1;
	Entropie_1[0].Code= 1; Entropie_1[0].Length= 2;
	Entropie_1[2].Code= 0; Entropie_1[2].Length= 2;

	//zweiter Quantisierer
	Entropie_2[2].Code=  3; Entropie_2[2].Length= 2;
	Entropie_2[3].Code=  1; Entropie_2[3].Length= 2;
	Entropie_2[1].Code=  0; Entropie_2[1].Length= 2;
	Entropie_2[4].Code=  5; Entropie_2[4].Length= 3;
	Entropie_2[0].Code=  4; Entropie_2[0].Length= 3;

	//dritter Quantisierer
	Entropie_3[3].Code=  3; Entropie_3[3].Length= 2;
	Entropie_3[2].Code=  1; Entropie_3[2].Length= 2;
	Entropie_3[4].Code=  0; Entropie_3[4].Length= 2;
	Entropie_3[1].Code=  5; Entropie_3[1].Length= 3;
	Entropie_3[5].Code=  9; Entropie_3[5].Length= 4;
	Entropie_3[0].Code= 17; Entropie_3[0].Length= 5;
	Entropie_3[6].Code= 16; Entropie_3[6].Length= 5;

	//vierter Quantisierer
	Entropie_4[4].Code=  0; Entropie_4[4].Length= 2;
	Entropie_4[5].Code=  6; Entropie_4[5].Length= 3;
	Entropie_4[3].Code=  5; Entropie_4[3].Length= 3;
	Entropie_4[6].Code=  4; Entropie_4[6].Length= 3;
	Entropie_4[2].Code=  3; Entropie_4[2].Length= 3;
	Entropie_4[7].Code= 15; Entropie_4[7].Length= 4;
	Entropie_4[1].Code= 14; Entropie_4[1].Length= 4;
	Entropie_4[0].Code=  5; Entropie_4[0].Length= 4;
	Entropie_4[8].Code=  4; Entropie_4[8].Length= 4;

	//fuenfter Quantisierer
	Entropie_5[7 ].Code=  4; Entropie_5[7 ].Length= 3;
	Entropie_5[8 ].Code=  3; Entropie_5[8 ].Length= 3;
	Entropie_5[6 ].Code=  2; Entropie_5[6 ].Length= 3;
	Entropie_5[9 ].Code=  0; Entropie_5[9 ].Length= 3;
	Entropie_5[5 ].Code= 15; Entropie_5[5 ].Length= 4;
	Entropie_5[4 ].Code= 13; Entropie_5[4 ].Length= 4;
	Entropie_5[10].Code= 12; Entropie_5[10].Length= 4;
	Entropie_5[11].Code= 10; Entropie_5[11].Length= 4;
	Entropie_5[3 ].Code=  3; Entropie_5[3 ].Length= 4;
	Entropie_5[12].Code=  2; Entropie_5[12].Length= 4;
	Entropie_5[2 ].Code= 29; Entropie_5[2 ].Length= 5;
	Entropie_5[1 ].Code= 23; Entropie_5[1 ].Length= 5;
	Entropie_5[13].Code= 22; Entropie_5[13].Length= 5;
	Entropie_5[0 ].Code= 57; Entropie_5[0 ].Length= 6;
	Entropie_5[14].Code= 56; Entropie_5[14].Length= 6;

	//sechster Quantisierer
	Entropie_6[15].Code=  9; Entropie_6[15].Length= 4;
	Entropie_6[16].Code=  8; Entropie_6[16].Length= 4;
	Entropie_6[14].Code=  7; Entropie_6[14].Length= 4;
	Entropie_6[18].Code=  6; Entropie_6[18].Length= 4;
	Entropie_6[17].Code=  5; Entropie_6[17].Length= 4;
	Entropie_6[12].Code=  3; Entropie_6[12].Length= 4;
	Entropie_6[13].Code=  2; Entropie_6[13].Length= 4;
	Entropie_6[19].Code=  0; Entropie_6[19].Length= 4;
	Entropie_6[11].Code= 31; Entropie_6[11].Length= 5;
	Entropie_6[20].Code= 30; Entropie_6[20].Length= 5;
	Entropie_6[10].Code= 29; Entropie_6[10].Length= 5;
	Entropie_6[9 ].Code= 27; Entropie_6[9 ].Length= 5;
	Entropie_6[21].Code= 26; Entropie_6[21].Length= 5;
	Entropie_6[22].Code= 25; Entropie_6[22].Length= 5;
	Entropie_6[8 ].Code= 24; Entropie_6[8 ].Length= 5;
	Entropie_6[7 ].Code= 23; Entropie_6[7 ].Length= 5;
	Entropie_6[23].Code= 21; Entropie_6[23].Length= 5;
	Entropie_6[6 ].Code=  9; Entropie_6[6 ].Length= 5;
	Entropie_6[24].Code=  3; Entropie_6[24].Length= 5;
	Entropie_6[25].Code= 57; Entropie_6[25].Length= 6;
	Entropie_6[5 ].Code= 56; Entropie_6[5 ].Length= 6;
	Entropie_6[4 ].Code= 45; Entropie_6[4 ].Length= 6;
	Entropie_6[26].Code= 41; Entropie_6[26].Length= 6;
	Entropie_6[2 ].Code= 40; Entropie_6[2 ].Length= 6;
	Entropie_6[27].Code= 17; Entropie_6[27].Length= 6;
	Entropie_6[28].Code= 16; Entropie_6[28].Length= 6;
	Entropie_6[3 ].Code=  5; Entropie_6[3 ].Length= 6;
	Entropie_6[29].Code= 89; Entropie_6[29].Length= 7;
	Entropie_6[1 ].Code= 88; Entropie_6[1 ].Length= 7;
	Entropie_6[30].Code=  9; Entropie_6[30].Length= 7;
	Entropie_6[0 ].Code=  8; Entropie_6[0 ].Length= 7;

	//siebter Quantisierer
	Entropie_7[25].Code=   0; Entropie_7[25].Length= 5;
	Entropie_7[37].Code=   1; Entropie_7[37].Length= 5;
	Entropie_7[62].Code=  16; Entropie_7[62].Length= 8;
	Entropie_7[ 0].Code=  17; Entropie_7[ 0].Length= 8;
	Entropie_7[ 3].Code=   9; Entropie_7[ 3].Length= 7;
	Entropie_7[ 5].Code=  10; Entropie_7[ 5].Length= 7;
	Entropie_7[ 6].Code=  11; Entropie_7[ 6].Length= 7;
	Entropie_7[38].Code=   3; Entropie_7[38].Length= 5;
	Entropie_7[35].Code=   4; Entropie_7[35].Length= 5;
	Entropie_7[33].Code=   5; Entropie_7[33].Length= 5;
	Entropie_7[24].Code=   6; Entropie_7[24].Length= 5;
	Entropie_7[27].Code=   7; Entropie_7[27].Length= 5;
	Entropie_7[26].Code=   8; Entropie_7[26].Length= 5;
	Entropie_7[12].Code=  18; Entropie_7[12].Length= 6;
	Entropie_7[50].Code=  19; Entropie_7[50].Length= 6;
	Entropie_7[29].Code=  10; Entropie_7[29].Length= 5;
	Entropie_7[31].Code=  11; Entropie_7[31].Length= 5;
	Entropie_7[36].Code=  12; Entropie_7[36].Length= 5;
	Entropie_7[34].Code=  13; Entropie_7[34].Length= 5;
	Entropie_7[28].Code=  14; Entropie_7[28].Length= 5;
	Entropie_7[49].Code=  30; Entropie_7[49].Length= 6;
	Entropie_7[56].Code=  62; Entropie_7[56].Length= 7;
	Entropie_7[ 7].Code=  63; Entropie_7[ 7].Length= 7;
	Entropie_7[32].Code=  16; Entropie_7[32].Length= 5;
	Entropie_7[30].Code=  17; Entropie_7[30].Length= 5;
	Entropie_7[13].Code=  36; Entropie_7[13].Length= 6;
	Entropie_7[55].Code=  74; Entropie_7[55].Length= 7;
	Entropie_7[61].Code= 150; Entropie_7[61].Length= 8;
	Entropie_7[ 1].Code= 151; Entropie_7[ 1].Length= 8;
	Entropie_7[14].Code=  38; Entropie_7[14].Length= 6;
	Entropie_7[48].Code=  39; Entropie_7[48].Length= 6;
	Entropie_7[ 4].Code=  80; Entropie_7[ 4].Length= 7;
	Entropie_7[58].Code=  81; Entropie_7[58].Length= 7;
	Entropie_7[47].Code=  41; Entropie_7[47].Length= 6;
	Entropie_7[15].Code=  42; Entropie_7[15].Length= 6;
	Entropie_7[16].Code=  43; Entropie_7[16].Length= 6;
	Entropie_7[54].Code=  88; Entropie_7[54].Length= 7;
	Entropie_7[ 8].Code=  89; Entropie_7[ 8].Length= 7;
	Entropie_7[17].Code=  45; Entropie_7[17].Length= 6;
	Entropie_7[46].Code=  46; Entropie_7[46].Length= 6;
	Entropie_7[45].Code=  47; Entropie_7[45].Length= 6;
	Entropie_7[53].Code=  96; Entropie_7[53].Length= 7;
	Entropie_7[ 9].Code=  97; Entropie_7[ 9].Length= 7;
	Entropie_7[43].Code=  49; Entropie_7[43].Length= 6;
	Entropie_7[19].Code=  50; Entropie_7[19].Length= 6;
	Entropie_7[18].Code=  51; Entropie_7[18].Length= 6;
	Entropie_7[44].Code=  52; Entropie_7[44].Length= 6;
	Entropie_7[ 2].Code= 212; Entropie_7[ 2].Length= 8;
	Entropie_7[60].Code= 213; Entropie_7[60].Length= 8;
	Entropie_7[10].Code= 107; Entropie_7[10].Length= 7;
	Entropie_7[42].Code=  54; Entropie_7[42].Length= 6;
	Entropie_7[41].Code=  55; Entropie_7[41].Length= 6;
	Entropie_7[20].Code=  56; Entropie_7[20].Length= 6;
	Entropie_7[21].Code=  57; Entropie_7[21].Length= 6;
	Entropie_7[52].Code= 116; Entropie_7[52].Length= 7;
	Entropie_7[51].Code= 117; Entropie_7[51].Length= 7;
	Entropie_7[40].Code=  59; Entropie_7[40].Length= 6;
	Entropie_7[22].Code=  60; Entropie_7[22].Length= 6;
	Entropie_7[23].Code=  61; Entropie_7[23].Length= 6;
	Entropie_7[39].Code=  62; Entropie_7[39].Length= 6;
	Entropie_7[11].Code= 126; Entropie_7[11].Length= 7;
	Entropie_7[57].Code= 254; Entropie_7[57].Length= 8;
	Entropie_7[59].Code= 255; Entropie_7[59].Length= 8;

	/*
#ifdef DECODER
	//Neusortierung erster Quantisierer
	Entropie_1[0].Code= 1<<31; Entropie_1[0].Length= 1; Entropie_1[0].Value= 1;
	Entropie_1[1].Code= 1<<30; Entropie_1[1].Length= 2; Entropie_1[1].Value= 0;
	Entropie_1[2].Code= 0<<30; Entropie_1[2].Length= 2; Entropie_1[2].Value= 2;

	//Neusortierung zweiter Quantisierer
	Entropie_2[0].Code= 3<<30; Entropie_2[0].Length= 2; Entropie_2[0].Value= 2;
	Entropie_2[1].Code= 5<<29; Entropie_2[1].Length= 3; Entropie_2[1].Value= 4;
	Entropie_2[2].Code= 4<<29; Entropie_2[2].Length= 3; Entropie_2[2].Value= 0;
	Entropie_2[3].Code= 1<<30; Entropie_2[3].Length= 2; Entropie_2[3].Value= 3;
	Entropie_2[4].Code= 0<<30; Entropie_2[4].Length= 2; Entropie_2[4].Value= 1;

	//Neusortierung dritter Quantisierer
	Entropie_3[0].Code=  3<<30; Entropie_3[0].Length= 2; Entropie_3[0].Value= 3;
	Entropie_3[1].Code=  5<<29; Entropie_3[1].Length= 3; Entropie_3[1].Value= 1;
	Entropie_3[2].Code=  9<<28; Entropie_3[2].Length= 4; Entropie_3[2].Value= 5;
	Entropie_3[3].Code= 17<<27; Entropie_3[3].Length= 5; Entropie_3[3].Value= 0;
	Entropie_3[4].Code= 16<<27; Entropie_3[4].Length= 5; Entropie_3[4].Value= 6;
	Entropie_3[5].Code=  1<<30; Entropie_3[5].Length= 2; Entropie_3[5].Value= 2;
	Entropie_3[6].Code=  0<<30; Entropie_3[6].Length= 2; Entropie_3[6].Value= 4;

	//Neusortierung vierter Quantisierer
	Entropie_4[0].Code= 15<<28; Entropie_4[0].Length= 4; Entropie_4[0].Value= 7;
	Entropie_4[1].Code= 14<<28; Entropie_4[1].Length= 4; Entropie_4[1].Value= 1;
	Entropie_4[2].Code=  6<<29; Entropie_4[2].Length= 3; Entropie_4[2].Value= 5;
	Entropie_4[3].Code=  5<<29; Entropie_4[3].Length= 3; Entropie_4[3].Value= 3;
	Entropie_4[4].Code=  4<<29; Entropie_4[4].Length= 3; Entropie_4[4].Value= 6;
	Entropie_4[5].Code=  3<<29; Entropie_4[5].Length= 3; Entropie_4[5].Value= 2;
	Entropie_4[6].Code=  5<<28; Entropie_4[6].Length= 4; Entropie_4[6].Value= 0;
	Entropie_4[7].Code=  4<<28; Entropie_4[7].Length= 4; Entropie_4[7].Value= 8;
	Entropie_4[8].Code=  0<<30; Entropie_4[8].Length= 2; Entropie_4[8].Value= 4;

	//Neusortierung fuenfter Quantisierer
	Entropie_5[0 ].Code= 15<<28; Entropie_5[0 ].Length= 4; Entropie_5[0 ].Value=  5;
	Entropie_5[1 ].Code= 29<<27; Entropie_5[1 ].Length= 5; Entropie_5[1 ].Value=  2;
	Entropie_5[2 ].Code= 57<<26; Entropie_5[2 ].Length= 6; Entropie_5[2 ].Value=  0;
	Entropie_5[3 ].Code= 56<<26; Entropie_5[3 ].Length= 6; Entropie_5[3 ].Value= 14;
	Entropie_5[4 ].Code= 13<<28; Entropie_5[4 ].Length= 4; Entropie_5[4 ].Value=  4;
	Entropie_5[5 ].Code= 12<<28; Entropie_5[5 ].Length= 4; Entropie_5[5 ].Value= 10;
	Entropie_5[6 ].Code= 23<<27; Entropie_5[6 ].Length= 5; Entropie_5[6 ].Value=  1;
	Entropie_5[7 ].Code= 22<<27; Entropie_5[7 ].Length= 5; Entropie_5[7 ].Value= 13;
	Entropie_5[8 ].Code= 10<<28; Entropie_5[8 ].Length= 4; Entropie_5[8 ].Value= 11;
	Entropie_5[9 ].Code=  4<<29; Entropie_5[9 ].Length= 3; Entropie_5[9 ].Value=  7;
	Entropie_5[10].Code=  3<<29; Entropie_5[10].Length= 3; Entropie_5[10].Value=  8;
	Entropie_5[11].Code=  2<<29; Entropie_5[11].Length= 3; Entropie_5[11].Value=  6;
	Entropie_5[12].Code=  3<<28; Entropie_5[12].Length= 4; Entropie_5[12].Value=  3;
	Entropie_5[13].Code=  2<<28; Entropie_5[13].Length= 4; Entropie_5[13].Value= 12;
	Entropie_5[14].Code=  0<<29; Entropie_5[14].Length= 3; Entropie_5[14].Value=  9;

	//sechster Quantisierer
	Entropie_6[0 ].Code= 31<<27; Entropie_6[0 ].Length= 5; Entropie_6[0 ].Value= 11;
	Entropie_6[1 ].Code= 30<<27; Entropie_6[1 ].Length= 5; Entropie_6[1 ].Value= 20;
	Entropie_6[2 ].Code= 29<<27; Entropie_6[2 ].Length= 5; Entropie_6[2 ].Value= 10;
	Entropie_6[3 ].Code= 57<<26; Entropie_6[3 ].Length= 6; Entropie_6[3 ].Value= 25;
	Entropie_6[4 ].Code= 56<<26; Entropie_6[4 ].Length= 6; Entropie_6[4 ].Value=  5;
	Entropie_6[5 ].Code= 27<<27; Entropie_6[5 ].Length= 5; Entropie_6[5 ].Value=  9;
	Entropie_6[6 ].Code= 26<<27; Entropie_6[6 ].Length= 5; Entropie_6[6 ].Value= 21;
	Entropie_6[7 ].Code= 25<<27; Entropie_6[7 ].Length= 5; Entropie_6[7 ].Value= 22;
	Entropie_6[8 ].Code= 24<<27; Entropie_6[8 ].Length= 5; Entropie_6[8 ].Value=  8;
	Entropie_6[9 ].Code= 23<<27; Entropie_6[9 ].Length= 5; Entropie_6[9 ].Value=  7;
	Entropie_6[10].Code= 45<<26; Entropie_6[10].Length= 6; Entropie_6[10].Value=  4;
	Entropie_6[11].Code= 89<<25; Entropie_6[11].Length= 7; Entropie_6[11].Value= 29;
	Entropie_6[12].Code= 88<<25; Entropie_6[12].Length= 7; Entropie_6[12].Value=  1;
	Entropie_6[13].Code= 21<<27; Entropie_6[13].Length= 5; Entropie_6[13].Value= 23;
	Entropie_6[14].Code= 41<<26; Entropie_6[14].Length= 6; Entropie_6[14].Value= 26;
	Entropie_6[15].Code= 40<<26; Entropie_6[15].Length= 6; Entropie_6[15].Value=  2;
	Entropie_6[16].Code=  9<<28; Entropie_6[16].Length= 4; Entropie_6[16].Value= 15;
	Entropie_6[17].Code=  8<<28; Entropie_6[17].Length= 4; Entropie_6[17].Value= 16;
	Entropie_6[18].Code=  7<<28; Entropie_6[18].Length= 4; Entropie_6[18].Value= 14;
	Entropie_6[19].Code=  6<<28; Entropie_6[19].Length= 4; Entropie_6[19].Value= 18;
	Entropie_6[20].Code=  5<<28; Entropie_6[20].Length= 4; Entropie_6[20].Value= 17;
	Entropie_6[21].Code=  9<<27; Entropie_6[21].Length= 5; Entropie_6[21].Value=  6;
	Entropie_6[22].Code= 17<<26; Entropie_6[22].Length= 6; Entropie_6[22].Value= 27;
	Entropie_6[23].Code= 16<<26; Entropie_6[23].Length= 6; Entropie_6[23].Value= 28;
	Entropie_6[24].Code=  3<<28; Entropie_6[24].Length= 4; Entropie_6[24].Value= 12;
	Entropie_6[25].Code=  2<<28; Entropie_6[25].Length= 4; Entropie_6[25].Value= 13;
	Entropie_6[26].Code=  3<<27; Entropie_6[26].Length= 5; Entropie_6[26].Value= 24;
	Entropie_6[27].Code=  5<<26; Entropie_6[27].Length= 6; Entropie_6[27].Value=  3;
	Entropie_6[28].Code=  9<<25; Entropie_6[28].Length= 7; Entropie_6[28].Value= 30;
	Entropie_6[29].Code=  8<<25; Entropie_6[29].Length= 7; Entropie_6[29].Value=  0;
	Entropie_6[30].Code=  0<<28; Entropie_6[30].Length= 4; Entropie_6[30].Value= 19;

	//Neusortierung des Huffmancodes fuer den siebten Quantisierer
	{
		unsigned int c[63];
		unsigned int v[63];
		unsigned int l[63];
		unsigned int i;
		int sorted=0;

		for (i=0; i<63; ++i)
		{
			Entropie_7[i].Value = i;
			c[i] = Entropie_7[i].Code<<(32-Entropie_7[i].Length);
			v[i] = Entropie_7[i].Value;
			l[i] = Entropie_7[i].Length;
		}
		
		//bubble sort
		while (!sorted)
		{
			sorted = 1;
			for (i=1; i<63; ++i)
			{
				if (c[i]>c[i-1])
				{
					unsigned int tmp;
					tmp = v[i]; v[i] = v[i-1]; v[i-1] = tmp;
					tmp = c[i]; c[i] = c[i-1]; c[i-1] = tmp;
					tmp = l[i];	l[i] = l[i-1]; l[i-1] = tmp;
					sorted = 0;
				}
			}
		}

		for (i=0; i<63; ++i)
		{
			Entropie_7[i].Code = c[i];
			Entropie_7[i].Length = l[i];
			Entropie_7[i].Value = v[i];
		}
	}

	//Neusortierung des Huffmancodes fuer DSCF
	{
		unsigned int c[13];
		unsigned int v[13];
		unsigned int l[13];
		unsigned int i;
		int sorted=0;

		for (i=0; i<13; ++i)
		{
			DSCF_Entropie[i].Value = i;
			c[i] = DSCF_Entropie[i].Code<<(32-DSCF_Entropie[i].Length);
			v[i] = DSCF_Entropie[i].Value;
			l[i] = DSCF_Entropie[i].Length;
		}
		
		//bubble sort
		while (!sorted)
		{
			sorted = 1;
			for (i=1; i<13; ++i)
			{
				if (c[i]>c[i-1])
				{
					unsigned int tmp;
					tmp = v[i]; v[i] = v[i-1]; v[i-1] = tmp;
					tmp = c[i]; c[i] = c[i-1]; c[i-1] = tmp;
					tmp = l[i];	l[i] = l[i-1]; l[i-1] = tmp;
					sorted = 0;
				}
			}
		}

		for (i=0; i<13; ++i)
		{
			DSCF_Entropie[i].Code = c[i];
			DSCF_Entropie[i].Length = l[i];
			DSCF_Entropie[i].Value = v[i];
		}
	}

	//Neusortierung des Huffmancodes fuer Region_A
	{
		unsigned int c[16];
		unsigned int v[16];
		unsigned int l[16];
		unsigned int i;
		int sorted=0;

		for (i=0; i<16; ++i)
		{
			Region_A[i].Value = i;
			c[i] = Region_A[i].Code<<(32-Region_A[i].Length);
			v[i] = Region_A[i].Value;
			l[i] = Region_A[i].Length;
		}
		
		//bubble sort
		while (!sorted)
		{
			sorted = 1;
			for (i=1; i<16; ++i)
			{
				if (c[i]>c[i-1])
				{
					unsigned int tmp;
					tmp = v[i]; v[i] = v[i-1]; v[i-1] = tmp;
					tmp = c[i]; c[i] = c[i-1]; c[i-1] = tmp;
					tmp = l[i];	l[i] = l[i-1]; l[i-1] = tmp;
					sorted = 0;
				}
			}
		}

		for (i=0; i<16; ++i)
		{
			Region_A[i].Code = c[i];
			Region_A[i].Length = l[i];
			Region_A[i].Value = v[i];
		}
	}

	//Neusortierung des Huffmancodes fuer Region_B
	{
		unsigned int c[8];
		unsigned int v[8];
		unsigned int l[8];
		unsigned int i;
		int sorted=0;

		for (i=0; i<8; ++i)
		{
			Region_B[i].Value = i;
			c[i] = Region_B[i].Code<<(32-Region_B[i].Length);
			v[i] = Region_B[i].Value;
			l[i] = Region_B[i].Length;
		}
		
		//bubble sort
		while (!sorted)
		{
			sorted = 1;
			for (i=1; i<8; ++i)
			{
				if (c[i]>c[i-1])
				{
					unsigned int tmp;
					tmp = v[i]; v[i] = v[i-1]; v[i-1] = tmp;
					tmp = c[i]; c[i] = c[i-1]; c[i-1] = tmp;
					tmp = l[i];	l[i] = l[i-1]; l[i-1] = tmp;
					sorted = 0;
				}
			}
		}

		for (i=0; i<8; ++i)
		{
			Region_B[i].Code = c[i];
			Region_B[i].Length = l[i];
			Region_B[i].Value = v[i];
		}
	}

	//Neusortierung des Huffmancodes fuer Region_C
	{
		unsigned int c[4];
		unsigned int v[4];
		unsigned int l[4];
		unsigned int i;
		int sorted=0;

		for (i=0; i<4; ++i)
		{
			Region_C[i].Value = i;
			c[i] = Region_C[i].Code<<(32-Region_C[i].Length);
			v[i] = Region_C[i].Value;
			l[i] = Region_C[i].Length;
		}
		
		//bubble sort
		while (!sorted)
		{
			sorted = 1;
			for (i=1; i<4; ++i)
			{
				if (c[i]>c[i-1])
				{
					unsigned int tmp;
					tmp = v[i]; v[i] = v[i-1]; v[i-1] = tmp;
					tmp = c[i]; c[i] = c[i-1]; c[i-1] = tmp;
					tmp = l[i];	l[i] = l[i-1]; l[i-1] = tmp;
					sorted = 0;
				}
			}
		}

		for (i=0; i<4; ++i)
		{
			Region_C[i].Code = c[i];
			Region_C[i].Length = l[i];
			Region_C[i].Value = v[i];
		}
	}

	//Neusortierung des Huffmancodes fuer SCFI_Bundle
	{
		unsigned int c[8];
		unsigned int v[8];
		unsigned int l[8];
		unsigned int i;
		int sorted=0;

		for (i=0; i<8; ++i)
		{
			SCFI_Bundle[i].Value = i;
			c[i] = SCFI_Bundle[i].Code<<(32-SCFI_Bundle[i].Length);
			v[i] = SCFI_Bundle[i].Value;
			l[i] = SCFI_Bundle[i].Length;
		}

		//bubble sort
		while (!sorted)
		{
			sorted = 1;
			for (i=1; i<8; ++i)
			{
				if (c[i]>c[i-1])
				{
					unsigned int tmp;
					tmp = v[i]; v[i] = v[i-1]; v[i-1] = tmp;
					tmp = c[i]; c[i] = c[i-1]; c[i-1] = tmp;
					tmp = l[i];	l[i] = l[i-1]; l[i-1] = tmp;
					sorted = 0;
				}
			}
		}

		for (i=0; i<8; ++i)
		{
			SCFI_Bundle[i].Code = c[i];
			SCFI_Bundle[i].Length = l[i];
			SCFI_Bundle[i].Value = v[i];
		}
	}
#endif //DECODER
	*/
}

