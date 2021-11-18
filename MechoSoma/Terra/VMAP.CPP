#include "StdAfx.h"

//#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <setjmp.h>

#ifdef _MECHOSOMA_
#include "mechosoma.h"
#endif

#define XMD_H
extern "C" {
#include "jpeglib.h"
}

#include "Mesh3ds.h"
#include "TERRA.H"
#include "tgai.h"

#ifdef _SURMAP_
#include "land.h"
#endif

#ifdef _SURMAP_
#include "points.h"
extern int 	ShowFog;
#endif

#pragma hdrstop
#pragma warning( disable : 4554 )  

#define LOWLEVEL_OUTPUT
#define DEPTH(u,y)	int(double(u)*double(ysize/2)/double(ysize/2 + y))

#ifdef _SURMAP_
#define SESSION
#endif

#ifndef _WIN32
#include "port.h"
#endif

/* ----------------------------- EXTERN SECTION ---------------------------- */
//extern int* SI;
//extern int* CO;
#ifdef _SURMAP_
extern int RestoreLog,CGenLog,MobilityLog,InitLog,SSSLog,ROLog,EncodeLog,DirectLog,ShotLog,GlassLog,CutvmpLog,WHLog,ExclusiveLog;
void loadSurmapWorldMenuVariable(char* filePathName);
#else 
int ROLog=0;
#endif

extern int ForcedCompressed,ForcedUncompressed;
extern int ConvertLog;
//extern int Verbose;
//extern int DepthShow;
extern int alt_show;
extern int Quit;
//extern int ViewY;
extern int debug_view;
extern unsigned short *zBuffer;


extern int NOISE_AMPL;
extern int DEFAULT_TERRAIN;

extern int TotalDrawFlag;
//extern int RAM16;
//extern char* mapFName;
/* --------------------------- PROTOTYPE SECTION --------------------------- */
char* GetINIstringV(const char* iniFile,const char* section,const char* key);
void saveState(void);
//int LayerWrite(int n,unsigned char* p);
//int LayerRead(int n,unsigned char** p);
void CUTVMP(void);
//void LINE_render(int y);
void RenderStr(int y);
void worldFree(void);
void RenderPrepare(void);
void RenderPrepare1(void);
//void InitSplay(XStream&);
//void ExpandBuffer(unsigned char* InBuffer,unsigned char* OutBuffer);
void LoadPRM(void);
void CompressMap( char* name, int X_SHIFT, int Y_SHIFT );
//void ClipboardInit(void);

#ifdef __HIGHC__
#define invturn _invturn
#define scale _scale
#define pscale _pscale
#endif
/*
void (*invturn)(void*,void*,int,int,void*,int);
void (*scale)(int,int,int,int,unsigned char**,char*,int,int,int);
void (*pscale)(int,int,char*,int,unsigned char*);

#ifdef _ROAD_
extern void (*slope_line)(int,void*,void*,int,int,int,int);
extern void (*slope_line2)(int,void*,void*,int,int,int,int);
extern void (*perp_slope_line)(void*,void*,void*,int,int,int);
extern void (*perp_slope_line1_2)(void*,void*,void*,int,int,int);
extern void (*low_shadow_line)(int,int,int);
extern void (*low_image_line)(int,int,int);
#endif

extern "C"{
	void invturn_10(void*,void*,int,int,void*,int);
	void invturn_11(void*,void*,int,int,void*,int);
	void invturn_12(void*,void*,int,int,void*,int);
	void invturn_13(void*,void*,int,int,void*,int);
	void invturn_14(void*,void*,int,int,void*,int);
	void invturn_15(void*,void*,int,int,void*,int);

	void scale_10(int,int,int,int,unsigned char**,char*,int,int,int);
	void scale_11(int,int,int,int,unsigned char**,char*,int,int,int);
	void scale_12(int,int,int,int,unsigned char**,char*,int,int,int);
	void scale_13(int,int,int,int,unsigned char**,char*,int,int,int);
	void scale_14(int,int,int,int,unsigned char**,char*,int,int,int);
	void scale_15(int,int,int,int,unsigned char**,char*,int,int,int);

	void mmx_scale_10(int,int,int,int,unsigned char**,char*,int,int,int);
	void mmx_scale_11(int,int,int,int,unsigned char**,char*,int,int,int);
	void mmx_scale_12(int,int,int,int,unsigned char**,char*,int,int,int);
	void mmx_scale_13(int,int,int,int,unsigned char**,char*,int,int,int);
	void mmx_scale_14(int,int,int,int,unsigned char**,char*,int,int,int);
	void mmx_scale_15(int,int,int,int,unsigned char**,char*,int,int,int);

	void pscale_10(int,int,char*,int,unsigned char*);
	void pscale_11(int,int,char*,int,unsigned char*);
	void pscale_12(int,int,char*,int,unsigned char*);
	void pscale_13(int,int,char*,int,unsigned char*);
	void pscale_14(int,int,char*,int,unsigned char*);
	void pscale_15(int,int,char*,int,unsigned char*);

#ifdef _ROAD_
	void slope_line_10(int,void*,void*,int,int,int,int);
	void slope_line_11(int,void*,void*,int,int,int,int);
	void slope_line_12(int,void*,void*,int,int,int,int);
	void slope_line_13(int,void*,void*,int,int,int,int);
	void slope_line_14(int,void*,void*,int,int,int,int);
	void slope_line_15(int,void*,void*,int,int,int,int);

	void slope_line2_10(int,void*,void*,int,int,int,int);
	void slope_line2_11(int,void*,void*,int,int,int,int);
	void slope_line2_12(int,void*,void*,int,int,int,int);
	void slope_line2_13(int,void*,void*,int,int,int,int);
	void slope_line2_14(int,void*,void*,int,int,int,int);
	void slope_line2_15(int,void*,void*,int,int,int,int);

	void perp_slope_line_10(void*,void*,void*,int,int,int);
	void perp_slope_line_11(void*,void*,void*,int,int,int);
	void perp_slope_line_12(void*,void*,void*,int,int,int);
	void perp_slope_line_13(void*,void*,void*,int,int,int);
	void perp_slope_line_14(void*,void*,void*,int,int,int);
	void perp_slope_line_15(void*,void*,void*,int,int,int);

	void perp_slope_line1_2_10(void*,void*,void*,int,int,int);
	void perp_slope_line1_2_11(void*,void*,void*,int,int,int);
	void perp_slope_line1_2_12(void*,void*,void*,int,int,int);
	void perp_slope_line1_2_13(void*,void*,void*,int,int,int);
	void perp_slope_line1_2_14(void*,void*,void*,int,int,int);
	void perp_slope_line1_2_15(void*,void*,void*,int,int,int);

	void low_shadow_line_10(int,int,int);
	void low_shadow_line_11(int,int,int);
	void low_shadow_line_12(int,int,int);
	void low_shadow_line_13(int,int,int);
	void low_shadow_line_14(int,int,int);
	void low_shadow_line_15(int,int,int);

	void low_image_line_10(int,int,int);
	void low_image_line_11(int,int,int);
	void low_image_line_12(int,int,int);
	void low_image_line_13(int,int,int);
	void low_image_line_14(int,int,int);
	void low_image_line_15(int,int,int);
#endif
	};*/

struct PrmFile {
	char* buf;
	int len;
	int index;

	void init(char* name);
	char* getAtom(void);
	void finit(void){ delete buf; }
	};
/* --------------------------- DEFINITION SECTION -------------------------- */
unsigned int V_SIZE_POWER;
unsigned int V_SIZE;
unsigned int clip_mask_y;

sVmpHeader VmpHeader;
unsigned int net_size;
unsigned int PART_MAX;


vrtMap* vMap;
///int KeepON = 0;

#ifdef _SURMAP_
#if defined(EXTSCREEN) || defined(POSTER) || defined(ACTINT)
int MAX_LINE = V_SIZE + 8;
#else
int MAX_LINE = 3000;
#endif
#else
int MAX_LINE = 2050;
#endif

//#ifdef SESSION
//static char* sssFName = "SESSION.V__";
//static char* sssFNameT = "SESSION.V_T";
//static char* sssFNameVPR = "SESSION.V_P";
//static unsigned char* linebuf;
//int sssUpdateLog = 1;
//#endif

#ifdef _SURMAP_
int sssInuse;
int lastUpRq,lastDownRq;
#endif

//int* FloodLvl;

static char* iniName;
static int MainQuantToggle = 1;
static int LockSuccess;

#ifdef FILEMAPPING
static unsigned char* mappingPtr;
#endif

//unsigned char** SkipLineTable;
//int preViewY;

char* GetINIstringV(const char* iniFile,const char* section,const char* key)
{
	static char buffer[255+1];
	if (GetPrivateProfileString(section,key,"",buffer,255,iniFile))	return buffer;
	else return NULL;
}
void SaveINIstringV(const char* iniFile,const char* section,const char* key,char* var)
{
	WritePrivateProfileString(section,key,var,iniFile);
}

void vrtMap::loadRenderVariable(void)
{
#ifdef _SURMAP_
	XBuffer tbuf;
	int i;
	tbuf.init();
	tbuf < (GetINIstringV(GetTargetName(wTable[cWorld].fname),"Render Parameters","dif_k")) ;
	tbuf.set(0,XB_BEG);
	for(i=0; i<TERRAIN_MAX; i++)	{ tbuf >= renderV.dif_k[i] ;terra.d_x[0][i]=1.0;}//tbuf >= terra.d_x[0][i]; 
	tbuf.init();
	tbuf < (GetINIstringV(GetTargetName(wTable[cWorld].fname),"Render Parameters","ref_k")) ;
	tbuf.set(0,XB_BEG);
	for(i=0; i<TERRAIN_MAX; i++)	{ tbuf >=renderV.ref_k[i]; terra.d_x[1][i]=1.0;}//tbuf >= terra.d_x[1][i]; 

	tbuf.init();
	tbuf < (GetINIstringV(GetTargetName(wTable[cWorld].fname),"Render Parameters","ref_n")) ;
	tbuf.set(0,XB_BEG);
	for(i=0; i<TERRAIN_MAX; i++) {tbuf >=renderV.ref_n[i]; terra.d_y[0][i]=1.0;}//tbuf >= terra.d_y[0][i]; 
	tbuf.init();
	tbuf < (GetINIstringV(GetTargetName(wTable[cWorld].fname),"Render Parameters","self_luminscence")) ;
	tbuf.set(0,XB_BEG);
	for(i=0; i<TERRAIN_MAX; i++) {tbuf >=renderV.self_luminescence[i];terra.d_y[1][i]=1.0;}//tbuf >= terra.d_y[1][i]; 

	tbuf.init();
	tbuf < (GetINIstringV(GetTargetName(wTable[cWorld].fname),"Render Parameters","AMBIENT")) ;
	tbuf.set(0,XB_BEG);
	for(i=0; i<TERRAIN_MAX; i++) tbuf >= terra.ambient_light[0][i]; 
	tbuf.init();
	tbuf < (GetINIstringV(GetTargetName(wTable[cWorld].fname),"Render Parameters","AMBIENT W")) ;
	tbuf.set(0,XB_BEG);
	for(i=0; i<TERRAIN_MAX; i++) tbuf >= terra.ambient_light[1][i]; 


#endif
}
void vrtMap::saveRenderVariable(void)
{
#ifdef _SURMAP_
	XBuffer tbuf;
	int i;
	tbuf.init();
	for(i=0; i<TERRAIN_MAX; i++) tbuf <= renderV.dif_k[i] < " "; 
	SaveINIstringV(GetTargetName(wTable[cWorld].fname),"Render Parameters","dif_k",tbuf) ;
	tbuf.init();
	for(i=0; i<TERRAIN_MAX; i++) tbuf <= renderV.ref_k[i] < " "; 
	SaveINIstringV(GetTargetName(wTable[cWorld].fname),"Render Parameters","ref_k",tbuf) ;

	tbuf.init();
	for(i=0; i<TERRAIN_MAX; i++) tbuf <= renderV.ref_n[i] < " "; 
	SaveINIstringV(GetTargetName(wTable[cWorld].fname),"Render Parameters","ref_n",tbuf) ;
	tbuf.init();
	for(i=0; i<TERRAIN_MAX; i++) tbuf <= renderV.self_luminescence[i] < " "; 
	SaveINIstringV(GetTargetName(wTable[cWorld].fname),"Render Parameters","self_luminscence",tbuf) ;

	tbuf.init();
	for(i=0; i<TERRAIN_MAX; i++) tbuf <= terra.ambient_light[0][i] < " "; 
	SaveINIstringV(GetTargetName(wTable[cWorld].fname),"Render Parameters","AMBIENT",tbuf) ;
	tbuf.init();
	for(i=0; i<TERRAIN_MAX; i++) tbuf <= terra.ambient_light[1][i] < " "; 
	SaveINIstringV(GetTargetName(wTable[cWorld].fname),"Render Parameters","AMBIENT W",tbuf) ;

	

#endif
}

void vrtMap::loadTrPal(void)
{
	// Set Color water;
	H2OC = atoi(GetINIstringV(GetTargetName(wTable[cWorld].fname),"Global Parameters","Color H2O"));
	H2OCR = (H2OC>>11)&0x1f; H2OCG = (H2OC>>5)&0x3f; H2OCB = (H2OC>>0)&0x1f;
	H2OA = atoi(GetINIstringV(GetTargetName(wTable[cWorld].fname),"Global Parameters","Alpha H2O"));

#ifdef _MECHOSOMA_
	vmapOpenResource(GetTargetName(paletteName), pmap);
#else
	pmap.open(GetTargetName(paletteName),XS_IN | XS_OUT);
#endif

	pmap.read(&TrPal[0][0],TERRAIN_MAX*256*2);
	pmap.close();

	calcH2Ocolor();
#ifndef _SURMAP_
#ifdef _USE_NOT_XGRAPH_
#else //_USE_NOT_XGRAPH_
	if(XGR_HighColorMode!=0){//5x6x5
		if(XGR_HighColorMode==1){//5x5x5 
			for(int i=0;i<TERRAIN_MAX*256*2;i++) {
				TrPal[0][i] = ((TrPal[0][i]&0xFFC0)>>1)|(TrPal[0][i]&0x1F);
			}
		}
		else{// ?x?x?
		}
	}
#endif//_USE_NOT_XGRAPH_
#endif
}

void vrtMap::saveTrPal(void)
{
	// Set Color water;
	char buf [16];
	SaveINIstringV(GetTargetName(wTable[cWorld].fname),"Global Parameters","Color H2O",port_itoa(H2OC,buf,10));
	SaveINIstringV(GetTargetName(wTable[cWorld].fname),"Global Parameters","Alpha H2O",port_itoa(H2OA,buf,10));

	vMap->pmap.open(GetTargetName(vMap->paletteName),XS_OUT);
	vMap->pmap.write(&(vMap->TrPal[0][0]),TERRAIN_MAX*256*2);
	vMap->pmap.close();
}

#ifdef _WIN32
int getTick(void)
#define CPUID __asm _emit 0xf __asm _emit 0xa2
#define RDTSC __asm _emit 0xf __asm _emit 0x31
{
	int time;
	__asm {
		push ebx
		push ecx
		push edx
		xor eax,eax
		CPUID
		RDTSC
		mov [time],eax
		xor eax,eax
		CPUID
		pop edx
		pop ecx
		pop ebx
	}
	return time;
}
#endif
void vrtMap::calcH2Ocolor(int terr)
{
#ifdef _USE_NOT_XGRAPH_
#else
	int r,g,b,a,a1,rw,gw,bw;
	a=H2OA<<(16-6);
	a1=(63-H2OA)<<(16-6);
	rw=H2OCR; gw=H2OCG; bw=H2OCB;
	for(int j=0; j<256;j++){
		if(XGR_HighColorMode!=0){//5x6x5
			r=(TrPal[terr][j]>>11)&0x1f; g=(TrPal[terr][j]>>5)&0x3f; b=(TrPal[terr][j]>>0)&0x1f;
			r=r*a+rw*a1>>16; g=g*a+gw*a1>>16; b=b*a+bw*a1>>16;
			TrPal[terr+16][j] = ((r&0x1f)<<11)+((g&0x3f)<<5)+(b&0x1f);
		}
		if(XGR_HighColorMode==1){//5x5x5 
			r=(TrPal[terr][j]>>10)&0x1f; g=(TrPal[terr][j]>>5)&0x1f; b=(TrPal[terr][j]>>0)&0x1f;
			r=r*a+rw*a1>>16; g=g*a+(gw>>1)*a1>>16; b=b*a+bw*a1>>16;
			TrPal[terr+16][j] = ((r&0x1f)<<10)+((g&0x1f)<<5)+(b&0x1f);
		}
	}
#endif
}
void vrtMap::calcH2Ocolor(void)
{
	int r,g,b,a,a1,rw,gw,bw;
//	int time=getTick();
	a=H2OA<<(16-6);
	a1=(63-H2OA)<<(16-6);
	rw=H2OCR; gw=H2OCG; bw=H2OCB;
	for(int i=0; i< TERRAIN_MAX;i++){
		for(int j=0; j<256;j++){
			r=(TrPal[i][j]>>11)&0x1f; g=(TrPal[i][j]>>5)&0x3f; b=(TrPal[i][j]>>0)&0x1f;
			r=r*a+rw*a1>>16; g=g*a+gw*a1>>16; b=b*a+bw*a1>>16;
			TrPal[i+16][j] = ((r&0x1f)<<11)+((g&0x3f)<<5)+(b&0x1f);
		}
	}
//	int time1=getTick();
//	XCon < "CPU Tick=" <=time < "=" <= time1 < "=" <=(time1-time);
}
/*void vrtMap::calcH2Ocolor(void)
{
	int r,g,b,a,a1,rw,gw,bw,time;
	time=getTick();
	for(int i=0; i< TERRAIN_MAX;i++){
		for(int j=0; j<256;j++){
			r=(TrPal[i][j]>>11)&0x1f; g=(TrPal[i][j]>>5)&0x3f; b=(TrPal[i][j]>>0)&0x1f;
			r=((H2OCR-r)*H2OA>>6)+r; g=((H2OCG-g)*H2OA>>6)+g; b=((H2OCB-b)*H2OA>>6)+b;
			TrPal[i+16][j] = ((r&0x1f)<<11)+((g&0x3f)<<5)+(b&0x1f);
		}
	}
	
	time=getTick()-time;
	XCon < "CPU Tick=" <=time;

}*/
/*void vrtMap::calcH2Ocolor(void)
{
	int c,a,cw,time;
	time=getTick();
	a=H2OA;
	cw=H2OC;
	for(int i=0; i< TERRAIN_MAX;i++){
		for(int j=0; j<256;j++){
			c=(TrPal[i][j]);
			__asm {
				xor ebx,ebx
				xor ecx,ecx
				mov esi,c
				mov edi,cw
				and esi,0F7DEh
				and edi,0F7DEh
				sub esi,edi
				mov eax,a
				//loop1
				rcr eax,1
				setc bl
				dec ebx
				and ebx,esi
				add ebx,edi
				shr ebx,1
				and ebx,0F7DEh
				//2
				rcr eax,1
				setc cl
				dec ecx
				and ecx,esi
				add ecx,edi
				add ebx,ecx
				shr ebx,1
				and ebx,0F7DEh
				//3
				xor ecx,ecx
				rcr eax,1
				setc cl
				dec ecx
				and ecx,esi
				add ecx,edi
				add ebx,ecx
				shr ebx,1
				and ebx,0F7DEh
				//4
				xor ecx,ecx
				rcr eax,1
				setc cl
				dec ecx
				and ecx,esi
				add ecx,edi
				add ebx,ecx
				shr ebx,1
				mov c,ebx



//				mov eax,c
				mov ebx,a
				mov ecx,cw
				and ecx,0F7DEh
				and eax,0F7DEh
				//
				rcr ebx,1
				jc loc1
				mov edx,eax
				jmp loc2
		loc1:	mov edx,ecx
		loc2:	shr edx,1
				and edx,0F7DEh
				rcr ebx,1
				jc loc3
				add edx,eax
				jmp loc4
		loc3:	add edx,ecx
		loc4:	shr edx,1
				and edx,0F7DEh
				rcr ebx,1
				jc loc5
				add edx,eax
				jmp loc6
		loc5:	add edx,ecx
		loc6:	shr edx,1
				and edx,0F7DEh
				rcr ebx,1
				jc loc7
				add edx,eax
				jmp loc8
		loc7:	add edx,ecx
		loc8:	shr edx,1//and edx,0F7DEh
				mov c,edx //
			}
			TrPal[i+16][j] = c;
		}
	}
	time=getTick()-time;
	XCon < "CPU Tick=" <=time;


}*/
/*
char* GetTargetName(char* name)
{
	static int first = 1;
	static char* buf = new char[256];
	static int off;
	if(!name){ first = 1; return NULL; }
	if(first){
		first = 0;
		char path_buffer[_MAX_PATH];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];

		XBuffer b;
		if( (*(name+1)==':' && *(name+2)=='\\') || *name=='\\') b < name;
		else {
			if (*(vMap -> dirWorldPrm)) b < vMap -> dirWorldPrm < '\\' < name;
			else b < name;
		}
		if(_fullpath(path_buffer,b,_MAX_PATH) == NULL) ErrH.Abort("Invalid input name");
		_splitpath(path_buffer,drive,dir,fname,ext);
		strcpy(buf,drive);
		strcat(buf,dir);
		off = strlen(buf);
		strcat(buf,fname);
		strcat(buf,ext);
		return buf;
		}
	buf[off] = '\0';
	strcat(buf,name);
	return buf;
}
*/

char* GetTargetName(char* name)
{
	static char* buf = new char[256];

	strcpy(buf,vMap->wTable[vMap->cWorld].dir);
	if(*buf) strcat(buf,"\\");
	strcat(buf,name);
	return buf;
}

/*
int GetKeepSpace(void)
{ 
	PrmFile pFile;
	pFile.init(mapFName);
	int maxWorld = atoi(pFile.getAtom());
	if(maxWorld < 1) ErrH.Abort("Empty world list");
	register int i;
	char* atom;
	char* buf = new char[256];
	XStream ff(0);
	int size = 0;
	for(i = 0;i < maxWorld;i++){
		atom = pFile.getAtom();
		atom = pFile.getAtom();
		if(!atom) ErrH.Abort("Null world path");
		GetTargetName(NULL);
		strcpy(buf,GetTargetName(atom));
		strcpy(buf,GetINIstringV(buf,"Storage","File Name"));
		strcat(buf,".VMT");
		if(ff.open(GetTargetName(buf))){
			size += ff.size();
			ff.close();
			}
		}
	delete buf;
	pFile.finit();
	GetTargetName(NULL);
	return size/1024/1024;
}

void KeepCleanUp(void)
{ 
	PrmFile pFile;
	pFile.init(mapFName);
	int maxWorld = atoi(pFile.getAtom());
	register int i;
	char* atom;
	char* buf = new char[256];
	XStream ff(0);
	int size = 0;
	for(i = 0;i < maxWorld;i++){
		atom = pFile.getAtom();
		atom = pFile.getAtom();
		GetTargetName(NULL);
		strcpy(buf,GetTargetName(atom));
		strcpy(buf,GetINIstringV(buf,"Storage","File Name"));
		strcat(buf,".VMT");
		if(ff.open(GetTargetName(buf))){
			ff.close();
			remove(GetTargetName(buf));
			}
		}
	delete buf;
	pFile.finit();
	GetTargetName(NULL);
}
*/
void YSetup(void)
{
	V_SIZE = 1 << V_SIZE_POWER;
	clip_mask_y = V_SIZE - 1;
//	V_POWER = V_SIZE_POWER;
	V_SIZE = V_SIZE;
//	if(V_POWER <= 11 && !RAM16) MAX_LINE = V_SIZE + 2;
	if(V_SIZE_POWER <= 11) MAX_LINE = V_SIZE + 2;
	QUANT = 1 << POWER;
	part_map_size_y = 1 << WPART_POWER;
	part_map_size = H_SIZE << WPART_POWER;
	PART_MAX = V_SIZE >> WPART_POWER;
	net_size = H_SIZE*V_SIZE/QUANT/QUANT;
//	if(FloodLvl) delete FloodLvl;
//	FloodLvl = new int[PART_MAX];

#ifdef _SURMAP_
	worldPrepare();
#endif

/*	switch(V_SIZE_POWER){
		case 10:
			invturn = invturn_10;
			scale = mmxUse ? mmx_scale_10 : scale_10;
			pscale = pscale_10;
#ifdef _ROAD_
			slope_line = slope_line_10;
			slope_line2 = slope_line2_10;
			perp_slope_line = perp_slope_line_10;
			perp_slope_line1_2 = perp_slope_line1_2_10;
			//low_shadow_line = low_shadow_line_10;
			//low_image_line = low_image_line_10;
#endif
			break;
		case 11:
			invturn = invturn_11;
			scale = mmxUse ? mmx_scale_11 : scale_11;
			pscale = pscale_11;
#ifdef _ROAD_
			slope_line = slope_line_11;
			slope_line2 = slope_line2_11;
			perp_slope_line = perp_slope_line_11;
			perp_slope_line1_2 = perp_slope_line1_2_11;
			//low_shadow_line = low_shadow_line_11;
			//low_image_line = low_image_line_11;
#endif
			break;
		case 12:
			invturn = invturn_12;
			scale = mmxUse ? mmx_scale_12 : scale_12;
			pscale = pscale_12;
#ifdef _ROAD_
			slope_line = slope_line_12;
			slope_line2 = slope_line2_12;
			perp_slope_line = perp_slope_line_12;
			perp_slope_line1_2 = perp_slope_line1_2_12;
			//low_shadow_line = low_shadow_line_12;
			//low_image_line = low_image_line_12;
#endif
			break;
		case 13:
			invturn = invturn_13;
			scale = mmxUse ? mmx_scale_13 : scale_13;
			pscale = pscale_13;
#ifdef _ROAD_
			slope_line = slope_line_13;
			slope_line2 = slope_line2_13;
			perp_slope_line = perp_slope_line_13;
			perp_slope_line1_2 = perp_slope_line1_2_13;
			//low_shadow_line = low_shadow_line_13;
			//low_image_line = low_image_line_13;
#endif
			break;
		case 14:
			invturn = invturn_14;
			scale = mmxUse ? mmx_scale_14 : scale_14;
			pscale = pscale_14;
#ifdef _ROAD_
			slope_line = slope_line_14;
			slope_line2 = slope_line2_14;
			perp_slope_line = perp_slope_line_14;
			perp_slope_line1_2 = perp_slope_line1_2_14;
			//low_shadow_line = low_shadow_line_14;
			//low_image_line = low_image_line_14;
#endif
			break;
		case 15:
			invturn = invturn_15;
			scale = mmxUse ? mmx_scale_15 : scale_15;
			pscale = pscale_15;
#ifdef _ROAD_
			slope_line = slope_line_15;
			slope_line2 = slope_line2_15;
			perp_slope_line = perp_slope_line_15;
			perp_slope_line1_2 = perp_slope_line1_2_15;
			//low_shadow_line = low_shadow_line_15;
			//low_image_line = low_image_line_15;
#endif
			break;
//		default:
//			ErrH.Abort("MAP POWER Y is out of range");
		}*/
}

void vMapInit(void)
{
//	if(RAM16) MAX_LINE = 900;

	vMap -> init();
//	Verbose = 0;

//#ifdef SESSION
//	linebuf = new unsigned char[H2_SIZE];
//#endif
}

void vMapPrepare(char* name,int nWorld,int nTrack)
{
///		KeepON = 0;
	
	vMap = new vrtMap;
	vMap -> load(name,nWorld,nTrack);

//!#ifdef _SURMAP_
	vMapInit();
//!#endif
}

vrtMap::~vrtMap(void)
{
//	delete FloodLvl; FloodLvl = NULL;
	free(fileName); free(paletteName); free(M3DFileName); free(CLSFileName); free(Ch_pointsFileName);free(FxFileName);
	free(fname); free(pname[0]);
	free(dirWorldPrm);
///	if(KeepON) free(kname);

	if(isCompressed){
#ifndef FILEMAPPING
		if(inbuf){ delete inbuf; inbuf = NULL; }
#endif
		if(st_table){ delete st_table; delete sz_table; st_table = NULL; sz_table = NULL; }
		}

	release();
	finit();

	delete dHeap; dHeap = NULL;
//	delete dHeap_c; dHeap_c = NULL;

//	delete VxBuf; VxBuf=NULL;
//	delete AtBuf; AtBuf=NULL;
//	delete ClTrBuf; ClTrBuf=NULL;


	delete lineT; lineT = NULL;
	delete lineTcolor;  lineTcolor = NULL;
//	delete SkipLineTable; SkipLineTable = NULL;
	delete changedT; changedT = NULL;
///	if(KeepON){ delete keepT; keepT = NULL; }

//	delete freeNodes; freeNodes = NULL;
//	delete freeNodes_c; freeNodes_c = NULL;
}

vrtMap::vrtMap(void)
: fmap(0)//, kmap(0)
{
	pFile = new PrmFile;
	cWorld = 0;
	inbuf = NULL;
	st_table = NULL; sz_table = NULL;

	lineT = NULL; lineTcolor = NULL;
	dHeap = NULL; 
//	dHeap_c = NULL;
	VxBuf=NULL; AtBuf=NULL; ClTrBuf=NULL; //SpecBuf=NULL;

//	freeNodes = NULL; freeNodes_c = NULL;
	wTable = NULL;
	fileName = NULL; paletteName = NULL; M3DFileName = NULL; CLSFileName = NULL; Ch_pointsFileName = NULL; scenarioName = NULL; fname = NULL;FxFileName=NULL;//kname =
	pname[0] = NULL; pname[1] = NULL;
	changedT = NULL;
//	keepT = NULL;
	Buf = NULL;
	ClBufPresent=0;
	LevelH2O=0;
//	fl_kmVxBuf=0;
}

void vrtMap::lockHeap(void)
{
/*
#ifdef _ROAD_
	int ret1 = 0;
	int ret2 = 0;
	if(ExclusiveLog){
		ret1 = win32_VLock(dHeap,MAX_LINE*H2_SIZE);
		ret2 = win32_VLock(dHeap_c,MAX_LINE*H_SIZE);
		if(ret1 || ret2){
			XCon < "\nCan't lock RAM\n";
			XCon < "Error1 " <= ret1 < ":\n";
			XCon < win32_errmsg(ret1) < "\n";
			XCon < "Error2 " <= ret2 < ":\n";
			XCon < win32_errmsg(ret2) < "\n";
			}
		else
			LockSuccess = 1;
		}
#endif
*/
}

/*void vrtMap::allocHeap(void)
{
	dHeap = new unsigned char[MAX_LINE*H2_SIZE];
	dHeap_c = new unsigned char[MAX_LINE*H_SIZE];
}*/

void vrtMap::alloc4Buf(void)
{
	if(Buf != NULL) release4Buf();
#ifdef _SURMAP_
	allocClBuf();
	Buf = (unsigned char*)malloc(YS_Buf*XS_Buf*5);//+XS_Buf];
	if(Buf == 0) {ErrH.Abort("MEMORY for VMAP is not allocation");}

	VxBuf = (unsigned short(*)[YS_Buf][XS_Buf])(Buf+offsetVx);
	AtBuf = (unsigned char(*)[YS_Buf][XS_Buf])(Buf+offsetAt);
	//SpecBuf = (unsigned char(*)[YS_Buf][XS_Buf])(Buf+offsetSpec);
	ClTrBuf = (unsigned char(*)[YS_Buf][XS_Buf])(Buf+offsetClTr);

#else
//#ifdef HI_COLOR_WORLD 
	allocClBuf();
//#endif
//	if(notH2O==0) {
//		//Buf = new unsigned char [YS_Buf*XS_Buf*4];
//		Buf = (unsigned char*) malloc(YS_Buf*XS_Buf*4);
//		if(Buf == 0) {ErrH.Abort("MEMORY for VMAP is not allocation");}
//		SpecBuf = (unsigned char(*)[YS_Buf][XS_Buf])(Buf+offsetSpec);
//	}
//	else {
		//Buf = new unsigned char [YS_Buf*XS_Buf*3];//+XS_Buf];
		Buf = (unsigned char*) malloc( YS_Buf*XS_Buf/4*1+(YS_Buf*XS_Buf/4) );
		if(Buf == 0) {ErrH.Abort("MEMORY for VMAP is not allocation");}
		//SpecBuf = (unsigned char(*)[YS_Buf][XS_Buf])(Buf+offsetVx);//NULL
//	}
	VxBuf = (unsigned char(*)[YS_Buf][XS_Buf])(Buf+offsetVx);
	AtBuf = (unsigned char(*)[YS_Buf][XS_Buf])(Buf+offsetAt);
	ClTrBuf = AtBuf;//(unsigned char(*)[YS_Buf][XS_Buf])(Buf+offsetClTr);
#endif

}
void vrtMap::release4Buf(void)
{
	free(Buf);
//#ifdef HI_COLOR_WORLD 
//#endif
	releaseClBuf();
}

void vrtMap::releaseClBuf(void)
{
	if(ClBufPresent){
		delete [] ClBuf;
		ClBufPresent=0;
	}
}
void vrtMap::allocClBuf(void)
{
	if(!ClBufPresent){
		ClBuf= new unsigned short [1][YS_Buf][XS_Buf];
		ClBufPresent=1;
	}
}

void vrtMap::release(void)
{/*
#ifdef _ROAD_
	if(ExclusiveLog && LockSuccess){
		win32_VUnLock(dHeap,MAX_LINE*H2_SIZE);
		win32_VUnLock(dHeap_c,MAX_LINE*H_SIZE);
		LockSuccess = 0;
		}
#endif*/
	fmap.close();
/*	if(KeepON){ 
		for(unsigned int i = 0;i < V_SIZE;i++)
			if(lineT[i] && changedT[i]){
				keepT[i] = 1;
				kmap.seek(i*H2_SIZE,XS_BEG);
				kmap.write(lineT[i],H2_SIZE);
				}
		kmap.seek(V_SIZE*H2_SIZE,XS_BEG);
		kmap.write(keepT,V_SIZE);
		kmap.close();
		}*/
}

void vrtMap::init(void)
{
//	memset(lineT = new unsigned char*[V_SIZE],0,V_SIZE*sizeof(unsigned char*));
//	memset(lineTcolor = new unsigned char*[V_SIZE],0,V_SIZE*sizeof(unsigned char*));
//	memset(SkipLineTable = new unsigned char*[V_SIZE],0,V_SIZE*sizeof(unsigned char*));
	memset(changedT = new unsigned char[V_SIZE],0,V_SIZE);

//	freeNodes = new vrtNode[MAX_LINE];
//	freeNodes_c = new vrtNode[MAX_LINE];

//	allocHeap();
//	alloc4Buf();

	freeMax = freeMax_c = MAX_LINE;
	freeTail = freeTail_c = 0;

//	vrtNode* p;
//	register int i;
//	for(i = 0,p = freeNodes;i < MAX_LINE - 1;i++,p++) p -> next = p + 1;
//	p -> next = freeNodes;
//	for(i = 0,p = freeNodes_c;i < MAX_LINE - 1;i++,p++) p -> next = p + 1;
//	p -> next = freeNodes_c;

/*#ifdef SESSION
	if(!DirectLog){
		sssT = new int[V_SIZE];
		if(SSSLog){
			fsss.open(sssFNameT,XS_IN);
			fsss.read(sssT,V_SIZE*sizeof(int));
			fsss.close();
			fsss.open(sssFName,XS_IN);
			sssUpdate();
			fsss.close();
			LoadVPR(1);
			}
		else {
			memset(sssT,0xFF,V_SIZE*sizeof(int));
			if(!ROLog){
				fsss.open(sssFName,XS_OUT);
				fsss.close();
				}
			}
		if(!ROLog) fsss.open(sssFName,XS_IN|XS_OUT);
		}
#endif*/
	upLine = downLine = 0;
}


void vrtMap::finit(void)
{
/*#ifdef SESSION
	if(!DirectLog){
		if(sssUpdateLog){
			sssUpdate();
			SaveVPR();
			}
		else {
			fsss.close();
			if(!ROLog){
				fsss.open(sssFNameT,XS_OUT);
				fsss.write(sssT,V_SIZE*sizeof(int));
				SaveVPR(1);
				}
			}
		fsss.close();
		}
#endif
#ifdef FILEMAPPING
	win32_FileMirror(0,0,1);
#endif
*/
}

#ifdef _SURMAP_
void ConvertProcedure(char* name)
{
	XStream fin(name,XS_IN);
	char* outname = strdup(name);
	memcpy(outname + strlen(name) - 3,"CNV",3);

	XStream fout(outname,XS_OUT);

	fin.seek(512,XS_BEG);

	const int xsize = H_SIZE;
	const int ysize = V_SIZE;

	unsigned char* buf = new unsigned char[xsize];

	XCon < "\n";
	register int i;
	for(i = 0;i < ysize;i++){
		fin.read(buf,xsize);
		fout.write(buf,xsize);
		fin.seek(xsize,XS_CUR);
		fin.read(buf,xsize);
		fout.write(buf,xsize);
		XCon < "\r" <= i;
		}
	XCon < "\n";

	delete buf;
	fin.close();
	fout.close();
}
#endif

void vrtMap::analyzeINI(const char* name)
{
	const char* version = "1.4";

	const char* secGlobal = "Global Parameters";
	const char* secStorage = "Storage";
	const char* secRender = "Rendering Parameters";
	const char* secPalette = "Dynamic Palette";
	const char* secCreation = "Creation Parameters";
	const char* secM3D = "M3D Parameters";
	const char* secCLS = "CLS Parameters";
	const char* secCh_points = "Ch_points Parameters";

	if(iniName) free(iniName);
	iniName = strdup(name);

	XStream ff((char*)name,XS_IN);
	ff.close();

	int val = atoi(GetINIstringV(name,secGlobal,"Map Power X"));
	if(val != H_SIZE_POWER) ErrH.Abort("Incorrect X-Size");
	V_SIZE_POWER = atoi(GetINIstringV(name,secGlobal,"Map Power Y"));
	POWER = atoi(GetINIstringV(name,secGlobal,"GeoNet Power"));
	WPART_POWER = atoi(GetINIstringV(name,secGlobal,"Section Size Power"));
	MINSQUARE = 1 << atoi(GetINIstringV(name,secGlobal,"Minimal Square Power"));

	if(strcmp(version,GetINIstringV(name,secStorage,"Version"))) ErrH.Abort("Incorrect Storage Version");
	isCompressed = atoi(GetINIstringV(name,secStorage,"Compressed Format Using"));
#ifdef _SURMAP_
	if(ForcedCompressed) isCompressed = 1;
	if(ForcedUncompressed) isCompressed = 0;
#endif

	fileName = strdup(GetINIstringV(name,secStorage,"File Name"));
	paletteName = strdup(GetINIstringV(name,secStorage,"Palette File"));
	M3DFileName = strdup(GetINIstringV(name,secM3D,"M3D File"));
	CLSFileName = strdup(GetINIstringV(name,secCLS,"CLS File"));
	Ch_pointsFileName = strdup(GetINIstringV(name,secCh_points,"Ch_points File"));
	//Ch_pointsFileName = "CH_P.TK0";
//	FxFileName = strdup(GetINIstringV(name,secFx,"Fx File"));

	int tmax = atoi(GetINIstringV(name,secRender,"Terrain Max"));
	if((!tmax && TERRAIN_MAX != 8) || (tmax && tmax != TERRAIN_MAX)) ErrH.Abort("Incorrect Terrain Max");

/*	char* p = GetINIstringV(name,secRender,"Begin Colors");
	XBuffer buf(p,128);
	register int i;
	for(i = 0;i < TERRAIN_MAX;i++) buf >= BEGCOLOR[i];
	GetINIstringV(name,secRender,"End Colors");
	buf.set(0);
	for(i = 0;i < TERRAIN_MAX;i++) buf >= ENDCOLOR[i];
*/
//	pal_iter_init();
/*	PAL_MAX = atoi(GetINIstringV(name,secPalette,"Terrain Number"));
	PAL_WAVE_TERRAIN = atoi(GetINIstringV(name,secPalette,"Wave Terrain"));
	if(PAL_MAX > 0){
		char* pp = GetINIstringV(name,secPalette,"Terrains");
		XBuffer b(pp,128);
		for(i = 0;i < PAL_MAX;i++) b >= PAL_TERRAIN[i];
		GetINIstringV(name,secPalette,"Speeds");
		b.set(0);
		for(i = 0;i < PAL_MAX;i++) b >= PAL_SPEED[i];
		GetINIstringV(name,secPalette,"Amplitudes");
		b.set(0);
		for(i = 0;i < PAL_MAX;i++) b >= PAL_AMPL[i];
		GetINIstringV(name,secPalette,"Red");
		b.set(0);
		for(i = 0;i < PAL_MAX;i++) b >= PAL_RED[i];
		GetINIstringV(name,secPalette,"Green");
		b.set(0);
		for(i = 0;i < PAL_MAX;i++) b >= PAL_GREEN[i];
		GetINIstringV(name,secPalette,"Blue");
		b.set(0);
		for(i = 0;i < PAL_MAX;i++) b >= PAL_BLUE[i];
		}*/

#ifdef _SURMAP_
	scenarioName = strdup(GetINIstringV(name,secCreation,"Build Scenario File"));
	GeonetMESH = atoi(GetINIstringV(name,secCreation,"Mesh Value"));
	NOISE_AMPL = atoi(GetINIstringV(name,secCreation,"Noise Amplitude"));
	DEFAULT_TERRAIN = atoi(GetINIstringV(name,secCreation,"Default Terrain Type"));// << TERRAIN_OFFSET;
#endif
}

void vrtMap::fileLoad(void)
{
//	XBuffer b;
//	b < dirWorldPrm < '\\' < wTable[cWorld].fname;
//	analyzeINI(GetTargetName(b));
	analyzeINI(GetTargetName(wTable[cWorld].fname));
	YSetup();

	XBuffer buf;
#ifdef _SURMAP_
	buf < fileName < (isCompressed ? ".VMC" : ".VMP");
#else 
	buf < fileName < ".VMM";
#endif
	fname = strdup(GetTargetName((char*)buf));
	pname[0] = strdup(fname); memcpy(pname[0] + strlen(pname[0]) - 3,"VPR",3);
//	if(KeepON){
//		kname = strdup(fname);
//		memcpy(kname + strlen(kname) - 3,"vmt",3);
//		}
//
//	if(isCompressed){
//#ifdef _SURMAP_
//		ROLog = 1;
//#endif
//		XCon < "\nCompressed format detected...";
//		}
#ifdef _SURMAP_
//	ClipboardInit();
	LoadPP();
//#ifdef SESSION
//	if(!DirectLog) pname[1] = sssFNameVPR;
//#endif
#endif

#ifdef _MECHOSOMA_
	mch_vmapResourceInit(); //���������� ���������� ����(� ��������)
#endif
}

void vrtMap::open(char* name,int nWorld)
{
	//����������� ������ ����� �������� � dirWorldPrm 
	char* dir,*dirtemp;
	char *dirend=dirtemp=dir=strdup(name);
	while(*dirtemp) {
		if (*dirtemp=='\\') dirend = dirtemp;
		dirtemp++;
	}
	*dirend = 0;
	dirWorldPrm = strdup(dir);
	free(dir);
	//
	// �������� ������� ����� �� PRM
	pFile -> init(name);
	maxWorld = atoi(pFile -> getAtom());
	if(maxWorld < 1) ErrH.Abort("Empty world list");
	int i;
	char* atom;
	wTable = new vrtWorld[maxWorld];
	for(i = 0;i < maxWorld;i++){
		atom = pFile -> getAtom();
		if(!atom) ErrH.Abort("Null world name");
		wTable[i].name = strdup(atom);
		atom = pFile -> getAtom();
		if(!atom) ErrH.Abort("Null world path");
		wTable[i].fname = strdup(atom);
		wTable[i].numberMechosomWorld = atoi(pFile -> getAtom());
		wTable[i].numberMechosomTrack = atoi(pFile -> getAtom());
		}
	pFile -> finit();
	//
	if(nWorld >= maxWorld || nWorld < 0) nWorld = 0;
	cWorld = nWorld;


}
void vrtMap::load(char* name,int nWorld,int nTrack)
{
	//����������� ������ ����� �������� � dirWorldPrm 
	char* dir,*dirtemp;
	char *dirend=dirtemp=dir=strdup(name);
	while(*dirtemp) {
		if (*dirtemp=='\\') dirend = dirtemp;
		dirtemp++;
	}
	*dirend = '\0';
	dirWorldPrm = strdup(dir);
	free(dir);
	//
	pFile -> init(name);
	maxWorld = atoi(pFile -> getAtom());
	if(maxWorld < 1) ErrH.Abort("Empty world list");
	int i;
	char* atom;
	wTable = new vrtWorld[maxWorld];
	char str_dir[256];
	for(i = 0;i < maxWorld;i++){
		atom = pFile -> getAtom();
		if(!atom) ErrH.Abort("Null world name");
		wTable[i].name = strdup(atom);
		atom = pFile -> getAtom();
		if(!atom) ErrH.Abort("Null world path");
		//wTable[i].fname = strdup(atom);
		//��������� world.ini �� ���� � �������� ����
		strcpy(str_dir,dirWorldPrm);//����������� � ������ ���� �� �������� � worlds.prm
		if(*dirWorldPrm) strcat(str_dir,"\\");		
		strcat(str_dir,atom);		//���������� � ������ ���� �� �������� � �����
		int k,L=strlen(str_dir);
		if(L) for(k=L-1; k>=0; k--){
			if(str_dir[k]=='\\') {
				str_dir[k]=0;
				break;
			}
		}
		wTable[i].dir=strdup(str_dir);
		if( k<(L-1) && k>=0 ) wTable[i].fname = strdup(&str_dir[k+1]);
		else ErrH.Abort("World INI file not found");
		wTable[i].numberMechosomWorld = atoi(pFile -> getAtom());
		wTable[i].numberMechosomTrack = atoi(pFile -> getAtom());
	}
	pFile -> finit();

	if(nWorld >= maxWorld || nWorld < 0) nWorld = 0;
#ifdef _SURMAP_
	cWorld = nWorld;
#else //���� ��������
	cWorld=0;
	for(i=0; i< maxWorld; i++){
		if ( (wTable[i].numberMechosomWorld==nWorld) && (wTable[i].numberMechosomTrack==nTrack) ){
			cWorld = i;
			break;
		}
	}
#endif

	fileLoad();
#ifdef _SURMAP_
	loadSurmapWorldMenuVariable(GetTargetName("swmv.dat"));
#endif

//#ifdef _SURMAP_
//	if(ConvertLog){
//		ConvertProcedure(fname);
//		Quit = XT_TERMINATE_ID;
//		}
//#endif

/*
#ifdef _ROAD_
	if(isCompressed == 2){
		memcpy(fname + strlen(fname) - 3,"vml",3);
		CompressMap(fname,H_SIZE_POWER,V_SIZE_POWER);
		remove(fname);
		memcpy(fname + strlen(fname) - 3,"vmc",3);
		KDWIN::WritePrivateProfileString("Storage","Compressed Format Using","1",iniName);
		}
#endif
*/
#ifdef _SURMAP_
	if(XKey.wasPressed('C')) CGenLog = 1;
	int exist = 1;
	if(!fmap.open(fname,XS_IN)) exist = 0;
	else fmap.close();
	if(isCompressed && !exist) ErrH.Abort("Map not found");
	if(/*!ROLog && */(InitLog || RestoreLog || CGenLog || ShotLog || MobilityLog || GlassLog || WHLog || !exist)) buildWorld();
	Quit = ShotLog ? XT_TERMINATE_ID : 0;

	if(ROLog)
		fmap.open(fname,XS_IN);
	else
		fmap.open(fname,XS_IN | XS_OUT);
#else
#ifdef _MECHOSOMA_
	//if(!fmap.open(fname,XS_IN)) ErrH.Abort("WorldData not found");
	if(!vmapOpenResource(fname,fmap)) ErrH.Abort("WorldData not found");
#endif //_MECHOSOMA_
#endif //_SURMAP_

//#ifdef FILEMAPPING
//	mappingPtr = (unsigned char*)win32_FileMirror(fmap.gethandler(),fmap.size());
//	if(!mappingPtr) ErrH.Abort("File Mapping Error");
//#endif

	if(st_table){ delete st_table; delete sz_table; st_table = NULL; sz_table = NULL; }
	if(isCompressed){
#ifndef FILEMAPPING
		inbuf = new unsigned char[H2_SIZE];
#endif
		st_table = new int[V_SIZE];
		sz_table = new short[V_SIZE];
		for(unsigned int i = 0;i < V_SIZE;i++){
			fmap > st_table[i];
			fmap > sz_table[i];
//			if(sz_table[i] >= H2_SIZE)
//				ErrH.Abort("Wrong compression");
			}
//		InitSplay(fmap);
		}
	else {
		//!! if(fmap.size() != (int)H_SIZE*(int)V_SIZE*3) ErrH.Abort("Incorrect vmp-file size");
		}

	offset = 0;
	foffset = fmap.tell();

#ifdef _SURMAP_
	if(CutvmpLog){
		CUTVMP();
		Quit = XT_TERMINATE_ID;
		}
#endif

	LoadVPR();
#ifdef _SURMAP_
	loadRenderVariable();
	RenderPrepare1();
#endif
}

void vrtMap::reload(int nWorld, int nTrack)
{
	if(nWorld >= maxWorld || nWorld < 0) ErrH.Abort("World Index out of range");
#ifdef _SURMAP_
//	if(cWorld == nWorld) return;//��������� �� �� �������� ���� ������ �� reload
#endif

#ifdef _SURMAP_
	cWorld = nWorld;
#else //���� ��������
	cWorld=0;
	int i;
	for(i=0; i< maxWorld; i++){
		if ( (wTable[i].numberMechosomWorld==nWorld) && (wTable[i].numberMechosomTrack==nTrack) ){
			cWorld = i;
			break;
		}
	}
#endif


#ifdef _SURMAP_
	worldFree();
#endif
	delM3DAll();
	delCLSAll();

//	delete FloodLvl; FloodLvl = NULL;
	free(fileName); free(paletteName);free(M3DFileName);free(CLSFileName);free(Ch_pointsFileName);free(FxFileName);
	free(fname); free(pname[0]);

	fileLoad();
#ifdef _SURMAP_
	loadSurmapWorldMenuVariable(GetTargetName("swmv.dat"));
#endif

#ifdef _SURMAP_
	ShowFog=0;
//	fxWaveFinit();
//	fxWaveInit();
//	fxWaveLoad(GetTargetName("fx.dat"));
	ShowFog=1;
#endif

	memset(changedT,0,V_SIZE);
//#ifdef _SURMAP_
	fmap.close();
	int exist = 1;
#ifdef _MECHOSOMA_
	if(!vmapOpenResource(fname,fmap)) exist = 0;
#else
	if(!fmap.open(fname,XS_IN | XS_OUT)) exist = 0;
#endif
	else fmap.close();
	if(!exist) ErrH.Abort("Can't load world,XERR_USER,-1,fname");

#ifdef _SURMAP_
	if(ROLog)
		fmap.open(fname,XS_IN);
	else
		fmap.open(fname,XS_IN | XS_OUT);
#else //_SURMAP_
#ifdef _MECHOSOMA_
	//if(!fmap.open(fname,XS_IN)) ErrH.Abort("WorldData not found");
	if(!vmapOpenResource(fname,fmap)) ErrH.Abort("WorldData not found");
#endif //_MECHOSOMA_
#endif //_SURMAP_


//#else
//	if(!fmap.open(fname,XS_IN|XS_NOSHARING)) ErrH.Abort("WorldData not found");
	LoadVPR();
#ifdef _SURMAP_
	loadRenderVariable();
	RenderPrepare1();
#endif
}

/*void vrtMap::reloadold(int nWorld)
{
	if(nWorld >= maxWorld || nWorld < 0) ErrH.Abort("World Index out of range");
#ifdef _SURMAP_
	if(cWorld == nWorld) return;
#endif
	cWorld = nWorld;
	unsigned int old_v_size = V_SIZE;

#ifdef _SURMAP_
	worldFree();
#endif
	delete FloodLvl; FloodLvl = NULL;
	free(fileName); free(paletteName);
	free(fname); free(pname[0]);
	GetTargetName(NULL);

	fileLoad();

#ifdef _ROAD_
#ifdef FILEMAPPING
	win32_FileMirror(0,0,1);
#endif
#endif

	if(V_SIZE != old_v_size){
		delete lineT;
		delete lineTcolor;
		delete SkipLineTable;
		delete changedT;
		memset(lineT = new unsigned char*[V_SIZE],0,V_SIZE*sizeof(unsigned char*));
		memset(lineTcolor = new unsigned char*[V_SIZE],0,V_SIZE*sizeof(unsigned char*));
		memset(SkipLineTable = new unsigned char*[V_SIZE],0,V_SIZE*sizeof(unsigned char*));
		memset(changedT = new unsigned char[V_SIZE],0,V_SIZE);
		if(KeepON){ delete keepT; memset(keepT = new unsigned char[V_SIZE],0,V_SIZE); }
		}
	else {
		memset(lineT,0,V_SIZE*sizeof(unsigned char*));
		memset(lineTcolor,0,V_SIZE*sizeof(unsigned char*));
		memset(SkipLineTable,0,V_SIZE*sizeof(unsigned char*));
		memset(changedT,0,V_SIZE);
		if(KeepON){ memset(keepT,0,V_SIZE); }
		}

	register int i;
#ifdef _SURMAP_
	int exist = 1;
	if(!fmap.open(fname,XS_IN)) exist = 0;
	else fmap.close();
	if(!exist) ErrH.Abort("Can't load world,XERR_USER,-1,fname");

	if(ROLog)
		fmap.open(fname,XS_IN);
	else
		fmap.open(fname,XS_IN | XS_OUT);
#else
	if(!fmap.open(fname,XS_IN|XS_NOSHARING)) ErrH.Abort("WorldData not found");
	if(KeepON){
		int need = 0;
		if(kmap.open(kname,XS_IN)){
			if(kmap.size() != (int)(V_SIZE*H2_SIZE + V_SIZE),XS_BEG){
				kmap.close(); need = 1;
				}
			else {
				kmap.seek(V_SIZE*H2_SIZE,XS_BEG);
				kmap.read(keepT,V_SIZE);
				}
			}
		else need = 1;
		if(need){
			int h = _open(kname,_O_BINARY|_O_CREAT|_O_WRONLY,_S_IREAD|_S_IWRITE);
			if(_chsize(h,V_SIZE*H2_SIZE)){
				_close(h);
				remove(kname);
#if defined(RUSSIAN_VERSION) && !defined(GERMAN_VERSION)
				ErrH.Abort("������������ ����� �� ������� ����� ��� ������ � ������ ���������� ���� ��������!");
#else
				ErrH.Abort("Can't create KEEP File. Please, free more disk space for this feature working.");
#endif
				}
			_close(h);*/
//			kmap.open(kname,XS_OUT|XS_NOSHARING/*|XS_NOBUFFERING*/);
//			kmap.seek(V_SIZE*H2_SIZE,XS_BEG);
//			kmap.write(keepT,V_SIZE);
//			}
//		kmap.close();
//		kmap.open(kname,XS_IN|XS_OUT|XS_NOSHARING/*|XS_NOBUFFERING*/);
/*		}
#endif

#ifdef FILEMAPPING
	mappingPtr = (unsigned char*)win32_FileMirror(fmap.gethandler(),fmap.size());
	if(!mappingPtr) ErrH.Abort("File Mapping Error");
#endif

	if(isCompressed){
#ifndef FILEMAPPING
		if(!inbuf) inbuf = new unsigned char[H2_SIZE];
#endif
		if(V_SIZE != old_v_size || !st_table){
			if(st_table){ delete st_table; delete sz_table; }
			st_table = new int[V_SIZE];
			sz_table = new short[V_SIZE];
			}
		for(i = 0;i < (int)V_SIZE;i++){
			fmap > st_table[i];
			fmap > sz_table[i];
			if(sz_table[i] >= H2_SIZE)
				ErrH.Abort("Wrong compression");
			}
//		InitSplay(fmap);
		}
	else
		if(fmap.size() != (int)H_SIZE*(int)V_SIZE*2) ErrH.Abort("Incorrect vmp-file size");

	offset = 0;
	foffset = fmap.tell();

	freeMax = freeMax_c = MAX_LINE;
	freeTail = freeTail_c = 0;

	vrtNode* p;
	for(i = 0,p = freeNodes;i < MAX_LINE - 1;i++,p++) p -> next = p + 1;
	p -> next = freeNodes;
	for(i = 0,p = freeNodes_c;i < MAX_LINE - 1;i++,p++) p -> next = p + 1;
	p -> next = freeNodes_c;

#ifdef SESSION
	if(!DirectLog){
		delete sssT;
		memset(sssT = new int[V_SIZE],0xFF,V_SIZE*sizeof(int));
		if(!ROLog){
			fsss.open(sssFName,XS_OUT);
			fsss.close();
			fsss.open(sssFName,XS_IN|XS_OUT);
			}
		}
#endif

	LoadVPR();
	RenderPrepare();

#ifdef _ROAD_
	if(V_SIZE_POWER <= 11 && )
		accept(0,V_SIZE - 1);
	else
//		accept(ViewY,ViewY);
#else
	if(V_SIZE_POWER <= 11)
		accept(0,V_SIZE - 1);
	else {
		upLine = 1;
		downLine = 0;
		}
#endif
}*/

void vrtMap::increase(int up,int down)
{
/*	if(V_SIZE_POWER <= 11 && !RAM16) return;

	up = YCYCL(up);
	down = YCYCL(down);

	int d = getDistY(upLine,up);
	if(d > 0){
		if(isCompressed) linkC(upLine - 1,up,-1);
		else link(upLine - 1,up,-1);
		upLine = up;
		}

	d = getDistY(downLine,down);
	if(d < 0){
		if(isCompressed) linkC(downLine + 1,down,1);
		else link(downLine + 1,down,1);
		downLine = down;
		}*/
}

void vrtMap::accept(int up,int down)
{/*
#ifdef _ROAD_
	if(!(V_SIZE_POWER<= 11 )){
		up -= MAX_LINE/2 - 4;
		down += MAX_LINE/2 - 4;
		}
#endif

	up = YCYCL(up);
	down = YCYCL(down);

	int max = YCYCL(down + 1);
	register int i = up;
	unsigned char* p;
	int off;
	if(!isCompressed)
		do {
			freeMax--;
			off = i*H2_SIZE;
			p = use();
			if(KeepON && keepT[i]){
				kmap.seek(off,XS_BEG);
				kmap.read(p,H2_SIZE);
				}
			else {
				if(off != offset){
					offset = off + H2_SIZE;
					fmap.seek(foffset + off,XS_BEG);
					}
				else
					offset += H2_SIZE;
				fmap.read(p,H2_SIZE);
				}
			lineT[i] = p;
			lineTcolor[i] = use_c();
			LINE_render(i);
			i = YCYCL(i + 1);
		} while(i != max);
	else
		do {
			freeMax--;
#ifdef FILEMAPPING
			lineT[i] = p = use();
			lineTcolor[i] = use_c();
			ExpandBuffer(mappingPtr + st_table[i],p);
#else
			p = use();
			if(KeepON && keepT[i]){
				kmap.seek(i*H2_SIZE,XS_BEG);
				kmap.read(p,H2_SIZE);
				}
			else {
				fmap.seek(st_table[i],XS_BEG);
				fmap.read(inbuf,sz_table[i]);
				ExpandBuffer(inbuf,p);
				}
			lineT[i] = p;
			lineTcolor[i] = use_c();
#endif
			LINE_render(i);
			i = YCYCL(i + 1);
		} while(i != max);

	upLine = up;
	downLine = down;
	preViewY = ViewY;*/
}
///////////////////////////////////////////////////////////I

void vrtMap::checkAndRecoverWater()
{
	unsigned char* ch = vMap -> changedT;
	for(int y=0; y<V_SIZE; y++){
		for(int x=0; x<H_SIZE; x++){
			if(GetW(x,y)) {
				if(GetAlt(x,y)>=LevelH2O) {
					SetNW(x,y);
					ch[y] = 1;
				}
			}
		}
	}

}

void vrtMap::load3Buf(int X,int Y,int num_track)
#ifdef _SURMAP_
{
	X=0;Y=YS_Buf;//������ ��� ������ !
	//�������� ����
	LevelH2O=0;
	LevelH2O = atoi(GetINIstringV(GetTargetName(wTable[cWorld].fname),"Global Parameters","LevelH2O"));
	if(LevelH2O<0)LevelH2O =0;

	alloc4Buf();

	loadTrPal();
	int i,j;
	const char id[4]={'S','M','W','0'};
	const char id1[4]={'S','M','W','1'};
	const char id2[4]={'S','M','W','2'};
	fmap.seek(0,XS_BEG);
	fmap.read(&VmpHeader,sizeof(VmpHeader));
	if (*(int*)VmpHeader.id == *(int*)id2 ){ //�������� 2 ������� VMP
		fmap.read(&VxBuf[0][0][0],XS_Buf*YS_Buf*2);
		fmap.read(&AtBuf[0][0][0],XS_Buf*YS_Buf);
		// WORLD RENDER 
		for(i=X;i<Y;i++){
			changedT[i]=0;
			RenderStr(i);
		}
	}
	else {
		if (*(int*)VmpHeader.id == *(int*)id1 ){ //�������� 1 ������� VMP

			unsigned char* tVxBuf;
			tVxBuf=new unsigned char [YS_Buf*XS_Buf];
			fmap.read(tVxBuf,XS_Buf*YS_Buf);
			for(i=0; i<YS_Buf; i++) for(j=0; j<XS_Buf; j++) VxBuf[0][i][j]=tVxBuf[j+i*XS_Buf]<<6;
			delete [] tVxBuf;

			fmap.read(&AtBuf[0][0][0],XS_Buf*YS_Buf);
			//fmap.read(&SpecBuf[0][0][0],XS_Buf*YS_Buf);
			// WORLD RENDER 
			for(i=X;i<Y;i++){
				changedT[i]=0;
				RenderStr(i);
			}
			// AND SAVE NEW FORMAT
			save3BufOnly();
		}
		else {
			if (*(int*)VmpHeader.id == *(int*)id ){ //�������� 0 ������� VMP (� SpecBuf ������ �������� ��� �����)
				//�������� � UPDATE 0 ������� VMP

				unsigned char* tVxBuf;
				tVxBuf=new unsigned char [YS_Buf*XS_Buf];
				fmap.read(tVxBuf,XS_Buf*YS_Buf);//&VxBuf[0][0][0],XS_Buf*YS_Buf);
				fmap.read(&AtBuf[0][0][0],XS_Buf*YS_Buf);
				for(i=0; i<YS_Buf; i++) for(j=0; j<XS_Buf; j++) VxBuf[0][i][j]=tVxBuf[j+i*XS_Buf]<<6;

				fmap.read(tVxBuf,XS_Buf*YS_Buf);//&SpecBuf[0][0][0],XS_Buf*YS_Buf);
				//����������� ����������� ��� ����� � �������� 
				for(i=0; i<YS_Buf; i++){
					for(j=0; j<XS_Buf; j++){
						if(vMap->AtBuf[0][i][j] & At_WATER){
							vMap->VxBuf[0][i][j]=tVxBuf[j+i*XS_Buf];
						}
					}
				}

				delete [] tVxBuf;

				// WORLD RENDER 
				for(i=X;i<Y;i++){
					changedT[i]=1;
					RenderStr(i);
				}
				// AND SAVE NEW FORMAT
				save3BufOnly();
			}
			else {
				//�������� � UPDATE Non standart OLD!(������� ��� ���������� ��������� ����)
				unsigned char* tVxBuf;
				tVxBuf=new unsigned char [XS_Buf];
				fmap.seek(0,XS_BEG);
				for(i=X;i<Y;i++){

			//		fmap.seek(i*XS_Buf*2,XS_BEG);
					fmap.read(tVxBuf,XS_Buf);//&VxBuf[0][i][0],XS_Buf);
					fmap.read(&AtBuf[0][i][0],XS_Buf);

					for(j=0;j<XS_Buf;j++){
						(AtBuf[0][i][j])= (AtBuf[0][i][j]&Tr_MASK) | ((AtBuf[0][i][j]&0x10)<<1) | ((AtBuf[0][i][j]&0x20)>>1) ;
						VxBuf[0][i][j]=tVxBuf[j+i*XS_Buf]<<6;
					}
				}
				delete [] tVxBuf;

				//fmap.seek(YS_Buf*XS_Buf*2,XS_BEG);
				//for(i=X;i<Y;i++){
				//	fmap.read(&SpecBuf[0][i][0],XS_Buf);
				//}

				//for(i=0;i<YS_Buf;i++){
				//	for(j=0; j<XS_Buf; j++){
				//		SpecBuf[0][i][0]=0;
				//	}
				//}

				// WORLD RENDER 
				for(i=X;i<Y;i++){
					changedT[i]=1;
					RenderStr(i);
				}
				// AND SAVE NEW FORMAT
				save3BufOnly();
			}
		}
	}

//��������� world_border
#ifdef WORLD_BORDER
	for(i=0;i<XS_Buf;i++) { 
		ClTrBuf[0][2047][i]=255; ClTrBuf[0][i][2047]=255;
	}
#endif

	if(loadClBufForRender(0,YS_Buf-1,num_track)==0)
		if(loadClBufJ(0,YS_Buf-1,num_track) ==0)
			loadClBuf(0,YS_Buf-1,num_track);

	loadM3DAll(GetTargetName(M3DFileName));

	loadCLSAll(GetTargetName(CLSFileName));
	RenderShadovM3DAll();
	Ch_points.load(GetTargetName(Ch_pointsFileName));
//	PList.load(GetTargetName("points.ins"));
//	fxWaveLoad(GetTargetName("fx.dat"));
	checkAndRecoverWater();
}

#else
{
	X=0;Y=YS_Buf;//������ ��� ������ !

	LevelH2O = atoi(GetINIstringV(GetTargetName(wTable[cWorld].fname),"Global Parameters","LevelH2O"));
	if(LevelH2O<0)LevelH2O =0;
	LevelH2O>>=VX_FRACTION; 

	alloc4Buf();

	loadTrPal();
	//int i,j;
	//const char id[4]={'M','M','W','0'};
	const char id[4]={'M','M','W','1'};
	fmap.seek(0,XS_BEG);
	fmap.read(&VmpHeader,sizeof(VmpHeader));

	if (*(int*)VmpHeader.id == *(int*)id ){
		//Standart load
		fmap.read(&VxBuf[0][0][0],(XS_Buf>>kmBuf)*(YS_Buf>>kmBuf));
		//fmap.read(&AtBuf[0][0][0],XS_Buf*YS_Buf);
		fmap.read(&AtBuf[0][0][0],(XS_Buf>>kmBuf)*(YS_Buf>>kmBuf));
	}
	else ErrH.Abort("������ ������ ����");

//#if defined (HI_COLOR_WORLD)
	if(loadClBufJ(0,YS_Buf-1,num_track) ==0) loadClBuf(0,YS_Buf-1,num_track);
//#endif

	unsigned int track_mask=(1<<num_track) | (STATIC_NUMBER);
	loadM3DAll(GetTargetName(M3DFileName),track_mask);

	loadCLSAll(GetTargetName(CLSFileName));
	Ch_points.load(GetTargetName(Ch_pointsFileName));
}
#endif



int vrtMap::loadClBufForRender(int X,int Y,int num_track)
{
	X=0;Y=YS_Buf;//������ ��� ������ !

	if(num_track==-1)num_track=0;
	int i,j;
	XBuffer buft;
	char tmpstr[10];
	buft < "i_track" < port_itoa(num_track%10,tmpstr,10) < ".tga";
	XStream ss(0);
#ifdef _MECHOSOMA_
	if(!vmapOpenResource(GetTargetName(buft),ss)) return 0;
#else
	if(!ss.open(GetTargetName(buft), XS_IN)) return 0;
#endif
	TGAHEAD thead;
	ss.read(&thead,sizeof(thead));
	unsigned char* line = new unsigned char[H_SIZE*3],*p;
	int ibeg,jbeg,iend,jend,ik,jk;
	if(thead.ImageDescriptor&0x20) { jbeg=0; jend=V_SIZE; jk=1;}
	else { jbeg=V_SIZE-1; jend=-1; jk=-1;}
	if((thead.ImageDescriptor&0x10)==0) { ibeg=0; iend=H_SIZE; ik=1;}
	else { ibeg=H_SIZE-1; iend=-1; ik=-1;}
	for(j=jbeg; j!=jend; j+=jk){
		p = line;
		ss.read(line,H_SIZE*3);
		for(i = ibeg; i!=iend; i+=ik){
			short col;
			col =(*p++ >>3)&0x1F;
			col+=(*p++ <<3)&0x7E0;
			col+=(*p++ <<8)&0x0F800;
			ClBuf[0][j][i]=col;
		}
	}
	ss.close();
	delete line;
	return 1;
}


int vrtMap::loadClBuf(int X,int Y,int num_track)
{
	X=0;Y=YS_Buf;//������ ��� ������ !

	if(num_track==-1)num_track=0;
	int i,j;
	XBuffer buft;
	char tmpstr[10];
	buft < "track" < port_itoa(num_track%10,tmpstr,10) < ".tga";
	XStream ss(0);
#ifdef _MECHOSOMA_
	if(!vmapOpenResource(GetTargetName(buft), ss)){
		vmapOpenResource(GetTargetName("output.tga"), ss);
	}
#else
	if(!ss.open(GetTargetName(buft), XS_IN)){
		ss.open(GetTargetName("output.tga"), XS_IN);
	}
#endif
	TGAHEAD thead;
	ss.read(&thead,sizeof(thead));
	unsigned char* line = new unsigned char[H_SIZE*3],*p;
	int ibeg,jbeg,iend,jend,ik,jk;
	if(thead.ImageDescriptor&0x20) { jbeg=0; jend=V_SIZE; jk=1;}
	else { jbeg=V_SIZE-1; jend=-1; jk=-1;}
	if((thead.ImageDescriptor&0x10)==0) { ibeg=0; iend=H_SIZE; ik=1;}
	else { ibeg=H_SIZE-1; iend=-1; ik=-1;}
	for(j=jbeg; j!=jend; j+=jk){
		p = line;
		ss.read(line,H_SIZE*3);
		for(i = ibeg; i!=iend; i+=ik){
			short col;
			col =(*p++ >>3)&0x1F;
			col+=(*p++ <<3)&0x7E0;
			col+=(*p++ <<8)&0x0F800;
			ClBuf[0][j][i]=col;
		}
	}
	ss.close();
	delete line;
	return 1;

/*  //�������� � ���������� �� ����� ������.
	int i,j;
	XBuffer buft;
	char tmpstr[10];
	buft < "track" < itoa(num_track%10,tmpstr,10) < ".tga";
	XStream ssT(0);
	if( (ssT.open(GetTargetName(buft), XS_IN)) && (num_track!=-1) ){

		TGAHEAD thead,theadT;
		XStream ss(GetTargetName("output.tga"), XS_IN);
		ss.read(&thead,sizeof(thead));
		ssT.read(&theadT,sizeof(theadT));
		unsigned char* line = new unsigned char[H_SIZE*3],*p;
		unsigned char* lineT = new unsigned char[H_SIZE*4],*pT;
		int ibeg,jbeg,iend,jend,ik,jk;
		if(thead.ImageDescriptor&0x20) { jbeg=0; jend=V_SIZE; jk=1;}
		else { jbeg=V_SIZE-1; jend=-1; jk=-1;}
		if((thead.ImageDescriptor&0x10)==0) { ibeg=0; iend=H_SIZE; ik=1;}
		else { ibeg=H_SIZE-1; iend=-1; ik=-1;}
		for(j=jbeg; j!=jend; j+=jk){
			p = line;
			pT = lineT;
			ss.read(line,H_SIZE*3);
			ssT.read(lineT,H_SIZE*4);
			for(i = ibeg; i!=iend; i+=ik){
				short col;
				int r,g,b,a;
				b=*pT++; g=*pT++; r=*pT++; a=*pT++;
				b=(b*a) + ((255-a)*(*p++)) >>8;
				g=(g*a) + ((255-a)*(*p++)) >>8;
				r=(r*a) + ((255-a)*(*p++)) >>8;
				col =(b >>3)&0x1F;
				col+=(g <<3)&0x7E0;
				col+=(r <<8)&0x0F800;
				ClBuf[0][j][i]=col;
			}
		}
		ss.close();
		delete line;
		ssT.close();
		delete lineT;
	}
	else {
		TGAHEAD thead;
		XStream ss(GetTargetName("output.tga"), XS_IN);
		ss.read(&thead,sizeof(thead));
		unsigned char* line = new unsigned char[H_SIZE*3],*p;
		int ibeg,jbeg,iend,jend,ik,jk;
		if(thead.ImageDescriptor&0x20) { jbeg=0; jend=V_SIZE; jk=1;}
		else { jbeg=V_SIZE-1; jend=-1; jk=-1;}
		if((thead.ImageDescriptor&0x10)==0) { ibeg=0; iend=H_SIZE; ik=1;}
		else { ibeg=H_SIZE-1; iend=-1; ik=-1;}
		for(j=jbeg; j!=jend; j+=jk){
			p = line;
			ss.read(line,H_SIZE*3);
			for(i = ibeg; i!=iend; i+=ik){
				short col;
				col =(*p++ >>3)&0x1F;
				col+=(*p++ <<3)&0x7E0;
				col+=(*p++ <<8)&0x0F800;
				ClBuf[0][j][i]=col;
			}
		}
		ss.close();
		delete line;
	}*/
}



extern JSAMPLE * image_buffer;	
extern int image_height;	
extern int image_width;		

struct my_error_mgr {
  struct jpeg_error_mgr pub;	

  jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  (*cinfo->err->output_message) (cinfo);

  longjmp(myerr->setjmp_buffer, 1);
}


int vrtMap::loadClBufJ(int X,int Y,int num_track)
{
	X=0;Y=YS_Buf;//������ ��� ������ !

	if(num_track==-1)num_track=0;
	int i,j=0;
	XBuffer buft;
	char tmpstr[10];
	buft < "track" < port_itoa(num_track%10,tmpstr,10) < ".jpg";


	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	JSAMPARRAY buffer;		
	int row_stride;		

/*	if ((infile = fopen(GetTargetName(buft), "rb")) == NULL) {
		return 0;
	}
*/
	XStream fileExsistin(0);
#ifdef _MECHOSOMA_
	if(!vmapOpenResource(GetTargetName(buft), fileExsistin) ) {
#else
	if(!fileExsistin.open(GetTargetName(buft), XS_IN) ) {
#endif
		fileExsistin.close();
		return 0;
	}

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		fileExsistin.close();
		return 0;
	}
	jpeg_create_decompress(&cinfo);

	//jpeg_stdio_src(&cinfo, infile);
	extern void my_jpeg_stdio_src (j_decompress_ptr cinfo, XStream * Xfile);
	my_jpeg_stdio_src(&cinfo, &fileExsistin);

	(void) jpeg_read_header(&cinfo, TRUE);

	(void) jpeg_start_decompress(&cinfo);

	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	while (cinfo.output_scanline < cinfo.output_height) {
		(void) jpeg_read_scanlines(&cinfo, buffer, 1);
		unsigned char* p=buffer[0];
		for (i=0; i< cinfo.output_width; i++){
			short col;
			col=(*p++ <<8)&0x0F800;
			col+=(*p++ <<3)&0x7E0;
			col+=(*p++ >>3)&0x1F;
			ClBuf[0][j][i]=col;
		}
		j++;
	}

	(void) jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);

	fileExsistin.close();
	//fclose(infile);
	return 1;
}


void vrtMap::save3BufOnly(void)
{
	const char id[4]={'S','M','W','2'};
	//int i;
	fmap.seek(0,XS_BEG);
	*(int*)VmpHeader.id = *(int*)id;
	fmap.write(&VmpHeader,sizeof(VmpHeader));
	fmap.write(&VxBuf[0][0][0],XS_Buf*YS_Buf*2);
	fmap.write(&AtBuf[0][0][0],XS_Buf*YS_Buf);
	/*for(i=0;i<YS_Buf;i++){
		if(changedT[i]){
			fmap.seek(sizeof(VmpHeader)+i*XS_Buf,XS_BEG);
			fmap.write(&VxBuf[0][i][0],XS_Buf);
		}
	}
	for(i=0;i<YS_Buf;i++){
		if(changedT[i]){
			fmap.seek(sizeof(VmpHeader)+i*XS_Buf+ XS_Buf*YS_Buf,XS_BEG);
			fmap.write(&AtBuf[0][i][0],XS_Buf);
		}
	}
	for(i=0;i<YS_Buf;i++){
		if(changedT[i]){
			changedT[i]=0;
			fmap.seek(sizeof(VmpHeader)+i*XS_Buf+XS_Buf*YS_Buf*2,XS_BEG);
			fmap.write(&SpecBuf[0][i][0],XS_Buf);
		}
	}*/
	saveTrPal();
}

void vrtMap::loadTrack(int number)
{
	Ch_points.load(GetTargetName(Ch_pointsFileName),number);
}

void vrtMap::lockMem(void)
{/*
	const int M2 = MAX_LINE*H2_SIZE;
	const int M = MAX_LINE*H_SIZE;

	unsigned char* p,t;
	int i,size;
	for(i = 0;i < 2;i++){
		p = dHeap;
		size = M2;
		while(size--){ t = *p; *p = 0; *p++ = t; }
		}
	for(i = 0;i < 2;i++){
		p = dHeap_c;
		size = M;
		while(size--){ t = *p; *p = 0; *p++ = t; }
		}*/
}

void vrtMap::another(int up,int down)
{
/*	up = YCYCL(up);
	down = YCYCL(down);

	delink(upLine,downLine);
	if(isCompressed) linkC(up,down,1);
	else link(up,down,1);

	upLine = up;
	downLine = down;
	preViewY = ViewY;*/
}

void vrtMap::change(int up,int down)
{
	int du,dd,req;
	up = YCYCL(up);
	down = YCYCL(down);

#ifdef _SURMAP_
	lastUpRq = up;
	lastDownRq = down;

	if(getDistY(up,downLine) > 0 || getDistY(upLine,down) > 0){
		another(up,down);
		return;
		}
#endif
	du = getDistY(upLine,up);
	dd = getDistY(downLine,down);
//	req = MAX(du,-dd);
	req = ((du) > (-dd))?(du):(-dd);
	if(req > 0 && freeMax <= req + 1){
		if(du < 0){
//			delink(upLine,upLine + req - 1);
			upLine = YCYCL(upLine + req);
			}
		if(dd > 0){
//			delink(downLine - req + 1,downLine);
			downLine = YCYCL(downLine - req);
			}
		}
	if(du > 0){
//		if(isCompressed) linkC(up,upLine - 1,1);
//		else link(up,upLine - 1,1);
		upLine = up;
		}
	if(dd < 0){
//		if(isCompressed) linkC(downLine + 1,down,1);
//		else link(downLine + 1,down,1);
		downLine = down;
		}
}

void vrtMap::updownSetup(void)
{
/*	unsigned char** lt = isCompressed ? lineTcolor : lineT;
	register int i = ViewY;
	while(lt[i]) i = YCYCL(i - 1);
	upLine = YCYCL(i + 1);
	i = ViewY;
	while(lt[i]) i = YCYCL(i + 1);
	downLine = YCYCL(i - 1);*/
}

//void vrtMap::request(int up,int down,int left, int right)
//{
//#ifdef _SURMAP_
//	if(V_SIZE_POWER > 10)
//#endif
//		change(up,down);
//}

void vrtMap::quant(void)
{
	const int D = 1;

	int center = YCYCL(upLine + getDelta(downLine,upLine)/2);
//	int dl = getDistY(center,ViewY);
	int dl = getDistY(center,0);

	int ytop = upLine;
	int ybottom = downLine;

	if(abs(dl) > D){
		dl /= 4;
		ytop -= dl;
		ybottom -= dl;
		}
	else {
		ytop -= D;
		ybottom += D;
		}

	ytop = YCYCL(ytop);
	ybottom = YCYCL(ybottom);

	while(getDelta(ybottom,ytop) >= MAX_LINE - 4){
		ytop = YCYCL(ytop + 1);
		ybottom = YCYCL(ybottom - 1);
		}

	change(ytop,ybottom);
}

/*inline unsigned char* vrtMap::use(void)
{
	unsigned char* p = dHeap + freeTail*H2_SIZE;
	freeTail = (freeNodes + freeTail) -> next - freeNodes;
	return p;
}*/

/*inline void vrtMap::unuse(int i)
{
	int m = (lineT[i] - dHeap)/H2_SIZE;
	(freeNodes + m) -> next = freeNodes + freeTail;
	freeTail = m;
}

inline unsigned char* vrtMap::use_c(void)
{
	unsigned char* p = dHeap_c + freeTail_c*H_SIZE;
	freeTail_c = (freeNodes_c + freeTail_c) -> next - freeNodes_c;
	return p;
}

inline void vrtMap::unuse_c(int i)
{
	int m = (lineTcolor[i] - dHeap_c)/H_SIZE;
	(freeNodes_c + m) -> next = freeNodes_c + freeTail_c;
	freeTail_c = m;
}*/

/*void vrtMap::link(int up,int down,int d)
{
	if(V_SIZE_POWER <= 11 && !RAM16) return;

	up = YCYCL(up);
	down = YCYCL(down);

	unsigned char* p;
	int max = YCYCL(down + d),off;
	register int i = up;
	do {
		if(!lineTcolor[i]){
			if(freeMax <= 1){
				return;
//				XBuffer buf;
//				buf < "up: " <= upLine < " down: " <= downLine < " size: " <= getDelta(downLine,upLine);
//				ErrH.Abort("Line Buffer overflow",XERR_USER,-1,(char*)buf);
				}
			freeMax--;
			lineTcolor[i] = (p = use());
#ifdef SESSION
//			if(!LayerRead(i,&p))
				if(!DirectLog && sssT[i] != -1){
					fsss.seek(sssT[i],XS_BEG);
					fsss.read(p,H2_SIZE);
					lineT[i] = p;
					lineTcolor[i] = use_c();
					LINE_render(i);
					}
				else {
#endif
					off = i*H2_SIZE;
					if(KeepON && keepT[i]){
						kmap.seek(off,XS_BEG);
						kmap.read(p,H2_SIZE);
						}
					else {
						if(off != offset){
							offset = off + H2_SIZE;
							fmap.seek(foffset + off,XS_BEG);
							}
						else
							offset += H2_SIZE;
						fmap.read(p,H2_SIZE);
						}
					lineT[i] = p;
					lineTcolor[i] = use_c();
					LINE_render(i);
#ifdef SESSION
					}
#endif
			}
		i = YCYCL(i + d);
	} while(i != max);
}
*/
/*
void vrtMap::linkC(int up,int down,int d)
{
	if(V_SIZE_POWER <= 11 && !RAM16) return;
	up = YCYCL(up);
	down = YCYCL(down);

	unsigned char* p;
	int max = YCYCL(down + d);
	register int i = up;
	do {
		if(!lineTcolor[i]){
			if(freeMax <= 1){
				return;
//				XBuffer buf;
//				buf < "up: " <= upLine < " down: " <= downLine < " size: " <= getDelta(downLine,upLine);
//				ErrH.Abort("Line Buffer overflow",XERR_USER,-1,(char*)buf);
				}
			freeMax--;
#ifdef FILEMAPPING
			lineT[i] = p = use();
			lineTcolor[i] = use_c();
			ExpandBuffer(mappingPtr + st_table[i],p);
			LINE_render(i);
#else
			p = use();
			if(KeepON && keepT[i]){
				kmap.seek(i*H2_SIZE,XS_BEG);
				kmap.read(p,H2_SIZE);
				}
			else {
				fmap.seek(st_table[i],XS_BEG);
				fmap.read(inbuf,sz_table[i]);
				ExpandBuffer(inbuf,p);
				}
			lineT[i] = p;
			lineTcolor[i] = use_c();
			LINE_render(i);
#endif
			}
		i = YCYCL(i + d);
	} while(i != max);
}
*/
/*void vrtMap::delink(int up,int down)
{
	static int keeped = 0;
	if(V_SIZE_POWER <= 11 && !RAM16) return;
	up = YCYCL(up);
	down = YCYCL(down);

	int max = YCYCL(down + 1);
	register int i = up;
	unsigned char* p;
	do {
		if(lineT[i]){
			freeMax++;
			p = lineT[i];
#ifdef _ROAD_
			if(KeepON)
#endif
			if(changedT[i]){
				changedT[i] = 0;
#ifdef _ROAD_
				keepT[i] = 1;
				if(p){
					kmap.seek(i*H2_SIZE,XS_BEG);
					kmap.write(p,H2_SIZE);
					if(++keeped > 1024){
						keeped = 0;
						kmap.seek(V_SIZE*H2_SIZE,XS_BEG);
						kmap.write(keepT,V_SIZE);
						}
					}
#else
//				if(!LayerWrite(i,lineT[i]))
					if(!DirectLog){
						sssInuse = 1;
						if(sssT[i] == -1){
							fsss.seek(0,XS_END);
							sssT[i] = fsss.tell();
							fsss.write(lineT[i],H2_SIZE);
							}
						else {
							fsss.seek(sssT[i],XS_BEG);
							fsss.write(lineT[i],H2_SIZE);
							}
						}
					else {
						int m = i*H2_SIZE;
						if(m != offset){
							offset = m + H2_SIZE;
							fmap.seek(foffset + m,XS_BEG);
							}
						else
							offset += H2_SIZE;
						fmap.write(lineT[i],H2_SIZE);
						}
#endif
				}
			unuse_c(i);
			unuse(i);
			lineT[i] = NULL;
			lineTcolor[i] = NULL;
			}
		i = YCYCL(i + 1);
	} while(i != max);
}
*/
#ifdef _SURMAP_
/*void vrtMap::refresh(void)
{
	if(isCompressed) return;

	int off;
	int max = YCYCL(downLine + 1);
	register int i = upLine;
	if(V_SIZE_POWER <= 11 ) i = max = 0;
	do {
		if(lineT[i]){
#ifdef SESSION
//			if(!LayerRead(i,&lineT[i]))
				if(!DirectLog && sssT[i] != -1){
					fsss.seek(sssT[i],XS_BEG);
					fsss.read(lineT[i],H2_SIZE);
					}
				else {
#endif
					off = i*H2_SIZE;
					if(off != offset){
						offset = off + H2_SIZE;
						fmap.seek(foffset + off,XS_BEG);
						}
					else
						offset += H2_SIZE;
					fmap.read(lineT[i],H2_SIZE);
#ifdef SESSION
					}
#endif
			LINE_render(i);
			}
		i = YCYCL(i + 1);
	} while(i != max);
}
*/
void vrtMap::flush(void)
{
/*
	int max = YCYCL(downLine + 1);
	int m;
	register int i = upLine;
	if(V_SIZE_POWER <= 11 && !RAM16) i = max = 0;
	do {
		if(lineT[i] && changedT[i]){
			changedT[i] = 0;
			if(!LayerWrite(i,lineT[i]))
				if(!DirectLog){
					sssInuse = 1;
					if(sssT[i] == -1){
						fsss.seek(0,XS_END);
						sssT[i] = fsss.tell();
						fsss.write(lineT[i],H2_SIZE);
						}
					else {
						fsss.seek(sssT[i],XS_BEG);
						fsss.write(lineT[i],H2_SIZE);
						}
					}
				else {
					m = i*H2_SIZE;
					if(m != offset){
						offset = m + H2_SIZE;
						fmap.seek(foffset + m,XS_BEG);
						}
					else
						offset += H2_SIZE;
					fmap.write(lineT[i],H2_SIZE);
					}
			}
		i = YCYCL(i + 1);
	} while(i != max);
*/
}

void vrtMap::save3Buf(void)
{
	const char id[4]={'S','M','W','2'};
	//int i,j;
	fmap.seek(0,XS_BEG);
	*(int*)VmpHeader.id = *(int*)id;
	fmap.write(&VmpHeader,sizeof(VmpHeader));
	fmap.write(&VxBuf[0][0][0],XS_Buf*YS_Buf*2);
	fmap.write(&AtBuf[0][0][0],XS_Buf*YS_Buf);

/*	const char id[4]={'S','M','W','1'};
	int i,j;
	fmap.seek(0,XS_BEG);
	*(int*)VmpHeader.id = *(int*)id;
	fmap.write(&VmpHeader,sizeof(VmpHeader));
	for(i=0;i<YS_Buf;i++){
		if(changedT[i]){
			fmap.seek(sizeof(VmpHeader)+i*XS_Buf,XS_BEG);
			fmap.write(&VxBuf[0][i][0],XS_Buf);
		}
	}
	for(i=0;i<YS_Buf;i++){
		if(changedT[i]){
			fmap.seek(sizeof(VmpHeader)+i*XS_Buf+ XS_Buf*YS_Buf,XS_BEG);
			fmap.write(&AtBuf[0][i][0],XS_Buf);
		}
	}
	for(i=0;i<YS_Buf;i++){
		if(changedT[i]){
			changedT[i]=0;
			fmap.seek(sizeof(VmpHeader)+i*XS_Buf+XS_Buf*YS_Buf*2,XS_BEG);
			fmap.write(&SpecBuf[0][i][0],XS_Buf);
		}
	}*/
	if(LevelH2O<0)LevelH2O =0;
	char buf[16];
	SaveINIstringV(GetTargetName(wTable[cWorld].fname),"Global Parameters","LevelH2O",itoa(LevelH2O,buf,10));
	//saveVMM();
	saveM3DAll(GetTargetName(M3DFileName));
	saveCLSAll(GetTargetName(CLSFileName));
	Ch_points.save(GetTargetName(Ch_pointsFileName));
#ifdef _SURMAP_
//	PList.save(GetTargetName("points.ins"));
//	fxWaveSave(GetTargetName("fx.dat"));
#endif
	saveTrPal();
}
void vrtMap::saveVMM(void)
	//������ VMM
{
	int j,i;
	XStream fo;
	fo.open(GetTargetName("output.vmm"), XS_OUT);
	// ������ ���������
	const char idm[4]={'M','M','W','1'};
	fmap.seek(0,XS_BEG);
	*(int*)VmpHeader.id = *(int*)idm;
	VmpHeader.XS=1024;VmpHeader.YS=1024;VmpHeader.NX=0;VmpHeader.NY=0;
	fo.write(&VmpHeader,sizeof(VmpHeader));
	//�.�. ��� SUrmap, kmVxBu=0 ����� 0, ������� �������� �� ������������� 1
	unsigned char VB[XS_Buf>>1];  
	for (j=0; j<(YS_Buf>>1); j++){
		for(i=0; i<(XS_Buf>>1); i++){
			VB[i]= vMap->VxBuf[0][j<<1][i<<1]>>8;//>>SHIFT_FRACTION_VX;// + vMap->VxBuf[0][j<<1][(i<<1)+1] + vMap->VxBuf[0][(j<<1)+1][i<<1] + vMap->VxBuf[0][(j<<1)+1][(i<<1)+1] >>2;
		}
		fo.write(VB,XS_Buf>>1);
	}
	for(j=0; j<(YS_Buf>>1); j++){
		for(i=0; i<(XS_Buf>>1); i++){
			VB[i]= vMap->AtBuf[0][j<<1][i<<1];
		}
		fo.write(VB,XS_Buf>>1);
	}
	fo.close();
}


void vrtMap::restore3Buf(void)
{
	loadTrPal();

	const char id[4]={'S','M','W','2'};
	fmap.seek(0,XS_BEG);
	fmap.read(&VmpHeader,sizeof(VmpHeader));
	if (*(int*)VmpHeader.id == *(int*)id ){
		int i;
		for(i=0;i<YS_Buf;i++){
			if(changedT[i]){
				fmap.seek(sizeof(VmpHeader)+i*XS_Buf*2,XS_BEG);
				fmap.read(&VxBuf[0][i][0],XS_Buf*2);
			}
		}
		for(i=0;i<YS_Buf;i++){
			if(changedT[i]){
				fmap.seek(sizeof(VmpHeader)+i*XS_Buf+ XS_Buf*YS_Buf*2,XS_BEG);
				fmap.read(&AtBuf[0][i][0],XS_Buf);
			}
		}
//		for(i=0;i<YS_Buf;i++){
//			if(changedT[i]){
//				fmap.seek(sizeof(VmpHeader)+i*XS_Buf+XS_Buf*YS_Buf*2,XS_BEG);
//				fmap.read(&SpecBuf[0][i][0],XS_Buf);
//			}
//		}
		// WORLD RENDER 
		for(i=0;i<YS_Buf;i++){
			changedT[i]=0;
			RenderStr(i);
		}
	}
	else ErrH.Abort("Invalid VMP file ");
	delM3DAll();
	delCLSAll();
	loadM3DAll(GetTargetName(M3DFileName));
	loadCLSAll(GetTargetName(CLSFileName));
	RenderShadovM3DAll();
	Ch_points.load(GetTargetName(Ch_pointsFileName));
#ifdef _SURMAP_
//	PList.load(GetTargetName("points.ins"));
///For FX
	ShowFog=0;
//	fxWaveFinit();
//	fxWaveInit();
//	fxWaveLoad(GetTargetName("fx.dat"));
	ShowFog=1;
#endif

}

void vrtMap::screenRender(void)
{
	int max = YCYCL(downLine + 1);
	int i = upLine;
	if(V_SIZE_POWER <= 11 ) i = max = 0;
	do {
		changedT[i] = 1;
		i = YCYCL(i + 1);
	} while(i != max);

	if(V_SIZE_POWER <= 11)
		WORLD_colcalc(0,V_SIZE - 1);
	else
		WORLD_colcalc(YCYCL(upLine + 1),downLine);
}

/*void vrtMap::readLine(int n,unsigned char* p)
{
	int off = n*H2_SIZE;
	if(off != offset){
		offset = off + H2_SIZE;
		fmap.seek(foffset + off,XS_BEG);
		}
	else
		offset += H2_SIZE;
	fmap.read(p,H2_SIZE);
}*/
#endif

#ifndef _SURMAP_
void LoadVPR(int ind)
{
/*	XStream ff(vMap -> pname[ind],XS_IN);
	if(ff.size() != (int)(2*4 + (1 + 4 + 4)*4 + 2*net_size + 2*POWER*4 + 2*PART_MAX*POWER*4 )) ErrH.Abort("Incorrect VPR size");//+ PART_MAX*4

	ff.seek(2*4 + 1*4,XS_BEG);

	ff.seek(2*4*4,XS_CUR);

	ff.seek(2*net_size + 2*POWER*sizeof(int) + 2*PART_MAX*POWER*sizeof(int),XS_CUR);
//	ff.read(FloodLvl,PART_MAX*sizeof(unsigned));

	ff.close();*/
}
#endif


#ifdef _SURMAP_
void vrtMap::scaling16(int XSrcSize,int cx,int cy,int xc,int yc,int xside,int yside)
{
	short* vp = (short*)XGR_VIDEOBUF + (yc - yside)*XGR_MAXX + (xc - xside);
	unsigned short* vz = zBuffer + (yc - yside)*XGR_MAXX + (xc - xside);
	int xsize = 2*xside;
	int ysize = 2*yside;
	int XADD = XGR_MAXX - xsize;

	cx = XCYCL(cx);
	cy = YCYCL(cy);

	int YSrcSize = ysize*XSrcSize/xsize;

	int k_xscr_x = (XSrcSize << 16)/xsize;
	int k_yscr_y = (YSrcSize << 16)/ysize;

	int tfx = (cx << 16) - (XSrcSize << 15) + (1 << 15);
	int x0 = tfx >> 16;
	int x1 = x0 + XSrcSize;
	int tfy = (cy << 16) - (YSrcSize << 15) + (1 << 15);
	int y0 = tfy >> 16;
	int y1 = y0 + YSrcSize;

//	request(MIN(y0,y1) - MAX_RADIUS/2,MAX(y0,y1) + MAX_RADIUS/2,MIN(x0,x1) - 4,MAX(x0,x1) + 4);

	register int i,j,fx,fy;
	unsigned char* lc = &(vMap -> ClTrBuf[0][0][0]);//
	unsigned char* la = &(vMap -> AtBuf[0][0][0]);
	unsigned short* lv = &(vMap -> VxBuf[0][0][0]);
//	unsigned char* ls = &(vMap -> SpecBuf[0][0][0]);
	unsigned char* data;
	unsigned char* data1;
	unsigned short* data2;
//	unsigned char* data3;


	for(i = 0;i < ysize;i++){
				fx = tfx;
				fy = tfy;
				data = lc + YCYCL(fy >> 16)*XS_Buf;
				data1= la + YCYCL(fy >> 16)*XS_Buf;
				data2= lv + YCYCL(fy >> 16)*XS_Buf;
				//data3= ls + YCYCL(fy >> 16)*XS_Buf;
/*#ifdef MMX
				if((k_xscr_x != 1<<16) ||(xt_mmxUse==0)){
#endif*/
					for(j = 0;j < xsize;j++,vp++,vz++){
						*vp = TrPal[(*(data1 + XCYCL(fx >> 16)) & TrW_MASK)][*(data + XCYCL(fx >> 16))];
						*vz = hCamera-(*(data2 + XCYCL(fx >> 16))>>SHIFT_FRACTION_VX);
						fx += k_xscr_x;
					}
				tfy += k_yscr_y;
				vp += XADD;
				vz += XADD;

/*#ifdef MMX
				}
				else {
					void * PAL=&TrPal[0][0];
					//data+=XCYCL(fx>>16);
					//data1+=XCYCL(fx>>16);
					//data2+=XCYCL(fx>>16);
					//data3+=XCYCL(fx>>16);
					int ofx=XCYCL(fx>>16);
					__asm{
//						push eax
//						push ebx
///						push ecx
//						push edx
//						push esi
//						push edi

						mov ecx,xsize
						shr ecx,2
						mov eax,hCamera //zBuf
						mov ebx,eax //zBuf
						shl eax,16d //zBuf
						mov ax,bx //zBuf
						movd MM5,eax //zBuf
						nop
						movd MM1,eax //zBuf
						psllq MM5,32d //zBuf
						por MM5,MM1 //zBuf
						mov edx,vz //zBuf

						mov esi,data2 //Vx(BASE Buf)
						mov eax,01f1f1f1fh //Tr & Water MASK
						movd MM7,eax
						xor eax,eax
						movd MM6,eax
						mov edi,vp
						mov ebx,PAL
						movd MM2,vz //zBuf
						mov eax,ofx
						push ebp
						mov ebp,eax
//
//
//
// Pred LOOP
						test ebp,3h
						je l_loop1t
						dec ecx //��������� �� 1 �������� loop �.�. Pred&End LOOP ���������� 4 pix.
						mov eax,ebp
						and eax,3h
						push eax // ���������� ���. �������� End LOOP
						neg eax
						and eax,3h
						push ecx // Save loop LOOP
						mov ecx,eax
p_loop1:
						bswap ecx
						xor eax,eax
						mov ah,[esi+offsetAt+ebp]
						mov al,[esi+offsetClTr+ebp]
						and ah,01fh //Tr & Water MASK
						shl eax,1
						mov cx,[ebx+eax]
						mov [edi],cx
						xor cx,cx
						test ah,020h // ��� (At_WATER << 1)
						jne p_loop2 //���� ����- �� �������� 0
						mov cl,[esi+offsetVx+ebp]
p_loop2:
						movd eax,MM5
						sub ax,cx
						mov [edx],ax
						xor cx,cx
						bswap ecx
						inc ebp
						and ebp,XS_Buf-1
						add edi,2
						add edx,2
						dec ecx
						cmp ecx,0
						jne p_loop1
						pop ecx //Restore loop LOOP
//End PRED LOOP						
						//add ebp,eax
						//shl eax,1
						//add edi,eax
						//add edx,eax
						//and ebp,XS_Buf-1
						jmp l_loop1
l_loop1t:
						push 0h
l_loop1:
						movd MM1,[esi+offsetAt+ebp]
						movd MM0,[esi+offsetClTr+ebp]
						pand MM1,MM7 //MASKA Tr & Water

						movd MM2,[esi+offsetVx+ebp] //zBuf

						punpcklbw MM1,MM6 //MM6 - 0
						punpcklbw MM0,MM6
						punpcklbw MM2,MM6 //zBuf
						psllw MM0,1
						psllw MM1,8+1
						paddw MM0,MM1
						movq MM3,MM0
						punpcklwd MM0,MM6
						movd eax,MM0
						bswap ecx
						mov cx,[eax+ebx]
						psrlq MM0,32d
						movd eax,MM0
						mov ax,[eax+ebx]
						shl eax,16d
						mov ax,cx
						mov [edi],eax
						add edi,4
						//movd MM4,edx
						psrlw MM1,8+1+4 //zBuf
						pcmpeqw MM1,MM6 //zBuf
						pand MM2,MM1 //zBuf
						movq MM1,MM5
						psubw MM1,MM2
						movq [edx],MM1 //zBuf
						add edx,8
//3&4 pix
						punpckhwd MM3,MM6
						movd eax,MM3
						mov cx,[eax+ebx]
						psrlq MM3,32d
						movd eax,MM3
						mov ax,[eax+ebx]
						shl eax,16d
						mov ax,cx
						mov cx,0
						bswap ecx
						mov [edi],eax //Base command
						add ebp,4
						add edi,4
						//TEST movq [edi-8],MM1
						///movd eax,MM4
						///mov [edi],eax
						///mov [edi+4],edx
						///add edi,8
						//movd MM0,edx
						//psllq MM0,32d
						//por MM0,MM4
						//movq [edi],MM0
						//add edi,8
						//
						and ebp,XS_Buf-1
						dec ecx
						cmp ecx,0
						jne l_loop1 

//End LOOP Begin
						pop ecx
						cmp ecx,0
						je l_end
e_loop1:
						bswap ecx
						xor eax,eax
						mov ah,[esi+offsetAt+ebp]
						mov al,[esi+offsetClTr+ebp]
						and ah,01fh //Tr & Water MASK
						shl eax,1
						mov cx,[ebx+eax]
						mov [edi],cx
						xor cx,cx
						test ah,020h // ��� (At_WATER << 1)
						jne e_loop2 //���� ����- �� �������� 0
						mov cl,[esi+offsetVx+ebp]
e_loop2:
						movd eax,MM5
						sub ax,cx
						mov [edx],ax
						xor cx,cx
						bswap ecx
						inc ebp
						and ebp,XS_Buf-1
						add edi,2
						add edx,2
						dec ecx
						cmp ecx,0
						jne e_loop1
//End END LOOP						
l_end:
						
						pop ebp

//						pop edi
//						pop esi
//						pop edx
//						pop ecx
//						pop ebx
//						pop eax

						EMMS
					}
				tfy += k_yscr_y;
				vp+=XGR_MAXX;
				//vp += XADD;
				//vz += XADD;
				vz+=XGR_MAXX;
				}
#endif*/
			
		}
}

float aa=0;//-3.141592654f/2;
void vrtMap::viewV(int XSrcSize,int cx,int cy,int xc,int yc,int xside,int yside)
{
	//short* vp = (short*)XGR_VIDEOBUF + (yc - yside)*XGR_MAXX + (xc - xside);
	short* vp = (short*)XGR_VIDEOBUF;
	unsigned short* vz = zBuffer + (yc - yside)*XGR_MAXX + (xc - xside);
	int xsize = 2*xside;
	int ysize = 2*yside;
	int XADD = XGR_MAXX - xsize;


//	void View(int x0,int y0,float aa)

	int lasty[640],         // Last pixel drawn on a given column
		lastc[640];         // Color of last pixel on a column
	int d;
	int a,b,hCamera,u0,v0,u1,v1,h0,h1,h2,h3;
	float FOV=3.141592654f/4;   // half of the xy field of view

	//??memset(Video,0,320*200);

	// Initialize last-y and last-color arrays
	for ( d=0; d<XGR_MAXX; d++ ){
		lasty[d]=XGR_MAXY;
		lastc[d]=-1;
	}
	int x0=XCYCL(cx)<<16;
	int y0=YCYCL(cy)<<16;

	// Compute the xy coordinates; a and b will be the position inside the
	// single map cell (0..255). //a,b ������� �����
	u0=(x0>>16)&clip_mask_x;			a=(x0>>8)&255;
	v0=((y0>>16)&clip_mask_y);			b=(y0>>8)&255;
	u1=(u0+1)&clip_mask_x;
	v1=((v0+1)&clip_mask_y);

	// Fetch the height at the four corners of the square the point is in
	unsigned short* lv = &(vMap -> VxBuf[0][0][0]);
	v1*=XS_Buf; v0*=XS_Buf;
	h0=lv[u0+v0]; h2=lv[u0+v1];
	h1=lv[u1+v0]; h3=lv[u1+v1];

	// Compute the height using bilinear interpolation
	h0=(h0<<8)+a*(h1-h0);
	h2=(h2<<8)+a*(h3-h2);
	hCamera=((h0<<8)+b*(h2-h0))>>16>>VX_FRACTION;
	hCamera+=30;//������ ������ ��� ������������

	// Draw the landscape from near to far without overdraw
	for ( d=0; d<1000; d+=1 ){//d+=1+(d>>6)
		//Line(x0+d*65536*cos(aa-FOV),y0+d*65536*sin(aa-FOV),
		//x0+d*65536*cos(aa+FOV),y0+d*65536*sin(aa+FOV),
		//h-30,100*256/(d+1));
		int xx0=x0+d*65536*cos(aa-FOV);
		int yy0=y0+d*65536*sin(aa-FOV);
		int xx1=x0+d*65536*cos(aa+FOV);
		int yy1=y0+d*65536*sin(aa+FOV);

		int scaling=1000*256/(d+1); //scaling=100*256/(d+1);



		//void Line(int x0,int y0,int x1,int y1,int hy,int s)
		int i,sx,sy;

		// Compute xy speed
		sx=(xx1-xx0)/XGR_MAXX; sy=(yy1-yy0)/XGR_MAXX;
		for ( i=0; i<XGR_MAXX; i++ ){
			int c,y,h;

			u0=(xx0>>16)&clip_mask_x;		a=(xx0>>8)&255;
			v0=((yy0>>16)&clip_mask_y);		b=(yy0>>8)&255;
			u1=(u0+1)&clip_mask_x;
			v1=((v0+1)&clip_mask_y);

			v1*=XS_Buf; v0*=XS_Buf;
			h0=lv[u0+v0]; h2=lv[u0+v1];
			h1=lv[u1+v0]; h3=lv[u1+v1];

			// Compute the height using bilinear interpolation
			h0=(h0<<8)+a*(h1-h0);
			h2=(h2<<8)+a*(h3-h2);
			h=((h0<<8)+b*(h2-h0))>>16>>VX_FRACTION;

			// Fetch the color at the four corners of the square the point is in
			unsigned short* lc = &(vMap -> ClBuf[0][0][0]);//
			h0=lc[u0+v0]; h2=lc[u0+v1];
			h1=lc[u1+v0]; h3=lc[u1+v1];

			// Compute the color using bilinear interpolation (in 16.16)
			//h0=(h0<<8)+a*(h1-h0);
			//h2=(h2<<8)+a*(h3-h2);
			//c=((h0<<8)+b*(h2-h0));
			c=h0<<16; //������������ ����� ���� ���������

			// Compute screen height using the scaling factor
			y=(((hCamera-h)*scaling)>>10)+240;//100

			// Draw the column
			if ( y<(a=lasty[i]) ){
				short *b=vp+a*XGR_MAXX+i;
				int sc,cc;
				if ( lastc[i]==-1 ) lastc[i]=c;
				//sc=(c-lastc[i])/(a-y); // ������� ����� ����� ������ �����
				sc=0;
				cc=c;//lastc[i];//������� ����� ����� ������ �����
				if ( a>(XGR_MAXY-1) ) {
					b-=(a-(XGR_MAXY-1))*XGR_MAXX; cc+=(a-(XGR_MAXY-1))*sc; a=(XGR_MAXY-1); 
				}
				if ( y<0 ) y=0;
				while ( y<a ) {
					*b=cc>>16;//>>18 
					cc+=sc; 
					b-=XGR_MAXX; a--;
				}
				lasty[i]=y;
			}
			lastc[i]=c;

			// Advance to next xy position
			xx0+=sx; yy0+=sy;
		}


	
	
	}
	for (int i=0; i<XGR_MAXX; i++ ){
		if ( (lasty[i])>0 ){
		short *b=vp+lasty[i]*XGR_MAXX+i;
			while ( (lasty[i])>0 ) {
				*b=0;
				b-=XGR_MAXX;
				lasty[i]--;
			}
		}
	}


}


int focusC=600;
int cameraDZ=100;
int AAA=140;
//float aa=-3.141592654f/2;
void vrtMap::viewV2D(int XSrcSize,int cx,int cy,int xc,int yc,int xside,int yside)
{
	//short* vp = (short*)XGR_VIDEOBUF + (yc - yside)*XGR_MAXX + (xc - xside);
	short* vp = (short*)XGR_VIDEOBUF;
	unsigned short* vz = zBuffer + (yc - yside)*XGR_MAXX + (xc - xside);
	int xsize = 2*xside;
	int ysize = 2*yside;
	int XADD = XGR_MAXX - xsize;


//	void View(int x0,int y0,float aa)

	int lasty[640],         // Last pixel drawn on a given column
		lastc[640];         // Color of last pixel on a column
	int d;
	int a,b,hCamera,u0,v0,u1,v1,h0,h1,h2,h3;
	float FOV=3.141592654f/4;   // half of the xy field of view

	//??memset(Video,0,320*200);

	// Initialize last-y and last-color arrays
	for ( d=0; d<XGR_MAXX; d++ ){
		lasty[d]=XGR_MAXY;
		lastc[d]=-1;
	}
	int x0=XCYCL(cx)<<16;
	int y0=YCYCL(cy)<<16;

	//hCamera=600;//������ ������ ��� ������������

	unsigned short* lv = &(vMap -> VxBuf[0][0][0]);

	float tga=tan(AAA*3.14/180);
	int dh=round(65536*(sin((AAA-90)*3.14/180)*focusC/(focusC+cameraDZ)) );
	int DY,DX;
	for (d=-yside-200; d<yside; d++){
		DY = round(sqrt( pow(d*(focusC+cameraDZ)/(d+tga*focusC),2) + pow(tga*d*(focusC+cameraDZ)/(d+tga*focusC),2) )*65536);
		if(d<0)DY=-DY;
		DX = round( 65536*xside*(focusC+( cameraDZ-(d*(focusC+cameraDZ)/(d+tga*focusC)) ) )/focusC );


	// Draw the landscape from near to far without overdraw
	//for ( d=0; d<1000; d+=1 ){//d+=1+(d>>6)
		//Line(x0+d*65536*cos(aa-FOV),y0+d*65536*sin(aa-FOV),
		//x0+d*65536*cos(aa+FOV),y0+d*65536*sin(aa+FOV),
		//h-30,100*256/(d+1));
		/*int xx0=(x0-DX)*cos(aa)-(y0-DY)*sin(aa);
		int yy0=(y0-DY)*cos(aa)+(x0-DX)*sin(aa);
		int xx1=(x0+DX)*cos(aa)-(y0-DY)*sin(aa);
		int yy1=(y0-DY)*cos(aa)+(x0+DX)*sin(aa);*/

		int xx0=x0+( (-DX)*cos(aa)-(-DY)*sin(aa) );
		int yy0=y0+( (-DY)*cos(aa)+(-DX)*sin(aa) );
		int xx1=x0+( (+DX)*cos(aa)-(-DY)*sin(aa) );
		int yy1=y0+( (-DY)*cos(aa)+(+DX)*sin(aa) );

		//int scaling=100;///(d+1); //scaling=100*256/(d+1);


		//void Line(int x0,int y0,int x1,int y1,int hy,int s)
		int i,sx,sy;

		// Compute xy speed
		sx=(xx1-xx0)/XGR_MAXX; sy=(yy1-yy0)/XGR_MAXX;
		for ( i=0; i<XGR_MAXX; i++ ){
			int c,y,h;

			u0=(xx0>>16)&clip_mask_x;		a=(xx0>>8)&255;
			v0=((yy0>>16)&clip_mask_y);		b=(yy0>>8)&255;
			u1=(u0+1)&clip_mask_x;
			v1=((v0+1)&clip_mask_y);

			v1*=XS_Buf; v0*=XS_Buf;
			h0=lv[u0+v0]; h2=lv[u0+v1];
			h1=lv[u1+v0]; h3=lv[u1+v1];

			// Compute the height using bilinear interpolation
			h0=(h0<<8)+a*(h1-h0);
			h2=(h2<<8)+a*(h3-h2);
			h=((h0<<8)+b*(h2-h0))>>16>>VX_FRACTION;

			// Fetch the color at the four corners of the square the point is in
/*			unsigned short* lc = &(vMap -> ClBuf[0][0][0]);//
			h0=lc[u0+v0]; h2=lc[u0+v1];
			h1=lc[u1+v0]; h3=lc[u1+v1];

			// Compute the color using bilinear interpolation (in 16.16)
			//h0=(h0<<8)+a*(h1-h0);
			//h2=(h2<<8)+a*(h3-h2);
			//c=((h0<<8)+b*(h2-h0));
			c=h0<<16; //������������ ����� ���� ���������*/
			unsigned char* lc1 = &(vMap -> ClTrBuf[0][0][0]);//
			unsigned char* lc2 = &(vMap -> AtBuf[0][0][0]);//
			//c=TrPal[lc2[u0+v0]& TrW_MASK][lc1[u0+v0]]<<16;
			h0=lc1[u0+v0];
			h1=lc1[u1+v0];
			h2=lc1[u0+v1];
			h3=lc1[u1+v1];

			h0=(h0<<8)+a*(h1-h0);
			h2=(h2<<8)+a*(h3-h2);
			c=((h0<<8)+b*(h2-h0));
			c=TrPal[lc2[u0+v0]& TrW_MASK][c>>16]<<16;

			// Compute screen height using the scaling factor
			y=XGR_MAXY/2 -d -(dh*h>>16);//100(((hCamera-h)*scaling)>>10)

			// Draw the column
			if ( y<(a=lasty[i]) ){
				short *b=vp+a*XGR_MAXX+i;
				int sc,cc;
				if ( lastc[i]==-1 ) lastc[i]=c;
				//sc=(c-lastc[i])/(a-y); // ������� ����� ����� ������ �����
				sc=0;
				cc=c;//lastc[i];//������� ����� ����� ������ �����
				if ( a>(XGR_MAXY-1) ) {
					b-=(a-(XGR_MAXY-1))*XGR_MAXX; cc+=(a-(XGR_MAXY-1))*sc; a=(XGR_MAXY-1); 
				}
				if ( y<0 ) y=0;
				while ( y<a ) {
					*b=cc>>16;//>>18 
					cc+=sc; 
					b-=XGR_MAXX; a--;
				}
				lasty[i]=y;
			}
			lastc[i]=c;

			// Advance to next xy position
			xx0+=sx; yy0+=sy;
		}


	
	
	}
	for (int i=0; i<XGR_MAXX; i++ ){
		if ( (lasty[i])>0 ){
		short *b=vp+lasty[i]*XGR_MAXX+i;
			while ( (lasty[i])>0 ) {
				*b=0;
				b-=XGR_MAXX;
				lasty[i]--;
			}
		}
	}


}



void vrtMap::scaling16HC(int XSrcSize,int cx,int cy,int xc,int yc,int xside,int yside)
{
	short* vp = (short*)XGR_VIDEOBUF + (yc - yside)*XGR_MAXX + (xc - xside);
	unsigned short* vz = zBuffer + (yc - yside)*XGR_MAXX + (xc - xside);
	int xsize = 2*xside;
	int ysize = 2*yside;
	int XADD = XGR_MAXX - xsize;

	cx = XCYCL(cx);
	cy = YCYCL(cy);

	int YSrcSize = ysize*XSrcSize/xsize;

	int k_xscr_x = (XSrcSize << 16)/xsize;
	int k_yscr_y = (YSrcSize << 16)/ysize;

	int tfx = (cx << 16) - (XSrcSize << 15) + (1 << 15);
	int x0 = tfx >> 16;
	int x1 = x0 + XSrcSize;
	int tfy = (cy << 16) - (YSrcSize << 15) + (1 << 15);
	int y0 = tfy >> 16;
	int y1 = y0 + YSrcSize;

//	request(MIN(y0,y1) - MAX_RADIUS/2,MAX(y0,y1) + MAX_RADIUS/2,MIN(x0,x1) - 4,MAX(x0,x1) + 4);

	register int i,j,fx,fy;
	unsigned short* lc = &(vMap -> ClBuf[0][0][0]);//
	unsigned char* la = &(vMap -> AtBuf[0][0][0]);
	unsigned short* lv = &(vMap -> VxBuf[0][0][0]);
//	unsigned char* ls = &(vMap -> SpecBuf[0][0][0]);
	unsigned short* data;
	unsigned char* data1;
	unsigned short* data2;
//	unsigned char* data3;


	for(i = 0;i < ysize;i++){
				fx = tfx;
				fy = tfy;
				data = lc + YCYCL(fy >> 16)*XS_Buf;
				data1= la + YCYCL(fy >> 16)*XS_Buf;
				data2= lv + YCYCL(fy >> 16)*XS_Buf;
//				data3= ls + YCYCL(fy >> 16)*XS_Buf;
					for(j = 0;j < xsize;j++,vp++,vz++){
						//*vp = TrPal[(*(data1 + XCYCL(fx >> 16)) & TrW_MASK)][*(data + XCYCL(fx >> 16))];
						//if( *(data1 + XCYCL(fx >> 16)) & At_SHADOW)	*vp = ( ((int)*(data + XCYCL(fx >> 16)))&0x0F7DF) >>1;
						*vp = *(data + XCYCL(fx >> 16));//else 
						//*vz = hCamera-*(data2 + XCYCL(fx >> 16));
						*vz = hCamera-(*(data2 + XCYCL(fx >> 16))>>SHIFT_FRACTION_VX);
						fx += k_xscr_x;
					}
				tfy += k_yscr_y;
				vp += XADD;
				vz += XADD;
			
		}
}


#else //Mechosoma version � ���������� ������� ���� ! � ����������� ���������� �������������
void vrtMap::scaling16(int XSrcSize,int cx,int cy,int xc,int yc,int xside,int yside)
{
//	xt_mmxUse=0;
	short* vp = (short*)XGR_VIDEOBUF + (yc - yside)*XGR_MAXX + (xc - xside);
	unsigned short* vz = zBuffer + (yc - yside)*XGR_MAXX + (xc - xside);
	int xsize = 2*xside;
	int ysize = 2*yside;
	int XADD = XGR_MAXX - xsize;

	cx = XCYCL(cx);
	cy = YCYCL(cy);

	int YSrcSize = ysize*XSrcSize/xsize;

	int k_xscr_x = (XSrcSize << 16)/xsize;
	int k_yscr_y = (YSrcSize << 16)/ysize;

	int tfx = (cx << 16) - (XSrcSize << 15) + (1 << 15);
	int x0 = tfx >> 16;
	int x1 = x0 + XSrcSize;
	int tfy = (cy << 16) - (YSrcSize << 15) + (1 << 15);
	int y0 = tfy >> 16;
	int y1 = y0 + YSrcSize;

//	request(MIN(y0,y1) - MAX_RADIUS/2,MAX(y0,y1) + MAX_RADIUS/2,MIN(x0,x1) - 4,MAX(x0,x1) + 4);

	int i,j,fx,fy;
	unsigned short* lc = &(vMap -> ClBuf[0][0][0]);//
	unsigned char* la = &(vMap -> AtBuf[0][0][0]);
	unsigned char* lv = &(vMap -> VxBuf[0][0][0]);
	//unsigned char* ls = &(vMap -> SpecBuf[0][0][0]);
	unsigned short* data;
	unsigned char* data1;
	unsigned char* data2;
//	unsigned char* data3;


	for(i = 0;i < ysize;i++){
				fx = tfx;
				fy = tfy;
				int yyy=YCYCL(fy >> 16);
				data = lc + yyy*XS_Buf;
				data1= la + yyy*XS_Buf;
				data2= lv + (yyy >> kmBuf)*(XS_Buf>>kmBuf);
				//data3= ls + YCYCL(fy >> 16)*XS_Buf;
//#ifdef MMX
//				if((k_xscr_x != 1<<16) ||(xt_mmxUse==0)){
//#endif
					for(j = 0;j < xsize;j++,vp++,vz++){
						int xxx=XCYCL(fx >> 16);
						*vp = *(data + xxx);
						*vz = hCamera-*(data2 + (xxx >> kmBuf));
						fx += k_xscr_x;
					}
				tfy += k_yscr_y;
				vp += XADD;
				vz += XADD;

//#ifdef MMX
//				}
//				else {
//					//��� ������ ���� ���������� ��� ��� MMX
//				}
//#endif
			
		}
}
#endif



// ������ ������ ��� �������� ( ��� ��� ���� ClTrBuf )
/*
void vrtMap::scaling16(int XSrcSize,int cx,int cy,int xc,int yc,int xside,int yside)
{
//	xt_mmxUse=0;
	short* vp = (short*)XGR_VIDEOBUF + (yc - yside)*XGR_MAXX + (xc - xside);
	unsigned short* vz = zBuffer + (yc - yside)*XGR_MAXX + (xc - xside);
	int xsize = 2*xside;
	int ysize = 2*yside;
	int XADD = XGR_MAXX - xsize;

	cx = XCYCL(cx);
	cy = YCYCL(cy);

	int YSrcSize = ysize*XSrcSize/xsize;

	int k_xscr_x = (XSrcSize << 16)/xsize;
	int k_yscr_y = (YSrcSize << 16)/ysize;

	int tfx = (cx << 16) - (XSrcSize << 15) + (1 << 15);
	int x0 = tfx >> 16;
	int x1 = x0 + XSrcSize;
	int tfy = (cy << 16) - (YSrcSize << 15) + (1 << 15);
	int y0 = tfy >> 16;
	int y1 = y0 + YSrcSize;

//	request(MIN(y0,y1) - MAX_RADIUS/2,MAX(y0,y1) + MAX_RADIUS/2,MIN(x0,x1) - 4,MAX(x0,x1) + 4);

	register int i,j,fx,fy;
	unsigned char* lc = &(vMap -> ClTrBuf[0][0][0]);//
	unsigned char* la = &(vMap -> AtBuf[0][0][0]);
	unsigned char* lv = &(vMap -> VxBuf[0][0][0]);
	//unsigned char* ls = &(vMap -> SpecBuf[0][0][0]);
	unsigned char* data;
	unsigned char* data1;
	unsigned char* data2;
//	unsigned char* data3;


	for(i = 0;i < ysize;i++){
				fx = tfx;
				fy = tfy;
				int yyy=YCYCL(fy >> 16);
				data = lc + yyy*XS_Buf;
				data1= la + yyy*XS_Buf;
				data2= lv + (yyy >> kmBuf)*(XS_Buf>>kmBuf);
				//data3= ls + YCYCL(fy >> 16)*XS_Buf;
#ifdef MMX
				if((k_xscr_x != 1<<16) ||(xt_mmxUse==0)){
#endif
					for(j = 0;j < xsize;j++,vp++,vz++){
	//						*vp = TrPal[(*(data1 + XCYCL(fx >> 16)) & TrW_MASK)][*(data + XCYCL(fx >> 16))];
						int xxx=XCYCL(fx >> 16);
						*vp = TrPal[(*(data1 + xxx) & TrW_MASK)][*(data + xxx)];
						*vz = hCamera-*(data2 + (xxx >> kmBuf));
						fx += k_xscr_x;
					}
				tfy += k_yscr_y;
				vp += XADD;
				vz += XADD;

#ifdef MMX
				}
				else {
					void * PAL=&TrPal[0][0];
					//data+=XCYCL(fx>>16);
					//data1+=XCYCL(fx>>16);
					//data2+=XCYCL(fx>>16);
					//data3+=XCYCL(fx>>16);
					int ofx=XCYCL(fx>>16);
					__asm{
//						push eax
//						push ebx
///						push ecx
//						push edx
//						push esi
//						push edi

						mov ecx,xsize
						shr ecx,2
						mov eax,hCamera //zBuf
						mov ebx,eax //zBuf
						shl eax,16d //zBuf
						mov ax,bx //zBuf
						movd MM5,eax //zBuf
						nop
						movd MM1,eax //zBuf
						psllq MM5,32d //zBuf
						por MM5,MM1 //zBuf
						mov edx,vz //zBuf

						mov esi,data1 //At (BASE Buf) !!!!!!!!!!!!!!!!!!!!!!
						mov eax,data2 //Vx
						movd MM4,eax
						mov eax,01f1f1f1fh //Tr & Water MASK
						movd MM7,eax
						xor eax,eax
						movd MM6,eax
						mov edi,vp
						mov ebx,PAL
						movd MM2,vz //zBuf
						mov eax,ofx
						push ebp
						mov ebp,eax
//
//
//
// Pred LOOP
						test ebp,3h
						je l_loop1t
						dec ecx //��������� �� 1 �������� loop �.�. Pred&End LOOP ���������� 4 pix.
						mov eax,ebp
						and eax,3h
						push eax // ���������� ���. �������� End LOOP
						neg eax
						and eax,3h
						push ecx // Save loop LOOP
						mov ecx,eax
p_loop1:
						bswap ecx
						xor eax,eax
						mov ah,[esi+offsetAt+ebp]
						mov al,[esi+offsetClTr+ebp]
						and ah,01fh //Tr & Water MASK
						shl eax,1
						mov cx,[ebx+eax]
						mov [edi],cx
						xor cx,cx
//.						test ah,020h // ��� (At_WATER << 1)
//.						jne p_loop2 //���� ����- �� �������� 0
						movd MM0,ebp //��� ����������������� VxBuf
						psrlq MM0,1
						paddd MM0,MM4
						movd eax,MM0
						mov cl,[eax]
//						mov cl,[esi+offsetVx+ebp]
///p_loop2:
						movd eax,MM5
						sub ax,cx
						mov [edx],ax

						xor cx,cx
						bswap ecx
						inc ebp
						and ebp,XS_Buf-1
						add edi,2
						add edx,2
						dec ecx
						cmp ecx,0
						jne p_loop1
						pop ecx //Restore loop LOOP
//End PRED LOOP						
						//add ebp,eax
						//shl eax,1
						//add edi,eax
						//add edx,eax
						//and ebp,XS_Buf-1
						jmp l_loop1
l_loop1t:
						push 0h
l_loop1:
						movd MM1,[esi+offsetAt+ebp]
						movd MM0,[esi+offsetClTr+ebp]
						pand MM1,MM7 //MASKA Tr & Water

						//movd MM2,[esi+offsetVx+ebp] //zBuf

						punpcklbw MM1,MM6 //MM6 - 0
						punpcklbw MM0,MM6
						//punpcklbw MM2,MM6 //zBuf
						psllw MM0,1
						psllw MM1,8+1
						paddw MM0,MM1
						movq MM3,MM0
						punpcklwd MM0,MM6
						movd eax,MM0
						bswap ecx
						mov cx,[eax+ebx]
						psrlq MM0,32d
						movd eax,MM0
						mov ax,[eax+ebx]
						shl eax,16d
						mov ax,cx
						mov [edi],eax
						add edi,4
						//movd MM4,edx
//.						psrlw MM1,8+1+4 //zBuf
//.						pcmpeqw MM1,MM6 //zBuf
//.						pand MM2,MM1 //zBuf
						movd MM0,ebp //��� ����������������� VxBuf
						psrlq MM0,1
						paddd MM0,MM4
						movd eax,MM0
						mov ax,[eax]
						movd MM2,eax
						//movd MM2,[eax]
						punpcklbw MM2,MM2 
						punpcklbw MM2,MM6 

						movq MM1,MM5
						psubw MM1,MM2
						movq [edx],MM1 //zBuf
						add edx,8
//3&4 pix
						punpckhwd MM3,MM6
						movd eax,MM3
						mov cx,[eax+ebx]
						psrlq MM3,32d
						movd eax,MM3
						mov ax,[eax+ebx]
						shl eax,16d
						mov ax,cx
						mov cx,0
						bswap ecx
						mov [edi],eax //Base command
						add ebp,4
						add edi,4
						//TEST movq [edi-8],MM1
						///movd eax,MM4
						///mov [edi],eax
						///mov [edi+4],edx
						///add edi,8
						//movd MM0,edx
						//psllq MM0,32d
						//por MM0,MM4
						//movq [edi],MM0
						//add edi,8
						//
						and ebp,XS_Buf-1
						dec ecx
						cmp ecx,0
						jne l_loop1 

//End LOOP Begin
						pop ecx
						cmp ecx,0
						je l_end
e_loop1:
						bswap ecx
						xor eax,eax
						mov ah,[esi+offsetAt+ebp]
						mov al,[esi+offsetClTr+ebp]
						and ah,01fh //Tr & Water MASK
						shl eax,1
						mov cx,[ebx+eax]
						mov [edi],cx
						xor cx,cx
//.						test ah,020h // ��� (At_WATER << 1)
//.						jne e_loop2 //���� ����- �� �������� 0
						movd MM0,ebp //��� ����������������� VxBuf
						psrlq MM0,1
						paddd MM0,MM4
						movd eax,MM0
						mov cl,[eax]
//						mov cl,[esi+offsetVx+ebp]
///e_loop2:
						movd eax,MM5
						sub ax,cx
						mov [edx],ax
						xor cx,cx
						bswap ecx
						inc ebp
						and ebp,XS_Buf-1
						add edi,2
						add edx,2
						dec ecx
						cmp ecx,0
						jne e_loop1
//End END LOOP						
l_end:
						
						pop ebp

//						pop edi
//						pop esi
//						pop edx
//						pop ecx
//						pop ebx
//						pop eax

						EMMS
					}
				tfy += k_yscr_y;
				vp+=XGR_MAXX;
				//vp += XADD;
				//vz += XADD;
				vz+=XGR_MAXX;
				}
#endif
			
		}
}
*/







/*void vrtMap::scaling162D(int XSrcSize,int cx,int cy,int xc,int yc,int xside,int yside)
{

	short* vp = (short*)XGR_VIDEOBUF;// + (yc - yside)*XGR_MAXX + (xc - xside);
	int xsize = 2*xside;
	int ysize = 2*yside;
	int XADD = XGR_MAXX - xsize;

	cx = XCYCL(cx);
	cy = YCYCL(cy);

	int YSrcSize = ysize*XSrcSize/xsize;

	int k_xscr_x = (XSrcSize << 16)/xsize;
	int k_yscr_y = (YSrcSize << 16)/ysize;

	int tfx = (cx << 16) - (XSrcSize << 15) + (1 << 15);
	int x0 = tfx >> 16;
	int x1 = x0 + XSrcSize;
	int tfy = (cy << 16) - (YSrcSize << 15) + (1 << 15);
	int y0 = tfy >> 16;
	int y1 = y0 + YSrcSize;

//	request(MIN(y0,y1) - MAX_RADIUS/2,MAX(y0,y1) + MAX_RADIUS/2,MIN(x0,x1) - 4,MAX(x0,x1) + 4);

	short (*vp1)[480][640]; 
	unsigned char vp2[640],vx,vx1;
	register int i,j;//fx,fy
	int k,xbeg=XCYCL(cx-xside),ybeg=YCYCL(cy+yside),xx,xx1;
	short cl,cl1;
	unsigned char* lc = &(vMap -> ClTrBuf[0][0][0]);
	unsigned char* la = &(vMap -> AtBuf[0][0][0]);
//	unsigned char* data;
//	unsigned char* data1;

	//vp1=vp;
	_asm {
		mov eax,vp
		mov vp1,eax
	}
	for (i=0;i<640;i++) vp2[i]=0;
	for (j=479;j>=0;j--){
		xx=xbeg;
		xx1=xbeg+1;
		for(i=0;i<319;i++){
			//xx=XCYCL(xx));
			//xx1=XCYCL(xx1+((~i)&1));
			cl= TrPal[ (vMap -> AtBuf[0][ybeg][xx+i]) & Tr_MASK][vMap -> ClTrBuf[0][ybeg][xx+i]]|0x0821;//0xf7de;
			cl1= TrPal[ (vMap -> AtBuf[0][ybeg][xx1+i]) & Tr_MASK][vMap -> ClTrBuf[0][ybeg][xx1+i]]|0x0821;//&0xf7de;
			cl1=((int)(cl+cl1) >>1);


			vx=vMap -> VxBuf[0][ybeg][xx+i];
			vx1=vMap -> VxBuf[0][ybeg][xx1+i];
			vx1=((int)(vx1+vx)>>1);
			//vx=vx>>1;

			if (vp2[i<<1]<vx){
				for(k=vp2[i<<1]; k <= vx;k++){
					if (j-k>=0)	vp1[0][j-k][i<<1]=cl;
				}
				vp2[i<<1]=k;
			}
			vp2[i<<1]-=1;
			if (vp2[i<<1]<0)vp2[i<<1]=0;

			if (vp2[(i<<1)+1]<vx1){
				for(k=vp2[(i<<1)+1]; k <= vx1;k++){
					if (j-k>=0)	vp1[0][j-k][(i<<1)+1]=cl1;
				}
				vp2[(i<<1)+1]=k;
			}
			vp2[(i<<1)+1]-=1;
			if (vp2[(i<<1)+1]<0)vp2[(i<<1)+1]=0;
		}
		ybeg=YCYCL(ybeg-1);
	}

*/
/*		if(k_xscr_x == 1 << 16){
			fx = XCYCL(tfx >> 16);
			if(fx + xsize < H_SIZE)
				for(i = 0;i < ysize;i++){
					fy = tfy;
					memcpy(vp,data,xsize);
					tfy += k_yscr_y;
					vp += xsize + XADD;
					}
			else {
				int sz1 = H_SIZE - fx;
				int sz2 = fx + xsize - H_SIZE;
				for(i = 0;i < ysize;i++){
					fy = tfy;
					memcpy(vp,data + fx,sz1);
					memcpy(vp + sz1,data,sz2);
					tfy += k_yscr_y;
					vp += xsize + XADD;
					}
				}
			return;
			}

*/
/*	for(i = 0;i < ysize;i++){
				fx = tfx;
				fy = tfy;
				data = lc + YCYCL(fy >> 16)*XS_Buf;
				data1= la + YCYCL(fy >> 16)*XS_Buf;
				for(j = 0;j < xsize;j++,vp++){
					*vp = TrPal[*(data1 + XCYCL(fx >> 16)) & Tr_MASK][*(data + XCYCL(fx >> 16))];
					fx += k_xscr_x;
					}
				tfy += k_yscr_y;
				vp += XADD;
				}*/
//}


void vrtMap::scaling(int XSrcSize,int cx,int cy,int xc,int yc,int xside,int yside)
{
/*	char* vp = (char*)XGR_VIDEOBUF + (yc - yside)*XGR_MAXX + (xc - xside);
	int xsize = 2*xside;
	int ysize = 2*yside;
	int XADD = XGR_MAXX - xsize;

	cx = XCYCL(cx);
	cy = YCYCL(cy);

	int YSrcSize = ysize*XSrcSize/xsize;

	int k_xscr_x = (XSrcSize << 16)/xsize;
	int k_yscr_y = (YSrcSize << 16)/ysize;

	int tfx = (cx << 16) - (XSrcSize << 15) + (1 << 15);
	int x0 = tfx >> 16;
	int x1 = x0 + XSrcSize;
	int tfy = (cy << 16) - (YSrcSize << 15) + (1 << 15);
	int y0 = tfy >> 16;
	int y1 = y0 + YSrcSize;

//	request(MIN(y0,y1) - MAX_RADIUS/2,MAX(y0,y1) + MAX_RADIUS/2,MIN(x0,x1) - 4,MAX(x0,x1) + 4);

#if defined(_ROAD_) && defined(_DEBUG)
	if(!TotalDrawFlag) return;
#endif

	register int i,j,fx,fy;
	unsigned char** lt = lineT;
	unsigned char** ltc = lineTcolor;
	int YSrc;
	unsigned char* data;

#ifdef _SURMAP_
	if(ColorShow){
#endif
		if(DepthShow)
			for(i = 0;i < ysize;i++){
				YSrc = (ysize - 2*i)*XSrcSize/xsize;
				fx = (cx << 16) - DEPTH((XSrcSize << 0x10)/2,i) + (1 << 15);
				fy = (cy << 16) - DEPTH((YSrc << 0x10)/2,i) + (1 << 15);
				k_xscr_x = DEPTH((XSrcSize << 0x10)/xsize,i);
				data = ltc[YCYCL(fy >> 16)];
				for(j = 0;j < xsize;j++){
					*vp++ = *(data + XCYCL(fx >> 16));
					fx += k_xscr_x;
					}
				vp += XADD;
				}
		else
#if defined(_ROAD_) && defined(_DEBUG)
		   if(debug_view){
			static unsigned char old_pal[256*3];
			static unsigned char new_pal[256*3];
			if(debug_view == 1){
				XGR_GetPal(old_pal);
				int i,j = 64*3;
				for(i = 0;i < 64;i++)
					new_pal[j++] = new_pal[j++] = new_pal[j++] = i;
				for(i = 0;i < 64;i++){
					new_pal[j++] = new_pal[j++] = i;
					j++;
					}
				debug_view = 0;
				XGR_SetPal(new_pal,0,255);
				debug_view = 2;
				}
			if(debug_view == 5){
				debug_view = 0;
				XGR_SetPal(old_pal,0,255);
				debug_view = -3;
				 }
			unsigned char* p;
			for(i = 0;i < ysize;i++){
				fx = tfx;
				fy = tfy;
				data = lt[YCYCL(fy >> 16)];
				for(j = 0;j < xsize;j++,vp++){
					p = data + XCYCL(fx >> 16);
					switch(debug_view){
						case 3: //Floor
							*vp = 64 + (*((unsigned char*)((int)p & ~1)) >> 2);
							break;
						case 4: //Ceiling
							*vp = 128 + (*((unsigned char*)((int)p | 1)) >> 2);
							break;

						case -1: //SHADOW Bits
							*vp = *(p + H_SIZE) & OBJSHADOW ? 255 : *(ltc[YCYCL(fy >> 16)] + XCYCL(fx >> 16));
							break;
						case 0:
							*vp = *(ltc[YCYCL(fy >> 16)] + XCYCL(fx >> 16));
							break;
						}
					fx += k_xscr_x;
					}
				tfy += k_yscr_y;
				vp += XADD;
				}
			return;
			}
		   else
#endif
		if(k_xscr_x == 1 << 16){
			fx = XCYCL(tfx >> 16);
			if(fx + xsize < H_SIZE)
				for(i = 0;i < ysize;i++){
					fy = tfy;
					data = ltc[YCYCL(fy >> 16)] + fx;
//					memcpy(vp,data,xsize);
					_asm{
						pushad
						mov edi,vp
						mov esi,data
						mov ecx,xsize
						shr ecx,7
						inc ecx
//						je not_r
loop_r:
						mov al,[esi+0d]
						mov al,[esi+32d]
						mov al,[esi+64d]
						mov al,[esi+64d+32d]
						add esi,128d
						dec ecx
						jne loop_r
//not_r:
						mov esi,data
						mov ecx,xsize
						shr ecx,2
						rep movsd
						mov ecx,xsize
						and ecx,3
						rep movsb
						popad
					}
					tfy += k_yscr_y;
					vp += xsize + XADD;
					}
			else {
				int sz1 = H_SIZE - fx;
				int sz2 = fx + xsize - H_SIZE;
				for(i = 0;i < ysize;i++){
					fy = tfy;
					data = ltc[YCYCL(fy >> 16)];
//					memcpy(vp,data + fx,sz1);
//					memcpy(vp + sz1,data,sz2);
					_asm{
						pushad
						mov edi,vp
						mov esi,data
						mov ecx,sz1
						add esi,fx
						mov ecx,sz1
						shr ecx,7
						//je nor_l1
						inc ecx
loop_r1:
						mov al,[esi+0d]
						mov al,[esi+32d]
						mov al,[esi+64d]
						mov al,[esi+64d+32d]
						add esi,128d
						dec ecx
						jne loop_r1
//nor_l1:
						mov esi,data
						mov ecx,sz1
						add esi,fx
						shr ecx,2
						rep movsd
						mov ecx,sz1
						and ecx,3
						rep movsb
//2
						mov edi,vp
						mov esi,data
						add edi,sz1
						mov ecx,sz2
						shr ecx,7
						je nor_l2
//						inc ecx
loop_r2:
						mov al,[esi+0d]
						mov al,[esi+32d]
						mov al,[esi+64d]
						mov al,[esi+64d+32d]
						add esi,128d
						dec ecx
						jne loop_r2
nor_l2:
						mov esi,data
						mov ecx,sz2
						shr ecx,2
						rep movsd
						mov ecx,sz2
						and ecx,3
						rep movsb
						popad
					}
					tfy += k_yscr_y;
					vp += xsize + XADD;
					}
				}
			return;
			}
#ifdef LOWLEVEL_OUTPUT
			scale(xsize,ysize,tfx,tfy,ltc,vp,k_xscr_x,k_yscr_y,XADD);
#else
			for(i = 0;i < ysize;i++){
				fx = tfx;
				fy = tfy;
				data = ltc[YCYCL(fy >> 16)];
				for(j = 0;j < xsize;j++,vp++){
					*vp = *(data + XCYCL(fx >> 16));
					fx += k_xscr_x;
					}
				tfy += k_yscr_y;
				vp += XADD;
				}
#endif
#ifdef _SURMAP_
		}
	else {
		for(i = 0;i < ysize;i++){
			fx = tfx;
			fy = tfy;
			data = lineT[YCYCL(fy >> 16)];
			for(j = 0;j < xsize;j++){
				*vp++ = *(data + XCYCL(fx >> 16)) >> 2;
				fx += k_xscr_x;
				}
			tfy += k_yscr_y;
			vp += XADD;
			}
		}
#endif
*/
}

static int* LineTable = 0;
static int LineTableLenght;
void calcLineTable(int curr_lenght,int k_vu,int base_step,int up_step)
{
	if(!LineTable){
		LineTableLenght = 1024;
		if(!(LineTableLenght & 1))
			LineTableLenght++;
		memset(LineTable = new int[LineTableLenght],0,sizeof(int)*LineTableLenght);
		}
	int fv = (-k_vu)*curr_lenght/2 + (1 << 15);
	int old_v,v;
	int v_diff;

	int* c = LineTable;
	old_v = (fv >> 16)*up_step;
	for(int u = 0;u <= curr_lenght;u++){
		fv += k_vu;
		v = (fv >> 16)*up_step;
		v_diff = v - old_v + base_step;
		old_v = v;
		*c++ = v_diff;
		}
}

#define GetCornerX(x,y) (((x)*cosTurn - (y)*sinTurn)/4)
#define GetCornerY(x,y) (((x)*sinTurn + (y)*cosTurn)/4)
#define GetCornerX1(x,y) (((x)*cosTurn - (y)*sinTurn)/2 + (1 << 15));
#define GetCornerY1(x,y) (((x)*sinTurn + (y)*cosTurn)/2 + (1 << 15));

#define CURR_COLOR *d

#ifndef TURN_TEST
	#define OptionalParameter 0
#endif

void vrtMap::turning(int XSrcSize,int Turn,int cx,int cy,int xc,int yc,int XDstSize,int YDstSize)
{
/*	char* vp = (char*)XGR_VIDEOBUF + (yc - YDstSize)*XGR_MAXX + (xc - XDstSize);
	char* vpp;
	XDstSize = 2*XDstSize;
	YDstSize = 2*YDstSize;
#ifndef TURN_TEST
	cx = cx & clip_mask_x;
	cy = cy & clip_mask_y;
#endif
	int YSrcSize = YDstSize*XSrcSize/XDstSize;
	int sinTurn,cosTurn;
	int k_base_x,k_base_y;
	int k_up_x,k_up_y;
	int k_x,k_vu;
	int fx,tfx,tfy;
	int tmp;
	int i0;
	int* tpTmp;

	sinTurn = SI[rPI(Turn)];
	cosTurn = CO[rPI(Turn)];

#ifndef TURN_TEST
	tfx = (cx << 16) - (XSrcSize*cosTurn - YSrcSize*sinTurn)/2 + (1 << 15);
	int vv0 = XSrcSize*sinTurn;
	int vv1 = YSrcSize*cosTurn;
	int v0 = vv0 + vv1 >> 1;
	int v1 = vv0 - vv1 >> 1;
	int vcy = cy << 16;
	tfy = vcy - v0 + (1 << 15);

	int y0 = tfy >> 16;
	int y1 = (vcy + v0) >> 16;
	int y2 = (vcy - v1) >> 16;
	int y3 = (vcy + v1) >> 16;

//	request(MIN(MIN(MIN(y0,y1),y2),y3) - MAX_RADIUS/2,MAX(MAX(MAX(y0,y1),y2),y3) + MAX_RADIUS/2,0,0);
	unsigned char** lt = vMap -> lineTcolor;
#else
	unsigned char** lt = TextureDataTable;
#endif

	int ParallelScanLines,ScreenDirectScan,TextureDirectScan;
	ParallelScanLines = abs(cosTurn) > abs(sinTurn);
	if(!ParallelScanLines){
		tmp = XDstSize;
		XDstSize = YDstSize;
		YDstSize = tmp;
		tmp = XSrcSize;
		XSrcSize = YSrcSize;
		YSrcSize = tmp;
//		  Turn = 3*PIx2/4 - Turn + PIx2;
//		  Turn %= PIx2;

//		  sinTurn = SI[rPI(Turn)];
//		  cosTurn = CO[rPI(Turn)];
		tmp = -sinTurn;
		sinTurn = -cosTurn;
		cosTurn = tmp;
		}

	k_base_x = cosTurn*XSrcSize/XDstSize;
	k_base_y = sinTurn*YSrcSize/YDstSize;
	k_up_x = -sinTurn*XSrcSize/XDstSize;
	k_up_y = cosTurn*YSrcSize/YDstSize;
//	  if(TestMode)
//		  k_x = int((double)XSrcSize/(double)cosTurn/(double)XDstSize*(double)0x10000*(double)0x10000);
//	  else
	k_x = ((XSrcSize << 16 + 5)/cosTurn << 16 - 5)/XDstSize;
	k_vu = -(sinTurn << 14)/(cosTurn >> 2);

	ScreenDirectScan = (sinTurn > 0) == (cosTurn > 0);
	TextureDirectScan = (sinTurn > 0) == (cosTurn > 0);
	if(ParallelScanLines){
		if(TextureDirectScan){
			tfx = GetCornerX(-2*XSrcSize + 1,-2*YSrcSize + 1);
			tfy = GetCornerY(-2*XSrcSize + 1,-2*YSrcSize + 1);
			}
		else{
			tfx = GetCornerX(2*XSrcSize - 1,-2*YSrcSize + 1);
			tfy = GetCornerY(2*XSrcSize - 1,-2*YSrcSize + 1);
			k_x = -k_x;
			}
		}
	else{
		if(TextureDirectScan){
			tfx = GetCornerX(-2*XSrcSize + 1,-2*YSrcSize + 1);
			tfy = GetCornerY(2*XSrcSize - 1,2*YSrcSize - 1);
			k_base_y = -k_base_y;
			k_up_y = -k_up_y;
			}
		else{
			tfx = GetCornerX(2*XSrcSize - 1,-2*YSrcSize + 1);
			tfy = GetCornerY(-2*XSrcSize + 1,2*YSrcSize - 1);
			k_base_y = -k_base_y;
			k_up_y = -k_up_y;
			k_x = -k_x;
			}
		}
	if(!ScreenDirectScan){
		k_base_x = - k_base_x;
		k_base_y = - k_base_y;
		}
	tfx += cx << 16;
	tfy += cy << 16;
	unsigned char* d;
	int u1,vb,ub;
	int base_step = ParallelScanLines ? 1 : XGR_MAXX;
	int up_step = ParallelScanLines ? XGR_MAXX : 1;
	if(!ScreenDirectScan)
		base_step = -base_step;

	calcLineTable(XDstSize,k_vu < 0 ? k_vu : -k_vu,base_step,up_step);
	int* tp;
	tpTmp = LineTable;
	if(ScreenDirectScan){
		u1 = 1;
		while(u1 < XDstSize && *tpTmp == base_step){
			tpTmp++;
			u1++;
			}
		}
	else{
		u1 = XDstSize - 1;
		while(u1 > 0 && *tpTmp == base_step){
			tpTmp++;
			u1--;
			}
		}
	for(vb = 0;vb < YDstSize;vb++){

		tp = LineTable + 0;
		if(ScreenDirectScan){
			if(ParallelScanLines)
				vpp = vp + (0) + (vb)*XGR_MAXX;
			else
				vpp = vp + (vb) + (0)*XGR_MAXX;
			}
		else{
			if(ParallelScanLines)
				vpp = vp + (XDstSize - 1) + (vb)*XGR_MAXX;
			else
				vpp = vp + (vb) + (XDstSize - 1)*XGR_MAXX;
			}
		if(ScreenDirectScan)
			i0 = u1;
		else
			i0 = XDstSize - 1 - u1 + 1;

		fx = tfx;
		int x = fx >> 16;
//		d = lt[YCYCL(tfy >> 16)] + XCYCL(fx >> 16);
		d = lt[YCYCL(tfy >> 16)];
//		  k_x2 = k_x & 0xFFFF;
//		  d_const = k_x >> 16;
#ifdef LOWLEVEL_OUTPUT
		if(d) invturn(d, vpp, fx, k_x, tp, i0);
//		  invturn(d, x, vpp, fx, k_x2, d_const, tp, i0);
		tp += i0;
#else
		for(i = i0;i > 0;i--){
			*vpp = *(d + (x & clip_mask_x));
			int prev_fx = fx;
			fx += k_x2;
			x += d_const + (prev_fx >> 16 != fx >> 16);
			vpp += *tp++;
			}
#endif
		if(ScreenDirectScan){
			tpTmp = tp;
			if(u1 < XDstSize)
				u1++;
			while(u1 < XDstSize && *tpTmp == base_step){
				tpTmp++;
				u1++;
				}
			}
		else{
			tpTmp = tp;
			if(u1 > 0)
				u1--;
			while(u1 > 0 && *tpTmp == base_step){
				tpTmp++;
				u1--;
				}
			}
		tfx += k_up_x;
		tfy += k_up_y;
		}
	tfx -= k_up_x;
	tfy -= k_up_y;
	if(ScreenDirectScan){
		ub = 0;
		while(ub < XDstSize - 1){
			tpTmp = LineTable + ub;
			do{
				ub++;
				tfx += k_base_x;
				tfy += k_base_y;
				}while(*tpTmp++ == base_step);

			tp = LineTable + ub;
			if(ParallelScanLines)
				vpp = vp + (ub) + (YDstSize - 1 + OptionalParameter)*XGR_MAXX;
			else
				vpp = vp + (YDstSize - 1 + OptionalParameter) + (ub)*XGR_MAXX;
			i0 = u1 - ub;

			fx = tfx;
			int x = fx >> 16;
//			d = lt[YCYCL(tfy >> 16)] + XCYCL(fx >> 16);
			d = lt[YCYCL(tfy >> 16)];
//			  k_x2 = k_x & 0xFFFF;
//			  d_const = k_x >> 16;
#ifdef LOWLEVEL_OUTPUT
			if(d) invturn(d, vpp, fx, k_x, tp, i0);
//			  invturn(d, x, vpp, fx, k_x2, d_const, tp, i0);
			tp += i0;
#else
			for(i = i0;i > 0;i--){
				*vpp = *(d + (x & clip_mask_x));
				int prev_fx = fx;
				fx += k_x2;
				x += d_const + (prev_fx >> 16 != fx >> 16);
				vpp += *tp++;
				}
#endif

			tpTmp = tp;
			if(u1 < XDstSize)
				u1++;
			while(u1 < XDstSize && *tpTmp == base_step){
				tpTmp++;
				u1++;
				}
			}
		}
	else{
		ub = XDstSize - 1;
		while(ub >= 0){
			tpTmp = LineTable + XDstSize - ub - 1;
			do{
				ub--;
				tfx += k_base_x;
				tfy += k_base_y;
				}while(*tpTmp++ == base_step);

			tp = LineTable + XDstSize - ub - 1;
			if(ParallelScanLines)
				vpp = vp + (ub) + (YDstSize - 1 + OptionalParameter)*XGR_MAXX;
			else
				vpp = vp + (YDstSize - 1 + OptionalParameter) + (ub)*XGR_MAXX;
			i0 = ub - u1 + 1;

			fx = tfx;
			int x = fx >> 16;
//			d = lt[YCYCL(tfy >> 16)] + XCYCL(fx >> 16);
			d = lt[YCYCL(tfy >> 16)];
//			  k_x2 = k_x & 0xFFFF;
//			  d_const = k_x >> 16;
#ifdef LOWLEVEL_OUTPUT
			if(d) invturn(d, vpp, fx, k_x, tp, i0);
//			  invturn(d, x, vpp, fx, k_x2, d_const, tp, i0);
			tp += i0;
#else
			for(i = i0;i > 0;i--){
				*vpp = *(d + (x & clip_mask_x));
				int prev_fx = fx;
				fx += k_x2;
				x += d_const + (prev_fx >> 16 != fx >> 16);
				vpp += *tp++;
				}
#endif

			tpTmp = tp;
			if(u1 > 0)
				u1--;
			while(u1 > 0 && *tpTmp == base_step){
				tpTmp++;
				u1--;
				}
			}
		}*/
}
/*
#ifndef _SURMAP_
void vrtMap::scaling_3D(DBM& A,int H,int focus,int cx,int cy,int xc,int yc,int xside,int yside,int Turn)
{
	char* vp = (char*)XGR_VIDEOBUF + (yc - yside)*XGR_MAXX + (xc - xside);
	int xsize = 2*xside;
	int ysize = 2*yside;
	int XADD = XGR_MAXX - xsize;

	cx = (cx & clip_mask_x) << 16;
	cy = (cy & clip_mask_y) << 16;

	double ai = Ha*xside;
	double aj = Va*yside;
	double bi = Hb*xside;
	double bj = Vb*yside;
	double ci = Hc*xside;
	double cj = Vc*yside;

	int y0 = (round((bi + bj)/(Oc + ci + cj)) + cy) >> 16;
	int y1 = (round((bi - bj)/(Oc + ci - cj)) + cy) >> 16;
	int y2 = (round((-bi + bj)/(Oc - ci + cj)) + cy) >> 16;
	int y3 = (round((-bi - bj)/(Oc - ci - cj)) + cy) >> 16;

//	request(MIN(MIN(MIN(y0,y1),y2),y3) - MAX_RADIUS/2,MAX(MAX(MAX(y0,y1),y2),y3) + MAX_RADIUS/2,0,0);

	double al = -ai - aj;
	double bl = -bi - bj;
	double cl = Oc - ci - cj;

	double ar = ai - aj;
	double br = bi - bj;

	double xsize_inv = 1/(double)xsize;
	double cl_inv;

	int k_xscr_x,k_xscr_y;
	register int i,j,fx,fy;
	unsigned char** lt = lineTcolor;
	unsigned char* data;

	if(Turn)
		for(i = 0;i < ysize;i++){
			cl_inv = 1/cl;
			fx = round(al*cl_inv);
			fy = round(bl*cl_inv);
			cl_inv *= xsize_inv;
			k_xscr_x = round((ar - al)*cl_inv);
			k_xscr_y = round((br - bl)*cl_inv);
			fx += cx;
			fy += cy;
			for(j = 0;j < xsize;j++){
				*vp++ = *(lt[(fy >> 16) & clip_mask_y] + ((fx >> 16) & clip_mask_x));
				fx += k_xscr_x;
				fy += k_xscr_y;
				}

			al += Va;
			bl += Vb;
			cl += Vc;

			ar += Va;
			br += Vb;

			vp += XADD;
			}
	else
		for(i = 0;i < ysize;i++){
			cl_inv = 1/cl;
			fx = round(al*cl_inv);
			fy = round(bl*cl_inv);
			cl_inv *= xsize_inv;
			k_xscr_x = round((ar - al)*cl_inv);
			fx += cx;
			fy += cy;
			data = lt[(fy >> 16) & clip_mask_y];
#ifdef LOWLEVEL_OUTPUT
			pscale(xsize,fx,vp,k_xscr_x,data);
			vp += xsize;
  #else
			for(j = 0;j < xsize;j++){
				*vp++ = *(data + ((fx >> 16) & clip_mask_x));
				fx += k_xscr_x;
				}
#endif
			al += Va;
			bl += Vb;
			cl += Vc;

			ar += Va;
			br += Vb;

			vp += XADD;
			}
}
#endif
*/
#ifdef _ROAD_
int PerpSlopTurn(int Turn,int Slop,int H,int F,int cx,int cy,int xc,int yc,int XDstSize,int YDstSize);
void vrtMap::SlopTurnSkip(int Turn,int Slop,int H,int F,int cx,int cy,int xc,int yc,int XDstSize,int YDstSize)
{
	PerpSlopTurn(Turn,Slop,H,F,cx,cy,xc,yc,XDstSize,YDstSize);
}
#endif

/*struct PrmFile {
	char* buf;
	int len;
	int index;

	void init(char* name);
	char* getAtom(void);
	void finit(void){ delete buf; }
	};
*/
void PrmFile::init(char* name)
{
	XStream ff(name,XS_IN);
	buf = new char[len = ff.size()];
	ff.read(buf,len);
	ff.close();

	int i = 0,mode = 1;
	char c;
	while(i < len){
		c = buf[i];
		if(c == '\"'){
			buf[i++] = '\0';
			while(i < len){
				if(buf[i] == '\"') break;
				i++;
				}
			if(i == len) ErrH.Abort("Wrong PRM format, second <\"> is absent");
			else buf[i] = '\0';
			}
		else
			if(c == ' ' || c == '\t' || c == '\r' || c == ',' || c == '\n') buf[i] = '\0';
		i++;
		}
	i = 0;
	while(i < len){
		c = buf[i];
		if(mode){ if(c == '/' && buf[i + 1] == '*'){ buf[i] = '\0'; mode = 0; } }
		else {
			if(c == '*' && buf[i + 1] == '/'){ buf[i] = buf[i + 1] = '\0'; i++; mode = 1; }
			else if(c != '\n') buf[i] = '\0';
			}
		i++;
		}
	index = 0;
}

char* PrmFile::getAtom(void)
{
	char* p = buf + index;
	while(index < len && !(*p)) p++, index++;
	if(index == len) return NULL;
	char* ret = p;
	while(index < len && *p) p++, index++;
	return ret;
}

sAllWorldsTable::sAllWorldsTable(char* worlds_prm_name)
{
	char* dir,*dirtemp;
	char *dirend=dirtemp=dir=strdup(worlds_prm_name);
	while(*dirtemp) {
		if (*dirtemp=='\\') dirend = dirtemp;
		dirtemp++;
	}
	*dirend = '\0';
	char *dirWorldPrm = strdup(dir);
	free(dir);
	//

	PrmFile* pFile;
	pFile = new PrmFile;
	pFile -> init(worlds_prm_name);
	maxWorld = atoi(pFile -> getAtom());
	if(maxWorld < 1) ErrH.Abort("Empty world list");
	it= new sWorldTable[maxWorld];
	int i;
	char* atom;
	for(i = 0;i < maxWorld;i++){
		atom = pFile -> getAtom();
		if(!atom) ErrH.Abort("Null world name");
		it[i].name = strdup(atom);
		atom = pFile -> getAtom();
		if(!atom) ErrH.Abort("Null world path");
		it[i].fname = strdup(atom);

		char* dir,*dirtemp;
		char *dirend=dirtemp=dir=strdup(atom);
		while(*dirtemp) {
			if (*dirtemp=='\\') dirend = dirtemp;
			dirtemp++;
		}
		*dirend = '\0';

		char tString[256];
		strcpy(tString,dirWorldPrm);//����������� � ������ ���� �� �������� � worlds.prm
		if(*dirWorldPrm) strcat(tString,"\\");		
		strcat(tString,dir);		//���������� � ������ ���� �� �������� � world.ini
									//�������� ���� �� �������� � �����
		free(dir);
		it[i].dir = strdup(tString);
		

		it[i].numberMechosomWorld = atoi(pFile -> getAtom());
		it[i].numberMechosomTrack = atoi(pFile -> getAtom());

		//�������� ! ��������� ����� �� world.ini � ��.
		XBuffer tS;
		tS.init();
		tS < dirWorldPrm;
		if(*dirWorldPrm) tS < "\\";
		tS < it[i].fname;

		const char* secCh_points = "Ch_points Parameters";
		char* ini_str=GetINIstringV(tS,secCh_points,"Ch_points File");
		//ini_str = "CH_P.TK0";
		char* Ch_pointsFileName;
		if(ini_str!=NULL) Ch_pointsFileName = strdup(ini_str);
		else continue;

		tS.init();
		tS < it[i].dir;
		if(*it[i].dir) tS < "\\";
		if(!mch_vmapResourceInit(tS)) continue;	//����� � ������ ZIP ����������
		tS < Ch_pointsFileName;

		Ch_points.load(tS,0);
		it[i].QuantityTrack=Ch_points.max_number; //������ � ������� ���������� ������

		free(Ch_pointsFileName);

	}
	pFile -> finit();

	free(dirWorldPrm);

}

sWorldTable::~sWorldTable(void)
{
		if(name) free(name); 
		if(dir) free(dir);
		if(fname) free(fname);
}
