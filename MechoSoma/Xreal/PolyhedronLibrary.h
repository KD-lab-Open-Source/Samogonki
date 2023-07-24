///////////////////////////////////////////////////////////////////////////////
//	class PolyhedronLibrary
///////////////////////////////////////////////////////////////////////////////
#ifndef __POLYHEDRON_LIBRARY_H__
#define __POLYHEDRON_LIBRARY_H__

#include <map>

class PolyhedronLibrary : public std::map<int, ShareHandle<Polyhedron> >
{
	class cMeshLibrary& mesh_lib;
public:
	PolyhedronLibrary(class cMeshLibrary& mesh_lib_);
	~PolyhedronLibrary();
	Polyhedron* operator[](int mesh_type);
};

extern PolyhedronLibrary* polyhedron_lib;

#endif // __POLYHEDRON_LIBRARY_H__
