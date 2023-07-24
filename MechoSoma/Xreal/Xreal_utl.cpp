#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Body.h"
#include "SimpleClip.h"
#include "mechosoma.h"
#include "XGR.h"
#include "Mesh3ds.h"
#include "IVisGeneric.h"
#include "IGraph3d.h"
#include "Xreal_utl.h"
#include "Params.h"
#include "DebugPrm.h"

int XCOL(int color, int intensity, int alpha) 
{ 
	return (alpha << 24) | 
		((((color >> 16) & 255)*intensity >> 8) << 16) |
		((((color >> 8) & 255)*intensity >> 8) << 8) |
		((color & 255)*intensity >> 8);
}

void clip_line(int x1,int y1,int x2,int y2,int color)
{
	xrealIGR -> DrawLine(x1,y1,x2,y2,(color>>16)&255,(color>>8)&255,(color>>0)&255,(color>>24)&255);
}

void clip_pixel(int x1,int y1,int color, int size)
{
	xrealIGR -> DrawPixel(x1,y1,(color>>16)&255,(color>>8)&255,(color>>0)&255,(color>>24)&255);
	if(size){
		xrealIGR -> DrawPixel(x1 + 1,y1,(color>>16)&255,(color>>8)&255,(color>>0)&255,(color>>24)&255);
		xrealIGR -> DrawPixel(x1,y1 + 1,(color>>16)&255,(color>>8)&255,(color>>0)&255,(color>>24)&255);
		xrealIGR -> DrawPixel(x1 + 1,y1 + 1,(color>>16)&255,(color>>8)&255,(color>>0)&255,(color>>24)&255);

		if(size == 2){
			xrealIGR -> DrawPixel(x1 - 1,y1,(color>>16)&255,(color>>8)&255,(color>>0)&255,(color>>24)&255);
			xrealIGR -> DrawPixel(x1,y1 - 1,(color>>16)&255,(color>>8)&255,(color>>0)&255,(color>>24)&255);
			xrealIGR -> DrawPixel(x1 - 1,y1 - 1,(color>>16)&255,(color>>8)&255,(color>>0)&255,(color>>24)&255);
			xrealIGR -> DrawPixel(x1 - 1,y1 + 1,(color>>16)&255,(color>>8)&255,(color>>0)&255,(color>>24)&255);
			xrealIGR -> DrawPixel(x1 + 1,y1 - 1,(color>>16)&255,(color>>8)&255,(color>>0)&255,(color>>24)&255);
			}
		}
}

void clip_circle(int xc, int yc, float r, int color)
{
	int ri = round(r);
	if(xc + ri <= 0 || xc - ri > XGR_MAXX || yc + ri <= 0 || yc - ri > XGR_MAXY || r < DBL_EPS || r > 2*XGR_MAXX)
		return;
	float dphi = 0.5/r;
	for(float phi = 0;phi < M_PI; phi += dphi){
		int dx = round(r*cos(phi));
		int dy = round(r*sin(phi));
		clip_pixel(xc + dx, yc + dy, color);
		clip_pixel(xc - dx, yc - dy, color);
		}
}


Vect3f G2S(const Vect3f &vg)
{
	Vect3f pw=vg,pv,pe;
	xrealIVG->ConvertorWorldToViewPort(NULL,&pw,&pv,&pe);
	return Vect3f(pe.x, pe.y, pv.z);
}

void show_vector(const Vect3f &vg,int color)
{
	Vect3f vs = G2S(vg);
	clip_pixel(vs.xi(), vs.yi(), color, 1);
}

int intensity_by_dist(float z)
{
	int i = round(255.*(1. - (z - show_vector_zmin)/(show_vector_zmax - show_vector_zmin)));
	if(i < 0)
		i = 0;
	if(i > 255)
		i = 255;
	return i;
}

void show_vector(const Vect3f &vg, const Vect3f &delta, int color)
{
	Vect3f vs0 = G2S(vg);
	Vect3f vs1 = G2S(vg + delta*show_vector_scale);
	clip_line(vs0.xi(), vs0.yi(), vs1.xi(), vs1.yi(), color);
//	xrealIGR -> DrawLine(vs0.xi(), vs0.yi(), vs1.xi(), vs1.yi(), XCOL(color, intensity_by_dist(vs0.z)), XCOL(color, intensity_by_dist(vs1.z)) );
}

void show_vector(const Vect3f &vg0, const Vect3f &vg1, const Vect3f &vg2, int color)
{
	Vect3f vs0 = G2S(vg0);
	Vect3f vs1 = G2S(vg1);
	Vect3f vs2 = G2S(vg2);
//	clip_line(vs0.xi(), vs0.yi(), vs1.xi(), vs1.yi(), color);
//	clip_line(vs1.xi(), vs1.yi(), vs2.xi(), vs2.yi(), color);
//	clip_line(vs2.xi(), vs2.yi(), vs0.xi(), vs0.yi(), color);
	xrealIGR -> DrawLine(vs0.xi(), vs0.yi(), vs1.xi(), vs1.yi(), XCOL(color, intensity_by_dist(vs0.z)), XCOL(color, intensity_by_dist(vs1.z)) );
	xrealIGR -> DrawLine(vs1.xi(), vs1.yi(), vs2.xi(), vs2.yi(), XCOL(color, intensity_by_dist(vs1.z)), XCOL(color, intensity_by_dist(vs2.z)) );
	xrealIGR -> DrawLine(vs2.xi(), vs2.yi(), vs0.xi(), vs0.yi(), XCOL(color, intensity_by_dist(vs2.z)), XCOL(color, intensity_by_dist(vs0.z)) );
}

void show_vector(const Vect3f &vg0, const Vect3f &vg1, const Vect3f &vg2, const Vect3f &vg3, int color)
{
	Vect3f vs0 = G2S(vg0);
	Vect3f vs1 = G2S(vg1);
	Vect3f vs2 = G2S(vg2);
	Vect3f vs3 = G2S(vg3);
	clip_line(vs0.xi(), vs0.yi(), vs1.xi(), vs1.yi(), color);
	clip_line(vs1.xi(), vs1.yi(), vs2.xi(), vs2.yi(), color);
	clip_line(vs2.xi(), vs2.yi(), vs3.xi(), vs3.yi(), color);
	clip_line(vs3.xi(), vs3.yi(), vs0.xi(), vs0.yi(), color);
}


void show_vector(const Vect3f& vg, float radius, int color)
{
	Vect3f vs = G2S(vg);
	float k = fCamera*XGR_MAXX/(vs.z*640);
	if(k < 1e-4)
		return;
	clip_circle(vs.xi(), vs.yi(), k*radius ,color);
}

void draw(const std::list<const Face*> &faces, int color)
{
	std::list<const Face*>::const_iterator fi;
	std::list<FaceConeNode>::const_iterator cni;
	FOR_EACH(faces, fi) {
		const Face* f = *fi;
		FOR_EACH(f->cone, cni) {
			const Edge* e = cni->nbr; 
			Vect3f h = G2S(e -> head -> coords());
			Vect3f t = G2S(e -> tail -> coords());
			int color_ = (asin(dot(e->dir, e->lplane.normal() % e->rplane.normal())) > 0) ? color << 5 : color;
			clip_line(t.xi(), t.yi(), h.xi(), h.yi(), color_);
			}
		}
}

void Polyhedron::draw(const MatXf& Xlg, int color) const
{
	Vect3f t,h;
	const Face* f;
	const Edge* e;
	std::list<Face>::const_iterator fi;
	std::list<FaceConeNode>::const_iterator cni;

	FOR_EACH(faces_, fi) {
		f = &*fi;
		FOR_EACH(f->cone, cni) {
			e = cni->nbr;
			Xlg.xformPoint(e -> head -> coords(),h);
			h = G2S(h);
			Xlg.xformPoint(e -> tail -> coords(),t);
			t = G2S(t);
			clip_line(t.xi(), t.yi(), h.xi(), h.yi(), color);
			}
		}
}

void PolyTree::draw(const MatXf& Xlg, int color) const
{
	const_iterator i;  
	FOR_EACH(*this, i) 
		(*i) -> draw(Xlg, color);
}

inline void g2map(Vect3f& v)
{
	float k = XGR_MAXX/640.;
	v.x *= show_map_x_scale*k;
	v.y *= show_map_y_scale*k;
	v.x += show_map_x_offset*k;
	v.y += show_map_y_offset*k;
}

void show_map_pixel(Vect3f v, int color)
{
	g2map(v);
	clip_pixel(v.xi(), v.yi(), color, 1);
}

void show_map_line(Vect3f v0, Vect3f v1, int color)
{
	g2map(v0);
	g2map(v1);
	clip_line(v0.xi(), v0.yi(), v1.xi(), v1.yi(), color);
}

void show_map_circle(Vect3f v, float radius, int color)
{
	g2map(v);
	clip_circle(v.xi(), v.yi(), radius, color);
}
