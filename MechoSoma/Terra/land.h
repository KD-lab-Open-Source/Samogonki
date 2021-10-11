#define MAX_DISTANCE 4096
#define MAX_LIGHT 255
#define MAX_VALUE_GB 255
#define MAX_GAUSS_RADIUS 10
struct s_renderV {
	unsigned char (*vidBufRI)[YS_Buf][XS_Buf];
	unsigned char (*vidBufGI)[YS_Buf][XS_Buf];
	unsigned char (*vidBufBI)[YS_Buf][XS_Buf];

	unsigned char (*vidBufRO)[YS_Buf][XS_Buf];
	unsigned char (*vidBufGO)[YS_Buf][XS_Buf];
	unsigned char (*vidBufBO)[YS_Buf][XS_Buf];

	char (*N_X)[YS_Buf][XS_Buf];
	char (*N_Y)[YS_Buf][XS_Buf];
	char (*N_Z)[YS_Buf][XS_Buf];

	unsigned char (*shadowBuf)[YS_Buf][XS_Buf];
	unsigned short (*vBuf)[YS_Buf][XS_Buf];

	short COSnG[TERRAIN_MAX][512];

	unsigned char self_luminescence[TERRAIN_MAX];
	unsigned char ref_n[TERRAIN_MAX]; //степень отражающей кривой
	unsigned char ref_k[TERRAIN_MAX]; //коэффициент отражения (*100)
	float tableLightOnDistance[MAX_DISTANCE];
	float dif_k[TERRAIN_MAX]; //коэффициент диффузного отражения данной поверхности

	unsigned char gaussBuf[(MAX_GAUSS_RADIUS+1)*(MAX_GAUSS_RADIUS+1)];
	int diametr_gb;
	int radius_gb;

	int k_Ambient,k_SAmbient;


	s_renderV(void){
		init();
	};

	void main(int num_track);
	void calculateCOSnG(void);
	void calculateLightOnDistance(int distance);
	void calculateShadowVx(int xl, int yl,int zl, int Radius_Light);
	void init(void);
	void prepareGaussFilter(int Radius);
	void RenderShadowM3DPL(int xL, int yL, int zL, int Radius);


	inline int calcDX(int v0,int v1){
		int cmpx=H_SIZE; cmpx=(cmpx>>1);
		int xx1= (v1)-(v0);
		if(xx1<-cmpx) xx1+=H_SIZE; if(xx1>cmpx)xx1-=H_SIZE;
		return xx1;
	};
	inline int calcDY(int v0,int v1){
		int cmpy=V_SIZE; cmpy=(cmpy>>1);
		int yy1= (v1)-(v0);
		if(yy1<-cmpy) yy1+=V_SIZE; if(yy1>cmpy)yy1-=V_SIZE;
		return yy1;
	};



};
extern s_renderV renderV;
