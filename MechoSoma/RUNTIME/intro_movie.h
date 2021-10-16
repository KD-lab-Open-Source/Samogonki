#ifndef __INTRO_MOVIE_H__
#define __INTRO_MOVIE_H__

#include "intro_movie_id.h"

class mchIntroMovieDispatcher;

class mchIntroMovieAction
{
	int Type;

	int duration;
	int cur_time;

	int slot;
	int img_ids[2];

	int alpha;

	float scale[2];

	int prev_coords;
	Vect2f coords[2];

	mchIntroMovieDispatcher* owner;

public:

	int ID;
	void* list;
	mchIntroMovieAction* prev;
	mchIntroMovieAction* next;

	void set_owner(mchIntroMovieDispatcher* ow){ owner = ow; }
	void set_duration(int tm){ duration = tm; }

	void start(void);
	void quant(int dt);
	void draw(void);

	int type(void) const { return Type; }
	int time(void) const { return cur_time; }
	int total_time(void) const { return duration; }

	int is_finished(void) const { return cur_time >= duration; }

	mchIntroMovieAction(void* scr_p);
	~mchIntroMovieAction(void);
};

class mchIntroMovieImage
{
	int SX;
	int SY;
	unsigned short* buf;

	float scale;
	int slot;

	char* fname;
public:

	int ID;
	void* list;

	mchIntroMovieImage* prev;
	mchIntroMovieImage* next;

	int size_x(void);
	int size_y(void);

	void load_image(int slot);
	void free_image(void);

	void draw(int x,int y,int alpha = 255);
	void clip(Vect2f& coords);

	void set_file(char* fn);
	void set_scale(float sc){ scale = sc; }

	mchIntroMovieImage(void* scr_p);
	~mchIntroMovieImage(void);
};

class mchIntroMovieText
{
	char* buf;
	int action_ID;

	int mpeg_actionID;
	int mpeg_phraseID;

	int start_time_;
	int cur_time;
	int duration;

	mchIntroMovieDispatcher* owner;

public:

	int ID;
	void* list;
	mchIntroMovieText* prev;
	mchIntroMovieText* next;

	void set_owner(mchIntroMovieDispatcher* ow){ owner = ow; }
	void set_action(int ac){ action_ID = ac; }
	void set_start_time(int tm){ start_time_ = tm; }
	void set_duration(int tm){ duration = tm; }
	void set_text(const char* txt,int txt_len = -1);
	void set_mpeg_ids(int aid,int pid){ mpeg_actionID = aid; mpeg_phraseID = pid; }

	char* text(void) const { return buf; }
	int action(void) const { return action_ID; }
	int start_time(void) const { return start_time_; }
	int is_finished(void) const { return (cur_time >= duration); }

	void start(void);
	void quant(int dt){ cur_time += dt; }
	void draw(void);

	mchIntroMovieText(void);
	~mchIntroMovieText(void);
};

class mchIntroMovieDispatcher
{
	mchIntroMovieAction* curAcn;
	mchIntroMovieText* curTxt;

	xtList<mchIntroMovieImage> imgList;
	xtList<mchIntroMovieAction> acnList;
	xtList<mchIntroMovieText> txtList;

	int timer;
public:

	void init(void);
	void init_texts(void);
	void init_text(char* txt);

	void set_time(void);

	void start(void);
	void text_quant(int dt);
	int quant(void);

	int skip(void);

	mchIntroMovieImage* get_image(int id){ return imgList.search(id); }

	mchIntroMovieDispatcher(void);
	~mchIntroMovieDispatcher(void);
};

extern RandomGenerator im_random_generator;
xm_inline unsigned imRnd(unsigned m){ return im_random_generator(m); }

extern mchIntroMovieDispatcher* mch_introMovieD;

extern int mchIntroMovieActive;

#endif /* __INTRO_MOVIE_H__ */
