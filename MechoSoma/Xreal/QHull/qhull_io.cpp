#include "StdAfx.h"
#include "..\Polyhedron.h"

#ifndef _FINAL_VERSION_
#define QHULL 1
#else
#define QHULL 0
#endif

#if QHULL
#include "qhull_a.h"
char qh_version[] = "vclip 1.0";

#define LONG_STR_SZ 1000  // for reading lines from files, etc.

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Given the Vertex list of a Polyhedron, compute the convex hull.
// Use this to determine and build the Edges and Faces lists.  If a
// Vertex does not lie on the convex hull, it will be removed
// from the Vertex list.
//////////////////////////////////////////////////////////////////////////////////////////////////////
int Polyhedron::buildHull()
{
  int nf, nv, npts, idx, i;
  Vertex *v;
  //List<Vertex> origVerts;

  boolT ismalloc;
  int curlong, totlong, exitcode;
  facetT *facet;
  vertexT *vertex;
  vertexT **vertexp;
  setT *vertices;
  coordT *qhullData, *qhd;
  list<Vertex>::iterator vi, vi0;
  char options [200];
  char name[LONG_STR_SZ];

  static vector<Vertex *> facelist(MAX_VERTS_PER_FACE);
#define MAX_VERTS_PER_HULL 1000  // initial size; exceeding it breaks nothing
  static vector<coordT> array	   (MAX_VERTS_PER_HULL * 3);
  static vector<Vertex *> hullVerts(MAX_VERTS_PER_HULL);
  static vector<int> vertUsed	   (MAX_VERTS_PER_HULL);
#undef MAX_VERTS_PER_HULL 

  //cout << "invoking qhull...   " << flush;

  faces_.clear();
  edges_.clear();
  FOR_EACH(verts_, vi)
        vi -> cone.clear();

  npts = verts_.size();

  if (npts > (int)hullVerts.capacity()) {
    array.reserve(3 * npts);
    hullVerts.reserve(npts);
    vertUsed.reserve(npts);
  }
  qhullData = &array.front();



  // build array of vertex coordinates
  i = 0;
  qhd = qhullData;
  FOR_EACH(verts_, vi) {
    v = &*vi;
    *qhd++ = v->coords_.x;
    *qhd++ = v->coords_.y;
    *qhd++ = v->coords_.z;
    vertUsed[i] = 0;
    hullVerts[i] = v;
    i++;
  }

  ismalloc= False;	// True if qh_freeqhull should 'free(qhullData)'
  qh_init_A (stdin, stdout, stderr, 0, NULL);
  exitcode= setjmp (qh errexit);
  if (exitcode) goto error;
  sprintf(options, "qhull Qx i s Tcv C-0");
  //sprintf(options, "qhull A0.9999 i s Tcv C-0");
  qh_initflags (options);
  qh_init_B (qhullData, npts, 3, ismalloc);
  qh_qhull();
  qh_check_output();

  // build hull Polyhedron

  nf = 0;
  FORALLfacets {
    sprintf(name, "f%d", nf++);
    vertices= qh_facet3vertex (facet);
    facelist.clear();
    FOREACHvertex_(vertices) {
      idx = qh_pointid(vertex->point);
      vertUsed[idx] = 1;
      facelist.push_back(hullVerts[idx]);
    }
    addFace(name, facelist, 1); // qhull generates a clockwise list
    qh_settempfree(&vertices);
  }

  // strip out unused vertices, count remaining ones
  for (vi = verts_.begin(), i = 0; vi != verts_.end(); i++)
    if (vertUsed[i]) vi++;
    else {
      vi0 = vi;
      vi++;
      verts_.erase(vi0);
    }
  nv = verts_.size();

  //cout << nf << " faces, " << nv << " vertices " << endl;

  qh NOerrexit= True;
  qh_freeqhull (!qh_ALL);
  qh_memfreeshort (&curlong, &totlong);

  return 0;

error:
  cerr << "error building convex hull of Polyhedron \a" << endl;
  cerr << "exitcode: " << exitcode << endl;

  qh NOerrexit= True;
  qh_freeqhull (!qh_ALL);
  qh_memfreeshort (&curlong, &totlong);

  return 1;

}

#else

// QHULL not available - crash and burn
int Polyhedron::buildHull()
{
  return 1;
}
#endif
