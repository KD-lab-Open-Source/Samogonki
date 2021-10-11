#ifndef _huff_old_h_
#define _huff_old_h_

/* K O N S T A N T E N */
#ifndef RESORT_DECL
#define LARGEST_TABLE 63
#endif

/* T Y P E N D E F I N I T I O N */
#ifndef HUFFTYP
  typedef struct Huffmancode{unsigned int Code; unsigned int Length; unsigned int Value;} HuffmanTyp;
#define HUFFTYP
#endif

/* P R O Z E D U R E N */
#ifndef RESORT_DECL
void Resort_HuffTables(unsigned int, HuffmanTyp*);
#define RESORT_DECL
#endif
void Huffman_SV6_Encoder();
void Huffman_SV6_Decoder();

/* V A R I A B L E N */
HuffmanTyp Entropie_1[ 3],Entropie_2[ 5],Entropie_3[ 7],Entropie_4[9];
HuffmanTyp Entropie_5[15],Entropie_6[31],Entropie_7[63];
HuffmanTyp SCFI_Bundle[8],DSCF_Entropie[13];						
HuffmanTyp Region_A[16],Region_B[8],Region_C[4];

#endif