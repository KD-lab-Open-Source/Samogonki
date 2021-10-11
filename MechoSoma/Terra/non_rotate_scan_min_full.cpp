#include "StdAfx.h"

#ifdef _HMV_

#include "HMV.h"
typedef unsigned short z_buffer_type;
z_buffer_type zBuffer[MAXX*MAXY];

#else 

#include "terra.h"
//#include "common.h"
//#include "vmap.h"
//#include "world.h"


#ifndef _SURMAP_
//template <class T> inline const T& min(const T& a, const T& b) { return a < b ? a : b; }
//template <class T> inline const T& max(const T& a, const T& b) { return a > b ? a : b; }
#endif

#define MAP_X_POWER	11
typedef unsigned short color_type;
typedef unsigned short z_buffer_type;
extern z_buffer_type *zBuffer;

// Maximal screen size for static buffer allocation
#undef MAXX
#define MAXX	1024

// Offset to map access. Note: x,y are unbounded and must be masked
#ifdef _SURMAP_
#define OFFSET(x,y) (XCYCL(x) + (YCYCL(y) << MAP_X_POWER))
#else
#define OFFSET(x,y) GetAlt(XCYCL(x),YCYCL(y))
#endif

// Access to color map by offset
//#define C(i) (color_buffer_ptr[i])
//#define C(i)	if(AtBuf[0][0][i]&At_WATER){ C(i)=(TrPal[AtBuf[0][0][i]&Tr_MASK][ClTrBuf[0][0][i]]);} \
//				else	C(i)|=15; 
inline unsigned short vrtMap::C(int i)
{
/*	if(AtBuf[0][0][i]&At_WATER){ 
		return (TrPal[16+(AtBuf[0][0][i]&Tr_MASK)][ClTrBuf[0][0][i]]);
	} 
	else {
		return (TrPal[AtBuf[0][0][i]&Tr_MASK][ClTrBuf[0][0][i]]);
	}*/
#ifndef FULLRENDER
	return (TrPal[AtBuf[0][0][i]&TrW_MASK][ClTrBuf[0][0][i]]);
#else
	return ClTrBuf[0][0][i];
#endif
}
// Access to height map by offset
//#define Z(i) (VxBuf[0][0][i])

//inline unsigned short& ZV(int i,int j) { return vMap->VxBuf[0][j][i]; }
//inline unsigned short& Z(int i) { return vMap->VxBuf[0][0][i]; }
#ifdef _SURMAP_
inline unsigned short ZF(int i)
{
	//if(AtBuf[0][0][i]&At_WATER)	return SpecBuf[0][0][i];
	//else 
	return (vMap->VxBuf[0][0][i]>>SHIFT_FRACTION_VX);
}
inline unsigned short Z(int i)
{
	//if(AtBuf[0][0][i]&At_WATER)	return SpecBuf[0][0][i];
	//else 
	return (vMap->VxBuf[0][0][i]>>SHIFT_FRACTION_VX);
}
#endif



// Switch for hi-color/256-colors modes. You may get rid of it if You doun't need it
#define PALETTE(c)	c

#endif

#define ZB(z)	(Height - (z))
#define CS	(MAXX/2 + 3)
typedef int Cell;

//void non_rotate_scan_min_zb(int Height,double focus,int u0,int v0,int xc,int yc,int xside,int yside)
void vrtMap::draw3D(int Height,double focus,int u0,int v0,int xc,int yc,int xside,int yside)
{
#ifdef _SURMAP_

#ifdef _USE_NOT_XGRAPH_
#else

	int CMAXX = XGR_MAXX;
					   
	int xsize = 2*xside;
	int ysize = 2*yside;

	int u0f = ((u0 = XCYCL(u0)) << 16) + (1 << 15);
	int v0f = ((v0 = YCYCL(v0)) << 16)  + (1 << 15);
	int z0f = (Height << 16) + (1 << 15);

	// Scan coefficients, step (-1,1,0):
	double dQx_y = -65536.;
	double dQy_y = 1;
	double dQz_y = 0;
	// Scan coefficients, step (1,-1,0):
	double dQx_x = 1;
	double dQy_x = -65536.;
	double dQz_x = 0;

	int x,y;
	int off;
	int xsd;
	int i,j,dj,dj0,ddj,z,dz;
	double Qx,Qy,Qz;
	double Q_inv,djf;
	color_type *vp, *vpc, *vpl, *vpr;
	static Cell cells[2*CS];
	Cell *left_cells, *right_cells, *p;
	z_buffer_type *zbp, *zbc, *zbl, *zbr;
	int zb;

	vpc = (color_type*)XGR_VIDEOBUF + yc*CMAXX + xc;
	vpc[0] = PALETTE(C(OFFSET(u0,v0)));
//	zbc = (z_buffer_type*)XGR_VIDEOBUF + yc*CMAXX + xc;
	zbc = (z_buffer_type*)zBuffer + yc*CMAXX + xc;
	zbc[0] = ZB(ZF(OFFSET(u0,v0)));
	
	// Erase upper line, because it isn't shown now 
	memset(vpc - yside*CMAXX - xside,0,xsize*sizeof(color_type));

	// Upward scan
	Qx = 0;
	Qy = 0;
	Qz = -65536.*focus;
	xsd = 0;
	left_cells = right_cells = cells + CS;
	*left_cells = 0;
	vpl = vpr = vpc;
	zbl = zbr = zbc;
	for(y = 1;y < yside;y++){
		Qx += dQx_y;
		Qy -= dQy_y;
		Qz -= dQz_y;
		Q_inv = 1/Qy;
		dz = round(Qz*Q_inv);
		dj0 = round(djf = Qx*Q_inv);
		ddj = -round(djf/(double)y);

		xsd++;
		left_cells[-1] = left_cells[0];
		right_cells[1] = right_cells[0];
		left_cells--;
		right_cells++;

		// Left sector
		dj = dj0;
		vp = vpl -= CMAXX + 1;
		zbp = zbl -= CMAXX + 1;
		p = left_cells;
		for(x = xsd;x > 0;x--){
			i = max(*p,*(p+1));
			j = u0f + i*dj;
			z = z0f + i*dz;
			i += v0;
			while(z >> 16 > (zb = Z(off = OFFSET(j >> 16,i)))){
				i--;
				j -= dj;
				z -= dz;
				}									      
			*vp++ = PALETTE(C(off));
//			*zbp++ = ZB(zb);
			*zbp++ = ZB(ZF(off));
			*p++ = i - v0;				  
			dj += ddj;
			}

		// Right sector
		dj = -dj0;
		vp = vpr -= CMAXX - 1;
		zbp = zbr -= CMAXX - 1;
		p = right_cells;
		for(x = xsd + 1;x > 0;x--){
			i = max(*p,*(p-1));
			j = u0f + i*dj;
			z = z0f + i*dz;
			i += v0;
			while(z >> 16 > (zb = Z(off = OFFSET(j >> 16,i)))){
				i--;
				j -= dj;
				z -= dz;
				}									      
			*vp-- = PALETTE(C(off));
//			*zbp-- = ZB(zb);
			*zbp-- = ZB(ZF(off));
			*p-- = i - v0;				  
			dj -= ddj;
			}
		}


	// Downward scan
	Qx = 0;
	Qy = 0;
	Qz = -65536.*focus;
	xsd = 0;
	left_cells = right_cells = cells + CS;
	*left_cells = 0;
	vpl = vpr = vpc;
	zbl = zbr = zbc;
	for(y = 1;y < yside;y++){
		Qx += dQx_y;
		Qy += dQy_y;
		Qz += dQz_y;
		Q_inv = 1/Qy;
		dz = round(Qz*Q_inv);
		dj0 = round(djf = Qx*Q_inv);
		ddj = -round(djf/(double)y);

		xsd++;
		left_cells[-1] = left_cells[0];
		right_cells[1] = right_cells[0];
		left_cells--;
		right_cells++;

		// Left sector
		dj = dj0;
		vp = vpl += CMAXX - 1;
		zbp = zbl += CMAXX - 1;
		p = left_cells;
		for(x = xsd;x > 0;x--){
			i = min(*p,*(p+1));
			j = u0f + i*dj;
			z = z0f + i*dz;
			i += v0;
			while(z >> 16 > (zb = Z(off = OFFSET(j >> 16,i)))){
				i++;
				j += dj;
				z += dz;
				}									      
			*vp++ = PALETTE(C(off));
//			*zbp++ = ZB(zb);
			*zbp++ = ZB(ZF(off));
			*p++ = i - v0;				  
			dj += ddj;
			}

		// Right sector
		dj = -dj0;
		vp = vpr += CMAXX + 1;
		zbp = zbr += CMAXX + 1;
		p = right_cells;
		for(x = xsd + 1;x > 0;x--){
			i = min(*p,*(p-1));
			j = u0f + i*dj;
			z = z0f + i*dz;
			i += v0;
			while(z >> 16 > (zb = Z(off = OFFSET(j >> 16,i)))){
				i++;
				j += dj;
				z += dz;
				}									      
			*vp-- = PALETTE(C(off));
//			*zbp-- = ZB(zb);
			*zbp-- = ZB(ZF(off));
			*p-- = i - v0;				  
			dj -= ddj;
			}
		}

	
	// Righthand scan
	Qx = 0;
	Qy = 0;
	Qz = -65536.*focus;
	xsd = 0;
	left_cells = right_cells = cells + CS;
	*left_cells = 0;
	vpl = vpr = vpc;
	zbl = zbr = zbc;
	for(y = 1;y < xside;y++){
		Qx += dQx_x;
		Qy += dQy_x;
		Qz += dQz_x;
		Q_inv = 1/Qx;
		dz = round(Qz*Q_inv);
		dj0 = round(djf = Qy*Q_inv);
		ddj = -round(djf/(double)y);

		if(y < yside){
			vpl -= CMAXX;
			vpr += CMAXX;
			zbl -= CMAXX;
			zbr += CMAXX;
			xsd++;
			left_cells[-1] = left_cells[0];
			right_cells[1] = right_cells[0];
			left_cells--;
			right_cells++;
			}
		else
			dj0 = -(yside - 1)*ddj;

		// Left sector
		dj = dj0;
		vp = ++vpl;
		zbp = ++zbl;
		p = left_cells;
		for(x = xsd;x > 0;x--){
			i = min(*p,*(p+1));
			j = v0f + i*dj;
			z = z0f + i*dz;
			i += u0;
			while(z >> 16 > (zb = Z(off = OFFSET(i,j >> 16)))){
				i++;
				j += dj;
				z += dz;
				}									      
			*vp = PALETTE(C(off));
			vp += CMAXX;
//			*zbp = ZB(zb);
			*zbp = ZB(ZF(off));
			zbp += CMAXX;
			*p++ = i - u0;				  
			dj += ddj;
			}

		// Right sector
		dj = -dj0;
		vp = ++vpr;
		zbp = ++zbr;
		p = right_cells;
		for(x = xsd + 1;x > 0;x--){
			i = min(*p,*(p-1));
			j = v0f + i*dj;
			z = z0f + i*dz;
			i += u0;
			while(z >> 16 > (zb = Z(off = OFFSET(i,j >> 16)))){
				i++;
				j += dj;
				z += dz;
				}									      
			*vp = PALETTE(C(off));
			vp -= CMAXX;
//			*zbp = ZB(zb);
			*zbp = ZB(ZF(off));
			zbp -= CMAXX;
			*p-- = i - u0;				  
			dj -= ddj;
			}
		}


	// Lefthand scan
	Qx = 0;
	Qy = 0;
	Qz = -65536.*focus;
	xsd = 0;
	left_cells = right_cells = cells + CS;
	*left_cells = 0;
	vpl = vpr = vpc;
	zbl = zbr = zbc;
	for(y = 1;y < xside + 1;y++){
		Qx -= dQx_x;
		Qy += dQy_x;
		Qz -= dQz_x;
		Q_inv = 1/Qx;
		dz = round(Qz*Q_inv);
		dj0 = round(djf = Qy*Q_inv);
		ddj = -round(djf/(double)y);

		if(y < yside){
			vpl -= CMAXX;
			vpr += CMAXX;
			zbl -= CMAXX;
			zbr += CMAXX;
			xsd++;
			left_cells[-1] = left_cells[0];
			right_cells[1] = right_cells[0];
			left_cells--;
			right_cells++;
			}
		else
			dj0 = -(yside - 1)*ddj;

		// Left sector
		dj = dj0;
		vp = --vpl;
		zbp = --zbl;
		p = left_cells;
		for(x = xsd;x > 0;x--){
			i = max(*p,*(p+1));
			j = v0f + i*dj;
			z = z0f + i*dz;
			i += u0;
			while(z >> 16 > (zb = Z(off = OFFSET(i,j >> 16)))){
				i--;
				j -= dj;
				z -= dz;
				}									      
			*vp = PALETTE(C(off));
			vp += CMAXX;
//			*zbp = ZB(zb);
			*zbp = ZB(ZF(off));
			zbp += CMAXX;
			*p++ = i - u0;				  
			dj += ddj;
			}

		// Right sector
		dj = -dj0;
		vp = --vpr;
		zbp = --zbr;
		p = right_cells;
		for(x = xsd + 1;x > 0;x--){
			i = max(*p,*(p-1));
			j = v0f + i*dj;
			z = z0f + i*dz;
			i += u0;
			while(z >> 16 > (zb = Z(off = OFFSET(i,j >> 16)))){
				i--;
				j -= dj;
				z -= dz;
				}									      
			*vp = PALETTE(C(off));
			vp -= CMAXX;
//			*zbp = ZB(zb);
			*zbp = ZB(ZF(off));
			zbp -= CMAXX;
			*p-- = i - u0;				  
			dj -= ddj;
			}
		}
#endif// _USE_NOT_XGRAPH_

#endif //_SURMAP_
}

// Converts from screen {xs,ys} to global {xg,yg,zg}
void vrtMap::S2G_3D_non_rotate(int camera_x,int camera_y,int camera_z,double focus,int screen_x_center,int screen_y_center,
			int xs, int ys, int &xg, int &yg, int &zg)
{
	double Qx,Qy,Qz,Q_inv;
	int x,y,z,dz,i,j,dj, u0,u0f, v0,v0f, z0f;

	x = xs - screen_x_center;
	y = ys - screen_y_center;

	u0f = ((u0 = XCYCL(camera_x)) << 16) + (1 << 15);
	v0f = ((v0 = YCYCL(camera_y)) << 16)  + (1 << 15);
	z0f = (camera_z << 16) + (1 << 15);
	
	if(abs(y) > abs(x)){ // Vertical case
		Qx = x << 16;
		Qy = y;
		Qz = -65536.*focus;
		Q_inv = 1/Qy;
		dz = round(Qz*Q_inv);
		dj = round(Qx*Q_inv);
		i = v0;
		j = u0f;
		z = z0f;
		if(y < 0) // Downward
			while(z >> 16 > GetAltC(j >> 16,i)){
				i--;
				j -= dj;
				z -= dz;
				}									      
		else // Upward
			while(z >> 16 > GetAltC(j >> 16,i)){
				i++;
				j += dj;
				z += dz;
				}
		xg = XCYCL(j >> 16);
		yg = YCYCL(i);
		zg = GetAltC(xg,yg);
		}
	else{ // Horizontal case
		Qx = x;
		Qy = y << 16;
		Qz = -65536.*focus;
		Q_inv = 1/Qx;
		dz = round(Qz*Q_inv);
		dj = round(Qy*Q_inv);
		i = u0;
		j = v0f;
		z = z0f;
		if(x < 0) // Left
			while(z >> 16 > GetAltC(i,j >> 16)){
				i--;
				j -= dj;
				z -= dz;
				}									      
		else // Right
			while(z >> 16 > GetAltC(i,j >> 16)){
				i++;
				j += dj;
				z += dz;
				}
		xg = XCYCL(i);
		yg = YCYCL(j >> 16);
		zg = GetAltC(xg,yg);
		}									      
}										      
void vrtMap::S2G_2D_non_rotate(int camera_x,int camera_y,int camera_z,double focus,int screen_x_center,int screen_y_center,
			int xs, int ys, int &xg, int &yg, int &zg)
{
	int x = xs - screen_x_center;
	int y = ys - screen_y_center;
	double k = (double)camera_z/focus;
	xg = camera_x + round(x*k);
	yg = camera_y + round(y*k);
	zg = GetAltC(xg,yg);
 }

void S2G_z_buffer(int camera_x,int camera_y,int camera_z,double focus,int screen_x_center,int screen_y_center,
			int xs, int ys, int plane_mode, int &xg, int &yg, int &zg)
{
	int x = xs - screen_x_center;
	int y = ys - screen_y_center;
#ifdef _USE_NOT_XGRAPH_
	int zs = zBuffer[xs + ys*0];
#else
	int zs = zBuffer[xs + ys*XGR_MAXX];
#endif
	double k = (double)(plane_mode ? camera_z : zs)/focus;
	xg = camera_x + round(x*k);
	yg = camera_y + round(y*k);
	zg = camera_z - zs;
 }

double Z_average(int xx, int yy, int D)
{
	int x,y,z = 0;
	for(y = yy - D;y <= yy + D;y++)
		for(x = xx - D;x <= xx + D;x++)
			z += vMap -> VxBuf[0][0][OFFSET(x,y)];
	return (double)z/sqr(2*D + 1);
}
		
