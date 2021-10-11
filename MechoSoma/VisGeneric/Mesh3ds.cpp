#include "Mesh3ds.h"
#include "Dispatcher.h"
#include "Object3d.h"
#include "TileMap.h"
#include "file3ds.h"
#include "Object.h"
#include "BaseObject.h"
#include "BaseDefine.h"

float	rAmbientInit=0.2f, gAmbientInit=0.2f, bAmbientInit=0.2f;
float	rDiffuseInit=0.8f, gDiffuseInit=0.8f, bDiffuseInit=0.8f;
sColor4f GlobalColorAmbientSky(0.3f,0.3f,0.3f,1.0f);
unsigned short *zBuffer=0;

#ifdef _MECHOSOMA_
#ifdef _DEBUG
extern void allocation_tracking(char* title);
void SaveMemoryMessage(char *message,int i=0)
{
	XBuffer buf;
	buf<message<=i;
	allocation_tracking(buf.address());
}
#else 
void SaveMemoryMessage(char *message,int i=0)	{}
#endif
#else 
void SaveMemoryMessage(char *message,int i=0)	{}
#endif

