#ifndef __MESHFILE_H__
#define __MESHFILE_H__

#include "cString.h"
#include "BaseClass.h"

#include "DefMeshFile.h"

//meshfile header
#define MESHFILE_IDENTIFIER				"m3d"
#define MESHFILE_TYPE_BINARY			'b'
#define MESHFILE_TYPE_TEXT				't'
#define MESHFILE_MAXVERSION				0x00000002

//meshfile error
#define MESHFILE_OK						0x00000000
#define MESHFILE_NOT_FOUND				0x00000001
#define MESHFILE_UNKNOWN_FORMAT			0x00000002
#define MESHFILE_ERROR_TYPE				0x00000003
#define MESHFILE_NOT_USE_FIELD			0x00000004
#define MESHFILE_ERROR					0x00000005

#define MESHFILE_BLOCK_OFS				3

extern int GlobalCurrentFieldType;

class cMeshFile
{
public:
	void	*f;
	char	type;
	int		error;
	int		ofs;						// смещение в текстовом формате

	cMeshFile();
	~cMeshFile();
	int		OpenRead(void *buf,int size);
	int		OpenWrite(int type);
	void*	GetBuffer();
	int		Length();
	int		SetError(int err)			{ if(err) Close(); return error=err; }
	void	Close();

	void	ReadHeaderFile();
	void	WriteHeaderFile(int version=MESHFILE_MAXVERSION);

	int		ReadBlock();
	int		ReadFieldType();
	void	ReadField(void *buf,int TypeField=GlobalCurrentFieldType);
	void	WriteField(void *buf,int TypeField);
	void	BeginWriteBlock(int BlockID);
	void	EndWriteBlock(int BlockID);

	int	eof();
private:
	void WriteOfs();
	int		SizeID;
	int		SizeType;
};

template <class cBaseMesh,const int SIZE> 
struct sBaseMeshArrayType
{
	cBaseMesh Base[SIZE];
	inline cBaseMesh& operator [] (int number) { return Base[number]; }
	~sBaseMeshArrayType()
	{
		Base[0]=0;
	}
};
typedef sBaseMeshArrayType <int,3> sInt3;
typedef sBaseMeshArrayType <int,4> sInt4;
typedef sBaseMeshArrayType <float,2> sFloat2;
typedef sBaseMeshArrayType <float,3> sFloat3;
typedef sBaseMeshArrayType <float,4> sFloat4;
typedef sBaseMeshArrayType <float,5> sFloat5;

template <class cBase,const int MF_ID_BLOCK_BASEMESH,const int MF_TYPE_BASEMESH> 
class cBaseFileMeshArray: public cBaseDynArray <cBase>
{
public:
	cBaseFileMeshArray()					{ }
	~cBaseFileMeshArray()					{ this->Release(); }
	void Release()							{ this->Delete(); }
	void New(int NewSize)					{ cBaseDynArray<cBase>::New(NewSize); }
	int Read(cMeshFile &f)					
	{
		int CurrentSize=0,NewSize=0;
		while(!f.eof())
			switch(f.ReadFieldType())
			{
				case MF_TYPE_NUMBER:
					f.ReadField(&NewSize);
					New(NewSize);
					break;
				case MF_TYPE_BASEMESH:
					assert(CurrentSize<this->length());
					f.ReadField(&this->Base[CurrentSize++]);
					break;
				case MF_TYPE_BLOCK:
					switch(f.ReadBlock())
					{
						case MF_ID_BLOCK_BASEMESH:
							return MESHFILE_OK;
					}
					break; 
			}
		return MESHFILE_ERROR;
	}
	int Write(cMeshFile &f)
	{ 
		if(this->length()<=0) return MESHFILE_OK;
		f.BeginWriteBlock(MF_ID_BLOCK_BASEMESH);
		f.WriteField(&this->length(),MF_TYPE_NUMBER);
		for(cBase *b=this->Base,*e=&this->Base[this->length()];b<e;b++)
			f.WriteField(b,MF_TYPE_BASEMESH);
		f.EndWriteBlock(MF_ID_BLOCK_BASEMESH);
		return MESHFILE_OK;
	}
};

template <class cBaseMesh> 
class cBaseMeshPointerLibrary: public cBaseDynArrayPointer <cBaseMesh>
{
public:
	cBaseMeshPointerLibrary()								{ }
	~cBaseMeshPointerLibrary()								{ this->Release(); }
	void Release()											{ this->Delete(); }
	cBaseMesh* New(cBaseMesh *BaseMesh)
	{
		if(BaseMesh==0) BaseMesh=new cBaseMesh;
		else for(int i=0;i<this->length();i++) if(BaseMesh==this->Base[i]) { BaseMesh->Release(); return BaseMesh; }
		this->Resize(this->length()+1);
		this->Base[this->length()-1]=BaseMesh;
		BaseMesh->ID=this->length()-1;
		return BaseMesh;
	}
	cBaseMesh* Get(unsigned int ID)
	{
		for(int i=0;i<this->length();i++)
			if(this->Base[i]->ID==ID)
				return this->Base[i];
		return 0;
	}
	cBaseMesh* Get(const void *Key)
	{
		for(int i=0;i<this->length();i++)
			if(this->Base[i]->cmp(Key)==0)
				return this->Base[i];
		return 0;
	}
	inline cBaseMesh* Append()								
	{ 
		return cBaseMeshPointerLibrary::New(new cBaseMesh); 
	}
	int Write(cMeshFile &f)
	{
		for(int i=0;i<this->length();i++)
			if(this->Base[i]->Write(f)!=MESHFILE_OK) return MESHFILE_ERROR;
		return MESHFILE_OK;
	}
	inline cBaseMeshPointerLibrary& operator = (const cBaseMeshPointerLibrary &Array)
	{
		*(cBaseDynArrayPointer <cBaseMesh>*)this=*(cBaseDynArrayPointer <cBaseMesh>*)&Array;
		((cBaseDynArrayPointer <cBaseMesh>*)&Array)->Clear();
		return *this;
	}
};

#endif //__MESHFILE_H__
