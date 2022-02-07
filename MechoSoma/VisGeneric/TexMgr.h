#ifndef __TEXTUREMGR_H__
#define __TEXTUREMGR_H__

#include "BaseClass.h"
#include "Material.h"

struct sTextureChild
{
	int			_x,_y;
	int			xofs,yofs;
	float		uofs,vofs,usize,vsize;
	sTexture	*Texture;
	sTextureChild()											{ Texture=0; _x=_y=0; xofs=yofs=0; }
};

class cTextureBuffer : public cBaseArrayPointer <sTexture>
{
	cBaseDynArray<sTextureChild>	TextureChildArray;
	int								xCurrent,yCurrent,yMax;
	sTexture						*MultiTexture;				// ������� �������� � ������� ���� ���������� �� Attach()
public:
	cTextureBuffer():cBaseArrayPointer<sTexture>(1000,100)	{ }
	~cTextureBuffer();

	sTexture* BeginList(int x,int y);			// ������� ������ �������� ������� x*y
	sTextureChild* AttachChild(char *name,int id);	// �������� � ������ �������� ��������
	sTexture* EndList();						// ��������� �������� �������������
	sTextureChild& GetTextureChild(int number)				{ assert(0<=number&&number<TextureChildArray.length()); return TextureChildArray[number]; }

	sTexture* Get(char *name,char *path,char *warning="");
	void Release(unsigned int Type);
private:
	sTexture* Load(char *fname,char *warning="");
	sTexture* LoadTGA(char *fname,char *warning="");
	sTexture* LoadJPG(char *fname,char *warning="");
	int LoadTGA(char *fname,void **buf,int &x,int &y,int &fmt);
	int SaveTGA(char *fname,void *buf,int x,int y,int fmt);
	void ConvertTextureFormat(void *pOld,int xOld,int yOld,int fmtOld,void *pNew,int xNew,int yNew,int fmtNew);
};
extern cTextureBuffer *TextureBuffer;

#endif // __TEXTUREMGR_H__