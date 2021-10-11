
#ifndef __KEYS_H__
#define __KEYS_H__

#define VK_MOUSE_LBUTTON	-1
#define VK_MOUSE_RBUTTON	-2
#define VK_MOUSE_MOVE		-3

#define KBD_CUR_KEY_PRESSED		0x01
#define KBD_CUR_KEY_UNPRESSED	0x02

#define	KBD_MAX_SIZE			500

#define	KBD_CTRL_PRESSED	0x01
#define	KBD_SHIFT_PRESSED	0x02
#define	KBD_ENABLE			0x04

#define KBD_UNPRESS_FLAG	0x400000

struct KeyBuffer
{
	int* table;
	char* state_table;

	int size;
	int first_index;
	int last_index;
	int flag;

	int size_;
	int first_index_;
	int last_index_;

	int cur_state;

	void clear(void);
	void quant(void);
	void put(int key,int state);
	int get(void);

	void backup(void){ 
		size_ = size;
		first_index_ = first_index;
		last_index_ = last_index;
	}

	void restore(void){ 
		size = size_;
		first_index = first_index_;
		last_index = last_index_;
	}

	void copy(KeyBuffer* p);

	KeyBuffer(void);
};

void key(int);
void unpress_key(int);

void KBD_init(void);

extern KeyBuffer* KeyBuf;

#endif // __KEYS_H__

