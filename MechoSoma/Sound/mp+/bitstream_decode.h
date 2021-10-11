#ifndef _bitstream_decode_h_
#define _bitstream_decode_h_

/* T Y P E N D E F I N I T O N */
#ifndef HUFFTYP
  typedef struct Huffmancode{unsigned int Code; unsigned int Length; unsigned int Value;} HuffmanTyp;
#define HUFFTYP
#endif

typedef struct {unsigned int SCFI; unsigned int DSCF;} BundleTyp;

/* K O N S T A N T E N */
static unsigned int mask[33] = {0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535,131071,262143,
						524287,1048575,2097151,4194303,8388607,16777215,33554431,67108863,134217727,268435455,
						536870911,1073741823,2147483647,4294967295};

/* V A R I A B L E N */
unsigned int Speicher[65536];	//enthaelt den Lese-Puffer
unsigned int dword=0;			//32Bit-Wort fuer Bitstrom-I/O
int pos=0;						//Position im aktuell decodierten 32Bit-Wort
unsigned short Zaehler = 0;		//akutelle Position im Lese-Puffer
int BITS = 0;					//Zaehler fuer Anzahl geschriebener Bits im Bitstrom
int BitsRead = 0;				//Zaehler fuer Anzahl decodierter Bits 

/* P R O Z E D U R E N */
unsigned int Bitstream_read(int);
unsigned int Bitstream_preview(int);
unsigned int Huffman_Decode(HuffmanTyp*); 
unsigned int Huffman_Decode_fast(HuffmanTyp*);
BundleTyp SCFI_Bundle_read();

#endif
