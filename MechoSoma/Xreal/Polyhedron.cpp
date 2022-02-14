#include <unordered_map>

#include "StdAfx.h"
#include "Polyhedron.h"
#include "Mesh3ds.h"
#include "Xreal_utl.h"
#include "mechosoma.h"

template<>
struct std::hash<const Vertex*> {
  size_t operator()(const Vertex* __x) const { return size_t(__x); }
};

Polyhedron::Polyhedron(const Polyhedron& poly, const Vect3f& scale, const Vect3f& displace)
{
	std::unordered_map<const Vertex*, Vertex*> indexedVerts(poly.verts().size()*2);
	list<Vertex>::const_iterator vi;
	FOR_EACH(poly.verts(), vi)
		indexedVerts[&*vi] = addVertex(vi -> name(), vi -> coords()*scale + displace );
		
	vector<Vertex*> facelist(MAX_VERTS_PER_FACE);
	list<Face>::const_iterator fi;
	FOR_EACH(poly.faces(), fi){
		facelist.clear();
		list<FaceConeNode>::const_iterator fcn;
		FOR_EACH(fi -> cone, fcn){
			const Edge* e = fcn -> nbr;
			facelist.push_back(indexedVerts[e -> left == &*fi ? e -> tail : e -> head]);
			}
		addFace(fi -> name(), facelist, 0);
		}

	//check();
	
	instancies++;	
	handleCount = 0;
	compVolInts();
}	

static inline void quantize(float& x, float eps)
{
	x = ceil(x/eps)*eps;
}
static inline void quantize(Vect3f& v, float eps)
{
	quantize(v.x, eps);
	quantize(v.y, eps);
	quantize(v.z, eps);
}

void Polyhedron::setMesh(cMesh& mesh, int recursive)
{
	string name = mesh.GetFileName() ? mesh.GetFileName().ptr() : mesh.GetGeneralParent() -> GetFileName().ptr();
	int dot_pos = name.find(".");
	int slash_pos = name.rfind("\\", dot_pos);
	name.replace(dot_pos, 4, recursive ? string(".hul") : string("_") + string(mesh.GetName()) + ".hul");
	name.replace(slash_pos, 1, "\\Hulls\\");
	Vect3f scale(mesh.xmaxTotal() - mesh.xminTotal(), mesh.ymaxTotal() - mesh.yminTotal(), mesh.zmaxTotal() - mesh.zminTotal());

#ifndef _FINAL_VERSION_
	if(check_command_line("build_hull")){
		vector<Vect3f> verts;
		vector<int> faces;
		mesh.GlobalMatrix = MatXf::ID;
		mesh.GetGlobalGeometry(verts, faces, recursive);

		Vect3f scale_inv = Vect3f::ID/scale;
		int vertCounter = 0;
		vector<Vect3f>::iterator i;
		FOR_EACH(verts, i){
			Vect3f coords(*i);
			coords *= scale_inv;
			quantize(coords, 1e-3f);
			
			Vertex* v = 0;
			list<Vertex>::iterator vi;
			FOR_EACH(verts_,vi)
				if(coords.distance(vi -> coords()) < 0.01){
					v = &*vi;
					break;
					}
			if(!v){
				VertFaceName vertName;
				sprintf(vertName, "v%d", vertCounter++);
				v = addVertex(vertName, coords);
				}
			}
			
		buildHull();
		compVolInts();
		//check();

		static ofstream ff("built_hulls", ios::app);

		XStream in(0);
		int do_save = 1;
		if(in.open(name.c_str(), XS_IN)){
			float vol;
			in > vol;
			if(vol == vol_)
				do_save = 0;
			else
				ff << vol << "\t" << vol_ << "\n";
			}
	
		if(do_save)
			save(name.c_str());

		ff << (do_save ? "SAVE: " : "    : ") << name.c_str() << "\n";
		}
#endif // _FINAL_VERSION_

	load(name.c_str(), scale);
}

void Polyhedron::save(const char* fname)
{
	XStream out(fname, XS_OUT);

	out < vol_< mov1_< mov2_ < pov_< rad_ < rmax_;

	int counter = 0;
	out < (int) verts().size();
	list<Vertex>::iterator vi;
	FOR_EACH(verts_, vi){
		out < vi -> coords_;
		vi -> index = counter++;
		}
	
	out < (int) faces().size();
	list<Face>::const_iterator fi;
	FOR_EACH(faces(), fi){
		out < (int) fi -> cone.size();
		list<FaceConeNode>::const_iterator fcn;
		FOR_EACH(fi -> cone, fcn){
			const Edge* e = fcn -> nbr;
			out < (e -> left == &*fi ? e -> tail -> index: e -> head -> index);
			}
		}
}

void Polyhedron::load(const char* fname, const Vect3f& scale)
{
	verts_.clear();
	edges_.clear();
	faces_.clear();

	XStream ff(1);
	if(!m3dOpenResource((char*)fname, ff))
		ErrH.Abort(fname);

	int len = ff.size();
	XBuffer in(len);
	ff.read(in.address(), len);
	ff.close();

	in > vol_ > mov1_ > mov2_ > pov_ > rad_ > rmax_;
	
	int n_verts;
	in > n_verts;
	std::unordered_map<int, Vertex*> indexedVerts(n_verts);
	char name_buf[256];
	Vect3f coords;
	int i;
	for(i = 0;i < n_verts; i++){
		in > coords;
		coords *= scale;
		sprintf(name_buf, "v%i", i);
		indexedVerts[i] = addVertex(name_buf, coords);
		}
		
	int n_faces;
	in > n_faces;
	vector<Vertex*> facelist(MAX_VERTS_PER_FACE);
	for(i = 0;i < n_faces; i++){
		facelist.clear();
		int face_size;
		in > face_size;
		for(int v, j = 0;j < face_size; j++){
			in > v;
			facelist.push_back(indexedVerts[v]);
			}
		sprintf(name_buf, "f%i", i);
		addFace(name_buf, facelist, 0);
		}

	compVolInts();
}