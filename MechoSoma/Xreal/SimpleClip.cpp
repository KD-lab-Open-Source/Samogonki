#include "StdAfx.h"
#include "SimpleClip.h"
#include "Mesh3ds.h"
#include "CycledMath.h"
#include "IVisGeneric.h"
#include "terra.h"
#include "xreal_utl.h"
#include "SST_Reader.h"
#include "CameraPrm.h"
#include "Mechosoma.h"
#include "Statistics.h"
#include "DebugPrm.h"
#include "Params.h"

/////////////////////////////////////
//	Triangle
////////////////////////////////////
Spot* Triangle::current_spot = 0;
int Triangle::current_property = Triangle::ForTestCollision | Triangle::ForRayCasting;

int Triangle::set(const Vect3f& p0, const Vect3f& p1,const Vect3f& p2)
{
	Vect3f n = (p2 - p0) % (p1 - p0);
	float norm = n.norm();
	if(norm < 1e-5f)
		return 0;
	n /= norm;
	fPlane::set(n, p0);

	Vect3f n0 = (p1 - p0) % n;
	n0.normalize();
	borders[0].set(n0, p0);

	Vect3f n1 = (p2 - p1) % n;
	float norm1 = n1.norm();
	if(norm1 < 1e-5f)
		return 0;
	n1 /= norm1;
	borders[1].set(n1, p1);

	Vect3f n2 = (p0 - p2) % n;
	n2.normalize();
	borders[2].set(n2, p2);

	len2_invs[0] = 1./(p1 - p0).norm2();
	len2_invs[1] = 1./(p2 - p1).norm2();
	len2_invs[2] = 1./(p0 - p2).norm2();

	verts[0] = p0;
	verts[1] = p1;
	verts[2] = p2;

	spot_ = current_spot;
	property_ = current_property;
	setSurface(Soil);
	return 1;
}

Triangle::~Triangle()
{
	if(brush)
		delete brush;
}

int Triangle::belongSquare(int x0_, int y0_, int sx, int sy) const
{
	if(brush)
		return brush -> belongSquare(x0_, y0_, sx, sy);


	// Separating axis algorithm

	float x0 = x0_;	
	float y0 = y0_;	
	float x1 = x0_ + sx;	
	float y1 = y0_ + sy;	

	const float& tx0 = verts[0].x;
	const float& ty0 = verts[0].y;
	const float& tx1 = verts[1].x;
	const float& ty1 = verts[1].y;
	const float& tx2 = verts[2].x;
	const float& ty2 = verts[2].y;

	// Square's Left side
	if(tx0 - x0 < 0 && tx1 - x0 < 0 && tx2 - x0 < 0)
		return 0;

	// Square's Upper side
	if(ty0 - y0 < 0 && ty1 - y0 < 0 && ty2 - y0 < 0)
		return 0;

	// Square's Right side
	if(tx0 - x1 > 0 && tx1 - x1 > 0 && tx2 - x1 > 0)
		return 0;

	// Square's Lower side
	if(ty0 - y1 > 0 && ty1 - y1 > 0 && ty2 - y1 > 0)
		return 0;

	// Test to triangle's edges
	for(int i = 0; i < 3; i++){
		int j = (i + 1) % 3;
		const float& vx = verts[i].x;
		const float& vy = verts[i].y;

		float nx, ny;
		if(normal().z < 0){
			nx = -(verts[j].y - vy);
			ny = verts[j].x - vx;
			}
		else{
			nx = verts[j].y - vy;
			ny = -(verts[j].x - vx);
			}

		if(      (x0 - vx)*nx + (y0 - vy)*ny < 0 
		  && (x1 - vx)*nx + (y0 - vy)*ny < 0
		  && (x0 - vx)*nx + (y1 - vy)*ny < 0
		  && (x1 - vx)*nx + (y1 - vy)*ny < 0)
			return 0;
		}

	return 1;
}

int Triangle::isInside(float x0, float y0) const
{
	// Separating axis algorithm
	// Test to triangle's edges
	for(int i = 0; i < 3; i++){
		int j = (i + 1) % 3;
		const float& vx = verts[i].x;
		const float& vy = verts[i].y;

		float nx = verts[j].y - vy;
		float ny = -(verts[j].x - vx);

		if((x0 - vx)*nx + (y0 - vy)*ny < -0.01f)
			return 0;
		}

	return 1;
}

void Triangle::show() const
{
	if(brush && triangle_show_brush){
		if(triangle_show_grid_area)
			tri_map -> show_area(*this, XCOL(CYAN, 190));
		brush -> show();
		}

	if(((triangle_show_m3d_objects && property() & M3dObject) || (triangle_show_terrain && !(property() & M3dObject))) &&
		((triangle_show_ForTestCollision && property() & ForTestCollision) || 
		(triangle_show_ForRayCasting && property() & ForRayCasting) ||
		(triangle_show_ForTestSpot && property() & ForTestSpot))){ 
			if(triangle_show_grid_area)
				tri_map -> show_area(*this, XCOL(CYAN, 190));
			if(!triangle_show_by_normals || triangle_show_by_normals == 1 &&  normal().z > 0 || triangle_show_by_normals == -1 &&  normal().z <= 0){
				show_vector(verts[0], verts[1], verts[2], XCOL(CYAN, 255));
				if(triangle_show_normal_by_color)
					show_vector((verts[0] + verts[1] + verts[2])/3, XCOL(normal().z > 0 ? GREEN : RED, 190));
				for(int i = 0;i < 3;i++)
					if(!(verts[i].xi() & 127) && !(verts[i].yi() & 127))
						show_vector(verts[i], BLUE);
				}
			}
}	

void Triangle::show(const MatXf& Xlg, int color) const
{
	Vect3f v0, v1, v2;
	Xlg.xformPoint(verts[0], v0);
	Xlg.xformPoint(verts[1], v1);
	Xlg.xformPoint(verts[2], v2);
	show_vector(v0, v1, v2, color);
}	

//////////////////////////////////////////////////////////////////////////
//	Triangle Cliping
//////////////////////////////////////////////////////////////////////////

inline void Triangle::project_to_edge(const Vect3f& point, int i, int j, Vect3f& cp) const
{
	const Vect3f& e = verts[i];
	Vect3f a, d;
	a.sub(point, e);
	d.sub(verts[j], e);
	float t = dot(d, a)*len2_invs[i];
	if(t <= 0)
		cp = e;
	else
		if(t >= 1)
			cp = verts[j];
		else
			cp = e + d*t;
}

int Triangle::clip(const Vect3f& point, float radius, float& distance, Vect3f& cp, Vect3f& n) const
{
	float face_dist = dist(point);
	float face_dist_abs = fabs(face_dist) - radius;
	if(face_dist_abs > 0)
		return 0;
	
	float d = borders[0].dist(point);
	if(d < 0){
		if(d < -radius)
			return 0;
		project_to_edge(point, 0, 1, cp);
		}
	else
		if((d = borders[1].dist(point)) < 0){
			if(d < -radius)
				return 0;
			project_to_edge(point, 1, 2, cp);
			}
		else
			if((d = borders[2].dist(point)) < 0){
				if(d < -radius)
					return 0;
				project_to_edge(point, 2, 0, cp);
				}
			else {
				cp.scale(normal(), -face_dist);
				cp += point;
				n = normal();
				if(face_dist < 0)
					n.negate();
				distance = face_dist_abs; 
				return 1;
				}

	n.sub(point, cp);
	d = n.norm();
	if(fabs(d) < DBL_EPS)
		return 0;
	n /= d;
	if((d -= radius) <= 0){
		distance = d;
		return 1;
		}
	else 
		return 0;
}

int Triangle::clipPenalty(const Vect3f& point, float radius, float& penetration, Vect3f& cp, Vect3f& n) const
{
	float face_dist = dist(point);
	penetration = radius - face_dist;
	if(penetration <= 0)
		return 0;
	
	float d = borders[0].dist(point);
	if(d < -triangle_clip_epsilon){
		if(d < -radius)
			return 0;
		project_to_edge(point, 0, 1, cp);
		}
	else
		if((d = borders[1].dist(point)) < -triangle_clip_epsilon){
			if(d < -radius)
				return 0;
			project_to_edge(point, 1, 2, cp);
			}
		else
			if((d = borders[2].dist(point)) < -triangle_clip_epsilon){
				if(d < -radius)
					return 0;
				project_to_edge(point, 2, 0, cp);
				}
			else {
				cp.scale(normal(), -face_dist);
				cp += point;
				n = normal();
				return 1;
				}

	penetration = radius - point.distance(cp);
	if(penetration > 0){
		n = normal();
		return 2;
		}
	else 
		return 0;
}

int Triangle::clipDisk(const Vect3f& center, const Vect3f& dn, float radius, float& penetration, Vect3f& pd, Vect3f& n) const
{
	if(fabs(dist(center)) - radius > 0)
		return 0;

	const Vect3f& pn = normal();
 
	float k = dot(pn,dn);
	if(k > 1-1E-3 || k < -1+1E-3)
		return 0;

	// let rv=pn-k*dn [=> rv.dn=0];
	// v points from the point on the disk deepest
	// into the plane towards the center of the disk	
	Vect3f rv = pn - k*dn;
	rv.normalize(-radius);
	pd = center + rv;
	penetration = -dist(pd);
	if(penetration <= 0)
		return 0;
	Vect3f pp = pd + penetration*pn;
	for(int i = 0; i < 3; i++)
		if(borders[i].dist(pp) < -triangle_clip_epsilon)
			return 0;
	n = normal();
	return 1;
}

float Triangle::distance(const Vect3f& point) const
{
	Vect3f cp;
	if(borders[0].dist(point) < 0)
		project_to_edge(point, 0, 1, cp);
	else
		if(borders[1].dist(point) < 0)
			project_to_edge(point, 1, 2, cp);
		else
			if(borders[2].dist(point) < 0)
				project_to_edge(point, 2, 0, cp);
			else 
				return dist(point);

	return point.distance(cp);
}


//////////////////////////////////////////////////////////////////////////////
//	Triangle Map :  Constructor
//////////////////////////////////////////////////////////////////////////////
void TriangleMap::clear() 
{ 
	Grid::clear(); 
	List::clear(); 
	spot_map.clear();
}


//////////////////////////////////////////////////////////////////////////////
//	Triangle Map :  Load
//////////////////////////////////////////////////////////////////////////////
void TriangleMap::add(const Vect3f& p0, const Vect3f& p1, const Vect3f& p2, Triangle::Surface surface, int clockwise) 
{ 
	if(p0 == p1 || p1 == p2 || p2 == p0)
		return;

	Triangle tri; 
	if((clockwise ? tri.set(p0, p1, p2) : tri.set(p0, p2, p1) == 1)){ 
		tri.setSurface(surface); 
		push_front(tri); 
		Grid::insert(front(), front().getBound()); 
		}
}
int vmapOpenResource(char* fname,XStream& fh);

void TriangleMap::load(char* name)
{
	//ifstream is(name);
	XStream ff;
	vmapOpenResource(name, ff);
	int len = ff.size();
	XBuffer is(len + 1);
	ff.read(is.address(), len);
	is[len] = 0;
	ff.close();

	// read vertices
	vector<Vect3f> verts;
	verts.reserve(20000); 
	while (1) {
		char c = 0;
		do is > c; while(isspace(c));
		if(c != 'v'){
			--is;
			break;
			}
		Vect3f coords;
		is >= coords;
		verts.push_back(coords);
		}

	// read faces
	while (1) {
		char c = 0;
		do is > c; while(isspace(c));
		if(c != 'f')
			break;
		int i0, i1, i2;
		is >= i0 >= i1 >= i2;
		i0--; i1--; i2--;
		//if(((verts[i1] - verts[i0]) % (verts[i2] - verts[i0])).z >= 0)
		add(verts[i0], verts[i1], verts[i2], Triangle::Soil, 0);
		}
}

///////////////////////////////////////////////////////////////////////////////////////////
//		Spot Reading
///////////////////////////////////////////////////////////////////////////////////////////

struct SpotRecord : UsualRecord 
{
	char type;
	int m3d_number;
	void read(XBuffer& b) 
	{	
		UsualRecord::read(b);
		while(isspace(b()))
			++b;
		b > type >= m3d_number; 
	}
};

typedef SST_Reader<SpotRecord> SpotReader;

void TriangleMap::loadSpots(char* name)
{
	SpotReader spots(name);
	SpotReader::iterator si;
	FOR_EACH(spots, si)
		spot_map.insert(pair<const int, Spot>(si -> m3d_number, Spot(si -> type, si -> x, si -> y)));
}

void TriangleMap::load(cM3D& M3D)
{
	cMesh* mesh = M3D.First();
	while(mesh){

		int triangle_mask = 0;
		if(mesh -> description && strstr(mesh -> description, "D")){
			if(mesh -> Bound){
				Triangle::set_current_spot(0, Triangle::ForTestCollision | Triangle::M3dObject);
				load(*mesh -> Bound);
				}
			else
				triangle_mask |= Triangle::ForTestCollision;
			if(camera_enable_static_objects_clipping)
				triangle_mask |= Triangle::ForRayCasting;
			}

		if(mesh -> description && strstr(mesh -> description, "C"))
			triangle_mask |= Triangle::ForRayCasting;

		if(triangle_mask & Triangle::ForRayCasting){ // Insert as a brush of triangles
			addBrush(mesh -> Bound ? *mesh -> Bound : *mesh);
			triangle_mask ^= Triangle::ForRayCasting;
			}

		SpotMap::iterator i = spot_map.find(M3D_KIND(mesh -> Type));
		if(i != spot_map.end()){
			Spot* s = &i -> second;
			if(s -> type == Spot::Jump)
				s -> z = H(s -> x, s -> y);
			else{
				(Vect3f&)*s = getDist(*s, mesh -> GlobalMatrix.trans());
				s -> z = 0;
				s -> normalize();
				}
			Triangle::set_current_spot(s, Triangle::ForTestSpot | Triangle::M3dObject);
			load(!mesh -> Bound ? *mesh : *mesh -> Bound);
			}

		if(triangle_mask){
			Triangle::set_current_spot(0, triangle_mask | Triangle::M3dObject);
			load(*mesh);
			}
		
		mesh = M3D.Next();
		}

	Triangle::set_current_spot();
}			
	
void TriangleMap::load(cMesh& mesh)
{
	vector<Vect3f> verts;
	vector<int> faces;
	mesh.GetGlobalGeometry(verts, faces, 1);
	vector<int>::iterator fi;
	FOR_EACH(faces, fi){
		int i0 = *fi;
		int i1 = *++fi;
		int i2 = *++fi;
		add(verts[i0], verts[i1], verts[i2], Triangle::Soil,  1);
		}
}

void TriangleMap::load(cInterfaceVisGeneric* IVG, int disable_Ice)
{
	int attr;
	Vect3f p0, p1, p2;				
	IVG -> SetWorldPolygonFirst();
	while(!IVG -> GetWorldPolygonNext(p0, p1, p2, attr)){
		Triangle::Surface surface = Triangle::Soil;
		switch(GetTer(attr)){
			case 0:
			case 1:
				surface = Triangle::Asphalt;
				break;
			case 2:
				surface = Triangle::Soil;
				break;
			case 15:
				surface = disable_Ice ? Triangle::Asphalt : Triangle::Ice;
				break;
			default:
				surface = Triangle::Sand;
				break;
			}
		add(p0, p1, p2, surface, 1);
		}
}



//////////////////////////////////////////////////////////////////////////////
//	Test Spot
//////////////////////////////////////////////////////////////////////////////

class TestSpotOp
{
	Sphere& sphere;
	const MatXf& X12;
	Spot* spot;
public:

	TestSpotOp(Sphere& sphere_, const MatXf& X12_) :
		sphere(sphere_), X12(X12_) { spot = 0; }

	void operator()(const Triangle& triangle, float dx, float dy)
	{
		if(!(triangle.property() & Triangle::ForTestSpot))
			return;

		float d;
		Vect3f cp2, normal, uncycled_r(sphere);
		uncycled_r.x += dx; uncycled_r.y += dy;
		if(triangle.clip(uncycled_r, sphere.radius, d, cp2, normal))
			spot = triangle.spot();
	}

	Spot* result() const { return spot; }
};

Spot* TriangleMap::testSpot(SphereList& sphere, const MatXf& X12)
{
	SphereList::iterator si;
	FOR_EACH(sphere, si){
		Sphere& s = *si;
		s.convert(X12);
		TestSpotOp op(s, X12);
		scan(s.getBound(), op);
		if(op.result())
			return op.result();
		}
	return 0;
}		

//////////////////////////////////////////////////////////////////////////////
//	Triangle Map :  Closest Triangle Searching
//////////////////////////////////////////////////////////////////////////////

class ClosetTriangleSearchOp
{
	Vect3f point;
	float min_dist;
	const Triangle* min_tri;
public:

	ClosetTriangleSearchOp(const Vect3f& point_) 
	: point(point_)
	{ 
		min_dist = DBL_INF;
		min_tri = 0;
	}

	void operator()(const Triangle& triangle, float dx, float dy)
	{
		if(!(triangle.property() & Triangle::ForTestCollision))
			return;

		Vect3f uncycled_r(point);
		uncycled_r.x += dx; uncycled_r.y += dy;
		float d = fabs(triangle.distance(uncycled_r));
		if(d < min_dist){
			min_dist = d;
			min_tri = &triangle;
			}
	}

	operator const Triangle*() { return min_tri; }
};

const Triangle* TriangleMap::closest_triangle(const Vect3f& point, float radius) const
{
	ClosetTriangleSearchOp op(point);
	scan(iRectangle(round(point.x - radius), round(point.y - radius), round(point.x + radius), round(point.y + radius)), op);
	return op;
}		

////////////////////////////////////////////////////////////////////////////////////////////
//		getPlacementPose
////////////////////////////////////////////////////////////////////////////////////////////
float Triangle::area() const
{
	Vect3f p1 = verts[1] - verts[0];
	Vect3f p2 = verts[2] - verts[0];
	Vect3f p_t, p_n;
	decomposition(p2, p1, p_t, p_n);
	return p_n.norm()*p2.norm()/2;
}	

MatXf TriangleMap::getPlacementPose(const Vect3f& center, int disabled_triangles_mask)
{
	const float dz = 0; //приподнимает над поверхностью
	const Triangle* tri = 0;
	float s, smax = 0;
	SList& list = (*this)(center.xi(), center.yi());
	SList::iterator i;
	FOR_EACH(list, i)
		if(!((*i) -> property() & disabled_triangles_mask) && smax < (s = (*i) -> area())){
			smax = s;
			tri = *i;
			}
	if(!tri){
		assert(0);
		return MatXf(Mat3f::ID, center);
		}
	const Vect3f& n = tri -> normal();
	Vect3f cross = Vect3f::K % n;
	return MatXf( Mat3f(cross, acos(n.z)), Vect3f(center.x, center.y, dz - (tri -> offset() + n.x*center.x + n.y*center.y)/n.z) );
}

//MatXf getPlacementPose(const Vect3f& center)
//{
//	return tri_map -> getPlacementPose(center);
//}

class ClosetNonVerticalTriangleSearchOp
{
	Vect3f point;
	float min_dist;
	const Triangle* min_tri;
public:

	ClosetNonVerticalTriangleSearchOp(const Vect3f& point_) 
	: point(point_)
	{ 
		min_dist = DBL_INF;
		min_tri = 0;
	}

	void operator()(const Triangle& triangle, float dx, float dy)
	{
		if(!(triangle.property() & Triangle::ForTestCollision) || triangle.normal().z < 1e-4)
			return;

		Vect3f uncycled_r(point);
		uncycled_r.x += dx; uncycled_r.y += dy;
		float d = fabs(triangle.distance(uncycled_r));
		if(d < min_dist){
			min_dist = d;
			min_tri = &triangle;
			}
	}

	operator const Triangle*() { return min_tri; }
};

MatXf getPlacementPose(const Vect3f& center)
{
	const float dz = 3; //приподнимает над поверхностью
	float radius = 20; // радиус окрестности для поиска (чем больше - тем медленней)
	const Triangle* tri;
	while(1){
		ClosetNonVerticalTriangleSearchOp op(center);
		tri_map -> scan(iRectangle(round(center.x - radius), round(center.y - radius), round(center.x + radius), round(center.y + radius)), op);
		if((tri = op) != 0)
			break;
		radius *= 2;
		}
	const Vect3f& n = tri -> normal();
	Vect3f cross = Vect3f::K % n;
	MatXf m( Mat3f(cross, acos(n.z)), Vect3f(center.x, center.y, dz - (tri -> offset() + n.x*center.x + n.y*center.y)/n.z) );
	int hw = GetWH(XCYCL(center.xi()), YCYCL(center.yi())) + dz;
	if(m.trans().z < hw){
		m.trans().z = hw + get_placement_pose_water_dz;;
		m.rot() = Mat3f::ID;
		}
	return m;
}

MatXf getPlacementGroundPose(const Vect3f& center)
{
	return tri_map -> getPlacementPose(center, Triangle::M3dObject);
}

//////////////////////////////////////////////////////////////////////////////
//	Triangle Map :  Height examination
//////////////////////////////////////////////////////////////////////////////

float TriangleMap::H(int x0, int y0)
{
	SList& cell = (*this)(x0, y0);
	SList::const_iterator ti;
	float zmax = 0;
	FOR_EACH(cell, ti){
		const Triangle& tri = **ti;
		float x = x0 - ti -> dx();
		float y = y0 - ti -> dy();
		if(tri.isInside(x, y)){
			const Vect3f& n = tri.normal();
			if(fabs(n.z) < DBL_EPS)
				continue;

			float z = -(tri.offset() + n.x*x + n.y*y)/n.z;
			if(z > zmax)
				zmax = z;
			}
		}
	return max((float)GetWH(XCYCL(x0), YCYCL(y0)), zmax);
} 

float TriangleMap::H(int x0, int y0, float upper_bound)
{
	SList& cell = (*this)(x0, y0);
	SList::const_iterator ti;
	float zmax = 0;
	float zbest = 0;
	FOR_EACH(cell, ti){
		const Triangle& tri = **ti;
		float x = x0 - ti -> dx();
		float y = y0 - ti -> dy();
		if(tri.isInside(x, y)){
			const Vect3f& n = tri.normal();
			if(fabs(n.z) < DBL_EPS)
				continue;

			float z = -(tri.offset() + n.x*x + n.y*y)/n.z;
			if(z > zmax)
				zmax = z;
			if(z < upper_bound && z > zbest)
				zbest = z;
			}
		}
	return zbest ? zbest : zmax;
} 

float triangleMapH(int x, int y, float z)
{
	return tri_map -> H(x,y,z);
}

float TriangleMap::ceiling(int x0, int y0, float lower_bound)
{
	SList& cell = (*this)(x0, y0);
	SList::const_iterator ti;
	float zbest = FLT_INF;
	FOR_EACH(cell, ti){
		const Triangle& tri = **ti;
		float x = x0 - ti -> dx();
		float y = y0 - ti -> dy();
		if(1 || tri.isInside(x, y)){
			const Vect3f& n = tri.normal();
			if(n.z > -0.1)
				continue;

			float z = -(tri.offset() + n.x*x + n.y*y)/n.z;
			if(z > lower_bound && zbest > z)
				zbest = z;
			}
		}
	return zbest;
} 


//////////////////////////////////////////////////////////////////////////////
//	Triangle Map :  RayCasting
//////////////////////////////////////////////////////////////////////////////
int Triangle::clip_ray(const Vect3f& origin, const Vect3f& direct, float ray_radius, Vect3f& point, cCamera* camera) const
{
	if(brush)
		return brush -> clip_ray(origin, direct, ray_radius, point, camera);
	
	float t = dot(normal(), direct);
	if(fabs(t) < 1e-8)
		return 0; 
	t = -dist(origin)/t;
	if(t < clip_ray_t_min)
		return 0;

	point.scaleAdd(origin, direct, t);

	Vect3f cp;
	float d = borders[0].dist(point);
	if(d < 0){
		if(d < -ray_radius)
			return 0;
		project_to_edge(point, 0, 1, cp);
		}
	else
		if((d = borders[1].dist(point)) < 0){
			if(d < -ray_radius)

				return 0;
			project_to_edge(point, 1, 2, cp);
			}
		else
			if((d = borders[2].dist(point)) < 0){
				if(d < -ray_radius)
					return 0;
				project_to_edge(point, 2, 0, cp);
				}
			else
				return 1;

	if(point.distance2(cp) < sqr(ray_radius))
		return 1;
	else 
		return 0;
}

class RayCastingOp
{
	int intersect_;
	Vect3f origin, direct, nearest_point;
	float min_dist;
	cCamera* camera;
	float ray_radius;

public:

	RayCastingOp(const Vect3f& origin_, const Vect3f& direct_, float ray_radius_, cCamera* camera_) 
		: origin(origin_), direct(direct_), camera(camera_), ray_radius(ray_radius_)
		{ 
			nearest_point = origin + direct; 
			intersect_ = 0;
			min_dist = 1e+10;
		}

	int operator()(const Triangle& triangle, float dx, float dy)
	{
		if(!(triangle.property() & Triangle::ForRayCasting))
			return 1;

		float d;
		Vect3f point;
		Vect3f uncycled(origin);
		uncycled.x += dx; uncycled.y += dy;
		if(triangle.clip_ray(uncycled, direct, ray_radius, point, camera) && (d = uncycled.distance(point)) < min_dist){
			point.x -= dx; point.y -= dy;
			nearest_point = point;
			min_dist = d;
			intersect_ = 1;
			}
		return 1;
	}

	const Vect3f& result(){ return nearest_point; }
	int intersect() const { return intersect_; }
};

int TriangleMap::cast_ray(const Vect3f& origin, Vect3f& offset, float ray_radius, cCamera* camera)
{
	RayCastingOp op(origin, offset, ray_radius, camera);
	Vect3f final = origin + offset;
	scan_line(origin.x, origin.y, final.x, final.y, op);
	if(op.intersect()){
		offset = op.result() - origin;
		return 1;
		}
	else
		return 0;
}		

int CastRayForS2G(const Vect3f& origin, Vect3f& offset, cCamera* camera) // 1 - ray intersects nearest triangle and offset_returned < offset
{
	float Lh2o = vMap -> LevelH2O;
	
	Vect3f point = origin;
	if(fabs(offset.z) > 1e-3)
		point += offset*((Lh2o - origin.z)/offset.z);
	point.z = Lh2o;

	int log =  tri_map -> cast_ray(origin, offset, S2G_clip_ray_radius, camera);
	
	int wh = GetWH(XCYCL(point.xi()), YCYCL(point.yi()));
	if(wh){
		point -= origin;
		if(!log || point.norm2() < offset.norm2()){
			offset = point;
			return 1;
			}
		}
	return log;
}

//////////////////////////////////////////////////////////////////////
//			Brushes
//////////////////////////////////////////////////////////////////////
void Triangle::setBrush(cMesh& mesh)
{
	brush = new TriangleBrush(mesh);
	float r = mesh.rmaxTotal();
	Vect3f dx(r, 0, 0);
	Vect3f dy(0, r, 0);
	Vect3f c = mesh.GlobalMatrix.trans();
	c.z = 0;
	set(c - dx - dy, c + dx - dy, c - dx + dy);
	property_ |= RootOfBrush;
}

void TriangleMap::addBrush(cMesh& mesh)
{
	Triangle::set_current_spot(0, Triangle::ForRayCasting | Triangle::M3dObject);
	push_front(Triangle()); 
	front().setBrush(mesh);
	Grid::insert(front(), front().getBound()); 
	Triangle::set_current_spot();
}

TriangleBrush::TriangleBrush(cMesh& mesh)
: pose(mesh.GlobalMatrix)
{	
	cMesh* parent = mesh.GetGeneralParent();
	MatXf X = parent -> GlobalMatrix;
	parent -> SetPosition(MatXf::ID);
	MatXf Xc = mesh.GlobalMatrix;
	Xc.invert();
	parent -> SetPosition(Xc);

	vector<Vect3f> verts;
	vector<int> faces;
	mesh.GetGlobalGeometry(verts, faces, 0);
	vector<int>::iterator fi;
	FOR_EACH(faces, fi){
		int i0 = *fi;
		int i1 = *++fi;
		int i2 = *++fi;
		Triangle tri;
		if(tri.set(verts[i0], verts[i1], verts[i2]))
			push_back(tri);
		}

	parent -> SetPosition(X);

	float r = mesh.rmaxReal();
	const Vect3f& c0 = pose.trans();
	bound = iRectangle(c0.x - r, c0.y - r, c0.x + r, c0.y + r);
	if(mesh.Parent){
		float period, phase;
		parent -> GetAnimation(period, phase);
		for(float t = 0; t < 1; t += 0.025f){
			parent -> SetAnimation(0, t);
			Vect3f c = mesh.GlobalMatrix.trans();
			UNCYCLE(c, c0);
			bound += iRectangle(c.x - r, c.y - r, c.x + r, c.y + r);
			}
		parent -> SetAnimation(period, phase);
		}

#ifndef _FINAL_VERSION_
//	fout < "Brush:" < (char*)mesh.fname < "\t" <= size() < "\t" <= pose.trans();
#endif
}

int TriangleBrush::clip_ray(const Vect3f& origin_global, const Vect3f& direct_global, float ray_radius,Vect3f& point_global, cCamera* camera) const
{
	start_timer(TriangleBrush_clip_ray, STAT_CAMERA);
	MatXf mx = pose;
	WorldShareMapping((cMatrix&)mx, camera);
	Vect3f origin(origin_global), direct;
	mx.invXformPoint(origin);
	mx.invXformVect(direct_global, direct);

	int intersect = 0;
	float min_dist = 1e+10;
	Vect3f nearest_point;
	const_iterator i;
	FOR_EACH(*this, i){
		const Triangle& triangle = *i;
		float d;
		Vect3f point;
		if(triangle.clip_ray(origin, direct, ray_radius, point, camera) && (d = origin.distance(point)) < min_dist){
			nearest_point = point;
			min_dist = d;
			intersect = 1;
			}
		}
	if(intersect)
		pose.xformPoint(nearest_point, point_global);
	stop_timer(TriangleBrush_clip_ray, STAT_CAMERA);
	return intersect;
}

void TriangleBrush::show() const
{
	show_vector(pose.trans(), CYAN);
	const_iterator i;
	FOR_EACH(*this, i)
		i -> show(pose, XCOL(CYAN, 100));
}		
