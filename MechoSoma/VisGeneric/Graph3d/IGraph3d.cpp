#include "assert.h"
#include "IGraph3d.h"
#include "Graph3d_Software.h"
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
		case GRAPH3D_SOFTWARE:
			return new cGraph3dSoftware;
		case GRAPH3D_DIRECT3D:
			return new cGraph3dDirect3D;
	}
	return 0;
}

