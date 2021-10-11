
#ifndef __INTRO_H__
#define __INTRO_H__

const int INTRO_CH_ROTATION	= 0x01;
const int INTRO_CH_SPLINE	= 0x02;

#ifndef _FINAL_VERSION_
//#define _MAKE_INTRO_MOVIE_
#endif

class mchIntroChar
{
	int chr;
	int color;

	int soundID;

	float phase;
	float dest_phase;

	Vect2f R0;
	Vect2f R1;

	Vect2f destR;
	Vect2f dR;

	int flags;
	float angle;
	float angle_delta;
public:
	void SetFlag(int f){ flags |= f; }
	void DropFlag(int f){ flags &= ~f; }
	void ToggleFlag(int f){ flags ^= f; }
	int CheckFlag(int f) const { return (flags & f); }

	int Color(void) const { return color; }
	void SetColor(int c){ color = c; }

	Vect2f R(void){ return (R0 + (R1 - R0) * phase); }
	Vect2f getR1(void) const { return R1; }
	Vect2f getdestR(void) const { return destR; }

	int isMoving(void);
	int isFinished(void);

	void setPhase(float ph){ phase = ph; }
	void setPhaseDest(float ph){ dest_phase = ph; }

	void setR(Vect2f r){ R1 = r; }
	void set_dR(float x,float y){ dR.x = x; dR.y = y; }
	void setRndR(int dist_flag = 0);
	void resetR(void){ R0 = R(); R1 = destR; phase = 0.0f; }
	void initR(Vect2f r){ R0 = R1 = destR = r; phase = 1.0f; }

	void Redraw(int shadow_flag = 0);
	void Quant(float dt);

	void enableRotation(void){ SetFlag(INTRO_CH_ROTATION); }
	void enableSound(void);

	void Init(int ch,int col,Vect2f r0,Vect2f r1);
};

class mchIntroCharSpline 
{
	float phase;
	Vect2f P0[4];
	Vect2f P1[4];

	mchIntroChar* data0;
	mchIntroChar* data1;

public:
	void Init(mchIntroChar* p0, mchIntroChar* p1);
	void Quant(float dt);

	void Clear(void);

	Vect2f R0(void);
	Vect2f R1(void);

	int isFree(void){ if(data0) return 0; return 1; }

	mchIntroCharSpline(void){ phase = 0.0f; data0 = data1 = NULL; }
	~mchIntroCharSpline(void){ }
};

const int INTRO_NUM_SPLINES	 = 5;

class mchIntroDispatcher
{
	int numBorderChars;
	int* borderCharsIdx;

	int numChars;
	mchIntroChar* data;

	mchIntroCharSpline spline[INTRO_NUM_SPLINES];

	int curPhase;

	int curTime;
	int startTime;
	int prevTime;

	int stringStartTime;
	int stringAlphaDir;
	unsigned char stringStatus[6];

	int startTime1;
	int startFinitTime;
public:

	void Quant(void);
	void Redraw(void);
	void Init(int sx,int sy,void* text,void* bitmap,void* mask);

	void AdvancePhase(void);
	int isEnd(void){ return (curPhase >= 3); }

	mchIntroDispatcher(void);
	~mchIntroDispatcher(void);
};

extern mchIntroDispatcher* mch_introD;

void mchInitIntro(void);
void mchFinitIntro(void);

#endif /* __INTRO_H__ */