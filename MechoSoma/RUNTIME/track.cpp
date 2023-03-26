/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "track.h"
#include "TERRA.H"

#include "TrackRecorder.h"
#include "mechosoma.h"
#include "Mesh3ds.h"

#ifdef _FCONTAINER_LOAD_
#include "fcontainer.h"
#endif

#include "mch_common.h" // For far target

#pragma warning( disable : 4244 )

/* ----------------------------- STRUCT SECTION ----------------------------- */

class mchTrackElementScanOp
{
	int X;
	int Y;
	int Z;

	int ID;

	int flag;

	int el_Dist;
	const mchTrackElement* el_Ptr;
public:

	mchTrackElementScanOp(int fl){ flag = fl; X = Y = -1; }
	mchTrackElementScanOp(int x,int y){ X = x; Y = y; Z = -1; el_Ptr = NULL; flag = 0; ID = -1; }
	mchTrackElementScanOp(int x,int y,int z){ X = x; Y = y; Z = Z; el_Ptr = NULL; flag = 0; ID = -1; }
	mchTrackElementScanOp(int x,int y,int z,int id){ X = x; Y = y; Z = Z; el_Ptr = NULL; flag = 0; ID = id; }

	void operator()(const mchTrackElement& te, float dx, float dy)
	{
		int r,x,y,z;

		if(ID != -1 && te.owner -> ID != ID)
			return;

		if(X != -1){
			x = getDistX(X,te.Cx);
			y = getDistX(Y,te.Cy);

			if(Z != -1) z = abs(Z - te.Cz);
			else z = 0;

			r = x * x + y * y;

			if(!el_Ptr || r < el_Dist){
				el_Ptr = &te;
				el_Dist = r;
			}
		}	
		else
			te.flags |= flag;
	}

	friend mchTrackMap;
};

/* ----------------------------- EXTERN SECTION ----------------------------- */

extern int mchCurrentTrack;

extern cInterfaceVisGeneric	*gb_IVisGeneric;
extern cInterfaceGraph3d	*gb_IGraph3d;

extern char* fcPath;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

const char* GetTargetName(const char* name);
void mchLoadTrackNode(XStream& fh);

void mchA_d3dRectangle(int x,int y,int sx,int sy,int col,int outline = 0);
void mchA_d3dRectangleZ(int x,int y,int z,int sx,int sy,int col,int outline = 0,int alpha = 255);
void mchA_d3dRectangle_cl(int x,int y,int sx,int sy,int col,int outline = 0);
unsigned mchA_d3dMakeColor(int r,int g,int b);

int mchLoadTrackBTR(void);

int mchA_ConvertCoords(int x,int y,int z,int& sx,int& sy,int viewport = 0);

/* --------------------------- DEFINITION SECTION --------------------------- */

xtList<mchTrackBranch>* mchTrackData = NULL;
mchTrackMap* mch_trackMap = NULL;

void mchLoadTrack(void)
{
	int i,n_node,n_branch;

	mchTrackBranch* p;

	const char* fname;

	if(!mchTrackData) mchTrackData = new xtList<mchTrackBranch>;
	if(!mch_trackMap) mch_trackMap = new mchTrackMap;

	if(mchLoadTrackBTR()) return;

	fname = GetTargetName("track0.dat");

	XStream fh(0);

	if(!fh.open(fname,XS_IN)) return;

	fh > n_node > n_branch;

	for(i = 0; i < n_node; i ++){
		mchLoadTrackNode(fh);
	}

	for(i = 0; i < n_branch; i ++){
		p = new mchTrackBranch;
		p -> Load(fh);

		mchTrackData -> append(p);
	}
	fh.close();
}

void mchFreeTrack(void)
{
	mchTrackBranch* p = mchTrackData -> first();
	if(mchTrackData){
		while(p){
			mchTrackData -> remove(p);
			delete p;
			p = mchTrackData -> first();
		}
	}
	if(mch_trackMap){
		mch_trackMap -> clear();
		delete mch_trackMap;
		mch_trackMap = NULL;
	}
}

void mchLoadTrackNode(XStream& fh)
{
	int i,index,x,y,h,n_branch;
	fh > index;
	fh > x > y > h;
	fh > n_branch;

	for( i = 0; i < n_branch; i++ ){
		fh > x > y > h;
	}
}

void mchTrackBranch::Load(XStream& fh)
{
	int i,j,idx,beg_node,end_node,z,numPoints;
	int x[4],y[4];

	mchTrackElement* p;

	fh > idx > beg_node > end_node > ID > index > flags > numPoints;

	fh > x[0] > y[0] > z;
	fh > x[1] > y[1] > z;

	for(i = 0; i < numPoints - 1; i ++){
		fh > x[3] > y[3] > z;
		fh > x[2] > y[2] > z;

		p = new mchTrackElement;
		p -> Cx = x[0];
		p -> Cy = y[0];
		p -> Cz = 0;

		for(j = 1; j < 4; j ++){
			p -> Cx += x[0] + getDistX(x[j],x[0]);
			p -> Cy += y[0] + getDistY(y[j],y[0]);
		}

		p -> Cx >>= 2;
		p -> Cy >>= 2;

		p -> Cx = XCYCL(p -> Cx);
		p -> Cy = YCYCL(p -> Cy);

		x[0] = x[3];
		x[1] = x[2];
		
		y[0] = y[3];
		y[1] = y[2];

		pointLst -> append(p);
	}

	init();
}

#ifndef _FCONTAINER_LOAD_
void mchTrackBranch::LoadBTR(XStream& fh,int version)
#else
void mchTrackBranch::LoadBTR(XBuffer& fh,int version)
#endif
{
	int i,sz,fl;

	mchTrackElement* p;

	fh > ID > index > sz;
	flags = 0;

	for(i = 0; i < sz; i ++){
		p = new mchTrackElement;

		fh > p -> Cx > p -> Cy > p -> Cz;

		if(version > 1)
			fh > fl;
		else 
			fl = 0;

		if(fl & TRK_REMOVE)
			p -> flags |= TRACK_EL_REMOVE;

		p -> Cx = XCYCL(p -> Cx);
		p -> Cy = YCYCL(p -> Cy);

		pointLst -> append(p);
	}

	init();
}

mchTrackBranch::mchTrackBranch(void)
{
	ID = -1;
	list = NULL;

	pointLst = new xtList<mchTrackElement>;
}

mchTrackBranch::~mchTrackBranch(void)
{
	mchTrackElement* p = pointLst -> first();
	while(p){
		pointLst -> remove(p);
		delete p;
		p = pointLst -> first();
	}
	delete pointLst;
}

void mchTrackElement::init(void)
{
}

void mchTrackElement::redraw(void)
{
}

void mchTrackBranch::clean(void)
{
	mchTrackElement* p = pointLst -> first(),*p1;
	while(p){
		p1 = p -> next;
		if(p -> flags & TRACK_EL_REMOVE){ 
			pointLst -> remove(p);
			delete p;
		}
		p = p1;
	}
}

void mchTrackBranch::redraw(void)
{
	mchTrackElement* p = pointLst -> first();

	gameWnd -> set_clipping();
	while(p){
		p -> redraw();
		p = p -> next;
		if(p) p = p -> next;
	}
	gameWnd -> remove_clipping();
}

void mchTrackBranch::merge(mchTrackBranch* p)
{
	mchTrackElement* el = p -> pointLst -> first();
	while(el){
		p -> pointLst -> remove(el);
		pointLst -> append(el);
		el = p -> pointLst -> first();
	}

	init();
}

void mchTrackBranch::init(void)
{
	mchTrackElement* p = pointLst -> first();
	while(p){
		p -> owner = this;
		p = p -> next;
	}
}

void mchTrackMap::SetFlag(int x,int y,int sx,int sy,int fl)
{
	mchTrackElement* el;

//	mchTrackElementScanOp op(fl);
	mchTrackElementScanOp op(x,y);
	scan(iRectangle(x,y,x + sx,y + sy),op);

	el = (mchTrackElement*)op.el_Ptr;
	if(el) el -> flags |= fl;
}

mchTrackElement* mchTrackMap::GetTrackEl(int x,int y,int sx,int sy)
{
	mchTrackElementScanOp op(x,y);
	scan(iRectangle(x,y,x + sx,y + sy),op);

	return (mchTrackElement*)op.el_Ptr;
}

mchTrackElement* mchTrackMap::GetTrackEl(Vect3f r,int sx,int sy,int id)
{
	mchTrackElementScanOp op(r.xi(),r.yi(),r.zi(),id);
	scan(iRectangle(r.xi() - sx/2,r.yi() - sy/2,r.xi() + sx/2,r.yi() + sy/2),op);

	return (mchTrackElement*)op.el_Ptr;
}

void mchTrackMap::InsertBranch(mchTrackBranch* p)
{
	mchTrackElement* el = p -> pointLst -> first();
	while(el){
		if(!(el -> flags & TRACK_EL_REMOVE))
			insert(*el,el -> getBound());
		el = el -> next;
	}
}

void mchTrackBranch::clear_flags(int mode)
{
	mchTrackElement* p = pointLst -> first();
	if(mode){
		while(p){
			p -> flags &= ~TRACK_EL_BUSY;
			p = p -> next;
		}
	}
	else {
		while(p){
			p -> flags &= TRACK_EL_BUSY;
			p = p -> next;
		}
	}
}

void mchTrackElement::show() const
{
	int col,x,y,z,sx,sy,sz;

	col = (flags & TRACK_EL_BUSY) ? 1 : 0;

	static int colors[2] = {
		XGR_RGB64K(31,0,0),
		XGR_RGB64K(0,63,0),
	};

//	sz = round((float)TRACK_CELL_SIZE / mch_prmScale);
	sz = round(16.0f);

	z = mchA_ConvertCoords(Cx,Cy,Cz + 20,sx,sy);

	if(RenderMode == XGRAPH_HICOLOR){
		XGR_Rectangle16(sx - sz/2,sy - sz/2,sz,sz,colors[col],colors[col],XGR_OUTLINED);

		x = sx - sz/2 + 1; 
		y = sy - sz/8;

		if(flags & TRACK_EL_FIRE)
			XGR_Rectangle16(x,y,sz/4 - 2,sz/4 - 2,XGR_RGB64K(31,16,0),XGR_RGB64K(31,16,0),XGR_FILLED);

		x += sz/4;
		if(flags & TRACK_EL_ELECTRICITY)
			XGR_Rectangle16(x,y,sz/4 - 2,sz/4 - 2,XGR_RGB64K(0,16,31),XGR_RGB64K(0,16,31),XGR_FILLED);

		x += sz/4;
		if(flags & TRACK_EL_SLIME)
			XGR_Rectangle16(x,y,sz/4 - 2,sz/4 - 2,XGR_RGB64K(0,63,0),XGR_RGB64K(0,63,0),XGR_FILLED);

		x += sz/4;
		if(flags & TRACK_EL_TELEPORT)
			XGR_Rectangle16(x,y,sz/4 - 2,sz/4 - 2,XGR_RGB64K(16,16,16),XGR_RGB64K(16,16,16),XGR_FILLED);
	}
	else {
		mchA_d3dRectangleZ(sx - sz/2,sy - sz/2,z,sz,sz,col * 2,!col);
/*
		x = sx - sz/2 + 1; 
		y = sy - sz/8;

		col = mchA_d3dMakeColor(255,128,0);
		if(flags & TRACK_EL_FIRE)
			mchA_d3dRectangle_cl(x,y,sz/4 - 2,sz/4 - 2,col);

		x += sz/4;
		col = mchA_d3dMakeColor(0,128,255);
		if(flags & TRACK_EL_ELECTRICITY)
			mchA_d3dRectangle_cl(x,y,sz/4 - 2,sz/4 - 2,col);

		x += sz/4;
		col = mchA_d3dMakeColor(0,255,0);
		if(flags & TRACK_EL_SLIME)
			mchA_d3dRectangle_cl(x,y,sz/4 - 2,sz/4 - 2,col);

		x += sz/4;
		col = mchA_d3dMakeColor(64,64,64);
		if(flags & TRACK_EL_TELEPORT)
			mchA_d3dRectangle_cl(x,y,sz/4 - 2,sz/4 - 2,col);
*/		
	}
}

void mchTrackBranch::init_dist(int dist0)
{
	int i = 0;
	mchTrackElement* el = pointLst -> first();
	while(el){
		el -> index = dist0 + i;
		i ++;

		el = el -> next;
	}
	start_index = dist0;
}

int mchTrackBranch::get_dist(mchTrackElement* el)
{
	return (pointLst -> size() - (el -> index - start_index));
}

int mchTrackElement::get_dist(void)
{ 
	return (owner -> pointLst -> size() - (index - owner -> start_index)); 
}

int mchLoadTrackBTR(void)
{
	int i,v,sz;
	mchTrackBranch* p;

#ifndef _FCONTAINER_LOAD_
	XStream fh(0);

	if(!vmapOpenResource(GetTargetName("track.btr"),fh))
		return 0;

#else
	int buf_sz;
	char* buf;

	fContainer fc;
	XStream fh0(0);

	if(!vmapOpenResource(GetTargetName("output.idx"),fh0))
		return 0;

	fc.load_idx(fh0);
	fh0.close();

	if(fcPath)
		fc.set_path(fcPath);
	else
		fc.set_file("");

	fc.read(buf,buf_sz);

	XBuffer fh(buf,buf_sz);
#endif
	fh > v > sz;
	for(i = 0; i < sz; i ++){
		p = new mchTrackBranch;
		p -> LoadBTR(fh,v);

		mchTrackData -> append(p);
	}

#ifdef _FCONTAINER_LOAD_
	delete buf;
#endif

	return 1;
}

void mchTrackBranch::init_ids(int& id0)
{
	mchTrackElement* el = pointLst -> first();
	while(el){
		el -> ID = ++id0;
		el = el -> next;
	}
}

