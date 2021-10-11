#include "stdlib.h"
#include "meshfile.h"
#include "string.h"
#include "StreamBuffer.h"

/*
sBaseFieldError MeshFileError[]=
{
	{	MESHFILE_OK,				"no error"									},
	{	MESHFILE_NOT_FOUND,			"Error: MeshFile not found"					},
	{	MESHFILE_UNKNOWN_FORMAT,	"Error: MeshFile unknown fileformat"		},
	{	MESHFILE_ERROR_TYPE,		"Error: MeshFile unknown file type"			},
	{	MESHFILE_NOT_USE_FIELD,		"Warning: MeshFile this field not use"		},
	{	MESHFILE_ERROR,				"Error: MeshFile error file format"			}
};
*/
extern int GlobalCurrentFieldType=0;

cMeshFile::cMeshFile()							
{ 
	f=0;
	type=0; 
	error=0; 
	ofs=0; 
	SizeID=SizeType=0;
}
cMeshFile::~cMeshFile()						
{ 
	Close();
}
int cMeshFile::OpenRead(void *buf,int size)
{
	Close();
	f=new cStream;
	if(((cStream*)f)->open(buf,size,STREAM_MEM|STREAM_IN)) return SetError(MESHFILE_NOT_FOUND);
	return MESHFILE_OK;
};
int cMeshFile::OpenWrite(int type)
{
	Close();
	cMeshFile::type=type;
	f=new cStream;
	if(((cStream*)f)->open(1000000,STREAM_MEM|STREAM_OUT)) return SetError(MESHFILE_NOT_FOUND);
	((cStream*)f)->seek(0);
	return MESHFILE_OK;
};
int	cMeshFile::Length()
{
	return ((cStream*)f)->length();
}
void* cMeshFile::GetBuffer()
{
	return ((cStream*)f)->str();
}
void cMeshFile::Close()
{
	if(f) delete ((cStream*)f); f=0;
	type=0; 
	error=0; 
	ofs=0; 
}
int cMeshFile::eof()
{
	return ((cStream*)f)->eof();
}
void cMeshFile::WriteOfs()
{ 
	assert(type==MESHFILE_TYPE_TEXT); 
	for(int count=0;count<ofs;count++) 
		(*(cStream*)f)<<" "; 
}
void cMeshFile::ReadHeaderFile()
{
	int version;
	char format[sizeof(MESHFILE_IDENTIFIER)+4];
	((cStream*)f)->read(format,sizeof(MESHFILE_IDENTIFIER));
	((cStream*)f)->read(&type,sizeof(type));
	((cStream*)f)->read(&version,sizeof(version));
	if(stricmp(format,MESHFILE_IDENTIFIER)) SetError(MESHFILE_UNKNOWN_FORMAT);
	if((type!=MESHFILE_TYPE_TEXT)&&(type!=MESHFILE_TYPE_BINARY)) SetError(MESHFILE_ERROR_TYPE);
	if(version<2)
		SizeID=sizeof(unsigned short),SizeType=sizeof(unsigned short);
	else if(version<=MESHFILE_MAXVERSION)
		SizeID=sizeof(unsigned char),SizeType=sizeof(unsigned char);
	if(version>MESHFILE_MAXVERSION) SetError(MESHFILE_UNKNOWN_FORMAT);
}
void cMeshFile::WriteHeaderFile(int version)
{
	if((type!=MESHFILE_TYPE_TEXT)&&(type!=MESHFILE_TYPE_BINARY)) SetError(MESHFILE_ERROR_TYPE);
	((cStream*)f)->write(MESHFILE_IDENTIFIER,sizeof(MESHFILE_IDENTIFIER));
	((cStream*)f)->write(&type,sizeof(type));
	if(version==1)
		SizeID=sizeof(unsigned short),SizeType=sizeof(unsigned short);
	else if(version==2)
		SizeID=sizeof(unsigned char),SizeType=sizeof(unsigned char);
	((cStream*)f)->write(&version,sizeof(version));
	if(type==MESHFILE_TYPE_TEXT) (*((cStream*)f))<<"\n";
}

int cMeshFile::ReadFieldType()
{
	GlobalCurrentFieldType=0;
	if(type==MESHFILE_TYPE_BINARY) 
		((cStream*)f)->read(&GlobalCurrentFieldType,SizeType);
	else if(type==MESHFILE_TYPE_TEXT) 
	{
		char buf[256];
		while(!((cStream*)f)->eof())
		{
			(*((cStream*)f))>>buf;
			for(int i=0;GlobalFieldType[i].var;i++)
				if(stricmp(buf,GlobalFieldType[i].str)==0)
					return GlobalCurrentFieldType=GlobalFieldType[i].type;
			((cStream*)f)->getline(buf,256);
		}
	}
	if(((cStream*)f)->eof()) return -1;
	return GlobalCurrentFieldType;
}
int cMeshFile::ReadBlock()
{
	int BlockID=0;
	if(type==MESHFILE_TYPE_BINARY) 
		((cStream*)f)->read(&BlockID,SizeID);
	else if(type==MESHFILE_TYPE_TEXT) 
	{
		char buf[256];
		while(!((cStream*)f)->eof())
		{
			(*((cStream*)f))>>buf;
			for(int i=0;GlobalBlockID[i].var;i++)
				if(stricmp(buf,GlobalBlockID[i].str)==0)
					return GlobalBlockID[i].type;
			((cStream*)f)->getline(buf,256);
		}
	}
	return BlockID;
}
void cMeshFile::ReadField(void *buf,int TypeField)
{
	if(type==MESHFILE_TYPE_BINARY) 
		if(GlobalFieldType[TypeField].var!=MF_TYPE_STRING)
			((cStream*)f)->read(buf,GlobalTypeVariable[GlobalFieldType[TypeField].var].size);
		else 
		{
			char tmp[256],ch=0;
			while((!((cStream*)f)->eof())&&(ch!='"'))
				((cStream*)f)->read(&ch,1);
			if(ch!='"') return; else tmp[0]=0;
			int count;
			for(count=0;(!((cStream*)f)->eof())&&(tmp[count]!='"')&&(++count<256);)
				((cStream*)f)->read(&tmp[count],1);
			tmp[count]=0;
			if(count>1)	
				((cString*)buf)->New(&tmp[1]);
		}
	else if(type==MESHFILE_TYPE_TEXT) 
	{
		switch(GlobalFieldType[TypeField].var)
		{
			case MF_TYPE_CHAR:
				(*((cStream*)f))>>((char*)buf)[0]; break;
			case MF_TYPE_UCHAR:
				(*((cStream*)f))>>((unsigned char*)buf)[0]; break;
			case MF_TYPE_SHORT:
				(*((cStream*)f))>>((short*)buf)[0]; break;
			case MF_TYPE_USHORT:
				(*((cStream*)f))>>((unsigned short*)buf)[0]; break;
			case MF_TYPE_INT:
				(*((cStream*)f))>>((int*)buf)[0]; break;
			case MF_TYPE_UINT:
				(*((cStream*)f))>>((unsigned int*)buf)[0]; break;
			case MF_TYPE_FLOAT:
				(*((cStream*)f))>>((float*)buf)[0]; break;
			case MF_TYPE_DOUBLE:
				(*((cStream*)f))>>((double*)buf)[0]; break;
			case MF_TYPE_FLOAT2:
				(*((cStream*)f))>>((float*)buf)[0]>>((float*)buf)[1]; break;
			case MF_TYPE_FLOAT3:
				(*((cStream*)f))>>((float*)buf)[0]>>((float*)buf)[1]>>((float*)buf)[2]; break;
			case MF_TYPE_FLOAT4:
				(*((cStream*)f))>>((float*)buf)[0]>>((float*)buf)[1]>>((float*)buf)[2]>>((float*)buf)[3]; break;
			case MF_TYPE_FLOAT5:
				(*((cStream*)f))>>((float*)buf)[0]>>((float*)buf)[1]>>((float*)buf)[2]>>((float*)buf)[3]>>((float*)buf)[4]; break;
			case MF_TYPE_FLOAT12:
				(*((cStream*)f))>>((float*)buf)[0]>>((float*)buf)[1]>>((float*)buf)[2]; 
				(*((cStream*)f))>>((float*)buf)[3]>>((float*)buf)[4]>>((float*)buf)[5]; 
				(*((cStream*)f))>>((float*)buf)[6]>>((float*)buf)[7]>>((float*)buf)[8]; 
				(*((cStream*)f))>>((float*)buf)[9]>>((float*)buf)[10]>>((float*)buf)[11]; 
				break;
			case MF_TYPE_INT3:
				(*((cStream*)f))>>((int*)buf)[0]>>((int*)buf)[1]>>((int*)buf)[2]; break;
			case MF_TYPE_INT4:
				(*((cStream*)f))>>((int*)buf)[0]>>((int*)buf)[1]>>((int*)buf)[2]>>((int*)buf)[3]; break;
			case MF_TYPE_STRING:
				{
					char tmp[256],ch=0;
					while((!((cStream*)f)->eof())&&(ch!='"'))
						((cStream*)f)->read(&ch,1);
					if(ch!='"') break; else tmp[0]=0;
					int count;
					for(count=0;(!((cStream*)f)->eof())&&(tmp[count]!='"')&&(++count<256);)
						((cStream*)f)->read(&tmp[count],1);
					tmp[count]=0;
					((cString*)buf)->New(&tmp[1]);
					break;
				}
			}
	}
}
void cMeshFile::BeginWriteBlock(int BlockID)
{
	if(type==MESHFILE_TYPE_BINARY) 
	{
		((cStream*)f)->write(&GlobalFieldType[MF_TYPE_BLOCK].type,SizeType);
		((cStream*)f)->write(&GlobalBlockID[BlockID].type,SizeID);
	}
	else if(type==MESHFILE_TYPE_TEXT) 
	{
		WriteOfs();
		(*((cStream*)f))<<GlobalFieldType[MF_TYPE_BLOCK].str<<" "<<GlobalBlockID[BlockID].str<<"\n";
	}
	ofs+=MESHFILE_BLOCK_OFS;
}
void cMeshFile::EndWriteBlock(int BlockID)
{
	ofs-=MESHFILE_BLOCK_OFS;
	if(type==MESHFILE_TYPE_BINARY) 
	{
		((cStream*)f)->write(&GlobalFieldType[MF_TYPE_BLOCK].type,SizeType);
		((cStream*)f)->write(&GlobalBlockID[BlockID].type,SizeID);
	}
	else if(type==MESHFILE_TYPE_TEXT) 
	{
		WriteOfs();
		(*((cStream*)f))<<GlobalFieldType[MF_TYPE_BLOCK].str<<" "<<GlobalBlockID[BlockID].str<<"\n";
	}
}
void cMeshFile::WriteField(void *buf,int TypeField)
{
	if(type==MESHFILE_TYPE_BINARY)
	{
		((cStream*)f)->write(&GlobalFieldType[TypeField].type,SizeType);
		if(GlobalTypeVariable[GlobalFieldType[TypeField].var].size>=0)
			((cStream*)f)->write(buf,GlobalTypeVariable[GlobalFieldType[TypeField].var].size);
		else if(*((cString*)buf))
		{
			((cStream*)f)->write("\"",1);
			((cStream*)f)->write((char*)((cString*)buf)->ptr(),strlen((char*)((cString*)buf)->ptr()));
			((cStream*)f)->write("\"",1);
		}
		else
			((cStream*)f)->write("\"\"",2);
	}
	else if(type==MESHFILE_TYPE_TEXT) 
	{
		WriteOfs();
		((cStream*)f)->write(GlobalFieldType[TypeField].str,strlen(GlobalFieldType[TypeField].str));
		switch(GlobalFieldType[TypeField].var)
		{
			case MF_TYPE_CHAR:
				(*((cStream*)f))<<"\t"<<((char*)buf)[0]<<"\n"; break;
			case MF_TYPE_UCHAR:
				(*((cStream*)f))<<"\t"<<((unsigned char*)buf)[0]<<"\n"; break;
			case MF_TYPE_SHORT:
				(*((cStream*)f))<<"\t"<<((short*)buf)[0]<<"\n"; break;
			case MF_TYPE_USHORT:
				(*((cStream*)f))<<"\t"<<((unsigned short*)buf)[0]<<"\n"; break;
			case MF_TYPE_INT:
				(*((cStream*)f))<<"\t"<<((int*)buf)[0]<<"\n"; break;
			case MF_TYPE_UINT:
				(*((cStream*)f))<<"\t"<<((unsigned int*)buf)[0]<<"\n"; break;
			case MF_TYPE_FLOAT:
				(*((cStream*)f))<<"\t"<<((float*)buf)[0]<<"\n"; break;
			case MF_TYPE_DOUBLE:
				(*((cStream*)f))<<"\t"<<((double*)buf)[0]<<"\n"; break;
			case MF_TYPE_FLOAT2:
				(*((cStream*)f))<<"\t"<<((float*)buf)[0]<<"\t"<<((float*)buf)[1]<<"\n"; break;
			case MF_TYPE_FLOAT3:
				(*((cStream*)f))<<"\t"<<((float*)buf)[0]<<"\t"<<((float*)buf)[1]<<"\t"<<((float*)buf)[2]<<"\n"; break;
			case MF_TYPE_FLOAT4:
				(*((cStream*)f))<<"\t"<<((float*)buf)[0]<<"\t"<<((float*)buf)[1]<<"\t"<<((float*)buf)[2]<<"\t"<<((float*)buf)[3]<<"\n"; break;
			case MF_TYPE_FLOAT5:
				(*((cStream*)f))<<"\t"<<((float*)buf)[0]<<"\t"<<((float*)buf)[1]<<"\t"<<((float*)buf)[2]<<"\t"<<((float*)buf)[3]<<"\t"<<((float*)buf)[4]<<"\n"; break;
			case MF_TYPE_FLOAT12:
				(*((cStream*)f))<<"\t"<<((float*)buf)[0]<<"\t"<<((float*)buf)[1]<<"\t"<<((float*)buf)[2]<<"\n"; 
				WriteOfs(); (*((cStream*)f))<<"\t"<<((float*)buf)[3]<<"\t"<<((float*)buf)[4]<<"\t"<<((float*)buf)[5]<<"\n"; 
				WriteOfs(); (*((cStream*)f))<<"\t"<<((float*)buf)[6]<<"\t"<<((float*)buf)[7]<<"\t"<<((float*)buf)[8]<<"\n"; 
				WriteOfs(); (*((cStream*)f))<<"\t"<<((float*)buf)[9]<<"\t"<<((float*)buf)[10]<<"\t"<<((float*)buf)[11]<<"\n"; 
				break;
			case MF_TYPE_INT3:
				(*((cStream*)f))<<"\t"<<((int*)buf)[0]<<"\t"<<((int*)buf)[1]<<"\t"<<((int*)buf)[2]<<"\n"; break;
			case MF_TYPE_INT4:
				(*((cStream*)f))<<"\t"<<((int*)buf)[0]<<"\t"<<((int*)buf)[1]<<"\t"<<((int*)buf)[2]<<"\t"<<((int*)buf)[3]<<"\n"; break;
			case MF_TYPE_STRING:
				if(((cString*)buf)->ptr()) (*((cStream*)f))<<" \""<<(char*)((cString*)buf)->ptr()<<"\"\n";
				else (*((cStream*)f))<<" \""<<"none"<<"\"\n";
				break;
		}
	}
}
