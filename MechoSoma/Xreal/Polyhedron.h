////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __POLYHEDRON_H__
#define __POLYHEDRON_H__

#include "Contact.h"

typedef float Real;

///////////////////////////////////////////////////////////////////////////////
// constants
///////////////////////////////////////////////////////////////////////////////


#define VF_NAME_SZ  8	 // max # of chars in a Face or Vertex name (incl. \0)

typedef char VertFaceName[VF_NAME_SZ];

// Used as the initial size for certain resizable vectors.  Generally,
// the number defined below is plenty big, however exceeding this
// limit at runtime won't break anything.
#define MAX_VERTS_PER_FACE 100

///////////////////////////////////////////////////////////////////////////////
// class Plane
///////////////////////////////////////////////////////////////////////////////

class Plane {

  Vect3f normal_;   // plane = { p | < p , normal_ > + offset_ = 0 }
  Real offset_;

public:

  const Vect3f &normal() const {return normal_;}
  const Real  &offset() const {return offset_;}

  void set(const Vect3f &normal, const Vect3f &thruPoint)
    {normal_ = normal; offset_ = - dot(thruPoint, normal);}
  
  // transforming
  void xform(const MatXf& X);

  // Compute signed distance from point to plane; assumes unit length normal_
  Real dist(const Vect3f &point) const
    {return dot(normal_, point) + offset_;}

  ostream& print(ostream &os) const;
};



///////////////////////////////////////////////////////////////////////////////
// struct VertConeNode & FaceConeNode
///////////////////////////////////////////////////////////////////////////////

struct VertConeNode
{
  const Plane *plane;
  class Edge *nbr;     // neighboring edge when plane violated

  ostream& print(ostream &os) const;

};


struct FaceConeNode
{
  const Plane *plane;
  const class Edge *nbr;     // neighboring edge when plane violated

  const FaceConeNode *ccw, *cw;
  int idx;  // ranges from 0 to n-1, where n = number of edges on face

  ostream& print(ostream &os) const;

};



///////////////////////////////////////////////////////////////////////////////
// class Feature
///////////////////////////////////////////////////////////////////////////////

class Feature
{

public:
  enum Type {VERTEX, EDGE, FACE};

protected:
  Type type_;

public:
  virtual ~Feature() = default;
  Type type() const {return type_;}
  virtual const char *name() const = 0;
  virtual void clip(class PolyhedronClipOp& data) const = 0;
};


typedef std::vector<const Feature*> FeaturesList;


///////////////////////////////////////////////////////////////////////////////
// class Vertex
///////////////////////////////////////////////////////////////////////////////

class Vertex : private Feature
{
  friend class Polyhedron;
  friend class PolyTree;
  friend class PolyhedronClipOp;
  friend struct Contact;

public:

  Vect3f coords_;
  std::list<VertConeNode> cone;
  union {
	VertFaceName name_;
	int index;
	};

  Vertex() {type_ = VERTEX;}
  const char *name() const;
  const Vect3f &coords() const {return coords_;}
  void clip(class PolyhedronClipOp& data) const;
};



///////////////////////////////////////////////////////////////////////////////
// class Edge
///////////////////////////////////////////////////////////////////////////////

class Edge : private Feature
{
  friend class Polyhedron;
  friend class PolyTree;
  friend class PolyhedronClipOp;
  friend  Contact;

public:

  const Vertex *tail, *head;
  const class Face *left, *right;
  Real len;
  Vect3f dir;
  Plane tplane, hplane, lplane, rplane;

  Edge() {type_ = EDGE;}
  const char *name() const;
  void clip(class PolyhedronClipOp& data) const;
};



///////////////////////////////////////////////////////////////////////////////
// class Face
///////////////////////////////////////////////////////////////////////////////

class Face : private Feature
{
  friend class Polyhedron;
  friend class PolyTree;
  friend class PolyhedronClipOp;
  friend Contact;

public:

  int sides;	 // number of edges around boundary
  Plane plane;
  std::list<FaceConeNode> cone;
  VertFaceName name_;

  Face() {type_ = FACE;}
  const char *name() const;
  void clip(class PolyhedronClipOp& data) const;
};



///////////////////////////////////////////////////////////////////////////////
// class Polyhedron
///////////////////////////////////////////////////////////////////////////////

class Polyhedron
{
  friend class PolyTree;
  friend struct SpherePolyhedronPair;
  friend class ShareHandle<Polyhedron>;

  int handleCount; 

  std::list<Vertex> verts_;
  std::list<Edge  > edges_;
  std::list<Face  > faces_;

  // Volume integrals, relative to this Polyhedron's reference frame
  Real vol_;   // volume:				 vol	= int(dV)
  Vect3f mov1_; // 1st moment of volume: 		 mov1.x = int(x dV)
  Vect3f mov2_; // undiagonalized 2nd moment of volume:	 mov2.x = int(x^2 dV)
  Vect3f pov_;  // product of volume:			 pov.x	= int(yz dV)

  Real rad_;   // "radius" of Polyhedron, relative to center of volume
  Real rmax_;   // "radius" of Polyhedron, relative to origin of coordinate system

  void processEdge(Face *f, Vertex *tail, Vertex *head);

  int buildHull();
  int check() const;
  void compVolInts();

public:
	static int instancies;
		
	Polyhedron();
	Polyhedron(const Polyhedron& p, const Vect3f& scale = Vect3f::ID, const Vect3f& displace = Vect3f::ZERO);
	~Polyhedron(){ instancies--; }

  void save(const char* fname);
  void load(const char* fname, const Vect3f& scale);

  // loading
  void setMesh(class cMesh &mesh, int recursive = 1);
	
  // construction
  inline Vertex* addVertex(const char *name, const Vect3f& coords);
  void addFace(const char* name, std::vector<Vertex*>& verts, int clockwise = 0);
  
  // transforming
  void xform(const MatXf& X);

  // examination
  ostream& print(ostream &os) const;
  const std::list<Vertex> &verts() const {return verts_;}
  const std::list<Edge  > &edges() const {return edges_;}
  const std::list<Face  > &faces() const {return faces_;}

  // volume integrals
  const Real& vol()  const {return vol_;}
  const Vect3f& mov1() const {return mov1_;}
  const Vect3f& mov2() const {return mov2_;}
  const Vect3f& pov()  const {return pov_;}
  const Real& rad()  const {return rad_;}
  Vect3f center()  const {return mov1_/vol_;}
  Real rmax()  const {return rmax_;}
  
  // visualisation
  void draw(const MatXf& Xgs, int color) const;

};



///////////////////////////////////////////////////////////////////////////////
//  class PolyTree
///////////////////////////////////////////////////////////////////////////////

class PolyTree : public std::list<ShareHandle<Polyhedron> >
{

  // Volume integrals, relative to this PolyTree's reference frame
  Real vol_;   // volume:				 vol	= int(dV)
  Vect3f mov1_; // 1st moment of volume: 		 mov1.x = int(x dV)
  Vect3f mov2_; // undiagonalized 2nd moment of volume:	 mov2.x = int(x^2 dV)
  Vect3f pov_;  // product of volume:			 pov.x	= int(yz dV)

  Real rad_;   // "radius" of PolyTree, relative to center of volume
  Real rmax_;   // "radius" of PolyTree, relative to origin of coordinate system

  int ID_;
  static int IDs;

public:

  PolyTree();
  void clear();
  void add(Polyhedron* p);
  int ID() const { return ID_; }

  // volume integrals
  const Real& vol()  const {return vol_;}
  const Vect3f& mov1() const {return mov1_;}
  const Vect3f& mov2() const {return mov2_;}
  const Vect3f& pov()  const {return pov_;}
  const Real& rad()  const {return rad_;}
  Vect3f center()  const {return mov1_/vol_;}
  Real rmax()  const {return rmax_;}

  // transforming
  void xform(const MatXf& X);

  // visualisation
  void draw(const MatXf& Xgs, int color) const;

};


///////////////////////////////////////////////////////////////////////////////
//  stream operators
///////////////////////////////////////////////////////////////////////////////

inline ostream &operator<<(ostream &os, const Plane &p) {return p.print(os);}
inline ostream &operator<<(ostream &os, const VertConeNode &vcn)
  {return vcn.print(os);}
inline ostream &operator<<(ostream &os, const FaceConeNode &fcn)
  {return fcn.print(os);}
inline ostream &operator<<(ostream &os, const Polyhedron *poly)
  {return poly->print(os);}



///////////////////////////////////////////////////////////////////////////////
//  inline methods
///////////////////////////////////////////////////////////////////////////////

Vertex *Polyhedron::addVertex(const char *name, const Vect3f &coords)
{
  Vertex v;
  v.coords_ = coords;
  strcpy(v.name_, name);
  verts_.push_back(v);
  return &verts_.back();
}


#endif	// #ifndef __POLYHEDRON_H__
