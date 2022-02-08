////////////////////////////////////////////////////////////////////
//		Mechos для магазина
////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "MechosForSale.h"
#pragma hdrstop

MechosForSale::MechosForSale(char* KindsStr, const Vect3f& pos)
{
	cInterfaceVisGeneric *IVisGeneric=CreateIVisGeneric();
	xrealM3D = (cM3D*)IVisGeneric->GetM3D();
	IVisGeneric->Release();

	int Kinds[NUM_PARTS];
	XBuffer buf(KindsStr, strlen(KindsStr));
	for(int i = 0; i < NUM_ACTIVE_PARTS; i++){
		buf >= Kinds[i];
		if(!Kinds[i])
			Kinds[i] = 1;
		}
	Kinds[PART_INDEX(M3D_LF_WHEEL)] = Kinds[PART_INDEX(M3D_RF_WHEEL)];
	Kinds[PART_INDEX(M3D_LB_WHEEL)] = Kinds[PART_INDEX(M3D_RB_WHEEL)];

	geometry = xrealM3D -> CreateObject(M3D_TOTAL_TYPE(Kinds[0], M3D_ENGINE));
	geometry -> attachDummyChildren(Kinds);

//	setRot(-M_PI/2.0f, Z_AXIS);
	setRot(Mat3f(Vect3f(1.0f,0,-1.0f),1.0f));

	setTrans(pos);
	geometry -> SetPosition(pose());
}

void MechosForSale::changePart(int part_index, int part_number)
{
	int Kinds[NUM_PARTS];
	for(int i = 0; i < NUM_ACTIVE_PARTS; i++)
		Kinds[i] = M3D_KIND(part_by_index(i) -> Type);

	Kinds[part_index] = part_number;
	Kinds[PART_INDEX(M3D_LF_WHEEL)] = Kinds[PART_INDEX(M3D_RF_WHEEL)];
	Kinds[PART_INDEX(M3D_LB_WHEEL)] = Kinds[PART_INDEX(M3D_RB_WHEEL)];
	
	float scale = geometry -> GetScale().x;
	xrealM3D -> Delete(geometry);
	geometry = xrealM3D -> CreateObject(M3D_TOTAL_TYPE(Kinds[0], M3D_ENGINE));
	geometry -> SetPosition(Alg(),R());
	geometry -> attachDummyChildren(Kinds);
	geometry -> SetScale(scale);
}

void MechosForSale::changePart(cMesh* part)
{
	int type = M3D_TYPE(part -> Type);
	int kind = M3D_KIND(part -> Type);
	int kinds[NUM_PARTS];
	for(int i = 0; i < NUM_ACTIVE_PARTS; i++)
		kinds[i] = M3D_KIND(part_by_index(i) -> Type);
	kinds[PART_INDEX(type)] = kind;
	kinds[PART_INDEX(M3D_LF_WHEEL)] = kinds[PART_INDEX(M3D_RF_WHEEL)];
	kinds[PART_INDEX(M3D_LB_WHEEL)] = kinds[PART_INDEX(M3D_RB_WHEEL)];

	float scale = geometry -> GetScale().x;
	xrealM3D -> Delete(geometry);
//	xrealM3D -> Delete(part);
	geometry = xrealM3D -> CreateObject(M3D_TOTAL_TYPE(kinds[0], M3D_ENGINE));
	geometry -> SetPosition(Alg(),R());
	geometry -> attachDummyChildren(kinds);
	geometry -> SetScale(scale);
}

void MechosForSale::rotate(int dx, int dy)
{
	if(!(dx | dy))
		return;
	const float R = 100;
	float dr = sqrt(sqr((float)dx) + sqr((float)dy));
	float nx = (float)-dy/dr;
	float ny = (float)dx/dr;
	float theta = acos(R/(sqrt(R*R + dr*dr) + FLT_EPS));
	Mat3f M(Vect3f(nx, ny, 0), theta);
	setRot(M*Alg());
	geometry-> SetOrientation(Alg());
}

char* MechosForSale::getConfig()
{
	XBuffer buf(30);
	for(int i = 0; i < NUM_ACTIVE_PARTS; i++)
		buf <= M3D_KIND(part_by_index(i) -> Type) < " ";
	char* str = new char [strlen(buf) + 1];
	strcpy(str, buf);
	return str;
}

// Обращение к детали по типу
cMesh* MechosForSale::part_by_type(int partType) const { return geometry ? geometry -> FindMesh(partType) : 0; }
// Обращение к детали по индексу: 0-engine, 1-front, 2-back, 3-rf, 4-rb, 5-lf, 6-lb
cMesh* MechosForSale::part_by_index(int index) const { return geometry ? geometry -> FindMesh(1 << index) : 0; }

		
