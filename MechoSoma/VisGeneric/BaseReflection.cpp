#include "BaseReflection.h"
#include "TileMap.h"

int cSurfaceReflection::inVolume(Vect3f &pos)
{
	float dx=GetShortDistX(pos.x,x());
	float dy=GetShortDistY(pos.y,y());
	return (0<=dx)&&(dx<(xsize*xstep))&&(0<=dy)&&(dy<(ystep*ysize));
}
