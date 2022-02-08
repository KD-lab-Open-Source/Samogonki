/* ---------------------------- INCLUDE SECTION ----------------------------- */
#include "StdAfx.h"

#include "iText.h"
#include "Mesh3ds.h"

#include "HFONT.H"

#include "arcane_menu.h"
#include "arcane_menu_d3d.h"

#include "SpriteDispatcher.h"
#include "TrackDispatcher.h"

#include "MechosForSale.h"

#include "parts_pool.h"
#include "race.h"

#include "ACI_EVNT.H"
#include "ACI_SCR.H"
#include "ACI_IDS.H"

#include "KEYS.H"
#include "sound.h"

#include "XJoystick.h"

#include "mechosoma.h"

#include "mch_common.h" // For far target
#include "IGraph3d.h"
#include "GameClient.h"

/* ----------------------------- STRUCT SECTION ----------------------------- */

// acsEssenceData::flags
const int ACS_ESS_STOPPED	= 0x01;
const int ACS_ESS_FREE		= 0x02;
const int ACS_ESS_ROTATE	= 0x04;

struct acsEssenceData
{
	int ID;
	int number;
	char* name;

	Vect3f r;
	Vect3f v;

	int flags;

	float psi;

	aciScreenObject* bound;

	void Init(void);
	void GetBound(int scr_id,int obj_id);
	void Quant(void);
	void Redraw(void);
	void ScreenCollision(void);

	void Stop(void);
	void Start(void);

	acsEssenceData(int id = 0,int num = 0);
	~acsEssenceData(void){ };
};

const int ACS_SCREEN_MATRIX_SIZE	= 700;

// acsScreenMatrixElement::flags
const int	ACS_MTX_MOVE_DOWN	= 0x01;

struct acsScreenMatrixElement 
{
	unsigned char ch;

	int flags;

	int color;
	int alpha;

	Vect3f Scale0;
	Vect3f Scale1;

	Vect3f R0;
	Vect3f R1;

	void set_r(acsScreenMatrixElement* p);
	void set_r(int mode);
	void clear(void){ ch = 0xFF; }
};

struct acsScreenMatrix
{
	int matrixSize;
	acsScreenMatrixElement matrix[ACS_SCREEN_MATRIX_SIZE];

	void add(unsigned ch,int x,int y,int fnt,int cl,int al,int fl = 0);
	void clear(void){ int i; for(i = 0; i < ACS_SCREEN_MATRIX_SIZE; i ++) matrix[i].clear(); matrixSize = 0; }
	void redraw(float phase);
	void build(aciScreen* p);

	int getEl(acsScreenMatrixElement* p);

	acsScreenMatrix(void){ clear(); }
	~acsScreenMatrix(void){ }
};

// acsTextureMosaicElement::flags
const int ATM_UPDATE	= 0x01;

struct acsTextureMosaicElement
{
	int X;
	int Y;
	int Size;

	int flags;

	unsigned short* data;

	void clear(void);	//{ memset(data,0,sizeof(unsigned short) * Size * Size); }
	void dump(void);

	acsTextureMosaicElement(int x,int y,int sz);
	~acsTextureMosaicElement(void);
};

struct acsTextureMosaic
{
	int elSize;

	int IndexX;
	int IndexY;

	acsTextureMosaicElement** mosaic;

	int PutStr(int x,int y,int fnt,int sp,void* p,void* pal,int col,int alpha,int align = 0);
	void PutSpr(int x,int y,int sx,int sy,void* p,void* pal,int col,int alpha);
	void dump(void);

	void Quant(void);
	void clearQuant(void);

	acsTextureMosaic(int x,int y);
	~acsTextureMosaic(void);
};

/* ----------------------------- EXTERN SECTION ----------------------------- */

extern int acsInputMode;

extern float mchA_FontScaleX[];
extern float mchA_FontScaleY[];

extern int acsNumSaves;
extern int acsCurrentSave;

extern float mchA_d3dResX;
extern float mchA_d3dResY;

extern cSurfaceReflectionMultiMaterial *GlobalMainMenuSurfaceReflection;

extern aciScreenObject* acsLastObj;

extern int mchResourcesFlag;

extern int acsAlpha;
extern int acsAlphaDest;
extern int acsAlphaDelta;
extern int acsAlphaEvent;

extern unsigned mchA_MenuPal[];
extern unsigned mchA_MenuPal2[];
extern HFont** acsFntTable;

extern cInterfaceVisGeneric	*gb_IVisGeneric;
extern cInterfaceGraph3d	*gb_IGraph3d;
extern cUnknownClass		*gb_UScene;
extern cUnknownClass		*iCamera;

extern aciScreenObject* acsMouseObj;
extern int acsFirstSlot;

/* --------------------------- PROTOTYPE SECTION ---------------------------- */

void acsShowVersionInfo(void);

void acsInitShop(int pl);
void acsFinitShop(int mode = 0);
void acsRedrawShop(void);
void acsRedrawShopArcanes(void);
void acsShopMoveMouse(int dx,int dy);

void mchA_ShowLayoutID(void);

void acsShopChangePart(int tp,int dir);

void acsDrawHS_CfgScreen(void);

void acsShowSlotInfo(void);
char* acsGetSaveString(int n);

int acsGetSaveSlotFigure(int n);
int acsGetSaveSlotIndex(int n);
void acsInitSaveOptions(void);

void mchDrawQuant(void);

int acsStrLen(int fnt,unsigned char* str,int space);
int acsTextStrLen(int fnt,unsigned char* str,int space);
int acsTextStrLenMax(int fnt,unsigned char* str,int space);

void mchA_DrawMouse(int mode = 0);
void acsUpdateMM_Option(int scr,int obj);

void acsGetTexture(int x,int y,unsigned char* p);

void acsOutMouse3D(void);
Vect3f acsCalcR(Vect3f r0,float phase);
float acsCalcForce(float z_min,float z_max,float z);

unsigned mch_toupper(unsigned ch);

void acsOutChar3D(float x,float y,float z,int fnt,int ch,float r,float g,float b,float a,int mat_fl = 0,Mat3f mat = Mat3f::ZERO,int rot = 1,float sc = 1.0f);
void mchA_OutChar3D(float x,float y,float z,int fnt,int ch,float r,float g,float b,float a,int mat_fl = 0,Mat3f mat = Mat3f::ZERO,int rot = 1,float sc = 1.0f);
void acsDrawM3D(cMesh* p,int rot = 0);
void mchA_DrawM3D(cMesh* p);

void acsInitFont3D(void);
void acsFinitFont3D(void);

char* getIniKey(char* fname,char* section,char* key);

void acsExtQuant(void);
void acsExtInit(void);

void acsEssenceQuant(void);
void acsRedrawEssence(void);

void acsPrepareChangeScreen3D(int scr_id);
float acsChangeScreenQuant3D(void);

void acsInitEssenceScreen(void);
void acsShowCredits(void);

void acsExtSetCamera(int zbuf_flag = 0);

/* --------------------------- DEFINITION SECTION --------------------------- */

#define SHOP_MECH_SCALE		4.0f

#define ACS_WATER_COL		(7 << 5)
#define ACS_CREDITS_COL		(2 << 5)
#define ACS_WATER_ALPHA		0

#define TEXTURE_AMBIENT		(1.0f)

int acsShopFlag = 0;

cMesh* acsMouse3D = NULL;
cMesh** acsFont3D = NULL;

float acsFont3D_ScaleX[5] = { 1.0f, 0.5f, 0.30f, 1.0f, 1.0f };
float acsFont3D_ScaleY[5] = { 1.0f, 0.5f, 0.30f, 1.0f, 1.0f };
float acsFont3D_ScaleZ[5] = { 2.0f, 1.9f, 1.0f, 0.5f, 0.5f };

float acsFont3D_SizeX = 40.0f;
float acsFont3D_SizeY = 50.0f;

float acsCameraFocus = 640;
int acsCameraPerspective = 1;
Vect3f acsCameraR;
Vect3f acsCameraA;
float acsCameraDZ = 0.0f;

float acsMouseZ = 10.0f;
float acsMouseScale = 1.0f;
float acsMouseAlpha = 0.7f;
Vect3f acsMouseRGB;

Vect3f acsColor3D[256];

Vect3f acsR0;
Vect3f acsR0_1;
float acsDist0;
int acsDistRnd;

acsScreenMatrix acsScr0;
acsScreenMatrix acsScr1;

char* mchIscreenINI = "RESOURCE/ISCREEN/iscreen.ini";
char* mchLocalINI = "RESOURCE/ISCREEN/local.ini";

acsTextureMosaic* acsTM = NULL;
acsEssenceData** acsBody = NULL;
acsEssenceData* acsChosenBody = NULL;
int acsChosenBodyFlag = 0;

float acsMouseMaxZ = 100.0f;
float acsMouseVz = 0.0f;
float acsWaterZ = 20.0f;
float acsG = 10.0f;
float acsA = 2.0f;
float acsDissipation = 0.01f;

float acs_dT = 0.1f;

unsigned char acsChCenterFlags[256];

int acsTextPhase = 0;
int acsTextPhaseMax = 100;

int acsMaxEssenceID = 10;
int acsEssence3DS[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }; 

MechosForSale* acsMechos;
int acsShopPlayer = 0;

char acsShopCfgBackup[100];

void acsExtInitPrm(void)
{
	char* p;
	XBuffer* XBuf,xb;

	acsMouseAlpha = atof(getIniKey(mchIscreenINI,"mouse","alpha"));
	acsMouseScale = atof(getIniKey(mchIscreenINI,"mouse","scale"));
	acsMouseZ = atof(getIniKey(mchIscreenINI,"mouse","z"));

	acsWaterZ = atof(getIniKey(mchIscreenINI,"mouse","water_z"));
	acsG = atof(getIniKey(mchIscreenINI,"mouse","gravity_force"));
	acsA = atof(getIniKey(mchIscreenINI,"mouse","archimed_force"));
	acs_dT = atof(getIniKey(mchIscreenINI,"mouse","dt"));
	acsDissipation  = atof(getIniKey(mchIscreenINI,"mouse","dissipation"));

	p = getIniKey(mchIscreenINI,"mouse","rgb");
	XBuf = new XBuffer(p,strlen(p));
	*XBuf >= acsMouseRGB.x >= acsMouseRGB.y >= acsMouseRGB.z;
	delete XBuf;

	acsCameraFocus = atof(getIniKey(mchIscreenINI,"camera","focus"));
	acsCameraPerspective = atoi(getIniKey(mchIscreenINI,"camera","perspective"));
	acsCameraDZ = atof(getIniKey(mchIscreenINI,"camera","z0"));

	p = getIniKey(mchIscreenINI,"camera","pos");
	XBuf = new XBuffer(p,strlen(p));
	*XBuf >= acsCameraR.x >= acsCameraR.y >= acsCameraR.z;
	delete XBuf;

	p = getIniKey(mchIscreenINI,"camera","angles");
	XBuf = new XBuffer(p,strlen(p));
	*XBuf >= acsCameraA.x >= acsCameraA.y >= acsCameraA.z;
	delete XBuf;

	p = getIniKey(mchIscreenINI,"screen change","r0");
	XBuf = new XBuffer(p,strlen(p));
	*XBuf >= acsR0.x >= acsR0.y >= acsR0.z;
	delete XBuf;

	acsR0.x = XGR_MAXX/2;
	acsR0.y = XGR_MAXY/2;

	p = getIniKey(mchIscreenINI,"screen change","r0_1");
	XBuf = new XBuffer(p,strlen(p));
	*XBuf >= acsR0_1.x >= acsR0_1.y >= acsR0_1.z;
	delete XBuf;

	acsDist0 = atof(getIniKey(mchIscreenINI,"screen change","dist0"));
	acsDistRnd = atoi(getIniKey(mchIscreenINI,"screen change","dist0_rnd"));

	acsExtSetCamera();
}

void acsExtInit(void)
{
	int i,idx;

	aciScreenResourcePAL* pal;

	acsExtInitPrm();

	pal = (aciScreenResourcePAL*)acsScrD -> GetResource(ACS_MAIN_MENU_PAL0);
	if(pal){
		for(i = 0; i < 256; i ++){
			idx = i * 3;
			acsColor3D[i].x = (float)pal -> data[idx] / 63.0f;
			acsColor3D[i].y = (float)pal -> data[idx + 1] / 63.0f;
			acsColor3D[i].z = (float)pal -> data[idx + 2] / 63.0f;
		}
	}

	if(!acsTM)
		acsTM = new acsTextureMosaic(4,4);
}

void acsExtQuant(void)
{
	acsTextPhase ++;
	if(acsTextPhase > acsTextPhaseMax)
		acsTextPhase = acsTextPhaseMax;

	mchSoundQuant();
	if(acsScrD -> curScr -> ID != ACS_CREDITS_SCR)
		mchDrawQuant();
	else {
		gb_IGraph3d->Fill(0,0,0);
	}

	if(acsScrD -> curScr -> ID == ACS_ESSENCE_SCR)
		acsEssenceQuant();
}

float GlobalPeriodRotate1=0;
float GlobalPeriodRotate2=0;

#define ACS_SHOP_TEXTURE_IDX	6
void acsExtRedraw(void)
{
	if(RenderMode == DIRECT3D_HICOLOR){
		mchA_d3dPrepare();
	}

	acsOutMouse3D();
	
	if(acsScrD -> curScr -> ID == ACS_CONSTRUCT_SCR)
		acsRedrawShop();

	if(acsScrD -> curScr -> ID == ACS_ESSENCE_SCR)
		acsRedrawEssence();

//	if(acsScrD -> curScr -> ID == ACS_CREDITS_SCR)
//		acsShowCredits();

	if(acsScrD -> curScr -> ID == ACS_LOAD_GAME_SCR)
		acsShowSlotInfo();

	if(acsScrD -> curScr -> ID == ACS_MAIN_MENU_SCR0 || acsScrD -> curScr -> ID == ACS_MAIN_MENU_SCR1 || acsScrD -> curScr -> ID == ACS_MAIN_MENU_SCR2 || acsScrD -> curScr -> ID == ACS_MAIN_MENU_SCR3 || acsScrD -> curScr -> ID == ACS_MAIN_MENU_SCR4 || acsScrD -> curScr -> ID == ACS_MAIN_MENU_SCR5 || acsScrD -> curScr -> ID == ACS_MAIN_MENU_SCR6)
		acsShowVersionInfo();

	acsTM -> Quant();
	GlobalPeriodRotate2 = GlobalPeriodRotate1 += 0.05f;

	if((!mchResourcesFlag || acsScrD -> curScr -> ID == ACS_CREDITS_SCR) && !(acsScrD -> curScr -> flags & ACS_NO_BACKGROUND))
		BaseObject_SurfaceReflection_Draw(iCamera);

	acsTM -> clearQuant();

	if(acsScrD -> curScr -> ID == ACS_SPLIT_HS_SCR){
		gb_IGraph3d->BeginScene();
		acsDrawHS_CfgScreen();
	}

	if(acsScrD -> curScr -> ID == ACS_CREDITS_SCR){
		gb_IGraph3d->BeginScene();
		acsShowCredits();
	}

	if(acsScrD -> curScr -> ID == ACS_CONSTRUCT_SCR){
		gb_IGraph3d->BeginScene();
		acsRedrawShopArcanes();
	}

	if(mchResourcesFlag && acsScrD -> curScr -> ID != ACS_CREDITS_SCR){
		gb_IGraph3d->BeginScene();
		mchA_DrawMouse(1);
	}

	if(acsScrD -> activeInput && acsInputMode != ACS_KEY_INPUT_MODE){
		gb_IGraph3d->BeginScene();
		mchA_ShowLayoutID();
		gb_IGraph3d->EndScene();
	}

#ifdef _ACS_MOVE_OBJECTS_
	if(aciShowBounds){
		gb_IGraph3d->BeginScene();
		acsScrD -> curScr -> show_bound();
		gb_IGraph3d->EndScene();
	}
#endif
}

void acsStrLen3D(void* str,int fnt,int space,int& sx,int& sy)
{
	int i,len,sz = 0,sz0 = 0,fsx,fsy;
	unsigned ch;

	unsigned char* p = (unsigned char*)str;

	float scale_x = (float)XGR_MAXX / 640.0f;
	float scale_y = (float)XGR_MAXY / 480.0f;

	fsx = round(acsFont3D_SizeX * acsFont3D_ScaleX[fnt]);
	fsy = round(acsFont3D_SizeY * acsFont3D_ScaleY[fnt]);

	sy = fsy;

	len = strlen((char*)str);
	for(i = 0; i < len; i ++){
		if(p[i] == '\n'){
			if(sz > sz0) sz0 = sz;
			sz = 0;
			sy += fsy;
		}
		else {
			if(p[i] != '\r'){
				ch = mch_toupper(p[i]);
				if(acsFont3D[ch])
					sz += round((acsFont3D[ch] -> Real.MaxOriginal[0] - acsFont3D[ch] -> Real.MinOriginal[0]) * acsFont3D_ScaleX[fnt]) + space;
				else
					sz += fsx + space;
			}
		}
	}
	if(sz > sz0) sz0 = sz;
	sy = round((float)sy * scale_y);
	sx = round((float)sz0 * scale_x);
}

void acsInitFont3D(void)
{
	int i;

	if(acsFont3D) return;
	acsFont3D = new cMesh*[256];

	for(i = 0; i < 256; i ++)
		acsFont3D[i] = MeshLibrary -> Find(M3D_TOTAL_TYPE((i + M3D_CHAR0_KIND),M3D_iSCREEN));

	acsMouse3D = MeshLibrary -> Find(M3D_TOTAL_TYPE(M3D_MOUSE3D_KIND,M3D_MECHOSOMA));

	memset(acsChCenterFlags,0,256);

	acsChCenterFlags['-'] = 1;
	acsChCenterFlags['='] = 1;
	acsChCenterFlags['*'] = 1;
/*
	int src_idx,dst_idx;

	char* str0,*str1;
	XBuffer* src,*dst;

	str0 = strdup(getIniKey(mchLocalINI,"font_3d","map_src"));
	src = new XBuffer(str0,strlen(str0));

	str1 = strdup(getIniKey(mchLocalINI,"font_3d","map_dest"));
	dst = new XBuffer(str1,strlen(str1));

	src_idx = 1;

	while(src_idx){
		*src >= src_idx;
		*dst >= dst_idx;

		if(src_idx){
			acsFont3D[src_idx] = acsFont3D[dst_idx];
			acsChCenterFlags[src_idx] = acsChCenterFlags[dst_idx];
		}
	}

	delete src;
	delete dst;

	free(str0);
	free(str1);
*/
}

void acsFinitFont3D(void)
{
	if(acsFont3D){
		delete acsFont3D;
		acsFont3D = NULL;
	}
}

void acsGetCharSize3D(int ch,float& sx,float& sy,float& sz)
{
	if(acsFont3D[ch]){
		sx = acsFont3D[ch] -> Real.MaxOriginal[0] - acsFont3D[ch] -> Real.MinOriginal[0];
		sy = acsFont3D[ch] -> Real.MaxOriginal[1] - acsFont3D[ch] -> Real.MinOriginal[1];
		sz = acsFont3D[ch] -> Real.MaxOriginal[2] - acsFont3D[ch] -> Real.MinOriginal[2];
	}
	else {
		sx = acsFont3D_SizeX;
		sy = acsFont3D_SizeY;
		sz = sx;
	}
}

void mchA_OutChar3D(float x,float y,float z,int fnt,int ch,float r,float g,float b,float a,int mat_fl,Mat3f mat,int rot,float sc)
{
	float scale_x,scale_y;
	if(!acsFont3D[ch]) return;

	Vect3f r0,rr,r_rot;
	Mat3f rot0;

	acsFont3D[ch] -> SetColor(sColor4f(r,g,b,a));

	scale_x = (float)XGR_MAXX / 640.0f;
	scale_y = (float)XGR_MAXY / 480.0f;

	Vect3f v1(acsFont3D_ScaleX[fnt] * scale_x * sc,acsFont3D_ScaleY[fnt] * scale_y * sc,acsFont3D_ScaleZ[fnt] * sc);
	gb_IVisGeneric -> SetObjectScale((cUnknownClass*)acsFont3D[ch],&v1);

	rr = Vect3f(0,0,0);
	r0 = Vect3f(x,y,z);

	if(rr.norm() > 3.0f) rr.normalize(3.0f);

	Vect3f v2(r0.x,r0.y,r0.z);
	Vect3f v3(0,0,0);
	gb_IVisGeneric -> SetObjectPosition((cUnknownClass*)acsFont3D[ch],&v2,&v3);

	if(mat_fl)
		acsFont3D[ch] -> SetOrientation(mat);

	mchA_DrawM3D(acsFont3D[ch]);
}

void acsOutChar3D(float x,float y,float z,int fnt,int ch,float r,float g,float b,float a,int mat_fl,Mat3f mat,int rot,float sc)
{
	float scale_x,scale_y,z0;
	if(!acsFont3D[ch]) return;

	Vect3f r0,rr,r_rot;
	Mat3f rot0;

	acsFont3D[ch] -> SetColor(sColor4f(r,g,b,a),sColor4f(0,0,0,0));

	scale_x = (float)XGR_MAXX / 640.0f;
	scale_y = (float)XGR_MAXY / 480.0f;

	Vect3f v1(acsFont3D_ScaleX[fnt] * scale_x * sc,acsFont3D_ScaleY[fnt] * scale_y * sc,acsFont3D_ScaleZ[fnt] * sc);
	gb_IVisGeneric -> SetObjectScale((cUnknownClass*)acsFont3D[ch],&v1);

	rr = Vect3f(0,0,0);
	r0 = Vect3f(x,y,z);
	BaseObject_SurfaceReflection_Pos(r0,z0);
	BaseObject_SurfaceReflection_Pos(r0,rr);

	if(rr.norm() > 3.0f) rr.normalize(3.0f);
//	r0 += z0 * rr;

	Vect3f v2(r0.x,r0.y,r0.z);
	Vect3f v3(0,0,0);
	gb_IVisGeneric -> SetObjectPosition((cUnknownClass*)acsFont3D[ch],&v2,&v3);

	if(z0 > 0.5f) z0 = 0.5f;
	if(z0 < -0.5f) z0 = -0.5f;
/*
	rot0.set(rr % rr.K,z0);
	acsFont3D[ch] -> SetOrientation(rot0);
*/
	if(mat_fl)
		acsFont3D[ch] -> SetOrientation(mat);

	acsDrawM3D(acsFont3D[ch],0);
}

void acsOutString3D(int x,int y,void* str,int fnt,int col,int space,int alpha,float z,float sc)
{
	float al;
	int i,len,sx,sy,ch,cl;
	float _x,_y,scale_x,scale_y,_sx,_sy;

	unsigned char* p = (unsigned char*)str;

	len = strlen((char*)str);
	_x = x;
	_y = y;

	scale_x = (float)XGR_MAXX / 640.0f * sc;
	scale_y = (float)XGR_MAXY / 480.0f * sc;
	sx = round(acsFont3D_SizeX * acsFont3D_ScaleX[fnt] * scale_x);
	sy = round(acsFont3D_SizeY * acsFont3D_ScaleY[fnt] * scale_y);

	cl = (col & 0xFF);
	al = (float)(256 - alpha) / 256.0f;

	for(i = 0; i < len; i ++){
		if(p[i] == '\n'){
			_x = x;
			_y += (float)sy * scale_y * sc;
		}
		else {
			if(p[i] != '\r'){
				ch = mch_toupper(p[i]);
				if(acsFont3D[ch]){
					_sx = (acsFont3D[ch] -> Real.MaxOriginal[0] - acsFont3D[ch] -> Real.MinOriginal[0]) * acsFont3D_ScaleX[fnt] * scale_x;
					_sy = (acsFont3D[ch] -> Real.MaxOriginal[1] - acsFont3D[ch] -> Real.MinOriginal[1]) * acsFont3D_ScaleY[fnt] * scale_y;
					if(acsChCenterFlags[ch])
						acsOutChar3D(_x + _sx/2.0f,_y + sy/2.0f,z,fnt,ch,acsColor3D[cl].x,acsColor3D[cl].y,acsColor3D[cl].z,al,0,Mat3f::ZERO,0,sc);
					else
						acsOutChar3D(_x + _sx/2.0f,_y + sy - _sy/2.0f,z,fnt,ch,acsColor3D[cl].x,acsColor3D[cl].y,acsColor3D[cl].z,al,0,Mat3f::ZERO,0,sc);
					_x += _sx + (float)space * scale_x;
				}
				else
					_x += (float)sx + (float)space * scale_x;
			}
		}
	}
}

void mchA_OutString3D(float x,float y,void* str,int fnt,int col,int space,int alpha,float z,float sc)
{
	float al,c_rgb;
	int i,len,sx,sy,ch,cl;
	float _x,_y,scale_x,scale_y,_sx,_sy;

	c_rgb = (col) ? 1.0f : 0.5f;

	unsigned char* p = (unsigned char*)str;

	len = strlen((char*)str);
	_x = x * mchA_d3dResX;
	_y = y * mchA_d3dResY;

	scale_x = (float)XGR_MAXX / 640.0f * sc;
	scale_y = (float)XGR_MAXY / 480.0f * sc;
	sx = round(acsFont3D_SizeX * acsFont3D_ScaleX[fnt] * scale_x);
	sy = round(acsFont3D_SizeY * acsFont3D_ScaleY[fnt] * scale_y);

	cl = (col & 0xFF);
	al = (float)(256 - alpha) / 256.0f;

	for(i = 0; i < len; i ++){
		if(p[i] == '\n'){
			_x = x;
			_y += (float)sy * scale_y * sc;
		}
		else {
			if(p[i] != '\r'){
				ch = mch_toupper(p[i]);
				if(acsFont3D[ch]){
					_sx = (acsFont3D[ch] -> Real.MaxOriginal[0] - acsFont3D[ch] -> Real.MinOriginal[0]) * acsFont3D_ScaleX[fnt] * scale_x;
					_sy = (acsFont3D[ch] -> Real.MaxOriginal[1] - acsFont3D[ch] -> Real.MinOriginal[1]) * acsFont3D_ScaleY[fnt] * scale_y;
					if(acsChCenterFlags[ch])
						mchA_OutChar3D(_x + _sx/2.0f,_y + sy/2.0f,z,fnt,ch,c_rgb,c_rgb,c_rgb,al,0,Mat3f::ZERO,0,sc);
					else
						mchA_OutChar3D(_x + _sx/2.0f,_y + sy - _sy/2.0f,z,fnt,ch,c_rgb,c_rgb,c_rgb,al,0,Mat3f::ZERO,0,sc);
					_x += _sx + (float)space * scale_x;
				}
				else
					_x += (float)sx + (float)space * scale_x;
			}
		}
	}
}

void mchA_StrLen3D(void* str,int fnt,int space,int& sx,int& sy)
{
	acsStrLen3D(str,fnt,space,sx,sy);

	float mchA_d3dResX = (float)XGR_MAXX / 640.0f;
	float mchA_d3dResY = (float)XGR_MAXY / 480.0f;

	sx = round(float(sx) / mchA_d3dResX);
	sy = round(float(sy) / mchA_d3dResY);
}

void acsOutMouse3D(void)
{
	int x,y;
	float f;

	if(!acsMouse3D || (mchResourcesFlag && acsScrD -> curScr -> ID != ACS_CREDITS_SCR) || acsScrD -> curScr -> ID == ACS_ESSENCE_SCR) return;

	XGR_MouseObj.Hide();

	x = XGR_MouseObj.PosX + XGR_MouseObj.SizeX/2;
	y = XGR_MouseObj.PosY + XGR_MouseObj.SizeY/2;

	Vect3f v1(acsMouseScale);
	gb_IVisGeneric -> SetObjectScale((cUnknownClass*)acsMouse3D,&v1);

	f = acsCalcForce(acsMouseZ - acsMouse3D -> rmaxTotal()/2.0f,acsMouseZ + acsMouse3D -> rmaxTotal()/2.0f,acsWaterZ);
	acsMouseVz += f * acs_dT;
	acsMouseVz *= 1.0f - acsDissipation;
	acsMouseZ += acsMouseVz * acs_dT;
	if(acsMouseZ > acsMouseMaxZ){ 
		acsMouseZ = acsMouseMaxZ;
		acsMouseVz = 0.0f;
	}
	acsMouseMaxZ = 100.0f;

	Vect3f target(320.0f,240.0f,-3000.0f);
	Vect3f pos(x,y,acsMouseZ);

	Vect3f r = getDist(target, pos);

	Mat3f M = Mat3f(M_PI/2 + r.psi(), Z_AXIS)*Mat3f(r.theta(), X_AXIS);
	Vect3f v2(x,y,acsMouseZ);
	Vect3f v3(0,0,0);
	gb_IVisGeneric -> SetObjectPosition((cUnknownClass*)acsMouse3D,&v2,&v3);
	acsMouse3D -> SetOrientation(M);

	BaseObject_SurfaceReflection_WaveWarp(acsMouse3D->x(),acsMouse3D->y(),acsMouse3D->z());

	acsDrawM3D(acsMouse3D,0);
}

void acsScreenMatrix::build(aciScreen* p)
{
	int i,len,x,y,sx,sy,_x,_y,_sx,_sy,ch,fnt,fl = 0;
	unsigned char* str;

	float scale_x = (float)XGR_MAXX / 640.0f;
	float scale_y = (float)XGR_MAXY / 480.0f;

	aciScreenInputField* ip;
	aciScreenObject* obj = (aciScreenObject*)p -> objList -> fPtr;

	clear();

	while(obj){
		if(obj -> type == ACS_INPUT_FIELD_OBJ && !(obj -> flags & ACS_HIDDEN)){
			ip = (aciScreenInputField*)obj;
			fl = (obj == acsLastObj) ? ACS_MTX_MOVE_DOWN : 0;
			acsStrLen3D(ip -> string,ip -> font,ip -> Space,sx,sy);

			if(ip -> flags & ACS_ALIGN_CENTER)
				x = ip -> PosX + (ip -> SizeX - sx)/2;
			else 
				x = ip -> PosX;

			y = ip -> PosY + (ip -> SizeY - sy)/2;
			_x = x;
			_y = y;

			fnt = ip -> font;

			len = strlen(ip -> string);
			sx = round(acsFont3D_SizeX * acsFont3D_ScaleX[ip -> font] * scale_x);
			sy = round(acsFont3D_SizeY * acsFont3D_ScaleY[ip -> font] * scale_y);

			str = (unsigned char*)ip -> string;

			for(i = 0; i < len; i ++){
				if(str[i] == '\n'){
					_x = x;
					_y += sy;
				}
				else {
					if(str[i] != '\r'){
						ch = mch_toupper(str[i]);
						if(acsFont3D[ch]){
							_sx = round((acsFont3D[ch] -> Real.MaxOriginal[0] - acsFont3D[ch] -> Real.MinOriginal[0]) * acsFont3D_ScaleX[fnt] * scale_x);
							_sy = round((acsFont3D[ch] -> Real.MaxOriginal[1] - acsFont3D[ch] -> Real.MinOriginal[1]) * acsFont3D_ScaleY[fnt] * scale_y);

							add(ch,_x + _sx/2,_y + sy - _sy/2,ip -> font,ip -> Color0,ip -> Alpha0,fl);

							_x += _sx + ip -> Space * scale_x;
						}
						else
							_x += (sx + ip -> Space) * scale_x;
					}
				}
			}
		}
		obj = (aciScreenObject*)obj -> next;
	}
}

void acsScreenMatrix::add(unsigned ch,int x,int y,int fnt,int cl,int al,int fl)
{
	if(matrixSize < ACS_SCREEN_MATRIX_SIZE){
		matrix[matrixSize].ch = ch;
		matrix[matrixSize].R0.x = x;
		matrix[matrixSize].R0.y = y;
		matrix[matrixSize].R0.z = 0;

		matrix[matrixSize].Scale0.x = acsFont3D_ScaleX[fnt];
		matrix[matrixSize].Scale0.y = acsFont3D_ScaleY[fnt];
		matrix[matrixSize].Scale0.z = acsFont3D_ScaleZ[fnt];

		matrix[matrixSize].color = (cl & 0xFF);
		matrix[matrixSize].alpha = 256 - al;

		matrix[matrixSize].flags = fl;

		matrixSize ++;
	}
}

void acsScreenMatrix::redraw(float phase)
{
	int i;
	Vect3f r,r1;
	Mat3f mat;

	for(i = 0; i < matrixSize; i ++){
		if(matrix[i].ch != 0xFF){
			r1 = (matrix[i].R0 - matrix[i].R1) % r1.K;
			mat.set(r1,M_PI * 2.0f * phase);

			r = matrix[i].R1 + (phase * (matrix[i].R0 - matrix[i].R1));
			acsFont3D_ScaleX[4] = matrix[i].Scale1.x + phase * (matrix[i].Scale0.x - matrix[i].Scale1.x);
			acsFont3D_ScaleY[4] = matrix[i].Scale1.y + phase * (matrix[i].Scale0.y - matrix[i].Scale1.y);
			acsFont3D_ScaleZ[4] = matrix[i].Scale1.z + phase * (matrix[i].Scale0.z - matrix[i].Scale1.z);

			acsOutChar3D(r.x,r.y,r.z,4,matrix[i].ch,acsColor3D[matrix[i].color].x,acsColor3D[matrix[i].color].y,acsColor3D[matrix[i].color].z,((float)matrix[i].alpha)/256.0f,1,mat);
		}
	}
}

int acsScreenMatrix::getEl(acsScreenMatrixElement* p)
{
	int i,idx = -1;
	float dist,dist0;
	Vect3f r,r0;

	for(i = 0; i < ACS_SCREEN_MATRIX_SIZE; i ++){
		if(!(matrix[i].flags & ACS_MTX_MOVE_DOWN) && matrix[i].ch == p -> ch){
			r = p -> R0 - matrix[i].R0;
			dist = r.norm2();

			if(idx == -1 || dist > dist0){
				idx = i;
				r0 = r;
				dist0 = dist;
			}
		}
	}

	return idx;
}

void acsPrepareChangeScreen3D(int scr_id)
{
	int i,idx;
	acsScreenMatrixElement* p0,*p1;

	aciScreen* p = acsScrD -> GetScreen(scr_id);
	if(!p){
		acsScr0.build(acsScrD -> curScr);
		acsScr1.clear();
		for(i = 0; i < ACS_SCREEN_MATRIX_SIZE; i ++){
			p0 = &acsScr0.matrix[i];
			if(p0 -> ch != 0xFF)
				p0 -> set_r(-1);
		}
		return;
	}

	acsScr0.build(acsScrD -> curScr);
	acsScr1.build(p);

	for(i = 0; i < ACS_SCREEN_MATRIX_SIZE; i ++){
		p0 = &acsScr1.matrix[i];
		if(p0 -> ch != 0xFF){
			idx = acsScr0.getEl(p0);
			if(idx != -1){
				p1 = &acsScr0.matrix[idx];
				p0 -> set_r(p1);
				p1 -> clear();
			}
			else
				p0 -> set_r(-1);
		}
	}
	for(i = 0; i < ACS_SCREEN_MATRIX_SIZE; i ++){
		p0 = &acsScr0.matrix[i];
		if(p0 -> ch != 0xFF)
			p0 -> set_r(1);
	}
}

void acsScreenMatrixElement::set_r(acsScreenMatrixElement* p)
{
	Scale1 = p -> Scale0;
	R1 = p -> R0;
}

void acsScreenMatrixElement::set_r(int mode)
{
	Vect3f r;
	float dist0 = acsDist0 + (float)XRnd(acsDistRnd);

	Scale1 = Scale0;

	r = R0 - acsR0;
	r.normalize(dist0);

	r += R0;

	if(mode == -1){	// r -> R0
		R1 = r;
	}
	else {		// R0 -> r
		if(flags & ACS_MTX_MOVE_DOWN){
			r = R0;
			r.z = acsR0_1.z;
		}

		R1 = R0;
		R0 = r;
	}
}

float acsChangeScreenQuant3D(void)
{
	float phase;

	if(!mchResourcesFlag)
		gb_IGraph3d->Fill(0,0,0);

	if(acsAlphaDelta > 0){	// fade
		phase = (float)acsAlpha / 512.0f;
	}
	else {
		phase = (float)(256 - acsAlpha) / 512.0f + 0.5f;
	}

	acsScr0.redraw(phase);
	acsScr1.redraw(phase);

	return phase;
}

void acsOutRect3D(float x,float y,float z,int sx,int sy,int col,int alpha,int mat_fl,int ch,Mat3f mat,float gamma)
{
	int cl;
	float al;

	Vect3f sc;

	cl = (col & 0xFF);
	al = (float)(256 - alpha) / 256.0f;

	acsGetCharSize3D(ch,sc.x,sc.y,sc.z);

	acsFont3D_ScaleX[4] = (float)sx / sc.x / mchA_d3dResX;
	acsFont3D_ScaleY[4] = (float)sy / sc.y / mchA_d3dResY;
	acsFont3D_ScaleZ[4] = 10.0f / sc.z / mchA_d3dResX;

	acsOutChar3D(x,y,z,4,ch,acsColor3D[cl].x * gamma,acsColor3D[cl].y * gamma,acsColor3D[cl].z * gamma,al,mat_fl,mat,0);
}

void acsDrawM3D(cMesh* p,int rot)
{
	int fl = acsShopFlag;

	rot = 0;
	acsShopFlag = 1;

	int CameraAttribute=gb_IVisGeneric->GetCameraAttribute(iCamera);
	gb_IVisGeneric->ClearCameraAttribute(iCamera,ATTRIBUTE_CAMERA_WORLD_SHARE);
	int RenderAttribute=gb_IVisGeneric->GetRenderObjectSwitch();
	gb_IVisGeneric->SetRenderObjectSwitch(RENDER_TUNING_OBJECT_REFLECTION_SURFACE|RENDER_TUNING_OBJECT_LIGHTING_CONST);

	cMatrix OldMatrix=p->GlobalMatrix,Rot;
	Rot.NewMatrix();

	if(rot){
		Rot.Rotate(30*sinf(GlobalPeriodRotate1),30*cosf(GlobalPeriodRotate1),5*sinf(GlobalPeriodRotate1));
		OldMatrix=OldMatrix*Rot;
	}
	cUnkClassDynArrayPointer ArrayCamera(KIND_ARRAYCAMERA);
	ArrayCamera(0)=iCamera;
	p->TestVisible(&ArrayCamera);
	gb_IGraph3d->BeginScene();
	if(!mchResourcesFlag)
	{
		p -> DrawReflection(gb_UScene,&ArrayCamera);
		if(rot)
			p->GlobalMatrix=OldMatrix;
	}
	p->Draw(gb_UScene,&ArrayCamera);

	ArrayCamera(0)=0;
	ArrayCamera.Delete();

	acsShopFlag = fl;

	gb_IVisGeneric->ClearRenderObjectSwitch();
	gb_IVisGeneric->SetRenderObjectSwitch(RenderAttribute);
}

void mchA_DrawM3D(cMesh* p)
{
	int attr0,attr1;

	int RenderAttribute=gb_IVisGeneric->GetRenderObjectSwitch();
	gb_IVisGeneric->SetRenderObjectSwitch(RENDER_TUNING_OBJECT_REFLECTION_SURFACE|RENDER_TUNING_OBJECT_LIGHTING_CONST);

	attr0 = p -> GetAttribute(MESH_NOT_SHADE);
	attr1 = p -> GetAttribute(MESH_LIGHTING_MAX);

	p -> SetAttribute(MESH_NOT_SHADE | MESH_LIGHTING_MAX);

	cUnkClassDynArrayPointer ArrayCamera(KIND_ARRAYCAMERA);
	ArrayCamera(0)=iCamera;
	p->TestVisible(&ArrayCamera);
	p->Draw(gb_UScene,&ArrayCamera);
	ArrayCamera(0)=0;
	ArrayCamera.Delete();

	gb_IVisGeneric->ClearRenderObjectSwitch();
	gb_IVisGeneric->SetRenderObjectSwitch(RenderAttribute);

	if(!attr0)
		p -> ClearAttribute(MESH_NOT_SHADE);
	if(!attr1)
		p -> ClearAttribute(MESH_LIGHTING_MAX);
}

Vect3f acsCalcR(Vect3f r0,float phase)
{
	Vect3f r = r0 - acsR0;
	r.normalize(acsDist0 * phase);
	r += r0;

	return r;
}

acsTextureMosaic::acsTextureMosaic(int x,int y)
{
	int i,j;
	IndexX = x;
	IndexY = y;
	elSize = 7;

	mosaic = new acsTextureMosaicElement*[x * y];

	for(i = 0; i < y; i ++){
		for(j = 0; j < x; j ++){
			mosaic[j + i * x] = new acsTextureMosaicElement(j,i,(1 << elSize));
		}
	}
}

acsTextureMosaic::~acsTextureMosaic(void)
{
	int i,j;

	for(i = 0; i < IndexY; i ++){
		for(j = 0; j < IndexX; j ++){
			delete mosaic[j + i * IndexX];
		}
	}
	delete mosaic;
}

acsTextureMosaicElement::acsTextureMosaicElement(int x,int y,int sz)
{
	X = x;
	Y = y;

	Size = sz;

	flags = 0;
	data = new unsigned short[sz * sz];

	clear();
}

acsTextureMosaicElement::~acsTextureMosaicElement(void)
{
	delete data;
}

void acsTextureMosaic::PutSpr(int x,int y,int sx,int sy,void* p,void* pal,int col,int alpha)
{
	int i,j,_x,_y,idx_x,idx_y,dx,dy,al;
	unsigned cl,cl0,cl1,r,g,b;
	alpha = 256 - alpha;

	unsigned short* dest_ptr;
	unsigned char* ptr = (unsigned char*)p;
	unsigned* pal_ptr = (unsigned*)pal;

	_x = x;
	_y = y;

	if(alpha){
		for(j = 0; j < sy; j ++){
			for(i = 0; i < sx; i ++){
				idx_x = _x >> elSize;
				idx_y = _y >> elSize;
				dx = _x - (idx_x * (1 << elSize));
				dy = _y - (idx_y * (1 << elSize));

				if(idx_x >= 0 && idx_y >= 0 && idx_x < IndexX && idx_y < IndexY){
					dest_ptr = mosaic[idx_x + idx_y * IndexX] -> data + dx + (dy << elSize);
					cl = ptr[i + j * sx];
					if(cl){
						mosaic[idx_x + idx_y * IndexX] -> flags |= ATM_UPDATE;

						cl0 = *dest_ptr;
						cl1 = pal_ptr[col + cl];

						al = ((cl << 3) * alpha) >> 8;

						r = (((cl0 & XGR_MASK_R0) * (256 - al) + (cl1 & XGR_MASK_R0) * al)) & (XGR_MASK_R0 << 8);
						g = (((cl0 & XGR_MASK_G0) * (256 - al) + (cl1 & XGR_MASK_G0) * al)) & (XGR_MASK_G0 << 8);
						b = (((cl0 & XGR_MASK_B0) * (256 - al) + (cl1 & XGR_MASK_B0) * al)) & (XGR_MASK_B0 << 8);
						*dest_ptr = (r + g + b) >> 8;
					}
				}
				_x ++;
			}
			_y ++;
			_x = x;
		}
	}
	else {
		for(j = 0; j < sy; j ++){
			for(i = 0; i < sx; i ++){
				idx_x = _x >> elSize;
				idx_y = _y >> elSize;
				dx = _x - (idx_x * (1 << elSize));
				dy = _y - (idx_y * (1 << elSize));

				if(idx_x >= 0 && idx_y >= 0 && idx_x < IndexX && idx_y < IndexY){
					dest_ptr = mosaic[idx_x + idx_y * IndexX] -> data + dx + (dy << elSize);
					cl = ptr[i + j * sx];
					if(cl){
						mosaic[idx_x + idx_y * IndexX] -> flags |= ATM_UPDATE;
						*dest_ptr = pal_ptr[col + cl];
					}
				}
				_x ++;
			}
			_y ++;
			_x = x;
		}
	}
}

void acsTextureMosaicElement::dump(void)
{
	int x,y;
	unsigned c,r,g,b;

	XBuffer XBuf;
	XStream fh;
	unsigned char* p = new unsigned char[Size * Size];

	for(y = 0; y < Size; y ++){
		for(x = 0; x < Size; x ++){
			c = data[x + y * Size];
			r = (((c) >> XGR_SHIFT_R0) & XGR_COLOR_MASK_R0) << 4;
			g = (((c) >> XGR_SHIFT_G0) & XGR_COLOR_MASK_G0) << 3;
			b = (((c) >> XGR_SHIFT_B0) & XGR_COLOR_MASK_B0) << 4;
			p[x + y * Size] = (r + g + b) / 3;
		}
	}

	XBuf < "ISCREEN\\SCRIPTS\\TEMP\\mosaic" <= X <= Y < ".bmp";
	fh.open(XBuf.address(),XS_OUT);
	fh < (short)Size < (short)Size;
	fh.write(p,Size * Size);
	fh.close();
	delete p;
}

void acsTextureMosaic::dump(void)
{
	int i;
	for(i = 0; i < IndexX * IndexY; i ++){
		mosaic[i] -> dump();
	}
}

int acsTextureMosaic::PutStr(int x,int y,int fnt,int space,void* str,void* pal,int col,int alpha,int align)
{
	int _x,_y,i,sx,ssx,sy,ss,sz = strlen((char*)str),str_num = 0,col1;
	HFont* p = acsFntTable[fnt];
	HChar* ch;
	unsigned char* ptr,*str_ptr = (unsigned char*)str;

	ssx = acsTextStrLenMax(fnt,str_ptr,space);

	if(align) x = (512 - ssx) / 2;

	_x = x;
	_y = y;

	if(align) _x += (ssx - acsTextStrLen(fnt,str_ptr,space))/2;

	for(i = 0; i < sz; i ++){
		ch = p -> data[str_ptr[i]];
		ptr = ch -> HeightMap;

		sx = ch -> SizeX;
		sy = ch -> SizeY;
		ss = sx * sy;

		if(str_ptr[i] == '\n'){
			_x = x;
			_y += sy + space;
			str_num ++;
			if(str_num > acsTextPhase/10) return x;
			if(align) _x += (ssx - acsTextStrLen(fnt,str_ptr + i + 1,space))/2;
		}
		else {
			col1 = (str_ptr[i] == 4) ? (3 << 5) : col;

			PutSpr(_x - ch -> LeftOffs,_y,sx,sy,ptr,pal,col1,alpha);
			_x += sx - ch -> RightOffs - ch -> LeftOffs + space;
		}
	}

	return x;
}

void acsGetTexture(int x,int y,unsigned char* p)
{
	if(acsTM)
		memcpy(p,acsTM -> mosaic[x + y * acsTM -> IndexX] -> data,(1 << (acsTM -> elSize * 2 + 1)));
}

void acsTextureMosaicElement::clear(void)
{
	int i,r,g,b;
	if(GlobalMainMenuSurfaceReflection==0) return;
	cMaterial* Material = &GlobalMainMenuSurfaceReflection->MaterialArray[X+Y*GlobalMainMenuSurfaceReflection->xMaterial];

	unsigned char* buf_texture = Material -> Texture -> texture;
	unsigned char* buf_rgb = Material -> Texture -> GetPal(),*rgb;
	unsigned short* buf;

	buf = data;
	int texfmt,rCount,gCount,bCount,aCount,rShift,gShift,bShift,aShift;
	sTextureFormatData TexFmtData;
	TexFmtData.TextureFormat=TEXFMT_RGB565;
	gb_IGraph3d->GetTextureFormatData(TexFmtData);
	TexFmtData.Get(texfmt,rCount,gCount,bCount,aCount,rShift,gShift,bShift,aShift);
	for(int j=0; j< (1 << Material -> Texture->_y); j++, buf += Size){
		for(i = 0; i < (1 << Material -> Texture -> _x); i ++,buf_texture ++){
			rgb = &buf_rgb[3 * buf_texture[0]];
			r = rgb[0]; g = rgb[1]; b = rgb[2];
			buf[i]=	((255>>(8-aCount))<<aShift)|
					((r>>(PALETTE_MAX_SIZE-rCount))<<rShift)|
					((g>>(PALETTE_MAX_SIZE-gCount))<<gShift)|
					((b>>(PALETTE_MAX_SIZE-bCount))<<bShift);
		}
	}
}

void acsEssenceQuant(void)
{
	int i;

	if(!acsChosenBodyFlag)
		acsChosenBody = NULL;

	for(i = 0; i < acsNumSaves; i ++)
		acsBody[i] -> Quant();

	if(acsChosenBody && !acsChosenBodyFlag)
		acsTM -> PutStr(20,400,1,1,acsGetSaveString(acsChosenBody -> number),mchA_MenuPal2,ACS_WATER_COL,ACS_WATER_ALPHA);
}

void acsRedrawEssence(void)
{
	int i;
	for(i = 0; i < acsNumSaves; i ++)
		acsBody[i] -> Redraw();
}

float acsCalcForce(float z_min,float z_max,float z)
{
	float f;

	if(z < z_min) z = z_min;
	if(z > z_max) z = z_max;

	f = -acsG + acsA * (z - z_min) / (z_max - z_min);

	return f;
}

acsEssenceData::acsEssenceData(int id,int num)
{
	ID = id;
	number = num;
	bound = NULL;
	flags = 0;
	name = NULL;

	Init();
}

void acsEssenceData::Init(void)
{
	float x,y;
	float y0 = 350.0f * mchA_d3dResY;

	XBuffer XBuf(20);
	XBuf < "essence" <= ID;

	flags = 0;

	if(name) free(name);
	psi = 0.0f;

	name = strdup(getIniKey(mchIscreenINI,"essences",XBuf.address()));
	flags |= (ACS_ESS_STOPPED | ACS_ESS_ROTATE);

	const float ess_sz = 100.0f;

	if(number <= acsMaxEssenceID/2){
		x = number * ess_sz + (640.0f - acsMaxEssenceID/2 * ess_sz) / 2;
		y = (480.0f - 2 * ess_sz) / 2;
	}
	else {
		x = (number - acsMaxEssenceID/2) * ess_sz + (640.0f - (acsMaxEssenceID - acsMaxEssenceID/2) * ess_sz) / 2;
		y = (480.0f - 2 * ess_sz) / 2 + ess_sz;
	}

	r = Vect3f(x,y,20.0f);

	if(bound)
		bound -> flags &= ~ACS_BLOCKED;
}

void acsEssenceData::GetBound(int scr_id,int obj_id)
{
	aciScreen* scr;

	scr = acsScrD -> GetScreen(scr_id);
	if(scr){
		bound = scr -> GetObject(obj_id);
		if(bound)
			bound -> flags &= ~ACS_BLOCKED;
	}
}

void acsEssenceData::Quant(void)
{
	int i;
	float d_psi,dx,dy,d_vx,d_vy;

	float dr = 70.0f;
	float dist0 = 90.0f;
	float y0 = 350.0f * mchA_d3dResY;

	if(flags & ACS_ESS_ROTATE){
		psi += 0.05f * acs_dT;
		if(psi < 0.0f) psi += 2.0f * M_PI;
		if(psi > 2.0f * M_PI) psi -= 2.0f * M_PI;
	}

	if(!(flags & ACS_ESS_STOPPED)){
		r += acs_dT * v;

		d_psi = v.psi() - psi - M_PI/2.0f;
		if(d_psi > M_PI) d_psi = -(2.0f * M_PI - d_psi);
		if(d_psi < -M_PI) d_psi = -(-2.0f * M_PI - d_psi);

		psi += 0.1 * d_psi;

		if(r.y > y0 && v.y > 0) v.y = -v.y;

		if(!(flags & ACS_ESS_FREE)){
			if(r.x < dr && v.x < 0) v.x = -v.x;
			if(r.x > XGR_MAXX - dr && v.x > 0) v.x = -v.x;

			if(r.y < dr && v.y < 0) v.y = -v.y;
		}
		else {
			if(r.x < -400 || r.x > XGR_MAXX + 400 || r.y < -400 || r.y > XGR_MAXY + 400)
				Stop();
		}

		for(i = 0; i < acsNumSaves; i ++){
			if(acsBody[i] != this){
				dx = acsBody[i] -> r.x - r.x;
				dy = acsBody[i] -> r.y - r.y;

				d_vx = acsBody[i] -> v.x - v.x;
				d_vy = acsBody[i] -> v.y - v.y;

				if(fabs(dx) < dist0 && fabs(dy) < dist0){
					if((dx < 0 && d_vx > 0) || (dx > 0 && d_vx < 0)){
						if(!(acsBody[i] -> flags & ACS_ESS_STOPPED)){
							d_vx = v.x;
							v.x = acsBody[i] -> v.x;
							acsBody[i] -> v.x = d_vx;
						}
						else 
							v.x = -v.x;
					}
					if((dy < 0 && d_vy > 0) || (dy > 0 && d_vy < 0)){
						if(!(acsBody[i] -> flags & ACS_ESS_STOPPED)){
							d_vy = v.y;
							v.y = acsBody[i] -> v.y;
							acsBody[i] -> v.y = d_vy;
						}
						else 
							v.y = -v.y;
					}
				}
			}
		}
		ScreenCollision();
	}

	if(bound){
		bound -> PosX = r.x - bound -> SizeX/2;
		bound -> PosY = r.y - bound -> SizeY/2;

		if(!(flags & ACS_ESS_FREE)){
			if(bound -> CheckXY(XGR_MouseObj.PosX,XGR_MouseObj.PosY)){
				flags |= ACS_ESS_STOPPED | ACS_ESS_ROTATE;
				acsChosenBody = this;
			}
			else {
				if(flags & ACS_ESS_STOPPED) Start();
			}
		}
	}
}

void acsEssenceData::Stop(void)
{
	flags |= ACS_ESS_STOPPED;
	v = v.ZERO;
}

void acsEssenceData::Start(void)
{
	float _psi;

	if(!(flags & ACS_ESS_STOPPED))
		_psi = (float)XRnd(360) / 180.0f * M_PI;
	else
		_psi = psi;

	flags &= ~(ACS_ESS_STOPPED | ACS_ESS_ROTATE);

	v = Vect3f(cos(_psi),sin(_psi),0);
	v.normalize(5.0f + (float)XRnd(4));
}

void acsEssenceData::Redraw(void)
{
	Mat3f rot,rot1;
	cMesh* p;

	p = MeshLibrary -> Find(M3D_TOTAL_TYPE(acsEssence3DS[ID],M3D_ENGINE));
	if(!p) return;

	rot.set(r.I,90,0);
	rot1.set(r.K,psi,0);

	p -> SetPosition(rot1 * rot,r);

	p -> SetScale(mchA_d3dResX * 2.5f);
	acsDrawM3D(p,0);
	p -> SetScale(1.0f);

	if(!(flags & ACS_ESS_STOPPED))
		BaseObject_SurfaceReflection_WaveWarp(r.x,r.y,r.z);
}

void acsInitEssenceScreen(void)
{
	int i;

	aciScreenInputField* p;
	aciScreen* scr = acsScrD -> GetScreen(ACS_ESSENCE_SCR);

	acsChosenBodyFlag = 0;
	acsChosenBody = NULL;

	p = (aciScreenInputField*)scr -> GetObject(ACS_ESSENCE_CHOOSE);
	if(p) p -> flags |= ACS_HIDDEN | ACS_BLOCKED;

	p = (aciScreenInputField*)scr -> GetObject(ACS_NEW_GAME_OPTION);
	if(p) p -> flags |= ACS_HIDDEN | ACS_BLOCKED;

	p = (aciScreenInputField*)scr -> GetObject(ACS_PLAYER_NAME_TXT);
	if(p) p -> flags |= ACS_HIDDEN | ACS_BLOCKED;

	p = (aciScreenInputField*)scr -> GetObject(ACS_PLAYER_NAME_PRM);
	if(p) p -> flags |= ACS_HIDDEN | ACS_BLOCKED;

	for(i = 0; i < acsNumSaves; i ++)
		acsBody[i] -> Init();
}

void acsEssenceEvent(int id)
{
	int i;
	aciScreenInputField* p;
	aciScreen* scr = acsScrD -> GetScreen(ACS_ESSENCE_SCR);

	if(acsChosenBodyFlag) return;

	acsChosenBodyFlag = 1;
	acsChosenBody = acsBody[id];

	p = (aciScreenInputField*)scr -> GetObject(ACS_ESSENCE_CHOOSE);
	if(p) p -> flags |= ACS_HIDDEN | ACS_BLOCKED;

	p = (aciScreenInputField*)scr -> GetObject(ACS_NEW_GAME_OPTION);
	if(p) p -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);

	p = (aciScreenInputField*)scr -> GetObject(ACS_PLAYER_NAME_TXT);
	if(p) p -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);

	p = (aciScreenInputField*)scr -> GetObject(ACS_PLAYER_NAME_PRM);
	if(p){ 
		p -> flags &= ~(ACS_HIDDEN | ACS_BLOCKED);
		strcpy(p -> string,acsChosenBody -> name);
		acsUpdateMM_Option(ACS_ESSENCE_SCR,ACS_PLAYER_NAME_PRM);
	}

	for(i = 0; i < acsNumSaves; i ++){
		if(i != id){
			acsBody[i] -> flags |= ACS_ESS_FREE;
			acsBody[i] -> bound -> flags |= ACS_BLOCKED;
			acsBody[i] -> v.normalize(15.0f);
		}
	}

	acsCurrentSave = acsGetSaveSlotIndex(id);
	acsInitSaveOptions();
}

void acsEssenceData::ScreenCollision(void)
{
	int x,y,dist_x0,dist_y0;
	float dx,dy,d_vx,d_vy;

	aciScreenObject* p = (aciScreenObject*)acsScrD -> curScr -> objList -> fPtr;
	while(p){
		if(!(p -> flags & ACS_BLOCKED) && (p -> ID < ACS_ESSENCE_OBJ || p -> ID > (ACS_ESSENCE_OBJ + 7))){
			dist_x0 = (bound -> SizeX + p -> SizeX)/2;
			dist_y0 = (bound -> SizeY + p -> SizeY)/2;

			x = p -> PosX + p -> SizeX/2;
			y = p -> PosY + p -> SizeY/2;

			dx = x - r.x;
			dy = y - r.y;

			d_vx = -v.x;
			d_vy = -v.y;

			if(fabs(dx) < dist_x0 && fabs(dy) < dist_y0){
				if((dx < 0 && d_vx > 0) || (dx > 0 && d_vx < 0))
					v.x = -v.x;
				if((dy < 0 && d_vy > 0) || (dy > 0 && d_vy < 0))
					v.y = -v.y;
			}
		}
		p = (aciScreenObject*)p -> next;
	}
}

void acsTextureMosaic::Quant(void)
{
	int i,sz = IndexX * IndexY;
	if(GlobalMainMenuSurfaceReflection==0) return;
	for(i = 0; i < sz; i ++){
		if(mosaic[i] -> flags & ATM_UPDATE)
			GlobalMainMenuSurfaceReflection -> MaterialArray[i].Texture -> SetStatus(STATUS_TEXTURE_RESET);
	}
}

void acsTextureMosaic::clearQuant(void)
{
	int i,sz = IndexX * IndexY;

	if(GlobalMainMenuSurfaceReflection==0) return;
	for(i = 0; i < sz; i ++){
		if(mosaic[i] -> flags & ATM_UPDATE){
			GlobalMainMenuSurfaceReflection->MaterialArray[i].Texture -> SetStatus(STATUS_TEXTURE_RESET);
			mosaic[i] -> clear();
			mosaic[i] -> flags ^= ATM_UPDATE;
		}
	}
}

void acsShowCredits(void)
{
	int i,sz,xx,yy,idx;
	static int y = 480;
	static char str[256];
	aciScreenInputField* p = (aciScreenInputField*)acsScrD -> GetObject(ACS_CREDITS_TEXT);

	acsTextPhase = 1000;

	if(!p) return;

	yy = y;

	idx = 0;
	sz = strlen(p -> string);
	for(i = 0; i < sz; i ++){
		if(p -> string[i] == '\n'){
			memcpy(str,p -> string + idx,i - idx);
			str[i - idx] = 0;

			xx = (640 - acsStrLen(1,(unsigned char*)str,1)) / 2;
			mchA_d3dOutString(xx,yy,mchA_FontScaleX[1],mchA_FontScaleY[1],str,mchA_ColorF[2],170,1,1);
			yy += 30;
			idx = i + 1;
		}
	}

	y --;
	if(yy < -30) y = 480;

/*
	static int y = 500;
	aciScreenInputField* p = (aciScreenInputField*)acsScrD -> GetObject(ACS_CREDITS_TEXT);

	acsTextPhase = 1000;

	if(p)
		acsTM -> PutStr(0,round((float)p -> PosY / mchA_d3dResY),2,0,p -> string,mchA_MenuPal2,ACS_CREDITS_COL,ACS_WATER_ALPHA,1);

	if(p){
		acsTM -> PutStr(0,y,1,0,p -> string,mchA_MenuPal2,ACS_CREDITS_COL,32,1);
	}

	if(--y < -515) y = 515;
*/
}

acsTimer::acsTimer(float st,float pr,float d)
{
	ValueCur = ValueStart = st;
	dValue = dValueCur = d;
	period = pr;
}

void acsTimer::Quant(float dt)
{
	ValueCur += dValueCur * dt;

	if(ValueCur < ValueStart){
		ValueCur = ValueStart;
		dValueCur = dValue;
	}
	if(ValueCur > ValueStart + period){
		ValueCur = ValueStart + period;
		dValueCur = -dValue;
	}
}

float acsTimer::Value(void)
{
	return (ValueCur - ValueStart) / period;
}

float acsTimer::Phase(void)
{
	float phase = (ValueCur - ValueStart) / period / 2.0f;
	if(dValueCur < 0.0f)
		phase = 0.5f + (0.5f - phase);

	phase *= 2.0f * M_PI;

	return phase;
}

void acsShowSlotInfo(void)
{
	int id;
	if(acsMouseObj && acsMouseObj -> ID <= 6){
		id = acsMouseObj -> ID + acsFirstSlot - 1;
		acsTM -> PutStr(200,400,2,1,acsGetSaveString(id),mchA_MenuPal2,ACS_WATER_COL,ACS_WATER_ALPHA);
	}
}

void acsExtSetCamera(int zbuf_flag)
{
	float z = (zbuf_flag) ? 0.99f : 0.2f; 

	acsCameraR.x = XGR_MAXX/2;
	acsCameraR.y = XGR_MAXY/2;

	Vect3f v1(acsCameraR.x,acsCameraR.y,acsCameraR.z);
	Vect3f v2(acsCameraA.x,acsCameraA.y,acsCameraA.z);
	gb_IVisGeneric->SetCameraPosition(iCamera,&v1,&v2);
	Vect2f v3(0.5f,0.5f);
	sRectangle4f r1(-0.499f,-0.499f,0.499f,0.499f);
	Vect2f v4((float)acsCameraFocus/XGR_MAXX,(float)acsCameraFocus/XGR_MAXX);
	Vect2f v5(10.0f,3000.0f);
	Vect2f v6(0.0f,z);
	gb_IVisGeneric->SetCameraFrustum(iCamera,	// \F3\F1\F2\E0\ED\E0\E2\EB\E8\E2\E0\E5\F2\F1\FF \EF\E8\F0\E0\EC\E8\E4\E0 \E2\E8\E4\E8\EC\EE\F1\F2\E8
		&v3,						// \F6\E5\ED\F2\F0 \EA\E0\EC\E5\F0\FB
		&r1,	// \E2\E8\E4\E8\EC\E0\FF \EE\E1\EB\E0\F1\F2\FC \EA\E0\EC\E5\F0\FB
		&v4,// \F4\EE\EA\F3\F1 \EA\E0\EC\E5\F0\FB
		&v5,					// \E1\EB\E8\E6\E0\E9\F8\E8\E9 \E8 \E4\E0\EB\FC\ED\E8\E9 z-\EF\EB\EE\F1\EA\EE\F1\F2\E8 \EE\F2\F1\E5\F7\E5\ED\E8\FF
		&v6);						// zNear \E8 zFar \E4\EB\FF \EC\E0\EF\E8\F0\EE\E2\E0\ED\E8\FF \E2 zBuffer
	if(acsCameraPerspective) gb_IVisGeneric->SetCameraAttribute(iCamera,ATTRIBUTE_CAMERA_PERSPECTIVE);
	else gb_IVisGeneric->ClearCameraAttribute(iCamera,ATTRIBUTE_CAMERA_PERSPECTIVE);
}

void acsDrawHS_CfgScreen(void)
{
	float ph,phase;
	int i,j,id,x,sx,y,arc_id,al;
	static XBuffer cfg_str;

	const float sc = 0.7f;

	mchTrackInfo* track;
	aciScreenInputField* p;
	aciScreen* scr = acsScrD -> GetScreen(ACS_SPLIT_HS_SCR);
	if(!scr) return;

	if(acsAlphaDelta > 0){	// fade
		phase = 1.0f - (float)acsAlpha / 256.0f;
	}
	else {
		phase = (float)(256 - acsAlpha) / 256.0f;
	}

	p = (aciScreenInputField*)scr -> GetObject(1);
	if(p){
		track = mch_trackD -> GetTrack(hsWorld,hsTrack);
		if(track){
//			al = round(float(p -> AlphaCur + 127) * phase);
			al = round(200.0f * phase);

			x = round(float(p -> PosX)/mchA_d3dResX);
			y = round(float(p -> PosY)/mchA_d3dResY);

			cfg_str.init();
			cfg_str < iGetText(iTXT_WORLD) < track -> Name();

			sx = acsStrLen(1,(unsigned char*)cfg_str.address(),1);

			x += (round(float(p -> SizeX) / mchA_d3dResX) - sx) / 2;

			mchA_d3dOutString(x,y,mchA_FontScaleX[1],mchA_FontScaleY[1],cfg_str.address(),mchA_ColorF[2],al,1,1);
		}
	}
	p = (aciScreenInputField*)scr -> GetObject(2);
	if(p){
		al = round(float(p -> AlphaCur + 127) * phase);

		x = round(float(p -> PosX + p -> SizeX/2)/mchA_d3dResX);
		y = round(float(p -> PosY + p -> SizeY/2)/mchA_d3dResY);
		mchA_d3dOutSprite(x,y,1.0f,1.0f,AE_D3DSPR_TRIANGLE,mchA_ColorF[2],al,M_PI,1.0f);
	}

	p = (aciScreenInputField*)scr -> GetObject(3);
	if(p){
		al = round(float(p -> AlphaCur + 127) * phase);

		x = round(float(p -> PosX + p -> SizeX/2)/mchA_d3dResX);
		y = round(float(p -> PosY + p -> SizeY/2)/mchA_d3dResY);
		mchA_d3dOutSprite(x,y,1.0f,1.0f,AE_D3DSPR_TRIANGLE,mchA_ColorF[2],al,0.0f,1.0f);
	}

	for(i = 0; i < HS_PLAYERS_MAX; i ++){
		p = (aciScreenInputField*)scr -> GetObject(ACS_PLAYER0_MECHOS + i);
		if(p){
			x = round(float(p -> PosX)/mchA_d3dResX);
			y = round(float(p -> PosY)/mchA_d3dResY) - 10;

			al = p -> AlphaCur;
			ph = float(al) / 128.0f;

			al += 127;

			al = round(float(al) * phase);

			cfg_str.init();
			cfg_str.write(hsPlayers[i].configStr,strlen(hsPlayers[i].configStr) + 1);
			cfg_str.set(0,XB_BEG);
			for(j = 0; j < NUM_ACTIVE_PARTS; j ++){
				cfg_str >= id;
				if(!j){
					mchA_SprD -> DrawSprite(x,y,sc,sc,1,mchA_ColorF[2],al,0.0f,0);
					mchA_SprD -> DrawSprite(x,y,sc,sc,99 + id,mchA_ColorF[7],al,0.0f,0);
					x += 10;
				}
				else {
					arc_id = mchGetPartArcane(j,id);
					mchA_SprD -> DrawArcaneSymbol(x,y + 5,arc_id,ph,0.0f,2,al,sc);
				}

				x += 35;
			}
		}
	}
}

void acsInitShop(int pl)
{
	int x,y;
	if(acsShopFlag) return;

	aciScreenInputField* p;
	aciScreen* scr = acsScrD -> GetScreen(ACS_CONSTRUCT_SCR);

	strcpy(acsShopCfgBackup,hsPlayers[acsShopPlayer].configStr);

	p = (aciScreenInputField*)scr -> GetObject(1);
	x = p -> PosX + p -> SizeX/2;
	y = p -> PosY + p -> SizeY/2;

	acsShopPlayer = pl;
	acsMechos = new MechosForSale(hsPlayers[acsShopPlayer].configStr,Vect3f(x,y,80));

	Vect3f v1(SHOP_MECH_SCALE * mchA_d3dResX);
	gb_IVisGeneric -> SetObjectScale((cUnknownClass*)acsMechos -> getGeometry(),&v1);

	acsMechos -> rotate(1,0);
	acsMechos -> rotate(-1,0);

//	acsMechos -> rotate(-200,200);

	acsShopFlag = 1;
}

void acsFinitShop(int mode)
{
	char* str;
	XBuffer XBuf,cfg_str;

	if(!acsShopFlag) return;

	if(!mode){
		str = acsMechos -> getConfig();
		strcpy(hsPlayers[acsShopPlayer].configStr,str);
		delete str;

		XBuf < "player" <= acsShopPlayer + 1;
		cfg_str < "\"" < hsPlayers[acsShopPlayer].configStr < "\"";
		putIniKey(mch_hotseatINI,XBuf.address(),"mechos_config",cfg_str);
	}
	else
		strcpy(hsPlayers[acsShopPlayer].configStr,acsShopCfgBackup);

	acsShopFlag = 0;
	delete acsMechos;
}

void acsRedrawShop(void)
{
	if(!acsShopFlag) return;

	acsDrawM3D(acsMechos -> getGeometry());
}

void acsRedrawShopArcanes(void)
{
	float ph,phase;
	int i,id,x,y,arc_id,al;
	static XBuffer cfg_str;

	char* str;
	char* name = NULL;

	const float sc = 1.0f;

	if(!acsShopFlag) return;

	str = acsMechos -> getConfig();

	aciScreenInputField* p;
	aciScreen* scr = acsScrD -> GetScreen(ACS_CONSTRUCT_SCR);

	if(!scr) return;

	if(acsAlphaDelta > 0){	// fade
		phase = 1.0f - (float)acsAlpha / 256.0f;
	}
	else {
		phase = (float)(256 - acsAlpha) / 256.0f;
	}

	cfg_str.init();
	cfg_str.write(str,strlen(str));
	cfg_str.set(0,XB_BEG);

	delete str;

	for(i = 0; i < NUM_ACTIVE_PARTS; i ++){
		cfg_str >= id;
		p = (aciScreenInputField*)scr -> GetObject(2 + i);

		if(i && p == acsMouseObj) 
			name = mchGetPartArcaneName(i,id);

		if(p){
			x = round(float(p -> PosX)/mchA_d3dResX);
			y = round(float(p -> PosY)/mchA_d3dResY) - 10;

			al = p -> AlphaCur;
			ph = float(al) / 128.0f;

			al += 127;

			al = round(float(al) * phase);

			if(!i){
				mchA_SprD -> DrawSprite(x,y,sc,sc,1,mchA_ColorF[2],al,0.0f,0);
				mchA_SprD -> DrawSprite(x,y,sc,sc,99 + id,mchA_ColorF[7],al,0.0f,0);
			}
			else {
				arc_id = mchGetPartArcane(i,id);
				mchA_SprD -> DrawArcaneSymbol(x,y + 5,arc_id,ph,0.0f,2,al,sc);
			}
		}
	}
	if(!name) return;

	p = (aciScreenInputField*)scr -> GetObject(10);
	if(p){
		x = round(float(p -> PosX)/mchA_d3dResX);
		y = round(float(p -> PosY)/mchA_d3dResY);

		al = round(255.0f * phase);

		mchA_d3dOutString(x,y,mchA_FontScaleX[1],mchA_FontScaleY[1],name,mchA_ColorF[2],al,1,1);
	}
}

void acsShopChangePart(int tp,int dir)
{
	int i,id,id1;

	char* str = acsMechos -> getConfig();
	static XBuffer cfg_str;

	cfg_str.init();
	cfg_str.write(str,strlen(str));
	cfg_str.set(0,XB_BEG);

	for(i = 0; i <= tp; i ++)
		cfg_str >= id;

	if(tp){
		if(dir)
			id1 = mchNextPart(tp,id);
		else
			id1 = mchPrevPart(tp,id);
	}
	else {
/*
		if(dir)
			id1 = id + 1;
		else
			id1 = id - 1;

		if(id1 > 5) id1 = 1;
		if(id1 < 1) id1 = 5;
*/
		if(dir){
			if((id1 = id + 1) > 10) id1 = 1;
			while(!mch_trackD -> FigureEnabledHS(id1 - 1)){
				if(++id1 > 10) id1 = 1;
			}
		}
		else {
			if((id1 = id - 1) < 1) id1 = 10;
			while(!mch_trackD -> FigureEnabledHS(id1 - 1)){
				if(--id1 < 1) id1 = 10;
			}
		}
	}

	if(id1 != -1)
		acsMechos -> changePart(tp,id1);

	acsMechos -> rotate(1,0);
	acsMechos -> rotate(-1,0);
}

void acsShopMoveMouse(int dx,int dy)
{
	acsMechos -> rotate(dx * 2,dy * 2);
}

void acsShowVersionInfo(void)
{
	acsTextPhase = 1000;
	acsTM -> PutStr(20,20,2,0,iGetText(iTXT_VERSION_ID),mchA_MenuPal2,ACS_WATER_COL,ACS_WATER_ALPHA);
}
