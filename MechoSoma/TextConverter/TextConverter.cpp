#include "stdafx.h"
#include "parse\ParamParse.h"
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include "MessageBlock.h"

char inname[_MAX_PATH]="rotate.cfg";
char outname[_MAX_PATH];
FILE* errorf=0;
bool b_english_string=false;

int main(int argc, char* argv[])
{
	bool brules=false;
	char rules_name[_MAX_PATH];

	for(int i=1;i<argc;i++)
	{
		if(strcmp(argv[i],"-eng")==0)
		{
			b_english_string=true;
		}
	}

	if(argc>=2 && argv[1][0]!='-')
		strcpy(inname,argv[1]);

	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath(inname,drive,dir,fname,ext);

		_makepath(rules_name,drive,dir,fname,".rul");
		FILE* f=fopen(rules_name,"r");
		brules=f!=NULL;
		if(f)fclose(f);
	}

	errorf=fopen("error.txt","w+t");

	int f=_open( inname, _O_RDONLY |_O_BINARY);
	if(f==-1)
	{
		fprintf(errorf,"cannot open %s",inname);
		exit(1);
		return 1;
	}

	int size=_lseek(f,0,SEEK_END);_lseek(f,0,SEEK_SET);
	char* text=new char[size+1];
	text[size]=0;
	_read(f,text,size);
	_close(f);

	_strlwr(inname);
	bool beginner=strstr(inname,"beginner")?true:false;
	if(brules)
		InitStringID(b_english_string,beginner);

	ParamBlock* pGetRulesConst=NULL;
	ParamBlock* paramblock;
	if(brules)
		pGetRulesConst=ReadRules(rules_name);

	int sizeparam;
	char* error=ParseText(text,&paramblock,&sizeparam,
		pGetRulesConst,brules?StringIDByConst:NULL);

	if(error)
	{
		fprintf(errorf,"file=%s\n",inname);
		fprintf(errorf,error);
		printf("See error.txt");
		exit(1);
		return 1;
	}

	delete text;

//	paramblock->Print(std::cout);

	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath(inname,drive,dir,fname,ext);

		if(b_english_string)
			strcat(fname,"_e");
		_makepath(outname,drive,dir,fname,"dat");
	}

	f=_open( outname, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, 
                      _S_IREAD | _S_IWRITE );
	if(f==-1)
	{
		fprintf(errorf,"cannot save %s",outname);
		exit(1);
		return 1;
	}

	_write(f,paramblock,sizeparam);
	_close(f);

	printf("Converted ok\n");

	exit(0);
	return 0;
}

