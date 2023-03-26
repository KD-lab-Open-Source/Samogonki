#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "assert.h"
#include "string.h"
#include "BaseClass.h"
#include "cString.h"
#include "UMATH.H"

enum eAttributeMaterial
{
	ATTRMAT_ATTRIBUTE_NULL						=	0,
	// material 0..5
	ATTRMAT_COLOR								=	1<<0,
	ATTRMAT_TEXTURE_PAL							=	1<<1,
	ATTRMAT_TILE_MAP							=	1<<2,
	// attribute material 5..11
	ATTRMAT_MASK								=	1<<5,
	ATTRMAT_OPACITY								=	1<<6,
	ATTRMAT_TRANSPARENCY						=	1<<7,
	ATTRMAT_COLORBLEND							=	1<<8,
	ATTRMAT_ALFABLEND							=	1<<9,
	ATTRMAT_METAL								=	1<<10,
	// reserv attribute material 12..31
	ATTRMAT_RGBA8888							=	1<<25,
	ATTRMAT_ENVIRONMENT							=	1<<26,
};

enum eAttributeTexture
{
	ATTRTEX_NULL								=	0,
	ATTRTEX_PAL8								=	1<<0,
	ATTRTEX_RGB565								=	1<<1,
	ATTRTEX_RGB555								=	1<<2,
	ATTRTEX_RGB888								=	1<<3,
	ATTRTEX_RGBA8888							=	1<<4,

	STATUS_TEXTURE_CREATE						=	1<<16,	// текстура создана на устройстве вывода
	STATUS_TEXTURE_CACHE						=	1<<17,	// текстура закэширована
	STATUS_TEXTURE_LOCK							=	1<<18,	// текстура залокирована
	STATUS_TEXTURE_LOAD							=	1<<19,	// текстуру загрузить из файла
	STATUS_TEXTURE_RESET						=	1<<20,	// текстура была перегружена необходимо обновить
	STATUS_TEXTURE_SHADOW						=	1<<21,	// только для ShadowMap 
};

struct sTexture : public sAttribute, public sPointerCount
{
	unsigned char	*texture;					// указатель на текстуру
	short			_x,_y;						// битовый размер текстуры
	short			nTexture;					// номер текстуры в текстурах PolyDispathcer'а
	short			bpl;						// размер текстуры в байтах на утройстве вывода
	cString			name;						// имя текстуры

	sTexture()											{ texture=0; _x=_y=0; nTexture=bpl=0; }
	~sTexture();
	void Release();

	void SetName(const char *Name)							{ name=Name; }
	inline int GetStatus(int status=0xFFFF0000)			{ return sAttribute::GetAttribute(status); }
	inline void SetStatus(int status)					{ sAttribute::SetAttribute(status); }
	inline void ClearStatus(int status)					{ sAttribute::ClearAttribute(status); }
	inline unsigned char* GetPal()						{ return &texture[1<<(_x+_y)]; }
};

struct sAnimMaterial : public sAttribute // eAttributeMaterial
{
	sColor4f		MulCol;			// мультипликативная компонента цвета
	sColor4f		AddCol;			// аддитивная компонента цвета
	sColor4f& GetDiffuse()								{ return MulCol; }
	sColor4f& GetSpecular()								{ return AddCol; }
	inline void interpolate(sAnimMaterial &a,sAnimMaterial &b,float f)
	{
		GetDiffuse().interpolate(a.GetDiffuse(),b.GetDiffuse(),f);
		GetSpecular().interpolate(a.GetSpecular(),b.GetSpecular(),f);
	}
	inline void set(sAnimMaterial &a)
	{
		GetDiffuse()=a.GetDiffuse();
		GetSpecular()=a.GetSpecular();
	}
};

class cMaterial : public sAnimMaterial
{
public:
	sTexture		*Texture;
	sTexture		*Opacity;
	
	cMaterial()											{ memset(this,0,sizeof(cMaterial)); }
	cMaterial(cMaterial *Material);				
	~cMaterial()							
	{ 
		if(Texture) 
			Texture->Release(); 
		if(Opacity) 
			Opacity->Release(); 
	}
	void SetType(int type=0);
	inline cMaterial& operator = (cMaterial &Material)	
	{ 
		if(Material.Texture) 
		{ Texture=Material.Texture; Texture->IncRef(); } 
		if(Material.Opacity) 
		{ Opacity=Material.Opacity; Opacity->IncRef(); } 
		Attribute=Material.GetAttribute(); 
		return *this; 
	}
	inline void SetMulColor(const sColor4f &MulColor)	{ MulCol=MulColor; }
	inline void SetAddColor(const sColor4f &AddColor)	{ AddCol=AddColor; }
	inline void GetCopy(cMaterial *Material)			{ if(Texture) { Material->Texture=Texture; Texture->IncRef(); } if(Opacity) { Material->Opacity=Opacity; Opacity->IncRef(); } Material->Attribute=Attribute; Material->MulCol=MulCol; Material->AddCol=AddCol; }
	void SetTexture(sTexture *pTexture,sTexture *pOpacity);
};

#endif // __MATERIAL_H__