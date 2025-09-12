
/* ---------------------------- INCLUDE SECTION ----------------------------- */

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>

#include "aci_parser.h"
#include "aci_define.h"

#include "script_err.h"

/* ----------------------------- STRUCT SECTION ----------------------------- */

struct scrString
{
	int startIndex;
	int endIndex;
	int strLen;

	char* data;

	scrString(void);
	~scrString(void);
};

/* ----------------------------- EXTERN SECTION ----------------------------- */

extern int scrVerbose;

std::function<const std::string_view (const std::string_view)> getLocalizedKey;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

int preprocessScript(const char* fname,int& outsz,char** outbuf);
void prepareBuffer(char* p,int sz);

void stripComments(char* ptr,int sz);
void processIncludes(char** ptr,int& sz);

int processDefines(char** ptr,int& sz);
void prepareDefines(char* ptr,int sz);
int replaceDefines(void);

void initStringData(char* ptr,int sz,int mode = 1);
void freeStringData(void);

int getStringNumber(int index);

int mth_isMathOp(int v);

/* --------------------------- DEFINITION SECTION --------------------------- */

static const char* inc_str =  "#include";
static const char* def_str =  "#define";
static const char* text_str = "#text";
static const char* end_str =  "#end";

int numStrings;
scrString** strList;
xtList<scrDefineData>* defineList;

int bufferSize = 0;

int preprocessScript(const char* fname,int& outsz,char** outbuf)
{
	int i,idx,idx1,flag,sz,sz1,q_flag;
	char* buf,*buf1;

	XStream fh(fname,XS_IN);
	sz = fh.size();
	buf = new char[sz + 2];
	fh.read(buf,sz);
	fh.close();
	buf[sz] = '\r';
	buf[sz + 1] = '\n';
	sz += 2;

	stripComments(buf,sz);
	if(scrVerbose)
		printf("Processing includes\n");
	processIncludes(&buf,sz);
	if(scrVerbose)
		printf("Preparing defines\n");
	if(!processDefines(&buf,sz)) return 0;

	sz1 = 0;
	q_flag = 0;
	for(i = 0; i < numStrings; i ++){
		flag = 0;
		for(idx = 0; idx < strList[i] -> strLen; idx ++){
			if(q_flag)
				flag = 1;

			if(strList[i] -> data[idx] == '\"')
				q_flag ^= 1;

			if(!flag && strList[i] -> data[idx] && !isspace(strList[i] -> data[idx]))
				flag = 1;
		}
		if(flag){
			sz1 += strList[i] -> strLen;
		}
	}
	buf1 = new char[sz1 + 2];
	idx1 = 0;
	q_flag = 0;
	for(i = 0; i < numStrings; i ++){
		flag = 0;
		for(idx = 0; idx < strList[i] -> strLen; idx ++){
			if(q_flag)
				flag = 1;

			if(strList[i] -> data[idx] == '\"')
				q_flag ^= 1;

			if(!flag && strList[i] -> data[idx] && !isspace(strList[i] -> data[idx]))
				flag = 1;
		}
		if(flag){
			memcpy(buf1 + idx1,strList[i] -> data,strList[i] -> strLen);
			idx1 += strList[i] -> strLen;
		}
	}
	freeStringData();
	delete buf;

	sz = sz1 + 2;
	buf = buf1;
	buf[sz1] = '\r';
	buf[sz1 + 1] = '\n';

	initStringData(buf,sz);

	sz1 = 0;
	q_flag = 0;
	for(i = 0; i < numStrings; i ++){
		flag = 0;
		for(idx = 0; idx < strList[i] -> strLen; idx ++){
			if(!flag && q_flag){
				sz1 += strList[i] -> strLen;
				flag = 1;
			}

			if(strList[i] -> data[idx] == '\"')
				q_flag ^= 1;

			if(!flag && strList[i] -> data[idx] && !isspace(strList[i] -> data[idx])){
				sz1 += strList[i] -> strLen - idx;
				flag = 1;
			}
		}
	}
	idx1 = 0;
	buf1 = new char[sz1];
	q_flag = 0;
	for(i = 0; i < numStrings; i ++){
		flag = 0;
		for(idx = 0; idx < strList[i] -> strLen; idx ++){
			if(!flag && q_flag){
				memcpy(buf1 + idx1,strList[i] -> data + idx,strList[i] -> strLen - idx);
				idx1 += strList[i] -> strLen - idx;
				flag = 1;
			}

			if(strList[i] -> data[idx] == '\"')
				q_flag ^= 1;

			if(!flag && strList[i] -> data[idx] && !isspace(strList[i] -> data[idx])){
				memcpy(buf1 + idx1,strList[i] -> data + idx,strList[i] -> strLen - idx);
				idx1 += strList[i] -> strLen - idx;
				flag = 1;
			}
		}
	}
	outbuf[0] = buf1;
	outsz = sz1;

	bufferSize = sz1;

	freeStringData();
	delete buf;

	return 1;
}

void stripComments(char* ptr,int sz)
{
	int flag,index;

	index = 0;
	while(index < sz){
		if(ptr[index] == '\"'){
			index ++;
			if(index < sz - 1){
				while(ptr[index] != '\"'){
					index ++;
					if(index >= sz) return;
				}
				index ++;
			}
		}
		else {
			if(ptr[index] == '/'){
				if(index < sz - 1){
					if(ptr[index + 1] == '/'){
						while(ptr[index] != '\n'){
							ptr[index] = ' ';
							index ++;
							if(index >= sz) return;
						}
					}
					else {
						if(ptr[index + 1] == '*'){
							ptr[index + 1] = ' ';
							flag = 1;
							while(flag){
								if(ptr[index] == '*' && ptr[index + 1] == '/'){
									flag = 0;
								}
								else {
									ptr[index] = ' ';
									index ++;
									if(index >= sz - 1) return;
								}
							}
							ptr[index] = ptr[index + 1] = ' ';
						}
						else
							index ++;
					}
				}
			}
			else
				index ++;
		}
	}
}

void processIncludes(char** buf,int& sz)
{
	char* p,*p1,*tmp,*ptr;
	XStream fh;

	int flag = 1,count = 0,index,index1,inc_len,sz1;

	inc_len = strlen(inc_str);

	ptr = *buf;

	while(flag){
		flag = 0;
		index = 0;
		while(index < sz - inc_len){
			if(ptr[index] == '\"'){
				index ++;
				if(index < sz - 1){
					while(ptr[index] != '\"'){
						index ++;
						if(index >= sz - inc_len) return;
					}
					index ++;
				}
			}
			else {
				if(ptr[index] == '#'){
					if(!strncmp(ptr + index,inc_str,inc_len)){
						flag = 1;
						index1 = index;
						index += inc_len;
						while(ptr[index] != '\"'){
							index ++;
							if(index >= sz) return;
						}
						index ++;
						p = ptr + index;
						while(ptr[index] != '\"'){
							index ++;
							if(index >= sz) return;
						}
						ptr[index] = 0;
						if(scrVerbose)
							printf("Loading file -> %s\n",p);
						fh.open(p,XS_IN);
						sz1 = fh.size();
						p1 = new char[sz1];
						fh.read(p1,sz1);
						stripComments(p1,sz1);
						fh.close();

						tmp = new char[sz + sz1 - (index - index1)];
						memcpy(tmp,ptr,index1);
						memcpy(tmp + index1,p1,sz1);
						memcpy(tmp + index1 + sz1,ptr + index,sz - index);

						delete p1;
						delete ptr;

						*buf = tmp;
						ptr = tmp;

						sz = sz + sz1 - (index - index1);
						break;
					}
					else
						index ++;
				}
				else
					index ++;
			}
		}
	}
}

int processDefines(char** ptr,int& sz)
{
	scrDefineData* p,*p1;
	defineList = new xtList<scrDefineData>;
	prepareDefines(*ptr,sz);

	if(scrVerbose)
		printf("Sorting defines\n");

	p = defineList -> first();
	while(p){
		p1 = p -> next;
		while(p1){
			if(p1 -> srcLen == p -> srcLen && !strncmp(p -> srcData,p1 -> srcData,p -> srcLen)){
				p -> endIndex = p1 -> startIndex;
				break;
			}
			p1 = p1 -> next;
		}
		p = p -> next;
	}

	initStringData(*ptr,sz);
	if(!replaceDefines()) return 0;

	p = defineList -> first();
	while(p){
		defineList -> remove(p);
		delete p;
		p = defineList -> first();
	}
	delete defineList;

	return 1;
}

void prepareDefines(char* ptr,int sz)
{
	scrDefineData* p;
	int index,def_len,text_len;

	def_len = strlen(def_str);
	text_len = strlen(text_str);

	index = 0;
	while(index < sz - def_len){
		if(ptr[index] == '\"'){
			index ++;
			if(index < sz - 1){
				while(ptr[index] != '\"'){
					index ++;
					if(index >= sz - def_len) return;
				}
				index ++;
			}
		}
		else {
			if(ptr[index] == '#'){
				if(!strncmp(ptr + index,def_str,def_len)){
					p = new scrDefineData;
					p -> init(ptr + index,sz - index);
					p -> startIndex = index;
					p -> endIndex = sz;
					if(p -> srcLen && p -> destLen){
						defineList -> append(p);
					}
					if(scrVerbose)
						printf("Define number -> %d\r",defineList -> size());
				}
				if(!strncmp(ptr + index,text_str,text_len)){
					p = new scrDefineData;
					p -> flags |= DEF_TEXT;
					p -> init(ptr + index,sz - index);
					p -> startIndex = index;
					p -> endIndex = sz;
					if(p -> srcLen && p -> destLen){
						defineList -> append(p);
					}
					if(scrVerbose)
						printf("Define number -> %d\r",defineList -> size());
				}
				index ++;
			}
			else
				index ++;
		}
	}
	if(scrVerbose)
		printf("\n");
}

scrDefineData::scrDefineData(void)
{
	list = NULL;

	flags = 0;
	startIndex = endIndex = 0;
	srcLen = destLen = 0;
	srcData = destData = NULL;
}

scrDefineData::~scrDefineData(void)
{
	if(srcData) delete srcData;
	if(destData) delete destData;
}

void scrDefineData::init(char* ptr,int sz)
{
	int index0,index1,index2,next_flag;

	if(!(flags & DEF_TEXT)){
		index0 = strlen(def_str);
		while(isspace(ptr[index0])){
			index0 ++;
			if(index0 >= sz) break;
		}
		if(index0 >= sz) return;

		index1 = index0;
		while(!isspace(ptr[index1])){
			index1 ++;
			if(index1 >= sz) break;
		}
		if(index1 >= sz) return;

		srcLen = index1 - index0;
		srcData = new char[srcLen];
		memcpy(srcData,ptr + index0,srcLen);

		index0 = index1;

		next_flag = 1;
		while(next_flag){
			next_flag = 0;
			while(ptr[index1] != '\n'){
				index1 ++;
				if(index1 >= sz) break;
			}
			if(index1 >= sz) return;

			index2 = index1;
			for(index2 = index1; index2 > index0; index2 --){
				if(!isspace(ptr[index2])){
					if(ptr[index2] == '\\'){
						ptr[index2] = ' ';
						next_flag = 1;
						index1 ++;
					}
					break;
				}
			}
		}

		const auto localized_key = getLocalizedKey ? getLocalizedKey(std::string_view(srcData, srcLen)) : "";
		if (!localized_key.empty())
		{
			destData = new char[localized_key.size()];
			destLen = localized_key.size();
			memcpy(destData,localized_key.data(),localized_key.size());
		}
		else
		{
			while(isspace(ptr[index0])){
				index0 ++;
				if(index0 >= sz) break;
			}
			if(index0 >= sz) return;

			destLen = index1 - index0 + 1;
			destData = new char[destLen];
			memcpy(destData,ptr + index0,destLen);
		}
	}
	else {
		index0 = strlen(text_str);
		while(isspace(ptr[index0])){
			index0 ++;
			if(index0 >= sz) break;
		}
		if(index0 >= sz) return;

		index1 = index0;
		while(!isspace(ptr[index1])){
			index1 ++;
			if(index1 >= sz) break;
		}
		if(index1 >= sz) return;

		srcLen = index1 - index0;
		srcData = new char[srcLen];
		memcpy(srcData,ptr + index0,srcLen);

		while(ptr[index1] != '\n'){
			index1 ++;
			if(index1 >= sz) break;
		}
		if(++index1 >= sz) return;

		index0 = index1;

		next_flag = 1;
		while(next_flag){
			while(ptr[index1] != '\n'){
				if(++index1 >= sz) return;
			}
			index1 += 1;
			if(index1 >= sz) return;

			if(ptr[index1] == '#' && !strncmp(ptr + index1,end_str,strlen(end_str))){
				memset(ptr + index1,' ',strlen(end_str));
				next_flag = 0;

				index1 --;

				while(ptr[index1] == '\n' || ptr[index1] == '\r')
					ptr[index1 --] = ' ';

				index1 ++;
			}
		}

		const auto localized_key = getLocalizedKey ? getLocalizedKey(std::string_view(srcData, srcLen)) : "";
		if (!localized_key.empty())
		{
			destData = new char[localized_key.size()];
			destLen = localized_key.size();
			memcpy(destData,localized_key.data(),localized_key.size());
		}
		else
		{
			if(ptr[index0] != '\"'){
				destLen = index1 - index0 + 2;
				destData = new char[destLen];
				destData[0] = '\"';
				memcpy(destData + 1,ptr + index0,destLen - 1);
				destData[destLen - 1] = '\"';
			}
			else {
				destLen = index1 - index0 + 1;
				destData = new char[destLen];
				memcpy(destData,ptr + index0,destLen);
			}
		}
	}

	memset(ptr,' ',index1);
}

int replaceDefines(void)
{
	int i,idx,idx1,start_flag,end_flag,define_flag;

	char* ptr,*tmp;

	scrDefineData* p;

	if(scrVerbose)
		printf("Replacing defines\n");

	idx = 0;
	p = defineList -> first();
	while(p){
		idx ++;
		if(scrVerbose)
			printf("Step -> %d/%d\r",idx,defineList -> size());
		start_flag = end_flag = 0;
		for(i = 0; i < numStrings; i ++){
			if(!start_flag){
				if(p -> startIndex >= strList[i] -> startIndex && p -> startIndex < strList[i] -> endIndex){
					p -> startIndex = i;
					start_flag = 1;
				}
			}
			if(!end_flag){
				if(p -> endIndex >= strList[i] -> startIndex && p -> endIndex < strList[i] -> endIndex){
					p -> endIndex = i;
					end_flag = 1;
				}
			}
			if(start_flag && end_flag) break;
		}
		p = p -> next;
	}
	if(scrVerbose)
		printf("\n");
	for(i = 0; i < numStrings; i ++){
		if(scrVerbose)
			printf("String -> %d/%d\r",i + 1,numStrings);
		for(idx = 0; idx < strList[i] -> strLen; idx ++){
			if(strList[i] -> data[idx] == '$'){
				p = defineList -> first();
				idx1 = idx;
				define_flag = 0;

				while(!isspace(strList[i] -> data[idx1]) && !mth_isMathOp(strList[i] -> data[idx1]) && strList[i] -> data[idx1] != '\"'){
					idx1 ++;
					if(idx1 >= strList[i] -> strLen - 1) break;
				}
				idx1 -= idx + 1;
				while(p){
					if(p -> startIndex <= i && p -> endIndex > i && p -> srcLen == idx1){
						if(!strncmp(p -> srcData,strList[i] -> data + idx + 1,p -> srcLen)){
							define_flag = 1;
							ptr = new char[strList[i] -> strLen + p -> destLen - p -> srcLen - 1];
							if(idx)
								memcpy(ptr,strList[i] -> data,idx);

							memcpy(ptr + idx,p -> destData,p -> destLen);
							memcpy(ptr + idx + p -> destLen,strList[i] -> data + idx + p -> srcLen + 1,strList[i] -> strLen - (idx + p -> srcLen + 1));

							delete strList[i] -> data;
							strList[i] -> data = ptr;
							strList[i] -> strLen = strList[i] -> strLen + p -> destLen - p -> srcLen - 1;
							idx = -1;
							break;
						}
					}
					p = p -> next;
				}
				if(!define_flag){
					tmp = new char[256];
					memset(tmp,0,256);
					if(strlen(strList[i] -> data + idx) >= 256)
						memcpy(tmp,strList[i] -> data + idx,255);
					else
						memcpy(tmp,strList[i] -> data + idx,strlen(strList[i] -> data + idx));

//					ErrH.Abort("Unknown #define:",XERR_USER,i,tmp);
					printf("Error: Unknown #define: %d\n %s",i,tmp);
					scriptError("Error: Unknown #define: %d\n %s",i,tmp);
					delete tmp;
					return 0;
				}
			}
		}
	}

	return 1;
}


void initStringData(char* ptr,int sz,int mode)
{
	int i,num,index0;
	numStrings = 1;

	if(scrVerbose)
		printf("Strings data init\n");

	for(i = 0; i < sz; i ++){
		if(ptr[i] == '\n') numStrings ++;
	}

	num = 0;
	index0 = 0;
	strList = new scrString*[numStrings];
	for(i = 0; i < numStrings; i ++)
		strList[i] = new scrString;

	for(i = 0; i < sz; i ++){
		if(ptr[i] == '\n'){
			strList[num] -> startIndex = index0;
			strList[num] -> endIndex = i;
			strList[num] -> strLen = i - index0 + 1;

			if(mode){
				strList[num] -> data = new char[strList[num] -> strLen];
				memcpy(strList[num] -> data,ptr + index0,strList[num] -> strLen);
			}

			index0 = i + 1;
			num ++;
		}
	}
	if(index0 < sz){
		strList[num] -> startIndex = index0;
		strList[num] -> endIndex = sz - 1;
		strList[num] -> strLen = sz - index0;
		if(mode){
			strList[num] -> data = new char[strList[num] -> strLen];
			memcpy(strList[num] -> data,ptr + index0,strList[num] -> strLen);
		}
	}
	if(scrVerbose)
		printf("\n");
}

void freeStringData(void)
{
	int i;
	for(i = 0; i < numStrings; i ++)
		delete strList[i];
	delete strList;
}

scrString::scrString(void)
{
	startIndex = endIndex = strLen = 0;
	data = NULL;
}

scrString::~scrString(void)
{
	if(data)
		delete data;
}

void prepareBuffer(char* ptr,int sz)
{
	int index;

	index = 0;
	while(index < sz){
		if(ptr[index] == '\"'){
			index ++;
			if(index < sz - 1){
				while(ptr[index] != '\"'){
					index ++;
					if(index >= sz) break;
				}
				if(index >= sz) break;
				index ++;
			}
		}
		else {
			if(isspace(ptr[index])) ptr[index] = 0;
			index ++;
		}
	}
	for(index = 0; index < sz; index ++){
		if(ptr[index] == '\"') ptr[index] = 0;
	}
}

int getStringNumber(int index)
{
	int i;
	for(i = 0; i < numStrings; i ++){
		if(index >= strList[i] -> startIndex && index < strList[i] -> endIndex){
			return i + 1;
		}
	}
	return 0;
}

