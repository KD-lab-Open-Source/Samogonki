/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "iText.h"
#include "scr_defs.h"
#include "aci_parser.h"

#include "mch_common.h" // For far target

/* ----------------------------- STRUCT SECTION ----------------------------- */

class iTextData 
{
	char* text;
public:
	int ID;
	void* list;
	iTextData* prev;
	iTextData* next;

	char* Text(void) const { return text; }

	iTextData(int id,const char* p);
	~iTextData(void);
};

/* ----------------------------- EXTERN SECTION ----------------------------- */
/* --------------------------- PROTOTYPE SECTION ---------------------------- */

int acsOpenResource(char* fname,XStream& fh,int res_flag = -1);

void iTextInit(void);
void iTextFinit(void);

/* --------------------------- DEFINITION SECTION --------------------------- */

xtList<iTextData> iTxtList;
static char* iEmptyTxt = "";

iTextData::iTextData(int id,const char* p)
{
	ID = id;
	text = strdup(p);
	list = NULL;
}

iTextData::~iTextData(void)
{
	if(text) free(text);
}

void iTextInit(void)
{
	XStream fh;

	iTextData* txt;
	scrDataBlock* root,*p,*p1;

	acsOpenResource("iText.scb",fh);
	root = loadScript(fh);

	p = root -> nextLevel -> first();
	while(p){
		if(p -> ID == ASCR_iTEXT){
			p1 = p -> nextLevel -> first();
			while(p1){
				switch(p1 -> ID){
					case ASCR_STRING:
						txt = new iTextData(p -> i_dataPtr[0],p1 -> c_dataPtr);
						iTxtList.append(txt);
						break;
				}
				p1 = p1 -> next;
			}
		}
		p = p -> next;
	}

	delete root;
}

void iTextFinit(void)
{
	iTextData* p;

	p = iTxtList.first();
	while(p){
		iTxtList.remove(p);
		delete p;
		p = iTxtList.first();
	}
}

char* iGetText(int id)
{
	iTextData* p = iTxtList.search(id);

	if(p)
		return p -> Text();

	return iEmptyTxt;
}
