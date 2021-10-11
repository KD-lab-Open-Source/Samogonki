#include "material.h"
#include "maths.h"
#include "malloc.h"
#include "PolyMgr.h"

sTexture::~sTexture()
{ 
	assert(nTexture==0);
	assert(texture==0);
}
void sTexture::Release()
{ 
	if(DecRef()) return;
	if(nTexture)
		P3D->DeleteTexture(this);
	FREE_MEM(texture); 
	delete this;
}

cMaterial::cMaterial(cMaterial *Material)
{
	Attribute=0;
	SetMulColor(sColor4f(1,1,1,1));
	SetAddColor(sColor4f(0,0,0,1));
}
void cMaterial::SetType(int type)
{
	Attribute=type;
	assert(Texture);
	if(Texture->texture) SetAttribute(ATTRMAT_TEXTURE_PAL); else SetAttribute(ATTRMAT_COLOR);
	if(Opacity) SetAttribute(ATTRMAT_OPACITY);
}
void cMaterial::SetTexture(sTexture *pTexture,sTexture *pOpacity)
{
	Texture=pTexture;
	Opacity=pOpacity;
	if(Texture==0&&Opacity) 
	if(Texture) SetAttribute(ATTRMAT_COLOR);
	if(Texture) SetAttribute(ATTRMAT_TEXTURE_PAL);
	if(Opacity) SetAttribute(ATTRMAT_OPACITY);
	if(Opacity&&(Opacity->_x!=Texture->_x||Opacity->_y!=Texture->_y))
	{
		XBuffer buf;
		buf<"Error: cMaterial::SetTexture()\r\nsize texture "<Texture->name<" != size opacity "<Opacity->name;
		ErrAbort(buf.address());
	}
}
