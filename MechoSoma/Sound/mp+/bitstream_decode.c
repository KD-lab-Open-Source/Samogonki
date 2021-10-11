#include "bitstream_decode.h"

//read desired number of bits out of the bitstream
unsigned int Bitstream_read(int bits)
{
	unsigned int out;
	int new_pos = pos+bits;

	//Erhoehe BitsRead um bits
	BitsRead += bits;

	if      (new_pos<32)
	{
		out = (dword>>(32-new_pos)) & mask[bits];
		pos = new_pos;
		return out;
	}
	else if (new_pos>32) 
	{
		out = (dword<<pos)>>(32-bits);
		dword=Speicher[++Zaehler];
		out += (dword>>(64-new_pos));
		pos = new_pos-32;
		return out;
	}
	else /*if (new_pos==32)*/
	{
		out = dword & mask[bits];
		dword=Speicher[++Zaehler];
		pos = 0;
		return out;
	}
	return 1;
}

//peform preview of next bits in bitstream
unsigned int Bitstream_preview(int bits)
{
	unsigned int out;
	unsigned int preview_dword=dword;
	unsigned short preview_Zaehler=Zaehler;
	int preview_pos=pos;
	int preview_new_pos=pos+bits;

	if      (preview_new_pos<32) return (preview_dword>>(32-preview_new_pos)) & mask[bits];
	else if (preview_new_pos>32) 
	{
		out = (preview_dword<<preview_pos)>>(32-bits);
		preview_dword=Speicher[++preview_Zaehler];
		return out+(preview_dword>>(64-preview_new_pos));
	}
	else /*preview_new_pos==32*/ return preview_dword & mask[bits];
	return 1;
}

//decode huffman
unsigned int Huffman_Decode(HuffmanTyp *Table)
{
	unsigned int code;
	int k=-1;

	code = Bitstream_preview(14)<<18;
	while (1) {if (Table[++k].Code<=code) break;}

	//Setze Bitstromposition ohne dummy-read
	if ((pos += Table[k].Length)>=32)
	{
		pos -= 32;
		dword=Speicher[++Zaehler];
	}

	//Erhoehe BitsRead um Table[k].Length
	BitsRead += Table[k].Length;

	return Table[k].Value;
}

//faster huffman through previewing less bits
unsigned int Huffman_Decode_fast(HuffmanTyp *Table)
{
	unsigned int code;
	int k=-1;

	code = Bitstream_preview(8)<<24;
	while (1) {if (Table[++k].Code<=code) break;}

	//Setze Bitstromposition ohne dummy-read
	if ((pos += Table[k].Length)>=32)
	{
		pos -= 32;
		dword=Speicher[++Zaehler];
	}

	//Erhoehe BitsRead um Table[k].Length
	BitsRead += Table[k].Length;

	return Table[k].Value;
}

//decode SCFI-bundle (sv4,5,6)
BundleTyp SCFI_Bundle_read()
{
	BundleTyp Word={0,0};
	unsigned int code;
	int k=-1;
	
	code = Bitstream_preview(6)<<26;
	while (1) {if (SCFI_Bundle[++k].Code<=code) break;}

	//Setze Bitstromposition ohne dummy-read
	if ((pos += SCFI_Bundle[k].Length)>=32)
	{
		pos -= 32;
		dword=Speicher[++Zaehler];
	}

	//Erhoehe BitsRead um SCFI_Bundle[k].Length
	BitsRead += SCFI_Bundle[k].Length;
	
	Word.SCFI = SCFI_Bundle[k].Value>>1;
	Word.DSCF = SCFI_Bundle[k].Value &1;

	return Word;
}

