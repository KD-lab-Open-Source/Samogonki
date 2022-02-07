#ifndef __DRAWPOLYGONSHADE_H__
#define __DRAWPOLYGONSHADE_H__

#include "UMATH.H"
#include "Material.h"
#include "IGraph3d.h"

#ifndef PREC_DRAW
#define PREC_DRAW					10
#endif //PREC_DRAW

struct sTracePolygon
{
public:	
	inline void SetTextureShade(sTexture *Texture,sTexture *Opacity=0,int Alfa=0)
	{
		t2=(unsigned char*) Texture->texture;
		_xt=Texture->_x; xtm=((1<<_xt)-1)<<PREC_DRAW;
		_yt=Texture->_y; ytm=((1<<_yt)-1)<<(PREC_DRAW+_xt);
	}

	void DrawPolygonShade(sVertexFix &p1,sVertexFix &p2,sVertexFix &p3,short *scr,int xScrMin,int yScrMin,int xScrMax,int yScrMax);
	void DrawPolygonShadeMask(sVertexFix &p1,sVertexFix &p2,sVertexFix &p3,short *scr,int xScrMin,int yScrMin,int xScrMax,int yScrMax);

	int GetAttribute(int xe1,int ye1,int xe2,int ye2,int xe3,int ye3);
	void SaveAttribute(char *fname);
	void SaveVoxel(char *fname);
private: // low level function render
	// render shade
	unsigned char	*t2;
	long			u1,v1,xe1,ye1,w1, u2,v2,xe2,ye2,w2, u3,v3,xe3,ye3,w3;
	long			xe,ye, xeb_,xec_,xee_,dxe,dxee_, yeb,yec,yee, _xt,_yt,xtm,ytm;

	inline int TestVisiblePolygonShadeMask(sVertexFix &v1,sVertexFix &v2,sVertexFix &v3)
	{
		xe1=round(v1.xe), xe2=round(v2.xe), xe3=round(v3.xe);
		ye1=round(v1.ye), ye2=round(v2.ye), ye3=round(v3.ye);
		return (xe3-xe2)*(ye3-ye1)-(xe3-xe1)*(ye3-ye2);
	}
	void PolygonLeftShade(short *scr,int xScrMin,int yScrMin,int xScrMax,int yScrMax);
	void PolygonRightShade(short *scr,int xScrMin,int yScrMin,int xScrMax,int yScrMax);
	void PolygonLeftShadeMask(short *scr,int xScrMin,int yScrMin,int xScrMax,int yScrMax);
	void PolygonRightShadeMask(short *scr,int xScrMin,int yScrMin,int xScrMax,int yScrMax);
	void PointShade(int xe,int ye,int w,short *scr,int xScrMin,int yScrMin,int xScrMax,int yScrMax);
	
	// render attribute map
	int FullAttribute,AttributeBit4,AttributeBit5,AttributeBit6,AttributeBit7;
	int AttributeTr0,AttributeTr1,AttributeTr2,AttributeTr3,AttributeTr4,AttributeTr5,AttributeTr6,
		AttributeTr7,AttributeTr8,AttributeTr9,AttributeTr10,AttributeTr11,AttributeTr12,AttributeTr13,
		AttributeTr14,AttributeTr15;

	inline void SetCoordAttribute(int xe1,int ye1,int xe2,int ye2,int xe3,int ye3)
	{
		sTracePolygon::xe1=xe1; sTracePolygon::ye1=ye1; 
		sTracePolygon::xe2=xe2; sTracePolygon::ye2=ye2; 
		sTracePolygon::xe3=xe3; sTracePolygon::ye3=ye3;
	}
	inline void CollectAttribute(int attr)
	{
		if(attr&(1<<4))	AttributeBit4++;
		if(attr&(1<<5))	AttributeBit5++;
		if(attr&(1<<6))	AttributeBit6++;
		if(attr&(1<<7))	AttributeBit7++;
		switch(attr&15)	
		{
			case 0:	AttributeTr0++;	break;
			case 1:	AttributeTr1++;	break;
			case 2:	AttributeTr2++;	break;
			case 3:	AttributeTr3++;	break;
			case 4:	AttributeTr4++;	break;
			case 5:	AttributeTr5++;	break;
			case 6:	AttributeTr6++;	break;
			case 7:	AttributeTr7++;	break;
			case 8:	AttributeTr8++;	break;
			case 9:	AttributeTr9++;	break;
			case 10: AttributeTr10++; break;
			case 11: AttributeTr11++; break;
			case 12: AttributeTr12++; break;
			case 13: AttributeTr13++; break;
			case 14: AttributeTr14++; break;
			case 15: AttributeTr15++; break;
		}
	}
	void PolygonRightMapAttribute();
	void PolygonLeftMapAttribute();
};

#endif //__DRAWPOLYGONSHADE_H__