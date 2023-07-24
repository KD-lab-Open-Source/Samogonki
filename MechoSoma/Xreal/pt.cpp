
#include "StdAfx.h"
#include "Polyhedron.h"


///////////////////////////////////////////////////////////////////////////////
//  globals
///////////////////////////////////////////////////////////////////////////////

// Used to hold Edge names, which are not stored explicitly but
// derived from stored Vertex names.  If application calls
// Edge::name() twice, first result will be overwritten by second.
// Copy first result to another character array if both are needed
// simultaneously!
char edgeName[2 * VF_NAME_SZ];	// space for 2 Vertex names, :, and \0.

///////////////////////////////////////////////////////////////////////////////
//  low level Polyhedron support
///////////////////////////////////////////////////////////////////////////////

// Output a Plane
ostream& Plane::print(ostream &os) const
{
  auto oldFlags = os.setf(std::ios::showpos);
  os << normal_.x << " x " << normal_.y << " y  "
     << normal_.z << " z " << offset_ << " >= 0";
  os.flags(oldFlags);
  return os;
}


const char *Vertex::name() const
{
  return name_;
}


const char *Face::name() const
{
  return name_;
}


const char *Edge::name() const
{
  sprintf(edgeName, "%s:%s", tail->name(), head->name());
  return edgeName;
}


ostream& VertConeNode::print(ostream &os) const
{
  return os << std::setw(16) << (nbr ? nbr->name() : "?") << "]  " << *plane;
}


ostream& FaceConeNode::print(ostream &os) const
{
  return os << std::setw(16) << (nbr ? nbr->name() : "?") << "]  " << *plane;
}



///////////////////////////////////////////////////////////////////////////////
//  class Polyhedron
///////////////////////////////////////////////////////////////////////////////

// This is called when the vertex sequence (tail, head) is encountered
// in Face f's boundary walk.  Either a new edge is created and added
// to the edgeList, or the previously existing one from head to tail
// is updated.

void Polyhedron::processEdge(Face *f, Vertex *tail, Vertex *head)
{
  std::list<VertConeNode>::iterator vci;
  std::list<FaceConeNode>::iterator fci;
  VertConeNode vcn;
  FaceConeNode fcn;

  Edge *e, e0;
  Vect3f v;

  // check if the reverse edge (from head to tail) already exists
  FOR_EACH(head->cone, vci) {
    if ((e = vci->nbr)->head == tail) {
      // set pointer to right, rplane
      e->right = f;
      v.cross(e->dir, f->plane.normal());
      v.normalize();
      e->rplane.set(v, head->coords_);
      // tell right about e
      fcn.nbr = e;
      fcn.plane = &e->rplane;
      f->cone.push_back(fcn);
      return;
    }
  }

  // set direction, length
  e0.dir.sub(head->coords_, tail->coords_);
  e0.len = e0.dir.norm();
  e0.dir.normalize();
  // set pointers to tail, head, left
  e0.tail = tail;
  e0.head = head;
  e0.left = f;
  // set tplane, hplane, and lplane
  v.negate(e0.dir);
  e0.tplane.set(v, tail->coords_);
  e0.hplane.set(e0.dir, head->coords_);
  v.cross(f->plane.normal(), e0.dir);
  v.normalize();
  e0.lplane.set(v, tail->coords_);
  // link into list of edges
  edges_.push_back(e0);
  e = &edges_.back();
  // tell tail about e
  vcn.nbr = e;
  vcn.plane = &e->tplane;
  tail->cone.push_back(vcn);
  // tell head about e
  vcn.plane = &e->hplane;
  head->cone.push_back(vcn);
  // tell left about e
  fcn.nbr = e;
  fcn.plane = &e->lplane;
  f->cone.push_back(fcn);
}


// No checking done, but numVerts better be at least 3!
void Polyhedron::addFace(const char *name,
			 std::vector<Vertex *> &verts, int clockwise)
{
  int i;
  Face f0, *f;
  std::vector<Vertex *>::iterator vi;
  std::list<FaceConeNode>::iterator cni;
  FaceConeNode *last;
  Vect3f u, v, normal;

  f0.sides = verts.size();
  strcpy(f0.name_, name);
  // compute face support plane
  u.sub(verts[1]->coords_, verts[0]->coords_);
  v.sub(verts[2]->coords_, verts[1]->coords_);
  normal.cross(u, v);
  assert(normal.norm() > 1e-6);
  normal.normalize();
  if (clockwise) normal.negate();
  f0.plane.set(normal, verts[0]->coords_);
  // add to list of faces
  faces_.push_back(f0);
  f = &faces_.back();

  // build edges around face
  if (clockwise) {
    for (vi = verts.end()-1; vi != verts.begin(); --vi)
      processEdge(f, *vi, *(vi-1));
    processEdge(f, verts.front(), verts.back());  // close the loop
  }
  else {
    for (vi = verts.begin(); vi != verts.end()-1; ++vi)
      processEdge(f, *vi, *(vi+1));
    processEdge(f, verts.back(), verts.front());  // close the loop
  }

  // compute ccw and cw links around FaceConeNodes, cn indices, and f->sides
  for (cni = f->cone.begin(), last = &f->cone.back(), i = 0;
       cni != f->cone.end(); last = &*cni, ++cni) {
    cni->cw = last;
    last->ccw = &*cni;
    cni->idx = i++;
  }
}


ostream& Polyhedron::print(ostream &os) const
{
  const Vertex *v;
  const Edge *e;
  const Face *f;
  std::list<VertConeNode>::const_iterator vcni;
  std::list<FaceConeNode>::const_iterator fcni;
  std::list<Vertex>::const_iterator vi;
  std::list<Edge  >::const_iterator ei;
  std::list<Face  >::const_iterator fi;
  //typename faces_.const_iterator fi;

  os << verts_.size() << " verts :  ";
  FOR_EACH(verts_, vi) os << vi->name() << " ";
  os << endl;

  os << edges_.size() << " edges :  ";
  FOR_EACH(edges_, ei) os << ei->name() << " ";
  os << endl;

  os << faces_.size() << " faces :  ";
  FOR_EACH(faces_, fi) os << fi->name() << " ";
  os << endl << endl;

  FOR_EACH(verts_, vi) {
    v = &*vi;
    os << "vertex " << v->name() << " "  << v->coords_ << "   "
       << v->cone.size() << " neighbors" << endl;
    FOR_EACH(v->cone, vcni) os << *vcni << endl;
    os << endl;
  }

  FOR_EACH(edges_, ei) {
    e = &*ei;
    os << "edge " << e->name() << " " <<  e->dir << endl;
    os << "tail: " << std::setw(16) << e->tail->name() << "]  "
       << e->tplane << endl;
    os << "head: " << std::setw(16) << e->head->name() << "]  "
       << e->hplane << endl;
    os << "left: " << std::setw(16) << e->left->name() << "]  "
       << e->lplane << endl;
    os << "rght: " << std::setw(16) << e->right->name() << "]  "
       << e->rplane << endl;
    os << endl;
  }

  FOR_EACH(faces_, fi) {
    f = &*fi;
    os << "face " << f->name() << " " << f->plane << "   "
       << f->sides << " neighbors" << endl;
    FOR_EACH(f->cone, fcni) os << *fcni << endl;
    os << endl;
  }

  return os;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Check convexity of Polyhedron and verify Euler formula.  Return 1
// on problem.
//////////////////////////////////////////////////////////////////////////////////////////////////////

int Polyhedron::check() const
{
  int error;
  int nv, ne, nf;
  const Edge *e, *e1, *e2;
  const Face *f;
  std::list<FaceConeNode>::const_iterator cni;
  std::list<Edge>::const_iterator ei;
  std::list<Face>::const_iterator fi;
  Real dp;
  Vect3f v;

  error = 0;

  // check to make sure all edges are convex
  FOR_EACH(edges_, ei) {
    e = &*ei;
    v.cross(e->lplane.normal(), e->rplane.normal());
    if ((dp = dot(e->dir, v)) >= 0) {
      error = 1;
      std::cerr << "\anonconvex edge:  "
	   << " tail=" << e->tail->name_
	   << " head=" << e->head->name_
	   << " left=" << e->left->name_
	   << " rght=" << e->right->name_
	   << " angle=" << asin(-dp) << endl;
    }
  }

  // check to make sure all faces are convex polygons
  FOR_EACH(faces_, fi) {
    f = &*fi;
    FOR_EACH(f->cone, cni) {
      e1 = (Edge *) cni->nbr;
      e2 = (Edge *) cni->ccw->nbr;
      v.cross(e1->dir, e2->dir);
      if ((e1->tail == e2->tail || e1->head == e2->head)) v.negate();
      if ((dp = dot(f->plane.normal(), v)) <= 0) {
	error = 1;
	std::cerr << "\anonconvex face:  " << f->name_
	     << "  vertex=" << (e1->left == f ? e1->head : e1->tail)->name_
	     << "  angle=" << asin(-dp) << endl;
      }
    }
  }


  // Check if Euler formula (#V - #E + #F - 2 = 0) is satisfied
  nv = verts_.size();
  ne = edges_.size();
  nf = faces_.size();
  if (nv - ne + nf - 2) {
    error = 1;
    std::cout << "\apolyhedral Euler formula failure: "
	 << "nv=" << nv << " ne=" << ne << " nf=" << nf << endl;
  }

  return error;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Compute volume integrals of PolyTree and the rad field.  For info
// on algo, see "Fast and Accurate Computation of Polyhedral Mass
// Properties," Brian Mirtich, journal of graphics tools, volume 1,
// number 2, 1996.
//////////////////////////////////////////////////////////////////////////////////////////////////////
int Polyhedron::instancies;

Polyhedron::Polyhedron()
{
  instancies++;	
  handleCount = 0;
  rad_ = vol_ = rmax_= 0.0;
  mov1_ = mov2_ = pov_ = Vect3f::ZERO;
}

void Polyhedron::compVolInts()
{
  Mat3f M, Rt;

  int a, b, c;
  Edge *e;
  const Face *f;
  std::list<Face>::const_iterator fi;
  std::list<FaceConeNode>::const_iterator cni;
  Real a0, a1, da;
  Real b0, b1, db;
  Real a0_2, a0_3, a0_4, b0_2, b0_3, b0_4;
  Real a1_2, a1_3, b1_2, b1_3;
  Real d, na, nb, nc, inv;
  Real I, Ia, Ib, Iaa, Iab, Ibb, Iaaa, Iaab, Iabb, Ibbb;
  Real Icc, Iccc, Ibbc, Icca;
  Real C0, Ca, Caa, Caaa, Cb, Cbb, Cbbb;
  Real Cab, Kab, Caab, Kaab, Cabb, Kabb;
  Vect3f h, w, v;
  MatXf X;

  vol_ = 0.0;
  mov1_ = mov2_ = pov_ = Vect3f::ZERO;

    FOR_EACH(faces_, fi) {
      f = &*fi;

      // compute projection direction
      v.set(fabs(f->plane.normal().x),
	    fabs(f->plane.normal().y),
	    fabs(f->plane.normal().z));
      c = (v.x >= v.y) ? ((v.x >= v.z) ? 0 : 2)
		       : ((v.y >= v.z) ? 1 : 2);
      a = (c + 1) % 3;
      b = (c + 2) % 3;

      I = Ia = Ib = Iaa = Iab = Ibb = Iaaa = Iaab = Iabb = Ibbb = 0.0;

      /* walk around face */

      FOR_EACH(f->cone, cni) {
	e = (Edge *) cni->nbr;
	if (e->left == f) {  // CCW edge
	  a0 = e->tail->coords_[a];
	  b0 = e->tail->coords_[b];
	  a1 = e->head->coords_[a];
	  b1 = e->head->coords_[b];
	}
	else {	// CW edge
	  a0 = e->head->coords_[a];
	  b0 = e->head->coords_[b];
	  a1 = e->tail->coords_[a];
	  b1 = e->tail->coords_[b];
	}

	da = a1 - a0;
	db = b1 - b0;
	a0_2 = a0 * a0;
	a0_3 = a0_2 * a0;
	a0_4 = a0_3 * a0;
	b0_2 = b0 * b0;
	b0_3 = b0_2 * b0;
	b0_4 = b0_3 * b0;
	a1_2 = a1 * a1;
	a1_3 = a1_2 * a1;
	b1_2 = b1 * b1;
	b1_3 = b1_2 * b1;
	C0 = a1 + a0;
	Ca = a1*C0 + a0_2;
	Caa = a1*Ca + a0_3;
	Caaa = a1*Caa + a0_4;
	Cb = b1*(b1 + b0) + b0_2;
	Cbb = b1*Cb + b0_3;
	Cbbb = b1*Cbb + b0_4;
	Cab = 3*a1_2 + 2*a1*a0 + a0_2;
	Kab = a1_2 + 2*a1*a0 + 3*a0_2;
	Caab = a0*Cab + 4*a1_3;
	Kaab = a1*Kab + 4*a0_3;
	Cabb = 4*b1_3 + 3*b1_2*b0 + 2*b1*b0_2 + b0_3;
	Kabb = b1_3 + 2*b1_2*b0 + 3*b1*b0_2 + 4*b0_3;
	I += db*C0;
	Ia += db*Ca;
	Iaa += db*Caa;
	Iaaa += db*Caaa;
	Ib += da*Cb;
	Ibb += da*Cbb;
	Ibbb += da*Cbbb;
	Iab += db*(b1*Cab + b0*Kab);
	Iaab += db*(b1*Caab + b0*Kaab);
	Iabb += da*(a1*Cabb + a0*Kabb);
      }

      I /= 2.0;
      Ia /= 6.0;
      Iaa /= 12.0;
      Iaaa /= 20.0;
      Ib /= -6.0;
      Ibb /= -12.0;
      Ibbb /= -20.0;
      Iab /= 24.0;
      Iaab /= 60.0;
      Iabb /= -60.0;

      d =  f->plane.offset();
      v = f->plane.normal();
      na = v[a];
      nb = v[b];
      nc = v[c];
      inv = 1.0 / nc;

      if (a == 0)      vol_ += inv * na * Ia;
      else if (b == 0) vol_ += inv * nb * Ib;
      else	       vol_ -= ((d*I + na*Ia + nb*Ib)/nc);

#define SQR(x) ((x) * (x))
#define CUBE(x) ((x) * (x) * (x))

      Icc = (SQR(na)*Iaa + 2*na*nb*Iab + SQR(nb)*Ibb
	     + d*(2*(na*Ia + nb*Ib) + d*I)) * SQR(inv);
      mov1_[a] += inv * na * Iaa;
      mov1_[b] += inv * nb * Ibb;
      mov1_[c] += Icc;

      Iccc = -(CUBE(na)*Iaaa + 3*SQR(na)*nb*Iaab
	       + 3*na*SQR(nb)*Iabb + CUBE(nb)*Ibbb
	       + 3*(SQR(na)*Iaa + 2*na*nb*Iab + SQR(nb)*Ibb)*d
	       + d*d*(3*(na*Ia + nb*Ib) + d*I)) * CUBE(inv);
      mov2_[a] += inv * na * Iaaa;
      mov2_[b] += inv * nb * Ibbb;
      mov2_[c] += Iccc;

      Ibbc = -(d*Ibb + na*Iabb + nb*Ibbb) * inv;
      Icca = (SQR(na)*Iaaa + 2*na*nb*Iaab + SQR(nb)*Iabb
	      + d*(2*(na*Iaa + nb*Iab) + d*Ia)) * SQR(inv);
      pov_[c] += inv * na * Iaab;
      pov_[a] += inv * nb * Ibbc;
      pov_[b] += Icca;

#undef SQR
#undef CUBE

    }

    mov1_.scale(0.5);
    mov2_.scale(1.0f/3.0f);
    pov_.scale(0.5);


  Vect3f cent = center();
  rmax_ = rad_ = 0.0;
  std::list<Vertex>::iterator vi;
  FOR_EACH(verts_, vi){
	Real d = cent.distance2(vi -> coords_);
	if(d > rad_) 
		rad_ = d;
	d = vi -> coords_.norm2();
	if(d > rmax_) 
		rmax_ = d;
	}

  rad_ = sqrt(rad_);
  rmax_ = sqrt(rmax_);
}


///////////////////////////////////////////////////////////////////////////////
//  class PolyTree
///////////////////////////////////////////////////////////////////////////////
int PolyTree::IDs = 0;

// constructor
PolyTree::PolyTree()
{
	clear();
}

void PolyTree::clear()
{
	ID_ = ++IDs;
	list<ShareHandle<Polyhedron> >::clear();
	rad_ = vol_ = rmax_= 0.0;
	mov1_ = mov2_ = pov_ = Vect3f::ZERO;
}

void PolyTree::add(Polyhedron* p)
{
	ID_ = ++IDs;
	push_back(p);
	
	vol_ += p -> vol();
	mov1_ += p -> mov1();
	mov2_ += p -> mov2();
	pov_ += p -> pov();

	if(size() > 1){
		Vect3f c = center();
		rmax_ = rad_ = 0.0;
		iterator i;
		FOR_EACH(*this, i){
			list<Vertex>::iterator vi;
			FOR_EACH((*i) -> verts_, vi){
				Real d = c.distance2(vi -> coords_);
				if(d > rad_) 
					rad_ = d;
				d = vi -> coords_.norm2();
				if(d > rmax_) 
					rmax_ = d;
				}
			}

		rad_ = sqrt(rad_);
		rmax_ = sqrt(rmax_);
		}
	else{
		rad_ = p -> rad();
		rmax_ = p -> rmax();
		}
}

///////////////////////////////////////////////////////////////////////////////
//		Polyhedron's transforming
///////////////////////////////////////////////////////////////////////////////
void Plane::xform(const MatXf& X)
{
	Vect3f p(normal_);
	p *= offset_;
	X.xformPoint(p);
	offset_ = SIGN(offset_)*p.norm();
	normal_.scale(p, 1/offset_);
}

void Polyhedron::xform(const MatXf& X)
{
	std::list<Vertex>::iterator vi;
	FOR_EACH(verts_, vi)
		X.xformPoint(vi -> coords_);

	std::list<Edge>::iterator ei;
	FOR_EACH(edges_, ei){
		Edge& e = *ei;
		e.dir.sub(e.head->coords_, e.tail->coords_);
		e.len = e.dir.norm();
		e.dir /= e.len;
		e.tplane.xform(X);
		e.hplane.xform(X);
		e.lplane.xform(X);
		e.rplane.xform(X);
		}

	std::list<Face>::iterator fi;
	FOR_EACH(faces_, fi)
		fi -> plane.xform(X);
}

void PolyTree::xform(const MatXf& X)
{
	iterator pi;
	FOR_EACH(*this, pi){
		if((*pi) -> handleCount > 1)
			*pi = new Polyhedron(**pi);
		(*pi) -> xform(X);
		}
}

