
/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include <StdAfx.h>
//#include "global.h"
#pragma hdrstop

#include "KEYS.H"

#include "mch_common.h" // For far target

/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */
/* --------------------------- DEFINITION SECTION --------------------------- */

KeyBuffer* KeyBuf = 0;

void KBD_init(void)
{
	if(!KeyBuf)
		KeyBuf = new KeyBuffer;

	XKey.init(reinterpret_cast<void*>(key),reinterpret_cast<void*>(unpress_key));
}

void key(int key)
{
	if(KeyBuf -> flag & KBD_ENABLE)
		KeyBuf -> put(key,KBD_CUR_KEY_PRESSED);
}

void unpress_key(int key)
{
	KeyBuf -> put(key,KBD_CUR_KEY_UNPRESSED);
}

KeyBuffer::KeyBuffer(void)
{
	table = new int[KBD_MAX_SIZE];
	state_table = new char[KBD_MAX_SIZE];

	clear();
	flag = KBD_ENABLE;
}

void KeyBuffer::put(int key,int state)
{
	if(size < KBD_MAX_SIZE){
		table[last_index] = key;
		state_table[last_index] = state;
		last_index ++;
		if(last_index >= KBD_MAX_SIZE)
			last_index = 0;
		size ++;
	}
}

int KeyBuffer::get(void)
{
	int key;
	if(size){
		key = table[first_index];
		cur_state = state_table[first_index];

		first_index ++;
		if(first_index >= KBD_MAX_SIZE)
			first_index = 0;

		if(cur_state == KBD_CUR_KEY_UNPRESSED)
			key |= KBD_UNPRESS_FLAG;

		size --;

		return key;
	}
	return -1;
}

void KeyBuffer::clear(void)
{
	size = 0;
	first_index = 0;
	last_index = 0;
	cur_state = 0;
}

void KeyBuffer::quant(void)
{
	if(XKey.Pressed(VK_CONTROL))
		flag |= KBD_CTRL_PRESSED;
	else
		flag &= ~KBD_CTRL_PRESSED;

	if(XKey.Pressed(VK_SHIFT))
		flag |= KBD_SHIFT_PRESSED;
	else
		flag &= ~KBD_SHIFT_PRESSED;
}

void KeyBuffer::copy(KeyBuffer* p)
{
	p->size = size;
	p->table = table;
	p->state_table = state_table;
	p->flag = flag;
	p->first_index = first_index;
	p->last_index = last_index;
}
