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
	sTexture						*MultiTexture;				// òåêóùàÿ òåêñòóðà â êîòîðóþ èäåò íàêîïëåíèå ïî Attach()
public:
	cTextureBuffer():cBaseArrayPointer<sTexture>(1000,100)	{ }
	~cTextureBuffer();

	sTexture* BeginList(int x,int y);			// создать мульти текстуру размера x*y
	sTextureChild* AttachChild(const char *name,int id);	// добавить в мульти текстуру текстуру
	sTexture* EndList();						// завершить создание мультитектуры
	sTextureChild& GetTextureChild(int number)				{ assert(0<=number&&number<TextureChildArray.length()); return TextureChildArray[number]; }

	sTexture* Get(const char *name,const char *path,const char *warning="");
	void Release(unsigned int Type);
private:
	sTexture* Load(const char *fname,const char *warning="");
	sTexture* LoadTGA(const char *fname,const char *warning="");
	sTexture* LoadJPG(const char *fname,const char *warning="");
	int LoadTGA(const char *fname,void **buf,int &x,int &y,int &fmt);
	int SaveTGA(const char *fname,void *buf,int x,int y,int fmt);
	void ConvertTextureFormat(void *pOld,int xOld,int yOld,int fmtOld,void *pNew,int xNew,int yNew,int fmtNew);
};
extern cTextureBuffer *TextureBuffer;

#endif // __TEXTUREMGR_H__
