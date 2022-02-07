#define CH_POINTS_MAX 200
#define ST_POINTS_MAX 20
#define CHECK_POINT_H_DEFAULT (-1)//Дефаултное значение высоты
//#define LAB_POINTS_MAX 200

struct sCh_point{
//	int ID;
	int x1,y1;
	int x2,y2;
	int h;
//	int Link[CH_POINT_LINK_MAX];
	//int Link_max;
//	sCh_point(void) {
//		x1=x2=y1=y2=-1;
//		for (int i=0; i<CH_POINT_LINK_MAX;i++) Link[i]=-1;
//	}
};
struct sSt_point{
	float x,y,z,ax,ay,az;
};
//struct sLab_point{
//	int x,y,ax,ay;
//};

const char fileChPointsHeader[]={'C','h','P','v','1','.','0'};
struct sCh_points{

	sCh_point El[CH_POINTS_MAX];
	char Link[CH_POINTS_MAX][CH_POINTS_MAX];
	int max_number;
	int edit;
	char fl_animation;
	int Hedit;
	////
	sSt_point St[ST_POINTS_MAX];
	int max_start;
	int editstart;
	////
	//sLab_point Lab[LAB_POINTS_MAX];
	//int max_lab;
	////
	int currient_track;
	sCh_points (void) { reset(); }
	void reset (void) {
		max_number=0;edit=-1;fl_animation=0;max_start=0,editstart=-1,currient_track=0;
		for(int i=0; i<CH_POINTS_MAX; i++){
			for(int j=0; j<CH_POINTS_MAX; j++) Link[i][j]=0;
			El[i].x1=El[i].x2=El[i].y1=El[i].y2=-1;
		}
	}
	~sCh_points (void) { }
	void ins (int _ID);
	void del(int _ID);
	void addLink(int _ID,int _Link);
	void delallLink(int _ID);
	void set1(int MapX,int MapY, int scx, int scy);
	void set2(int MapX,int MapY, int scx, int scy);
	void save(char* fname,int number_track=-1);
	void load(char* fname,int number_track=-1);
	void insstart(int _num,int x,int y,int z);
	void delstart(int num);
#ifdef _SURMAP_
	void drawstart();
#endif
	int cmpVersionHeader(char * cmpstr){
		for(int i=0; i<sizeof(fileChPointsHeader); i++){
			if (cmpstr[i]!=fileChPointsHeader[i]) return 0;
		}
		return 1;
	};
};
//extern int editCh_point;
extern sCh_points Ch_points;

