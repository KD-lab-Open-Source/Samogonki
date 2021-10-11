/////////////////////////////////////////////////////////////////////////////////////////////////
//	PolyhedronLibrary
/////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Polyhedron.h"
#include "Dispatcher.h"
#include "PolyhedronLibrary.h"
#include <list>

/*	
PolyhedronLibrary::PolyhedronLibrary(class cMeshLibrary& mesh_lib)
{
	const TYPE = M3D_ENGINE | M3D_FRONT | M3D_RF_WHEEL | M3D_RB_WHEEL | 	                              
		M3D_LF_WHEEL | 	M3D_LB_WHEEL | M3D_EFFECT;
	cMesh* mesh = mesh_lib.First();
	while(mesh){
		if(mesh -> Type & TYPE){
			Polyhedron* poly = (*this)[mesh -> Type] = new Polyhedron;
			//poly -> setMesh(*mesh);
			}
		mesh = mesh_lib.Next();
		}
}			
*/

PolyhedronLibrary::PolyhedronLibrary(class cMeshLibrary& mesh_lib_)
: mesh_lib(mesh_lib_)
{
	cMesh* mesh = mesh_lib.First();
	while(mesh){
		int type = M3D_TYPE(mesh -> Type);
		switch(type){
			case M3D_ENGINE:
			case M3D_FRONT:
			case M3D_RF_WHEEL:
			case M3D_RB_WHEEL:
			case M3D_LF_WHEEL:
			case M3D_LB_WHEEL:
			case M3D_XREAL: {
				Polyhedron* poly = new Polyhedron;
				poly -> setMesh(*mesh);
				insert( value_type(mesh -> Type, poly) );
				break;
				}
			}
		mesh = mesh_lib.Next();
		}
}

PolyhedronLibrary::~PolyhedronLibrary()
{
	clear();
	assert(!Polyhedron::instancies);
}

Polyhedron* PolyhedronLibrary::operator[](int mesh_type)
{
	iterator i = find(mesh_type);
	if(i != end())
		return i -> second;
	cMesh *mesh = mesh_lib.Find(mesh_type);
	assert(mesh);
	Polyhedron* poly = new Polyhedron;
	poly -> setMesh(*mesh);
	insert( value_type(mesh_type, poly) );
	return poly;
}
