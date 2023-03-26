
#ifndef _SIMPLE_CLIP_H_
#define  _SIMPLE_CLIP_H_

#include "Polyhedron.h"
#include "Grid2D.h"

class cCamera;

////////////////////////////////////
//	Plane
////////////////////////////////////
class fPlane 
{
	Vect3f normal_;   // plane = { p | < p , normal_ > + offset_ = 0 }
	float offset_;

public:
	const Vect3f& normal() const {return normal_;}
	const float& offset() const {return offset_;}

	void set(const Vect3f& normal, const Vect3f& thruPoint) {normal_ = normal; offset_ = -dot(thruPoint, normal);}

	// Compute signed distance from point to plane; assumes unit length normal_
	float dist(const Vect3f& point) const {return dot(normal_, point) + offset_;}
};

////////////////////////////////////////////////////////////////////////////////
//	Spot
////////////////////////////////////////////////////////////////////////////////
struct Spot : Vect3f // Jump - position, Acc - normalized direction
{
	enum Type { Jump, Acceleration } type;
	Spot(){}
	Spot(char type_, int x_, int y_) : type(type_ == 'j' || type_ == 'J' ? Jump : Acceleration), Vect3f(x_, y_, 0){}
};

////////////////////////////////////////////////////////////////////////////////
//
//	Triangle with Voronoi-regions
//
//           _  [3]  |  
//                  _  |      
//			p1_		     [2]
//			|      	  _	b1
//            [1]    b0 ->  	     _	  
//			|	[0]             _
//	    _  _  _ p0 _   _  b2  _	_  _ p2 _  _
//			|				    _ 
//	       [5]    |		  [4]				_  [6]
//
////////////////////////////////////////////////////////////////////////////////
class Triangle : public fPlane, public Grid2DElement
{
	typedef Vect3f Vect3fTriple[3];
public:
	enum Surface { Asphalt = 1, Soil = 2, Sand = 4, Ice = 8 };
	enum Property { ForTestCollision = 1, ForRayCasting = 2, ForTestSpot = 4, M3dObject = 8, RootOfBrush = 16 };

	Triangle(){ brush = 0; }
	Triangle(const Vect3f& p0, const Vect3f& p1,const Vect3f& p2){ brush = 0; set(p0, p1, p2); }
	~Triangle();
	int set(const Vect3f& p0, const Vect3f& p1,const Vect3f& p2); // 0 - points configuration is bad
	void setSurface(Surface surf) { surface_ = surf; }
	void setBrush(cMesh& mesh);
	int clip(const Vect3f& point, float radius, float& dist, Vect3f& cp, Vect3f& n) const;
	int clipPenalty(const Vect3f& point, float radius, float& penetration, Vect3f& cp, Vect3f& n) const;
	int clipDisk(const Vect3f& center, const Vect3f& dn, float radius, float& penetration, Vect3f& pd, Vect3f& n) const;
	int clip_ray(const Vect3f& origin, const Vect3f& direct, float ray_radius, Vect3f& point, cCamera* camera) const;
	void draw_to_z_buffer(class ZBuffer& z_buffer, const MatXf& sXg, cCamera* camera) const;
	float distance(const Vect3f& point) const;
	const Vect3fTriple& vertices() const { return verts; }
	Surface surface() const { return surface_; }
	int property() const { return property_; }
	Spot* spot() const { return spot_; }

	iRectangle getBound() const; // Bounding Rectangle
	int belongSquare(int x0, int y0, int sx, int sy) const; // checks if triangle intersects square 
	int isInside(float x0, float y0) const;
	float area() const;
	void show() const;
	void show(const MatXf& Xlg, int color) const;

private:
	Vect3f verts[3];
	fPlane borders[3];
	float len2_invs[3];
	Surface surface_;
	Spot* spot_;
	int property_;

	static Spot* current_spot;
	static int current_property;
	static void set_current_spot(Spot* spot = 0, int property = ForTestCollision | ForRayCasting){ current_spot = spot; current_property = property; }

	class TriangleBrush* brush;

	friend class TriangleMap;
	friend class TriangleClipOp;
	friend class ClosetTriangleSearchOp;
	friend class ClosetNonVerticalTriangleSearchOp;
	friend class RayCastingOp;
	friend class TestSpotOp;
	friend class TriangleGrid;

	void project_to_edge(const Vect3f& point, int i, int j, Vect3f& cp) const;
};

//////////////////////////////////////
//	TriangleBrush
//////////////////////////////////////
class TriangleBrush : std::vector<Triangle>
{
	const MatXf& pose;
	iRectangle bound;
public:
	TriangleBrush() : pose(MatXf::ID) {}
	TriangleBrush(cMesh& mesh);
	iRectangle getBound() const { return bound; }
	int belongSquare(int x0, int y0, int sx, int sy) const { return 1; }
	void setPos(const MatXf& pose);
	int clip_ray(const Vect3f& origin, const Vect3f& direct, float ray_radius, Vect3f& point, cCamera* camera) const;
	void draw_to_z_buffer(class ZBuffer& z_buffer, const MatXf& sXg, cCamera* camera) const;
	void show() const;
};


inline iRectangle Triangle::getBound() const
{
	return !brush ? iRectangle( 
		min(verts[0].xi(), min(verts[1].xi(), verts[2].xi())), 
		min(verts[0].yi(), min(verts[1].yi(), verts[2].yi())), 
		max(verts[0].xi(), max(verts[1].xi(), verts[2].xi())), 
		max(verts[0].yi(), max(verts[1].yi(), verts[2].yi()))) : brush -> getBound();
}

//////////////////////////////////////
//	Sphere
//////////////////////////////////////
struct Sphere : Vect3f
{
	enum { radius_min = 7 };
	float radius;
	Vect3f center;
	int index; // wheel for example

	Sphere(){}
	Sphere(const Vect3f& center_, float radius_, int index_ = 0) { radius = radius_ > radius_min ? radius_ : radius_min; center = center_; index = index_; }

	void convert(const MatXf& X) { X.xformPoint(center, *this); }

	iRectangle getBound() const 
	{ return iRectangle(round(x - radius), round(y - radius), round(x + radius), round(y + radius)); }
		
};

class SphereList : public vector<Sphere> {}; 


//////////////////////////////////////////
//	Triangle Map
//////////////////////////////////////////
class TriangleMap : public std::list<Triangle>, public Grid2D<Triangle, 4>
{
	typedef std::list<Triangle> List;
	typedef Grid2D<Triangle, 4> Grid;
	typedef std::unordered_map<int, Spot> SpotMap;
	
	SpotMap spot_map;

	void add(const Vect3f& p0, const Vect3f& p1, const Vect3f& p2, Triangle::Surface surface, int clockwise);

public:
	void clear();

	void load(const char* name);
	void load(class cInterfaceVisGeneric* IVG, int disable_Ice); 
	void load(class cM3D& M3D);
	void load(class cMesh& mesh);
	void loadSpots(const char* name);
	void addBrush(class cMesh& mesh);

	float H(int x, int y); // Max height 
	float H(int x, int y, float upper_bound); // Max height below bound
	float ceiling(int x0, int y0, float lower_bound);
	const Triangle* closest_triangle(const Vect3f& point, float radius) const; 
	int cast_ray(const Vect3f& origin, Vect3f& offset, float ray_radius, cCamera* camera); // 1 - ray intersects nearest triangle and offset_returned < offset
	Spot* testSpot(SphereList& sphere, const MatXf& X12);
	MatXf getPlacementPose(const Vect3f& center, int disabled_triangles_mask = 0);
};


//////////////////////////////////////////////////////////////
//		Extern Symbols
//////////////////////////////////////////////////////////////
extern TriangleMap* tri_map;

#endif //  _SIMPLE_CLIP_H_
