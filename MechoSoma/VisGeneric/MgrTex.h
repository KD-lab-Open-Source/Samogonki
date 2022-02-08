#ifndef __MANAGERTEXTURE_H__
#define __MANAGERTEXTURE_H__

#include "Texture.h"
#include "ITexture.h"

class cManagerTexture : public cInterfaceTexture
{
public:
	// объявления cInterfaceTexture 
	cManagerTexture();
	~cManagerTexture();
	virtual int Release();
	virtual void Delete(cUnknownClass* Texture);								// удаление любой текстуры

	virtual cUnknownClass* Get(char *NameTexture,char *NameOpacity=0);			// загрузка анимированной текстуры cTextureAnimation
	virtual void Free();														// освобождение неиспользуемых текстур

protected:
	cUnknownLibrary			TextureLibrary;
	// функции для работы с библиотекой текстур
	cUnknownClass* GetTextureAnimation(char *NameTexture,char *NameOpacity=0);	// возвращает cTextureAnimation
	void DeleteTexture(cUnknownClass *Texture);									// для удаления текстур, выделенных библиотекой
};

#endif //__MANAGERTEXTURE_H__