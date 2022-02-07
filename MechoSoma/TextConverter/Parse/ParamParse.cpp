#include "stdafx.h"
#include "ParamParse.h"
#include <stdio.h>

static char error[2048];

void ParseParam::SkipSpace()
{
Retry:
	while(IsSpace(*cur))cur++;
	if(*cur=='/' && cur[1]=='/')
	{
		while(*cur && *cur!=0xD && *cur!=0xA)
			cur++;

		while(*cur && *cur==0xD && *cur==0xA)
			cur++;

		if(cur)
			goto Retry;
	}

}


ParseParam::ParseParam(char* _text,ParamBlock* _pRules,
					   NonStaticByConst _pGetIntByConstNonstatic)
:text(_text),line(1)
{
	b_char.Set('a','z');
	b_char.Set('A','Z');
	b_char.Set('_');

	space.Set((BYTE)32);
	space.Set((BYTE)9);
	space.Set((BYTE)10);
	space.Set((BYTE)13);

	cur=text;

	pGetIntByConst=NULL;
	pGetIn=NULL;
	pGetType=NULL;

	if(_pRules)
	{
		OneParam* op;
		op=_pRules->Find("const");
		pGetIntByConst=op?op->GetBlock():NULL;

		op=_pRules->Find("in");
		pGetIn=op?op->GetBlock():NULL;

		op=_pRules->Find("type");
		pGetType=op?op->GetBlock():NULL;
	}

	pGetIntByConstNonstatic=_pGetIntByConstNonstatic;
}

void ParseParam::ReadName(char* buf)
{
	char* p=buf;
	SkipSpace();

	if(IsString())
	{
		ReadString(buf);
		return;
	}

	if(b_char.In(*cur))
		*p++=*cur++;

	while(b_char.In(*cur) || (*cur>='0' && *cur<='9'))
		*p++=*cur++;

	*p=0;

	if(*buf==0)
		throw "Невозможно прочитать имя объекта";
}

bool ParseParam::IsInt()
{
	char* p=cur;

	if(*p=='+' || *p=='-')
		p++;

	if(!(*p>='0' && *p<='9'))
		return false;

	while(*p>='0' && *p<='9')
		p++;

	return true;

}

bool ParseParam::IsFloat()
{
	char* p=cur;
	if(*p=='+' || *p=='-')
		p++;

	if(!(*p>='0' && *p<='9'))
		return false;

	while(*p>='0' && *p<='9')
		p++;

	if(*p=='.')
		return true;

	return *p=='e' || *p=='E';
}

bool ParseParam::IsString()
{
	return *cur=='"';
}

bool ParseParam::IsFArray()
{
	return (*cur=='F')&&(cur[1]=='(');
}


bool ParseParam::IsConstant()
{
	return pGetIntByConst!=NULL && b_char.In(*cur);
}

int ParseParam::ReadConstant(LPCSTR name)
{
	char val[MAXSTRLEN];
	char* v=val;
	
	while(*cur && *cur!=';' && !IsSpace(*cur))
		*v++=*cur++;
	*v=0;


	if(pGetIntByConstNonstatic)
	{
		int ret;
		if(pGetIntByConstNonstatic(name,val,ret))
			return ret;
	}

	OneParam* op=pGetIntByConst->Find((char*)name);
	if(op==NULL)
	{
		sprintf(error,"Для %s нет сопоставленных констант\n",name);
		throw error;
	}

	ParamBlock* pb=op->GetBlock();
	if(pb==NULL)
		throw "Внутренняя ошибка программы - неправильный блок констант";

	op=pb->Find(val);
	if(op==NULL)
	{

		int n=sprintf(error,"Для %s=%s нет сопоставленных констант\n"
			"Известные константы:\n",name,val);

		for(int i=0;i<pb->GetSize();i++)
		{
			OneParam* op=(*pb)[i];
			if(op->type==OneParam::P_INT)
				n+=sprintf(error+n,"%s\n",op->GetName());
		}

		throw error;
	}

	if(op->type!=OneParam::P_INT)
		throw "Внутренняя ошибка программы - неправильный блок констант";

	return op->GetInt();
}

int ParseParam::ReadInt()
{
	char buf[MAXSTRLEN];
	char* p=buf;
	if(*cur=='+' || *cur=='-') *p++=*cur++;
	while(*cur>='0' && *cur<='9')
	{
		*p++=*cur++;
	}
	*p=0;

	return atoi(buf);
}

float ParseParam::ReadFloat()
{
	char buf[MAXSTRLEN];
	char* p=buf;

	if(*cur=='+' || *cur=='-') *p++=*cur++;
	while(*cur>='0' && *cur<='9')*p++=*cur++;
	
	if(*cur=='.')
	{
		*p++=*cur++;
		while(*cur>='0' && *cur<='9')*p++=*cur++;
	}

	if(*cur=='e' || *cur=='E')
	{
		*p++=*cur++;
		if(*cur=='+' || *cur=='-') *p++=*cur++;
		while(*cur>='0' && *cur<='9')*p++=*cur++;
	}

	*p=0;

	return (float)atof(buf);
}

void ParseParam::ReadString(char* buf)
{
	if(*cur!='"')
		throw "Требуется \"";
	cur++;

	for(;*cur && *cur!='"';cur++)
		if(*cur!=10)
			*buf++=*cur;

	if(*cur!='"')
		throw "Требуется \"";
	cur++;
	*buf=0;
}

P_FArray* ParseParam::ReadFArray()
{
	cur+=2;//skip F(

	array<float> ar;

	while(1)
	{
		SkipSpace();
		if(IsFloat())
			ar.add(ReadFloat());
		else
		if(IsInt())
			ar.add(ReadInt());
		else
			break;


		SkipSpace();
		if(*cur==',')
		{
			cur++;
			continue;
		}

		if(*cur==')')
		{
			P_FArray *p=(P_FArray*)malloc(
				sizeof(int)+sizeof(float)*ar.GetSize());
			p->size=ar.GetSize();
			for(int i=0;i<p->size;i++)
			{
				p->p[i]=ar[i];
			}

			cur++;
			return p;
		}

		throw "Требуется , или )";
	}
	
	throw "Требуется float";
	return NULL;
}

//////////////////OneBlock////////////////////
OneBlock::OneBlock(ParseParam& _pp,OneBlock* _parent)
:pp(_pp),child(8,true),parent(_parent),favalue(NULL)
{
	pGetIn=NULL;
	if(parent)
	{
		if(parent->pGetIn)
		{
			OneParam* op=parent->pGetIn->Find(parent->name);
			pGetIn=op?op->GetBlock():NULL;
		}

		Read();
	}else
	{
		pGetIn=pp.pGetIn;
		pp.SkipSpace();
		ReadBlock();
	}
}

OneBlock::~OneBlock()
{
	if(favalue)
		free(favalue);
}

void OneBlock::Read()
{
	char word[MAXSTRLEN];
	pp.ReadName(word);
	name=word;
	pp.SkipSpace();

	if(*pp.cur=='{')
	{
		pp.cur++;
		ReadBlock();
	}else
	if(*pp.cur=='=')
	{
		pp.cur++;
		pp.SkipSpace();

		if(pp.IsString())
		{
			type=OneParam::P_STRING;
			pp.ReadString(word);
			svalue=word;
		}else
		if(pp.IsFloat())
		{
			type=OneParam::P_FLOAT;
			fvalue=pp.ReadFloat();
		}else
		if(pp.IsInt())
		{
			type=OneParam::P_INT;
			ivalue=pp.ReadInt();
		}else
		if(pp.IsFArray())
		{
			type=OneParam::P_FARRAY;
			favalue=pp.ReadFArray();
		}else
		if(pp.IsConstant())
		{
			type=OneParam::P_INT;
			ivalue=pp.ReadConstant(name);
		}else
			throw "Непонятное значение";
	
		pp.SkipSpace();

	}else
	if(*pp.cur==';')
	{
		type=OneParam::P_NULL;
	}else
		throw "Требуется = или {";

	if(*pp.cur!=';')
		throw "Здесь необходима ;";
	*pp.cur++;
}

void OneBlock::ReadBlock()
{
	type=OneParam::P_PARAMBLOCK;

	while(*pp.cur)
	{
		pp.SkipSpace();
		if(*pp.cur==0)break;
		if(*pp.cur=='}')
		{
			pp.cur++;
			return;
		}


		OneBlock* p=new OneBlock(pp,this);
		child.add(p);
	}
	
	if(parent!=NULL)
		throw "Здесь необходима }";
}

#define WR(x) m.add(&x,sizeof(x));

void OneBlock::Save(MemBlock& m)
{
	int t=m.tell();
	if(parent)
	{
		WR(type);
		m.add(name,strlen(name)+1);
	}

	if(type==OneParam::P_NULL)
	{
	}else
	if(type==OneParam::P_STRING)
	{
		m.add(svalue,strlen(svalue)+1);
	}else
	if(type==OneParam::P_INT)
	{
		WR(ivalue);
	}else
	if(type==OneParam::P_FLOAT)
	{
		WR(fvalue);
	}else
	if(type==OneParam::P_PARAMBLOCK)
	{
		int n=child.GetSize();
		int tt=m.tell();
		WR(n);
		int offset=m.tell();
		for(int i=0;i<n;i++)
		{
			WR(n);//Скипуем offset
		}

		for(i=0;i<n;i++)
		{
			int* offseti=(int*)m[offset+i*sizeof(int)];
			*offseti=m.tell()-tt;
			child[i]->Save(m);
		}
	}else
	if(type==OneParam::P_FARRAY)
	{
		WR(favalue->size);
		for(int i=0;i<favalue->size;i++)
		{
			WR(favalue->p[i]);
		}
	}else
		ASSERT(0);
}

//////////////////////////////////////
char* ParseText(const char* text,ParamBlock** paramblock,int* size,ParamBlock* pRules,
				NonStaticByConst pGetIntByConstNonstatic)
{
	if(paramblock)
		*paramblock=NULL;
	if(size)
		*size=0;

	ParseParam pp((char*)text,pRules,pGetIntByConstNonstatic);
	try 
	{
		OneBlock ob(pp,NULL);
		//ob.Print("");

		MemBlock m;
		ob.Save(m);
		if(paramblock)
		{
			*paramblock=(ParamBlock*)m.p;
			m.p=NULL;
		}

		if(size)
			*size=m.size;
	}catch(char* err)
	{
		static char word[MAXSTRLEN];
		sprintf(word,"line=%i %s\n",pp.line,err);
#ifdef _DEBUG
		OutputDebugString(word);
#endif _DEBUG
		return word;
	}

	return NULL;
}
