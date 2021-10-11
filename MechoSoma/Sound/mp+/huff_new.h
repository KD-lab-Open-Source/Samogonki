#ifndef _huff_new_h_
#define _huff_new_h

/* T Y P E N D E F I N I T I O N */
#ifndef HUFFTYP
  typedef struct Huffmancode{unsigned int Code; unsigned int Length; unsigned int Value;} HuffmanTyp;
  #define HUFFTYP
#endif

/* K O N S T A N T E N */
#ifndef RESORT_DECL
#define LARGEST_TABLE 63
#endif

/* P R O Z E D U R E N */
#ifndef RESORT_DECL
void Resort_HuffTables(unsigned int, HuffmanTyp*);
#define RESORT_DECL
#endif
void Huffman_SV7_Encoder();
void Huffman_SV7_Decoder();

/* V A R I A B L E N */
HuffmanTyp HuffHdr[10];
HuffmanTyp HuffSCFI[4];
HuffmanTyp HuffDSCF[16];
HuffmanTyp HuffQ1[2][27];
HuffmanTyp HuffQ2[2][25];
HuffmanTyp HuffQ3[2][ 7];
HuffmanTyp HuffQ4[2][ 9];
HuffmanTyp HuffQ5[2][15];
HuffmanTyp HuffQ6[2][31];
HuffmanTyp HuffQ7[2][63];

#endif



