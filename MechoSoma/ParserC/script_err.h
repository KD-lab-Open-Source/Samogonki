#ifndef __SCRIPT_ERR_H__
#define __SCRIPT_ERR_H__

void scriptSetErrorMode(int mode);
void scriptSetErrorOutHandler(void (*hfnc)(const char*));

void scriptError(const char* msg);
void scriptError(const char* msg,int code,const char* text = NULL);


#endif /* __SCRIPT_ERR_H__ */
