#include "assert.h"
#include "IGraph3d.h"
// TODO: @caiiiycuk investigate this looks like software render
#ifdef SOFTWARE_RENDER
#include "Graph3d_Software.h"
#endif
#include "Graph3d_Direct3d.h"

cInterfaceGraph3d::cInterfaceGraph3d()
{
}
cInterfaceGraph3d::~cInterfaceGraph3d()
{
}

cInterfaceGraph3d* CreateIGraph3D(int InterfaceGraph3d)
{
	switch(InterfaceGraph3d)
	{
#ifdef SOFTWARE_RENDER
		case GRAPH3D_SOFTWARE:
			return new cGraph3dSoftware;
#endif
		case GRAPH3D_DIRECT3D:
			return new cGraph3dDirect3D;
	}
	return 0;
}

