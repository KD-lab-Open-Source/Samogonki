#include "_xtool.h"

//const unsigned int H_POWER = H_SIZE_POWER;
//const unsigned int H_SIZE = 1 << H_POWER;

//extern unsigned int V_POWER;
//extern unsigned int V_SIZE;

//extern int* FloodLvl;
extern unsigned char FloodLEVEL;

struct sVmpHeader{
	char id[4];
	int XS;
	int YS;
	int NX;
	int NY;
	sVmpHeader() {
		//id[0]='S'; id[1]='M'; id[2]='W'; id[3]='0';
	}
};
extern sVmpHeader VmpHeader;

struct vrtWorld {
	char* name;
	char* fname;
	char* dir;

	int numberMechosomWorld;
	int numberMechosomTrack;
	
		vrtWorld(void){ name = fname = NULL; }
	};

struct vrtNode { vrtNode* next; };

struct PrmFile;

#ifdef _SURMAP_ 
#define kmBuf 0
#else
#define kmBuf 1
#endif
#define kmVxBuf kmBuf

#define XS_Buf 2048
#define YS_Buf 2048
#if defined (_SURMAP_)
	#define offsetAt (XS_Buf * YS_Buf*0)
	#define offsetClTr (XS_Buf * YS_Buf*1)
	#define offsetVx (XS_Buf * YS_Buf*2)
	//#define offsetSpec (XS_Buf * YS_Buf*3)
#else 
	#define offsetAt (XS_Buf * YS_Buf*0)
	#define offsetVx (XS_Buf * YS_Buf/4*1) //Размер VxBuf в мехосоме - 1M
	#define offsetClTr (XS_Buf * YS_Buf/4*1 + (XS_Buf * YS_Buf/4) )
	//#define offsetSpec (XS_Buf * YS_Buf*2 + (XS_Buf * YS_Buf/4) )
#endif
struct vrtMap {
	unsigned char** lineT;
	unsigned char** lineTcolor;

	unsigned char *Buf; 
#if defined (_SURMAP_)
	unsigned short (*VxBuf)[YS_Buf][XS_Buf];//2K X 2K //XS_Buf X YS_Buf
	unsigned char (*AtBuf)[YS_Buf][XS_Buf];//2K X 2K
#else 
	unsigned char (*VxBuf)[YS_Buf][XS_Buf];//2K X 2K //XS_Buf X YS_Buf //>>kmBuf
	unsigned char (*AtBuf)[YS_Buf][XS_Buf];//2K X 2K
#endif
//#ifndef FULLRENDER
	unsigned char (*ClTrBuf)[YS_Buf][XS_Buf];//2K X 2K
//#else
//	unsigned short (*ClTrBuf)[YS_Buf][XS_Buf];//2K X 2K
//#endif
	//unsigned char (*SpecBuf)[YS_Buf][XS_Buf];//2K X 2K
	//unsigned short* ClBuf;//2K X 2K *2b
	unsigned short (*ClBuf)[YS_Buf][XS_Buf];
	unsigned char ClBufPresent;

	unsigned short TrPal[16*2][256];
	unsigned char H2OCR,H2OCG,H2OCB;
	unsigned short H2OC;
	unsigned short H2OA;
	int LevelH2O;
//	char fl_kmVxBuf;

	int upLine,downLine;

	unsigned char* dHeap;
	int freeMax,freeTail;
//	vrtNode* freeNodes;

//	unsigned char* dHeap_c;
	int freeMax_c,freeTail_c;
//	vrtNode* freeNodes_c;

	PrmFile* pFile;
	int maxWorld,cWorld;
	vrtWorld* wTable;

	unsigned char* changedT;
//	unsigned char* keepT;

	XStream fmap,pmap;//,kmap
	int foffset,offset;
	char* fileName;
	char* paletteName;
	char* M3DFileName;
	char* CLSFileName;
	char* Ch_pointsFileName;
	char* FxFileName;
	char* scenarioName;
	char* fname;//,*kname;
	char* pname[2];
	char* dirWorldPrm;

	int isCompressed;
	int* st_table;
	short* sz_table;
	unsigned char* inbuf;

#ifdef _SURMAP_
	XStream fsss;
	int* sssT;
#endif

		vrtMap(void);
		~vrtMap(void);

//temp
	inline unsigned short C(int i);
	//inline unsigned char ZF(int i);
	void init(void);
	void finit(void);
	void open(char* name,int nWorld);
	void load(char* name,int nWorld,int nTrack=0);
	void reload(int nWorld,int nTrack=0);
	void release(void);
//	void allocHeap(void);
	void alloc4Buf(void);
	void release4Buf(void);
	void allocClBuf(void);
	void releaseClBuf(void);
//	void reallocBuf(void);
	void lockHeap(void);
	void accept(int up,int down);
	void load3Buf(int X,int Y,int num_track=-1);
	void saveVMM(void);
	int  loadClBuf(int X,int Y,int num_track=-1);
	int  loadClBufJ(int X,int Y,int num_track=-1);
	int  loadClBufForRender(int X,int Y,int num_track=-1);
	void save3BufOnly(void);
	void loadTrPal(void);
	void saveTrPal(void);
	void loadRenderVariable(void);
	void saveRenderVariable(void);
	void loadTrack(int number=-1);
	void calcH2Ocolor(void);
	void calcH2Ocolor(int terr);
	void change(int up,int down);
	void another(int up,int down);
//	void request(int up,int down,int left, int right);
	void quant(void);
//	void link(int up,int down,int d);
//	void linkC(int up,int down,int d);
//	void delink(int up,int down);
	void turning(int XSrcSize,int Turn,int cx,int cy,int xc,int yc,int xside,int yside);
	void scaling(int XSrcSize,int cx,int cy,int xc,int yc,int xside,int yside);
	void scaling16(int XSrcSize,int cx,int cy,int xc,int yc,int xside,int yside);
	void scaling16HC(int XSrcSize,int cx,int cy,int xc,int yc,int xside,int yside);
	void viewV(int XSrcSize,int cx,int cy,int xc,int yc,int xside,int yside);
	void viewV2D(int XSrcSize,int cx,int cy,int xc,int yc,int xside,int yside);
	void scaling162D(int XSrcSize,int cx,int cy,int xc,int yc,int xside,int yside);
	void draw3D(int Height,double focus,int u0,int v0,int xc,int yc,int xside,int yside);
	void S2G_3D_non_rotate(int camera_x,int camera_y,int camera_z,double focus,int screen_x_center,int screen_y_center,
			int xs, int ys, int &xg, int &yg, int &zg);
	void S2G_2D_non_rotate(int camera_x,int camera_y,int camera_z,double focus,int screen_x_center,int screen_y_center,
			int xs, int ys, int &xg, int &yg, int &zg);

/*#ifndef _SURMAP_
	void scaling_3D(DBM& A,int H,int focus,int cx,int cy,int xc,int yc,int xside,int yside,int Turn);
	void SlopTurnSkip(int Turn,int Slop,int H,int F,int cx,int cy,int xc,int yc,int XDstSize,int YDstSize);
#endif*/
	void draw_voxel(int TurnAngle,int SlopeAngle,int TurnSecX,int CX,int CY,int xc,int yc,int xside,int yside);
	void checkAndRecoverWater();

	void render(int up,int down);
	void screenRender(void);
	void increase(int up,int down);

	void updownSetup(void);
	void lockMem(void);

#ifdef _SURMAP_
	void flush(void);
	void save3Buf(void);
	void restore3Buf(void);
//	void refresh(void);
//	void sssUpdate(void);
//	void sssKill(void);
//	void readLine(int n,unsigned char* p);
//	void sssReserve(void);
//	void sssRestore(void);
//	void squeeze(void);
	
//	void openMirror(void);
//	void closeMirror(void);
#endif

	inline unsigned char* use(void);
	inline void unuse(int i);

	inline unsigned char* use_c(void);
	inline void unuse_c(int i);

	void analyzeINI(const char* name);
	void fileLoad(void);
	};

void vMapPrepare(char* name,int nWorld = 0,int nTrack=0);
char* GetTargetName(char* name);

void S2G_z_buffer(int camera_x,int camera_y,int camera_z,double focus,int screen_x_center,int screen_y_center,
			int xs, int ys, int plane_mode, int &xg, int &yg, int &zg);

extern vrtMap* vMap;

inline int getDistX(int v0,int v1)
{
	int d = v0 - v1;
	int ad = abs(d);
	int dd = H_SIZE - ad;
	if(ad <= dd) return d;
	return d < 0 ? d + H_SIZE : d - H_SIZE;
}

inline int calcDX(int v0,int v1)
{
	int d = v1 - v0;
	int ad = abs(d);
	if(ad < (H_SIZE/2)) return d;
	return d < 0 ? d + H_SIZE : d - H_SIZE;
}

inline int getDistY(int v0,int v1)
{
	int d = v0 - v1;
	int ad = abs(d);
	int dd = V_SIZE - ad;
	if(ad <= dd) return d;
	return d < 0 ? d + V_SIZE : d - V_SIZE;
}

inline int calcDY(int v0,int v1)
{
	int d = v1 - v0;
	unsigned int ad = abs(d);
	if(ad < (V_SIZE/2)) return d;
	return d < 0 ? d + V_SIZE : d - V_SIZE;
}

inline int getDelta(int v0,int v1)
{
	return YCYCL(v0 - v1 + V_SIZE);
}

inline int getDeltaX(int v0,int v1)
{
	return XCYCL(v0 - v1 + H_SIZE);
}

const int MLNAMELEN = 15;

//Работа с INI Файлом
extern char* GetINIstringV(const char* iniFile,const char* section,const char* key);
extern void SaveINIstringV(const char* iniFile,const char* section,const char* key,char* var);


struct sWorldTable {
	char* name; //Имя мира
	char* dir;  //Путь до каталога с миром
	char* fname;//имя INI файла мира пока с частичным путем

	int numberMechosomWorld;
	int numberMechosomTrack;
	int QuantityTrack;
	
	sWorldTable (void) {
		name = dir = fname = 0; 
	};
	~sWorldTable (void);
};

struct sAllWorldsTable {
	sWorldTable * it;
	int maxWorld;

	sAllWorldsTable (char* worlds_prm_name);

	~sAllWorldsTable (void){
	};

};
//extern sAllWorldTable AllWorldTable;