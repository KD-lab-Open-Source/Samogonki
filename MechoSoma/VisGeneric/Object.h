#ifndef __LIGHT_H__ 
#define __LIGHT_H__ 

#include "Unknown.h"
#include "Camera.h"

class cLight
{
	Vect3f		Pos;
	Vect3f		Angle;			
	Vect3f		Tangent;		// вектор направления источника освещения
	sColor4f	Ambient;		// фоновая составляющая
	sColor4f	Diffuse;		// диффузная составляющая
	cMatrix		Matrix;
public:
	cLight();	
	~cLight();

	void SetAmbient(const sColor4f &ambient)			{ Ambient=ambient; }
	void SetDiffuse(const sColor4f &diffuse)			{ Diffuse=diffuse; }
	void SetPosition(Vect3f &pos,Vect3f &angle)		{ Pos=pos; SetRotate(angle); }
	void SetRotate(const Vect3f &angle)						
	{ 
		Angle=angle; 
		Matrix.NewMatrix(); 
		Matrix.Rotate(Angle.x,Angle.y,Angle.z); 
		Vect3f t(0,0,1);
		Tangent=Matrix.xformVect(t); 
	}
	sColor4f& GetAmbient()								{ return Ambient; }
	sColor4f& GetDiffuse()								{ return Diffuse; }
	Vect3f& GetTangent()								{ return Tangent; }
	Vect3f& GetAngle()									{ return Angle; }
	Vect3f& GetPos()									{ return Pos; }
	__forceinline int CalcColor(int cosAngle)					{ int c=cosAngle>>(MAX_SIZE_NORMAL_BIT-COLOR_GRADATION_BIT); if(c<(1<<COLOR_GRADATION_BIT)) { if(c>-(1<<COLOR_GRADATION_BIT)) return c; else return -((1<<COLOR_GRADATION_BIT)-1); } else return (1<<COLOR_GRADATION_BIT)-1; }
	__forceinline int CalcColor(int xNormal,int yNormal,int zNormal,
						 int xvLight,int yvLight,int zvLight,
						 int xvView, int yvView, int zvView,int cosLightView);
	__forceinline int CalcColorD(int xNormal,int yNormal,int zNormal,
						 int xvLight,int yvLight,int zvLight,
						 int xvView, int yvView, int zvView,int cosLightView);
	__forceinline int CalcColorM(int xNormal,int yNormal,int zNormal,
						 int xvLight,int yvLight,int zvLight,
						 int xvView, int yvView, int zvView,int cosLightView);
	__forceinline int CalcColorDM(int xNormal,int yNormal,int zNormal,
						 int xvLight,int yvLight,int zvLight,
						 int xvView, int yvView, int zvView,int cosLightView);
	// расчет яркости источника диффузной
	__forceinline int CalcToneDiffuse(Vect3f &vNormal,Vect3f &vLight);
	__forceinline int CalcToneMetal(Vect3f &vNormal,Vect3f &vLight,Vect3f &vView,float cosLightView);
	__forceinline int CalcToneDiffuse(sVect3c &vNormal,Vect3f &vLight);
	__forceinline int CalcToneMetal(sVect3c &vNormal,Vect3f &vLight,Vect3f &vView,float cosLightView);
};

__forceinline int cLight::CalcColor(int xNormal,int yNormal,int zNormal,
					 int xvLight,int yvLight,int zvLight,
					 int xvView, int yvView, int zvView,int cosLightView)
{
	return CalcColorD(xNormal,yNormal,zNormal, xvLight,yvLight,zvLight, xvView,yvView,zvView, cosLightView);
}

__forceinline int cLight::CalcColorD(int xNormal,int yNormal,int zNormal,
					 int xvLight,int yvLight,int zvLight,
					 int xvView, int yvView, int zvView,int cosLightView)
{
	return CalcColor(CosAngle(xNormal,yNormal,zNormal,xvLight,yvLight,zvLight));
}
__forceinline int cLight::CalcToneDiffuse(Vect3f &vNormal,Vect3f &vLight)
{
	float cosNormalLight=dot(vNormal,vLight);
	return 128+round(127*cosNormalLight);
}
__forceinline int cLight::CalcToneMetal(Vect3f &vNormal,Vect3f &vLight,Vect3f &vView,float cosLightView)
{
	float cosNormalLight=dot(vNormal,vLight);
	float cosNormalView =dot(vNormal,vView);
	float cosReflectView=2*(cosNormalLight*cosNormalView)-cosLightView;
	if(cosReflectView<0) return 0;
	int ToneSpecular=COSN_CHAR(int(round(127*cosReflectView)));
	return ToneSpecular;
}
__forceinline int cLight::CalcToneDiffuse(sVect3c &vNormal,Vect3f &vLight)
{
	float cosNormalLight=vNormal.x*vLight.x+vNormal.y*vLight.y+vNormal.z*vLight.z;
	return 127+round(cosNormalLight);
}
__forceinline int cLight::CalcToneMetal(sVect3c &vNormal,Vect3f &vLight,Vect3f &vView,float cosLightView)
{
	float cosNormalLight=vNormal.x*vLight.x+vNormal.y*vLight.y+vNormal.z*vLight.z;
	float cosNormalView =vNormal.x*vView.x+vNormal.y*vView.y+vNormal.z*vView.z;
	float cosReflectView=2*(cosNormalLight*cosNormalView)/(127.f*127.f)-cosLightView;
	if(cosReflectView<0) return 0;
	int ToneSpecular=COSN_CHAR(int(round(cosReflectView*127)));
	return ToneSpecular;
}

__forceinline int cLight::CalcColorM(int xNormal,int yNormal,int zNormal,
					 int xvLight,int yvLight,int zvLight,
					 int xvView, int yvView, int zvView,int cosLightView)
{
	int cosNormalLight=CosAngle(xNormal,yNormal,zNormal,xvLight,yvLight,zvLight);
	int cosNormalView =CosAngle(xNormal,yNormal,zNormal,xvView,yvView,zvView);
	int cosReflectView=(((cosNormalLight*cosNormalView)>>MAX_SIZE_NORMAL_BIT)<<1)-cosLightView;
	cosReflectView=COSN_CHAR(cosReflectView);
	return CalcColor(cosReflectView);
}

__forceinline int cLight::CalcColorDM(int xNormal,int yNormal,int zNormal,
					 int xvLight,int yvLight,int zvLight,
					 int xvView, int yvView, int zvView,int cosLightView)
{
	int cosNormalLight=CosAngle(xNormal,yNormal,zNormal,xvLight,yvLight,zvLight);
	if(cosNormalLight<0) return CalcColor(cosNormalLight);
	int cosNormalView =CosAngle(xNormal,yNormal,zNormal,xvView,yvView,zvView);
	if(cosNormalView<0) return CalcColor(cosNormalLight);
	int cosReflectView=(((cosNormalLight*cosNormalView)>>MAX_SIZE_NORMAL_BIT)<<1)-cosLightView;
	cosReflectView=COSN_CHAR(cosReflectView);
	if(cosNormalLight>cosReflectView) return CalcColor(cosNormalLight);
	else return CalcColor(cosReflectView);
}

#endif //__LIGHT_H__ 

#ifndef __OMNI_H__
#define __OMNI_H__

#define PALETTE_MAX_SIZE									6		/* for dos palette 0..63, for win palette 0..255 */

class cOmni
{
	Vect3f			Pos;
	sColor4f		Color;
	float			Radius;
	int				Visible;
	char			TotalVisibleCamera[NUMBER_CAMERA_MAX];	// видимость из камеры 
public:
	unsigned int	ID;

	cOmni(float xOmni=0.0f,float yOmni=0.0f,float zOmni=0.0f,float radius=0.0f,float r=0.0f,float g=0.0f,float b=0.0f);
	~cOmni();

	void Draw(cUnknownClass *UCameraList);
	__forceinline void SetColor(const sColor4f &color)				{ Color=color; }
	__forceinline void SetPosition(const Vect3f &pos)				{ Pos=pos;  }
	__forceinline void Translate(const Vect3f &dpos)				{ Pos+=dpos; }
	__forceinline void SetRadius(float radius)						{ Radius=radius; }
	__forceinline sColor4f& GetColor()								{ return Color; }
	__forceinline Vect3f& GetPos()								{ return Pos;  }
	__forceinline float& GetRadius()								{ return Radius; }
	__forceinline int isVisible(int visible=-1)					{ if(visible<0) return Visible&CONST_VISIBLE_SHOW; else if(visible>0) Visible|=CONST_VISIBLE_SHOW; else Visible&=~CONST_VISIBLE_SHOW; return Visible; }
	__forceinline int GetVisible(int visible)						{ return Visible&visible; }
	__forceinline void SetVisible(int visible)						{ Visible|=visible; }
	__forceinline void ClearVisible(int visible)					{ Visible&=~visible; }
	__forceinline int isVisibleTotal(cUnknownClass *UCameraList)	{ int test=0; for(int i=0;i<((cUnkClassDynArrayPointer*)UCameraList)->length();i++) test|=GetVisibleTotal(i); return test; }
	__forceinline char& GetVisibleTotal(int number)				{ assert(number<NUMBER_CAMERA_MAX); return TotalVisibleCamera[number]; }
	__forceinline void BuildDrawMatrix(cCamera *Camera,MatXf &DrawMatrix,int ShareWorld)
	{
		DrawMatrix.trans()=Pos;
		DrawMatrix.rot()=Mat3f::ID;
		WorldToCameraCutting(DrawMatrix,Camera);
		if(ShareWorld) WorldShareMapping(DrawMatrix.trans(),Camera);
		DrawMatrix=Camera->GetMatrix()*DrawMatrix;
	}
	void DrawID(cCamera *Camera);
};

#endif // __OMNI_H__

