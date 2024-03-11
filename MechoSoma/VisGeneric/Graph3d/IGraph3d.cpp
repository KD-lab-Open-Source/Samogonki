#include "IGraph3d.h"
#include "Graph3d_Direct3d.h"

cInterfaceGraph3d::cInterfaceGraph3d()
{
}
cInterfaceGraph3d::~cInterfaceGraph3d()
{
}

cInterfaceGraph3d* CreateIGraph3D(int InterfaceGraph3d)
{
	return new cGraph3dDirect3D();
}

