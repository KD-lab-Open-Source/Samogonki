#ifndef __GRID_2D__
#define __GRID_2D__

#include <list>
#include "Xreal_utl.h"

/////////////////////////////////////////
//	Integer Rectangle
/////////////////////////////////////////
struct iRectangle 
{
	int x0, y0, x1, y1;
	
	iRectangle(){}
	iRectangle(int x0_, int y0_, int x1_, int y1_) : x0(x0_), y0(y0_), x1(x1_), y1(y1_) {}
	int operator == (const iRectangle& r) const { return x0 == r.x0 && y0 == r.y0 && x1 == r.x1 && y1 == r.y1; }
	iRectangle& operator += (const iRectangle& r) { if(x0 > r.x0) x0 = r.x0; if(y0 > r.y0) y0 = r.y0; if(x1 < r.x1) x1 = r.x1; if(y1 < r.y1) y1 = r.y1; return *this; }
};

////////////////////////////////////////////////////////////////////////////
// Base for Objects wich will be stored in Grid2D
////////////////////////////////////////////////////////////////////////////
class Grid2DElement
{
	mutable int pass_counter;
	friend class PassDispatcher;
public:
	Grid2DElement() { pass_counter = 0; }
	int belongSquare(int x0, int y0, int sx, int sy) const { return 1; } // pseudo-virtual due to template using
};

////////////////////////////////////////////////////////////////////////////
//	One-time-call system
////////////////////////////////////////////////////////////////////////////
class PassDispatcher 
{
	mutable int pass_counter;
public:
	PassDispatcher(){ pass_counter = 0; }
	void beginPass() const { pass_counter++; }
	int doPass(const Grid2DElement& el) const { int log = el.pass_counter != pass_counter; el.pass_counter = pass_counter; return log; }
};


//////////////////////////////////////////////////////////
//	2D Grid Configuration Class
//////////////////////////////////////////////////////////
template <int cell_size_len> 
struct Grid2DConfig {
	enum {
		map_sx = 2048,
		map_sy = 2048,
		shift_x = cell_size_len,
		shift_y = cell_size_len,
		cell_sx = 1 << shift_x,
		cell_sy = 1 << shift_y,
		sx = map_sx/cell_sx,
		sy = map_sy/cell_sy,
		mask_x_ = sx - 1,
		mask_y_ = sy - 1
		};
	
	static int mask_x(int x) { return x & mask_x_; }
	static int mask_y(int y) { return y & mask_y_; }

	static int masked_prep_x(int x) { return mask_x(x >> shift_x); }
	static int masked_prep_y(int y) { return mask_y(y >> shift_y); }

	// Смещения для зацикливания/расцикливания 
	static short cycling_dx(int x) { return x < 0 ? map_sx : (x > mask_x_ ? -map_sx : 0); }
	static short cycling_dy(int y) { return y < 0 ? map_sy : (y > mask_y_ ? -map_sy : 0); }
	
	// Подготовка области для сканирования
	static void prep_rectangle(iRectangle& rectangle) 
	{
		rectangle.x0 >>= shift_x;
		rectangle.y0 >>= shift_y;
		rectangle.x1 >>= shift_x;
		rectangle.y1 >>= shift_y;
	}
};		

//////////////////////////////////////////////////////////
//	Grid 2D
//////////////////////////////////////////////////////////
template <class T, int cell_size_len>	
class Grid2D : PassDispatcher, Grid2DConfig<cell_size_len>
{
	using G2D = Grid2DConfig<cell_size_len>;
public:	
	class Node 
	{
		const T *ptr;
		short dx_, dy_;
	public:
		Node(const T* p = 0, short dx__ = 0, short dy__ = 0) : ptr(p), dx_(dx__), dy_(dy__) {}
		const T* operator->() const {return ptr;}
		operator const T* () const { return ptr; }
		short dx() const { return dx_; }
		short dy() const { return dy_; }
	};

	using SList = std::list<Node>;

	////////////////////////////////////////////////////////////////////////////////////
	//		Insertion & removing
	////////////////////////////////////////////////////////////////////////////////////
	void insert(const T& obj, iRectangle rect)
	{
		G2D::prep_rectangle(rect);
		for(int y = rect.y0;y <= rect.y1;y++)
			for(int x = rect.x0;x <= rect.x1;x++)
				if(obj.belongSquare(x*G2D::cell_sx, y*G2D::cell_sy, G2D::cell_sx, G2D::cell_sy))
					table[G2D::mask_y(y)][G2D::mask_x(x)].push_front( Node(&obj, G2D::cycling_dx(x), G2D::cycling_dy(y)) );
	}

	void erase(const T& obj, iRectangle rect)
	{
		G2D::prep_rectangle(rect);
		for(int y = rect.y0;y <= rect.y1;y++)
			for(int x = rect.x0;x <= rect.x1;x++)
				table[G2D::mask_y(y)][G2D::mask_x(x)].remove( Node(&obj, G2D::cycling_dx(x), G2D::cycling_dy(y)) );
	}

	void move(const T& obj,  iRectangle prev_rect,  iRectangle rect)
	{
		G2D::prep_rectangle(rect);
		G2D::prep_rectangle(prev_rect);
		if(rect == prev_rect)
			return;
		int y;
		for(y = prev_rect.y0;y <= prev_rect.y1;y++)
			for(int x = prev_rect.x0;x <= prev_rect.x1;x++)
				table[G2D::mask_y(y)][G2D::mask_x(x)].remove( Node(&obj, G2D::cycling_dx(x), G2D::cycling_dy(y)) );
		for(y = rect.y0;y <= rect.y1;y++)
			for(int x = rect.x0;x <= rect.x1;x++)
				if(obj.belongSquare(x*G2D::cell_sx, y*G2D::cell_sy, G2D::cell_sx, G2D::cell_sy))
					table[G2D::mask_y(y)][G2D::mask_x(x)].push_front( Node(&obj, G2D::cycling_dx(x), G2D::cycling_dy(y)) );
	}

	void clear()
	{
		for(int y = 0;y < G2D::sy;y++)
			for(int x = 0;x < G2D::sx;x++)
				table[y][x].clear();
	}

	////////////////////////////////////////////////////////////////////////////////////
	//		Accessing
	////////////////////////////////////////////////////////////////////////////////////
	SList& operator() (int x, int y) { return table[G2D::masked_prep_y(y)][G2D::masked_prep_x(x)]; }

	template <class Op>
	void scan(iRectangle rect, Op& op) const
	{
		beginPass();
		G2D::prep_rectangle(rect);
		for(int y = rect.y0;y <= rect.y1;y++){
			short dy = G2D::cycling_dy(y);
			for(int x = rect.x0;x <= rect.x1;x++){
				short dx = G2D::cycling_dx(x);
				const SList& root = table[G2D::mask_y(y)][G2D::mask_x(x)];
				typename SList::const_iterator i;
				FOR_EACH(root, i)
					if(doPass(**i))
						op(**i, dx - i -> dx(), dy - i -> dy());
				}
			}
	}

	template <class Op>
	int process_cell(int x, int y, Op& op) const
	{
		short dx = G2D::cycling_dx(x);
		short dy = G2D::cycling_dy(y);
		const SList& root = table[G2D::mask_y(y)][G2D::mask_x(x)];
		typename SList::const_iterator i;
		FOR_EACH(root, i)
			if(doPass(**i))
				if(!op(**i, dx - i -> dx(), dy - i -> dy()))
					return 0;
		return 1;
	}

	template <class Op>
	void scan_line(int x1_, int y1_, int x2_, int y2_, Op& op, int width = 1) const
	{
		beginPass();
		const int F_PREC = 16;

		iRectangle rect(x1_, y1_, x2_, y2_);
		G2D::prep_rectangle(rect);
		int x1 = rect.x0;
		int y1 = rect.y0;
		int x2 = rect.x1;
		int y2 = rect.y1;

		if(x1 == x2 && y1 == y2){
			process_cell(x1, y1, op);
			return;
			}

		if(abs(x2 - x1) > abs(y2 - y1)){
			int a = x2 - x1;
			int b = y2 - y1;
			int x = x1;
			int y = (y1 << F_PREC) + (1 << F_PREC - 1);
			int incr = 1;
			int k = (b << F_PREC)/a;
			if(x1 > x2){
				incr = -1;
				k = -k;
				a = -a;
				}
			do{
				if(!process_cell(x, y >> F_PREC, op))
						return;
				for(int i = 1; i <= width; i++){
					if(!process_cell(x, (y >> F_PREC) + i, op))
						return;
					if(!process_cell(x, (y >> F_PREC) - i, op))
							return;
					}
				x += incr;
				y += k;
				} while(--a >= 0);
			}
		else{
			int a = x2 - x1;
			int b = y2 - y1;
			int x = (x1 << F_PREC) + (1 << F_PREC - 1);
			int y = y1;
			int incr = 1;
			int k = (a << F_PREC)/b;
			if(y1 > y2){
				incr = -1;
				k = -k;
				b = -b;
				}
			do{
				if(!process_cell(x >> F_PREC, y, op))
						return;
				for(int i = 1; i <= width; i++){
					if(!process_cell((x >> F_PREC) + i, y, op))
						return;
					if(!process_cell((x >> F_PREC) - i, y, op))
						return;
					}
				y += incr;
				x += k;
				} while(--b >= 0);
			}
	}

	template <class Op>
	void scan_poly(int pX[], int pY[], int n, Op& op)
	{
		#define DIV(a, b)	(((a) << 16)/(b))
		#define CCW(i)	(i == 0 ? n - 1 : i - 1)
		#define CW(i)	(i == n - 1 ? 0 : i + 1)

		beginPass();

		int i;
		for(i = 0; i < n; i++){
			pX[i] >>= G2D::shift_x;
			pY[i] >>= G2D::shift_y;
			}

		int vals_up = 0;
		for(i = 1; i < n; i++)
			if(pY[vals_up] > pY[i])
				vals_up = i;

		int lfv = vals_up;
		int rfv = vals_up;
		int ltv = CCW(lfv);
		int rtv = CW(rfv);

		int Y = pY[lfv]; 
		int xl = pX[lfv];
		int al = pX[ltv] - xl; 
		int bl = pY[ltv] - Y;
		int ar = pX[rtv] - xl; 
		int br = pY[rtv] - Y;
		int xr = xl = (xl << 16) + (1 << 15);

		if(bl)
			al = DIV(al, bl);
		if(br)
			ar = DIV(ar, br);

		int d, where;

		while(1){
			if(bl > br){
				d = br;
				where = 0;
				}
			else{
				d = bl;
				where = 1;
				}
			while(d-- > 0){
				int x1 = xl >> 16;
				int x2 = xr >> 16;

				if(x1 > x2)
					swap(x1, x2);

				while(x1 <= x2)
					process_cell(x1++, Y, op);

				Y++;
				xl += al;
				xr += ar;
				}
			if(where){
				if(ltv == rtv) 
					return;
				lfv = ltv;
				ltv = CCW(ltv);

				br -= bl;
				xl = pX[lfv];
				al = pX[ltv] - xl;
				bl = pY[ltv] - Y;
				xl = (xl << 16) + (1 << 15);
				if(bl)
					al = DIV(al, bl);
				}
			else {
				if(rtv == ltv) 
					return;
				rfv = rtv;
				rtv = CW(rtv);

				bl -= br;
				xr = pX[rfv];
				ar = pX[rtv] - xr;
				br = pY[rtv] - Y;
				xr = (xr << 16) + (1 << 15);
				if(br)
					ar = DIV(ar, br);
				}
			}

		#undef DIV
		#undef CCW
		#undef CW
	}

	////////////////////////////////////////////////////////////////////////////////////
	//		Debug shows
	////////////////////////////////////////////////////////////////////////////////////
	void show(int x, int y) const
	{
		beginPass();
		int x0 = G2D::masked_prep_x(x);
		int y0 = G2D::masked_prep_y(y);
		const SList& root = table[y0][x0];
		typename SList::const_iterator i;
		FOR_EACH(root, i)
			if(doPass(**i))
				(*i) -> show();
		Vect3f p0(x0*G2D::cell_sx, y0*G2D::cell_sy, 0);
		Vect3f dx(G2D::cell_sx, 0, 0);
		Vect3f dy(0, G2D::cell_sy, 0);
		show_vector(p0, p0 + dx, p0 + dx + dy, p0 + dy, XCOL(CYAN, 100));
	}

	void show(int xc, int yc, int sz, int greed_color = 0) const 
	{
		beginPass();
		iRectangle rect(xc - sz, yc - sz, xc + sz, yc + sz);
		G2D::prep_rectangle(rect);
		for(int y = rect.y0;y <= rect.y1;y++)
			for(int x = rect.x0;x <= rect.x1;x++){
				const SList& root = table[G2D::mask_y(y)][G2D::mask_x(x)];
				typename SList::const_iterator i;
				FOR_EACH(root, i)
					if(doPass(**i))
						(*i) -> show();

				if(greed_color){
					Vect3f p0(x*G2D::cell_sx, y*G2D::cell_sy, 0);
					Vect3f dx(G2D::cell_sx, 0, 0);
					Vect3f dy(0, G2D::cell_sy, 0);
					show_vector(p0, p0 + dx, p0 + dx + dy, p0 + dy, greed_color);
					}
				}
	}

	void show_area(const T& obj, int color) const
	{
		for(int y = 0;y < G2D::sy;y++)
			for(int x = 0;x < G2D::sx;x++){
				const SList& root = table[y][x];
				typename SList::const_iterator i;
				FOR_EACH(root, i)
					if(&obj == *i){
						Vect3f p0(x*G2D::cell_sx, y*G2D::cell_sy, 0);
						Vect3f dx(G2D::cell_sx, 0, 0);
						Vect3f dy(0, G2D::cell_sy, 0);
						show_vector(p0, p0 + dx, p0 + dx + dy, p0 + dy, color);
						}
				}
	}

	int size() const
	{
		int sz = 0;
		for(int y = 0;y < G2D::sy;y++)
			for(int x = 0;x < G2D::sx;x++)
				sz += table[y][x].size();
		return sz;
	}

protected:
	SList table[G2D::sy][G2D::sx];
};

#endif  // __GRID_2D__
