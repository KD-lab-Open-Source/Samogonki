#ifndef __MANAGERTEXTURE_H__
#define __MANAGERTEXTURE_H__

#include "Texture.h"
#include "ITexture.h"

class cManagerTexture : public cInterfaceTexture
{
public:
	// объ€влени€ cInterfaceTexture 
	cManagerTexture();
	~cManagerTexture();
	virtual int Release();
	virtual void Delete(cUnknownClass* Texture);								// удаление любой текстуры

	virtual cUnknownClass* Get(char *NameTexture,char *NameOpacity=0);			// загрузка анимированной текстуры cTextureAnimation
	virtual void Free();														// освобождение неиспользуемых текстур

protected:
	cUnknownLibrary			TextureLibrary;
	// функции дл€ работы с библиотекой текстур
	cUnknownClass* GetTextureAnimation(char *NameTexture,char *NameOpacity=0);	// возвращает cTextureAnimation
	void DeleteTexture(cUnknownClass *Texture);									// дл€ удалени€ текстур, выделенных библиотекой
};

#endif //__MANAGERTEXTURE_H__