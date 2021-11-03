#ifndef __XREAL_UTL__
#define __XREAL_UTL__

extern int controlfp_flags;
extern int controlfp_mask;
#ifdef _WIN32
#define CONTROL_FP()	_controlfp(controlfp_flags, controlfp_mask); 
#else
#define CONTROL_FP()
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
//			Extern Symbols
////////////////////////////////////////////////////////////////////////////////////////////////////

////  Axilliary Externals   //////////////////
extern XBuffer bout;
extern XBuffer msg_buf;
extern int xreal_log;
extern XStream recorder_logging;
extern int Xreal_key_active;

////  Axilliary Prototypes  //////////////////////
void draw_text(char* text);
const char* check_command_line(const char* switch_str);
int check_if_file_exist(const char* name);
void append_message_to_file(const char* message, const char* fname);
void show_vector(const Vect3f& vg, int color);
void show_vector(const Vect3f& vg, float radius, int color);
void show_vector(const Vect3f& vg, const Vect3f& delta, int color);
void show_vector(const Vect3f &vg0, const Vect3f &vg1, const Vect3f &vg2, int color);
void show_vector(const Vect3f &vg0, const Vect3f &vg1, const Vect3f &vg2, const Vect3f &vg3, int color);
void show_map_pixel(Vect3f v, int color);
void show_map_line(Vect3f v0, Vect3f v1, int color);
void show_map_circle(Vect3f v, float radius, int color);
void clip_pixel(int x1,int y1,int color, int size = 0);
void clip_line(int x1,int y1,int x2,int y2,int color);

#define XCOL_(r, g, b) ((r << 16) | (g << 8) | b)
enum XColors {
	 WHITE = XCOL_(255, 255, 255),
	 RED = XCOL_(255, 0, 0),
	 GREEN = XCOL_(0, 255, 0),
	 BLUE = XCOL_(0, 0, 255),
	 YELLOW = XCOL_(255, 255, 0),
	 MAGENTA = XCOL_(255, 0, 255),
	 CYAN = XCOL_(0, 255, 255)
	 };
int XCOL(int color, int intensity = 255, int alpha = 255);

inline int xPressed(int key){ return Xreal_key_active && XKey.Pressed(key); }

class SmartBuffer : public XBuffer
{
	char* fname;
public:
	SmartBuffer(char* fname_) : XBuffer(1000, 1), fname(fname_) {}
	~SmartBuffer() { if(tell()){ XStream f(fname, XS_OUT); f.write(address(), tell()); } }
};
//extern SmartBuffer fout;
extern XStream fout;

#endif // __XREAL_UTL__

