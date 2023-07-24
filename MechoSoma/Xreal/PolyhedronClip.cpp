#include "StdAfx.h"
#include "Polyhedron.h"
#include "SimpleClip.h"
#include "Body.h"

class PolyhedronClipOp 
{
	friend Vertex;
	friend Edge;
	friend Face;

	Vect3f point;
	Sphere& sphere;
	const MatXf& X12;
	Body* body1;
	Body* body2;
	const Feature*& feat;
	const Polyhedron& polyhedron;
	int iterations;
	int meta_iterations;
	int collisions;
	int put_contacts;

public:


	PolyhedronClipOp(Sphere& sphere_, const MatXf& X12_, Body* body1_, Body* body2_, const Polyhedron& polyhedron_, const Feature*& feat_, int put_contacts_) :
		point(sphere_), sphere(sphere_), X12(X12_), body1(body1_), body2(body2_), polyhedron(polyhedron_), feat(feat_), put_contacts(put_contacts_)
	{
		iterations = 0;
		meta_iterations = 0;
		collisions = 0;
	}

	int operator()() // test
	{
		if(!feat)
			feat = &polyhedron.verts().front();
		//try {
			feat -> clip(*this);
		//	}
		//catch(char* str){}
		if(meta_iterations) // exception
			feat = 0;
		return collisions;
	}

	void put(const Feature* f, const Vect3f& cp2)
	{	
		feat = f;
		Vect3f normal;
		normal.sub(point, cp2);
		float norm = normal.norm();
		float distance = norm - sphere.radius;
		if(distance > 0)
			return;
		collisions++;
		if(!put_contacts)
			return;
		if(norm > DBL_EPS)
			normal /= norm;
		else
			normal = Vect3f::K;
		Vect3f cp1;
		cp1.scale(normal, -sphere.radius);
		cp1 += point;
		X12.invXformPoint(cp1);
		body2 -> rot().xform(normal);
		if(put_contacts == 1){
			body1 -> contacts.push_back( Contact() );
			body1 -> contacts.back().set(distance, cp1, cp2, normal, &sphere, 0, body1, body2);
			body2 -> contacts2.push_back( ContactRef(body1 -> contacts.size() - 1, body1 -> contacts) );
			}
		else
			if(distance < 0){
				body1 -> Xlg.xformPoint(cp1);
				body1 -> add_contact(-distance, cp1, normal, sphere, body2);
				}
	}

	void put(const Face* f, float dist)
	{	
		feat = f;
		float dist_abs = dist - sphere.radius;
		if(dist_abs > 0)
			return;
		collisions++;
		if(!put_contacts)
			return;
		Vect3f cp1, cp2;
		Vect3f normal(f -> plane.normal());
		cp1.scale(normal, -sphere.radius);
		cp1 += sphere;
		cp2.scale(normal, -dist);
		cp2 += point;
		X12.invXformPoint(cp1);
		body2 -> rot().xform(normal);
		if(put_contacts == 1){
			body1 -> contacts.push_back( Contact() );
			body1 -> contacts.back().set(dist_abs, cp1, cp2, normal, &sphere, 0, body1, body2);
			body2 -> contacts2.push_back( ContactRef(body1 -> contacts.size() - 1, body1 -> contacts) );
			}
		else
			if(dist_abs < 0){
				body1 -> Xlg.xformPoint(cp1);
				body1 -> add_contact(-dist_abs, cp1, normal, sphere, body2);
				}
	}

	void scan_faces()
	{
		float d;
		const Face* fmax;
		float face_dist = -DBL_INF;
		std::list<Face>::const_iterator fi;
		FOR_EACH(polyhedron.faces(), fi)
			if((d = fi -> plane.dist(point)) > face_dist){
				face_dist = d;
				fmax = &*fi;
				}

		if(face_dist < 0)
			put(fmax, face_dist);
		else
			fmax -> clip(*this);
	}

	int check()
	{
		if(iterations++ > 300){
			if(meta_iterations)
				return 1;
				//throw "Clip is looped";			
			meta_iterations = 1;
			iterations = 0;
			scan_faces();
			return 1;
			}
		return 0;
	}
};

void Vertex::clip(PolyhedronClipOp& op) const
{
	if(op.check())
		return;
	std::list<VertConeNode>::const_iterator cni;
	float d; 
	FOR_EACH(cone, cni)
		if((d = cni -> plane -> dist(op.point)) < 0){
			cni -> nbr -> clip(op);
			return;
			}

	op.put(this, coords());
}

void Edge::clip(PolyhedronClipOp& op) const 
{
	if(op.check())
		return;
	float d;
	if((d = tplane.dist(op.point)) > 0){
		tail -> clip(op);
		return;
		}
	if((d = hplane.dist(op.point)) > 0){
		head -> clip(op);
		return;
		}
	if((d = lplane.dist(op.point)) > 0){
		left -> clip(op); 
		return;
		}
	if((d = rplane.dist(op.point)) > 0){
		right -> clip(op); 
		return;
		}

	op.put( this, tail -> coords() + dot(dir, op.point - tail -> coords())*dir );
}

void Face::clip(PolyhedronClipOp& op) const 
{
	if(op.check())
		return;
	std::list<FaceConeNode>::const_iterator cni;
	float d;
	FOR_EACH(cone, cni)
		if((d = cni -> plane -> dist(op.point)) < 0){
			cni -> nbr -> clip(op);
			return;
			}

	float face_dist = plane.dist(op.point);
	if(face_dist < 0)
		op.scan_faces();
	else
		op.put(this, face_dist);
}


///////////////////////////////////////////
//	Polyhedron clip
///////////////////////////////////////////
int Body::test_body_to_body_collision(const MatXf& X12, Body* body2, FeaturesList& features, int put_contacts)
{
	int counter = 0;
	FeaturesList::iterator fi = features.begin();
	SphereList::iterator si;
	FOR_EACH(spheres, si){
		Sphere& s = *si;
		s.convert(X12);
		PolyTree::const_iterator pi;
		FOR_EACH(body2 -> polyhedrons, pi){
			PolyhedronClipOp op(s, X12, this, body2, **pi, *fi, put_contacts);
			counter += op();
			++fi;
			}
		}
	return counter;
}		

//////////////////////////////////////////////////////////////////////////////
//	Triangle Map :  Clip SphereList
//////////////////////////////////////////////////////////////////////////////
class TriangleClipOp
{
	Sphere& sphere;
	const MatXf& X12;
	Body* body;
	int put_contacts;
	int collisions;
public:

	TriangleClipOp(Sphere& sphere_, const MatXf& X12_, Body* body_, int put_contacts_) :
		sphere(sphere_), X12(X12_), body(body_), put_contacts(put_contacts_), collisions(0) {}
	
	int result() { return collisions; }

	void operator()(const Triangle& triangle, float dx, float dy)
	{
		if(!(triangle.property() & Triangle::ForTestCollision))
			return;

		float d;
		Vect3f cp2, normal, uncycled_r(sphere);
		uncycled_r.x += dx; uncycled_r.y += dy;
		if(!triangle.clip(uncycled_r, sphere.radius, d, cp2, normal))
			return;
		collisions++;
		if(!put_contacts)
			return;
		cp2.x -= dx; cp2.y -= dy;
		ContactList::iterator ci;
		FOR_EACH(body -> contacts, ci)
			if(cp2.distance2(ci -> cp2) < 1e-4)
				return;
		Vect3f cp1;
		cp1.scale(normal, -sphere.radius);
		cp1 += sphere;
		X12.invXformPoint(cp1);
		body -> contacts.push_back( Contact() );
		body -> contacts.back().set(d, cp1, cp2, normal, &sphere, &triangle, body, 0);
	}
};

int Body::test_collision(int put_contacts)
{
	if(put_contacts){
		contacts.clear();
		contacts2.clear();
		}

	if(disable_collision_test)
		return 0;

	int n = 0;
	SphereList::iterator si;
	FOR_EACH(spheres, si){
		Sphere& s = *si;
		s.convert(pose());
		TriangleClipOp op(s, pose(), this, put_contacts);
		tri_map -> scan(s.getBound(), op);
		n += op.result();
		}

	ground_colliding = n ? 1 : 0;
	return n;
}

