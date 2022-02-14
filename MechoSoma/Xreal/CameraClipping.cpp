#include "StdAfx.h"
#include "SimpleClip.h"
#include "CameraDispatcher.h"
#include "CameraPrm.h"
#include "DebugPrm.h"
#include "Xreal_utl.h"
#include "Body.h"
#include "mechosoma.h"
#include "Statistics.h"
#include "XJoystick.h"

///////////////////////////////////////////////////////////////////////////////////////////////
//		ZBuffer's declarations
///////////////////////////////////////////////////////////////////////////////////////////////
class ZBuffer
{
	enum { 
		x_shift = 5,
		y_max = 32, 
		int_max = 0x7fffffff,

		x_max = 1 << x_shift - 1,
		x_min = -x_max,
		y_min = -y_max,
		x_size = x_max - x_min,
		y_size = y_max - y_min,
		xy_size = x_size*y_size,
		xy_mask = xy_size - 1
		};
	float x_focus;
	float y_focus;
	Plane vis_pyramid[4];
	int z_buffer[xy_size];
	int x0, y0, x1, y1;
	int x_delta, y_delta;

	inline int& Z(int x, int y){ 
		int i = x - x_min + ((y - y_min) << x_shift); 
		assert(i >= 0 && i < xy_size);
		return z_buffer[i];
		}
	void clip_triangle(Vect3f vv[3], int side);

public:

	ZBuffer();
	void reset(float z_max);
	void draw_triangle(Vect3f vv[3]);
	int find_nearest_point(Vect3f& v, int enable_slope);
	int test_point(int x, int y, int& dist);
	void draw();
};


//////////////////////////////////////////////////////////////////////////////
//	New Clipping
//////////////////////////////////////////////////////////////////////////////
void Triangle::draw_to_z_buffer(ZBuffer& z_buffer, const MatXf& sXg, cCamera* camera) const
{
	if(brush){
		brush -> draw_to_z_buffer(z_buffer, sXg, camera);
		return;
		}
	Vect3f vv[3];		
	for(int i = 0; i < 3; i++){
		vv[i] = getDist(verts[i], sXg.trans());
		vv[i].z -= SHARE_FLOAT(int(round(sqr(vv[i].x) + sqr(vv[i].y))));
		sXg.rot().xform(vv[i]);
		}
	z_buffer.draw_triangle(vv);
}

void TriangleBrush::draw_to_z_buffer(ZBuffer& z_buffer, const MatXf& sXg, cCamera* camera) const
{
	MatXf X = pose; // l -> g
	WorldShareMapping((cMatrix&)X, camera);
	X.trans() = getDist(X.trans(), sXg.trans());
	X.premult(MatXf(sXg.rot(), Vect3f::ZERO));

	const_iterator i;
	FOR_EACH(*this, i){
		const Triangle& triangle = *i;
		Vect3f vv[3];
		for(int j = 0; j < 3; j++)
			X.xformPoint(triangle.vertices()[j], vv[j]);
		z_buffer.draw_triangle(vv);
		}
}

class ScanZBufferOp : public ZBuffer
{
	cCamera* camera;
	MatXf sXg;

public:

	void set(int z_max, const MatXf& sXg_, cCamera* camera_)
	{
		sXg = sXg_;
		camera = camera_;
		reset(z_max);
	}

	int operator()(const Triangle& triangle, float dx, float dy)
	{
		if(!(triangle.property() & Triangle::ForRayCasting))
			return 1;
		
		MatXf uncycled(sXg);
		uncycled.trans().x += dx; uncycled.trans().y += dy;
		triangle.draw_to_z_buffer(*this, uncycled, camera);
		return 1;
	}
};

static ScanZBufferOp z_buffer;

int CameraDispatcher::new_clip(CameraCoords& coords)
{
	start_autostop_timer(new_clip, STAT_CAMERA);
	if(!camera_enable_clipping || !(input_mode == BodyInput || input_mode == TrackInput || input_mode == SplineInput || (input_mode == StopTimeInput && camera_mode == CAM_MODE_SLOPE_ROTATE)))
		return 0;

	if(clipping_to_reuse_calculated){
		clipping_to_reuse_calculated--;
		coords.cameraDistance -= clipped_coords_to_reuse.cameraDistance;
		coords.angles -= clipped_coords_to_reuse.angles;
		restriction(coords);
		return 1;
		}
	
	Vect3f origin = coords.position;	
	float distance = (coords.cameraDistance + camera_clip_distance_delta)*camera_clip_distance_factor;
	Vect3f offset_(0, -coords.CenteringDelta*distance, distance);
	Vect3f final1, final2;
	float z_max = coords.cameraDistance;
	float delta_angle = atan(SCAN_X_MAX/z_max);
	(Mat3f(M_PI/2, X_AXIS)*Mat3f(G2R(coords.angles.z) - delta_angle, Z_AXIS)).invXform(offset_, final1);
	(Mat3f(M_PI/2, X_AXIS)*Mat3f(G2R(coords.angles.z) + delta_angle, Z_AXIS)).invXform(offset_, final2);
	Vect3f d_origin = final1 + final2;
	Vect3f d_final1(-final1.y, final1.x, 0);
	Vect3f d_final2(final2.y, -final2.x, 0);
	d_origin.normalize(-camera_clip_delta_origin);
	d_final1.normalize(camera_clip_delta_final);
	d_final2.normalize(camera_clip_delta_final);
	Vect3f origin1 = origin + d_origin + d_final1;
	Vect3f origin2 = origin + d_origin + d_final2;
	final1 += origin + d_final1;
	final2 += origin + d_final2;
	int pX[4], pY[4];
	pX[0] = origin1.xi(); pY[0] = origin1.yi();
	pX[1] = final1.xi(); pY[1] = final1.yi();
	pX[2] = final2.xi(); pY[2] = final2.yi();
	pX[3] = origin2.xi(); pY[3] = origin2.yi();

	Mat3f sAg = Mat3f(G2R(MAX_SLOPE_ANGLE)/2 - atan(coords.CenteringDelta), X_AXIS)*Mat3f(G2R(coords.angles.z), Z_AXIS);
	z_buffer.set(z_max, MatXf(sAg, origin), ivsCamera);

	start_timer(tri_map_scan_poly, STAT_CAMERA);
	tri_map -> scan_poly(pX, pY, 4, z_buffer);
	stop_timer(tri_map_scan_poly, STAT_CAMERA);

	clipped_coords_to_reuse = coords;

	Vect3f offset(0, -coords.CenteringDelta*coords.cameraDistance, coords.cameraDistance);
	coords.rot().invXform(offset);
	float psi_prev = offset.psi();
	sAg.xform(offset);
	if(z_buffer.find_nearest_point(offset, flags_ & EnableSlope )){
		sAg.invXform(offset);
		coords.angles.x = fabs(getDist_f(psi_prev, offset.psi(), 2*M_PI)) < M_PI/2 ? R2G(offset.theta() + atan(coords.CenteringDelta)) : R2G(-offset.theta() + atan(coords.CenteringDelta));
		if(flags_ & EnableSlope && R2G(offset.theta()) > camera_min_slope_angle_to_change_psi) 
			coords.angles.z = R2G(M_PI/2 - offset.psi());
		coords.rot().xform(offset);
		if(coords.cameraDistance > offset.z)
			coords.cameraDistance = offset.z;
		}

	clipped_coords_to_reuse -= coords;
	clipping_to_reuse_calculated = camera_new_clip_reuse_clipping_number;

	restriction(coords);
	return 1;
}


class TestOp
{
public:

	int operator()(const Triangle& triangle, float dx, float dy)
	{
		if(!(triangle.property() & Triangle::ForRayCasting))
			return 1;
		show_map_line(triangle.vertices()[0], triangle.vertices()[1], GREEN);
		show_map_line(triangle.vertices()[1], triangle.vertices()[2], GREEN);
		show_map_line(triangle.vertices()[2], triangle.vertices()[0], GREEN);
		return 1;
	}
};

void CameraDispatcher::show()
{
	if(camera_show_track)
		track_spline.show();

	if(camera_show_clip_buffer){
		Vect3f origin = currentCoords.position;	
		float distance = (currentCoords.cameraDistance + camera_clip_distance_delta)*camera_clip_distance_factor;
		Vect3f offset_(0, -currentCoords.CenteringDelta*distance, distance);
		Vect3f final1, final2;
		float z_max = currentCoords.cameraDistance;
		float delta_angle = atan(SCAN_X_MAX/z_max);
		(Mat3f(M_PI/2, X_AXIS)*Mat3f(G2R(currentCoords.angles.z) - delta_angle, Z_AXIS)).invXform(offset_, final1);
		(Mat3f(M_PI/2, X_AXIS)*Mat3f(G2R(currentCoords.angles.z) + delta_angle, Z_AXIS)).invXform(offset_, final2);
		Vect3f d_origin = final1 + final2;
		Vect3f d_final1(-final1.y, final1.x, 0);
		Vect3f d_final2(final2.y, -final2.x, 0);
		d_origin.normalize(-camera_clip_delta_origin);
		d_final1.normalize(camera_clip_delta_final);
		d_final2.normalize(camera_clip_delta_final);
		Vect3f origin1 = origin + d_origin + d_final1;
		Vect3f origin2 = origin + d_origin + d_final2;
		final1 += origin + d_final1;
		final2 += origin + d_final2;
		int pX[4], pY[4];
		pX[0] = origin1.xi(); pY[0] = origin1.yi();
		pX[1] = final1.xi(); pY[1] = final1.yi();
		pX[2] = final2.xi(); pY[2] = final2.yi();
		pX[3] = origin2.xi(); pY[3] = origin2.yi();

		for(int i = 0; i < 4; i++)
			show_map_line(Vect3f(pX[i], pY[i], 0), Vect3f(pX[(i + 1)%4], pY[(i + 1)%4], 0), RED);

		{
			TestOp op;
			tri_map -> scan_poly(pX, pY, 4, op);
		}

		if(observer){
			show_vector(observer -> R(), XCOL(GREEN));
			show_map_circle(observer -> R(), 4, XCOL(GREEN));
			}
		show_vector(currentCoords.position, YELLOW);
		show_map_circle(currentCoords.position, 4, YELLOW);
		show_vector(targetCoords().position, XCOL(CYAN));
		show_map_circle(targetCoords().position, 4, XCOL(CYAN));

		z_buffer.draw();
		}
}	

void CameraDispatcher::write_state(XBuffer& buf)
{
	char* input_modes[InputModeMax] = {
		"ConstInput", 
		"BodyInput",
		"TrackInput",
		"SplineInput",
		"StopTimeInput",
		"MakeTurnInvitationInput",
		"FlyWhenStopTimeInput",
		"StartTimeInput", 
		"MAX_Input"
		};
	buf < "input_mode: " < input_modes[input_mode] < "\n";	
	//buf < "flags_: " <= flags_ < "\n";	
	//buf < "clip_mode: " <= clip_mode < "\n";	
	//buf < "delta_theta: " <= delta_theta < "\n";	
	//buf < "search_hole_delta: " <= search_hole_delta < "\n";	
	//buf < "camera_metric: " <= camera_metric < "\n";	
	buf < "camera_pos: " <= currentCoords.position;	
	buf < "camera_psi: " <= currentCoords.angles.z < "\n";	
	buf < "camera_theta: " <= currentCoords.angles.x < "\n";	
	buf < "camera_theta_target: " <= targetCoords().angles.x < "\n";	
	buf < "camera_dist: " <= currentCoords.cameraDistance < "\n";	
	buf < "camera_dist_target: " <= targetCoords().cameraDistance < "\n";	
	buf < "is_interpolating_timer: " <= is_interpolating_timer() < "\n";	
	buf < "rotation_switch_timer: " <= rotation_switch_timer() < "\n";	
	extern int allow_transparency_of_bridges_by_camera;
	buf < "allow_transparency_of_bridges_by_camera: " <= allow_transparency_of_bridges_by_camera < "\n";	
	buf < "S2G last result: " <= S2G_last_result;	
	buf < "Camera Matrix.rot:\n" <= ivsCamera -> GetMatrix().rot();
	buf < "Camera Matrix.trans: " <= ivsCamera -> GetMatrix().trans();
	buf < "Camera Pos: " <= ivsCamera -> GetPos();
	buf < "currentCoords.position: " <= currentCoords.position;
	buf < "currentCoords.angles: " <= currentCoords.angles;
	extern Vect3f mi_angles;
	buf < "mi_angles: " <= mi_angles;
}	


///////////////////////////////////////////////////////////////////////////////////////////////
//		ZBuffer's definitions
///////////////////////////////////////////////////////////////////////////////////////////////
ZBuffer::ZBuffer()
{ 
	y1 = 0;
	reset(512); 
}

void ZBuffer::reset(float z_max)
{
	for(int i =0; i < xy_size; i++)
		z_buffer[i] = int_max;

	x_focus = (x_max - 2)*z_max/SCAN_X_MAX; 
	y_focus = (y_max - 2)/tan(G2R(MAX_SLOPE_ANGLE)/2 + atan(centering_delta_default)); 

	x_delta = round(x_focus*camera_new_clip_x_size/camera_new_clip_average_distance);
	y_delta = round(y_focus*camera_new_clip_y_size/camera_new_clip_average_distance);

	Vect3f normal;
	
	normal.set(-x_focus, 0, x_max - 1);
	normal.normalize();
	vis_pyramid[0].set(normal, Vect3f::ZERO);

	normal.set(x_focus, 0, -x_min);
	normal.normalize();
	vis_pyramid[1].set(normal, Vect3f::ZERO);

	normal.set(0, -y_focus, y_max - 1);
	normal.normalize();
	vis_pyramid[2].set(normal, Vect3f::ZERO);

	normal.set(0, y_focus, -y_min);
	normal.normalize();
	vis_pyramid[3].set(normal, Vect3f::ZERO);
}

void ZBuffer::draw()
{
// TODO: @caiiiycuk inevstigate this
#ifdef WTF
	HDC hdc;
	GetBackBufferDC(&hdc);
	int y;
	for(y = y_min; y < y_max; y++)
		for(int x = x_min; x < x_max; x++){
			SetPixel(hdc, x + 400, y + 300, Z(x, y) == int_max ? WHITE : Z(x, y));
			SetPixel(hdc, x + 402 + x_size, y + 300, Z(x, y) == int_max ? WHITE : Z(x, y));
			}

	int xb = x1 - x_delta; if(xb < x_min) xb = x_min;
	int xe = x1 + x_delta; if(xe >= x_max) xe = x_max - 1;
	int yb = y1 - y_delta; if(yb < y_min) yb = y_min;
	int ye = y1 + y_delta; if(ye >= y_max) ye = y_max - 1;
	for(y = yb; y < ye; y++)
		for(int x = xb; x < xe; x++)
			SetPixel(hdc, x + 402 + x_size, y + 300, MAGENTA);

	SetPixel(hdc, x0 + 402 + x_size, y0 + 300, BLUE);
	SetPixel(hdc, x0 + 403 + x_size, y0 + 300, BLUE);
	SetPixel(hdc, x0 + 402 + x_size, y0 + 301, BLUE);
	SetPixel(hdc, x0 + 401 + x_size, y0 + 300, BLUE);
	SetPixel(hdc, x0 + 402 + x_size, y0 + 299, BLUE);

	SetPixel(hdc, x1 + 402 + x_size, y1 + 300, GREEN);
	SetPixel(hdc, x1 + 403 + x_size, y1 + 300, GREEN);
	SetPixel(hdc, x1 + 402 + x_size, y1 + 301, GREEN);
	SetPixel(hdc, x1 + 401 + x_size, y1 + 300, GREEN);
	SetPixel(hdc, x1 + 402 + x_size, y1 + 299, GREEN);

	ReleaseBackBufferDC(hdc);
#endif
}

inline int ZBuffer::test_point(int x, int y, int& dist)
{
	int d;
	if(Z(x, y) != int_max || dist < (d = camera_z_buffer_x_dominance*sqr(x - x0) + sqr(y - y0)))
		return 0;
	int xb = x - x_delta; if(xb < x_min) xb = x_min;
	int xe = x + x_delta; if(xe > x_max) xe = x_max;
	int yb = y - y_delta; if(yb < y_min) yb = y_min;
	int ye = y + y_delta; if(ye > y_max) ye = y_max;
	for(int yy = yb; yy < ye - 1; yy++)
		for(int xx = xb; xx < xe - 1; xx++)
			if(Z(xx, yy) != int_max)
				return 0;
	dist = d;
	x1 = x;
	y1 = y;
	return 1;
}

int ZBuffer::find_nearest_point(Vect3f& v, int enable_slope)
{
	start_autostop_timer(find_nearest_point, STAT_CAMERA);

	x0 = round(v.x*x_focus/v.z);
	y0 = round(v.y*y_focus/v.z);

	//assert(x0 >= x_min && x0 < x_max && y0 >= y_min && y0 < y_max);

	if(x0 < x_min)
		x0 = x_min;
	if(x0 >= x_max)
		x0 = x_max - 1;
	if(y0 < y_min)
		y0 = y_min;
	if(y0 >= y_max)
		y0 = y_max - 1;

	x1 = x0; 
	y1 = y0;
	int dist = int_max;
	int log = test_point(x1, y1, dist);
	if(!log && enable_slope){
		for(int y = y_min; y < y_max - 1; y++)
			for(int x = x_min; x < x_max - 1; x++)
				log |= test_point(x, y, dist);
		}

	if(log){
		v.x = x1*v.z/x_focus;
		v.y = y1*v.z/y_focus;
		return !(x1 == x0 && y1 == y0);
		}

	float z = Z(x0, y0) - camera_z_buffer_delta_z;
	if(z < camera_z_buffer_z_min)
		z = camera_z_buffer_z_min;
	float k = z/v.z;
	v.x *= k;
	v.y *= k;
	v.z = z;
	return 2;
}

void ZBuffer::draw_triangle(Vect3f vv[3])
{
	start_autostop_timer(draw_triangle, STAT_CAMERA);
	clip_triangle(vv, 0);
}

void ZBuffer::clip_triangle(Vect3f vv[3], int side)
{
	float eps = 0.00001f;
	for(int j = side; j < 4; j++){
		Plane& p = vis_pyramid[j];
		int in = 0;
		int out = 0;
		int out_cnt = 0;
		float dd[3];
		for(int i = 0; i < 3; i++)
			if((dd[i] = p.dist(vv[i])) < eps){ // �������
				out_cnt++;
				out = i;
				}
			else
				in = i;
		switch(out_cnt){
			case 0: // ���� ������
				break;
			case 1: { // 1 ����� ������� (out)
				int i1 = (out + 1) % 3;
				int i2 = (out + 2) % 3;
				float d0 = -dd[out];
				Vect3f& v0 = vv[out];
				//xassert(d0 + dd[i1] > FLT_EPS);
				//xassert(d0 + dd[i2] > FLT_EPS);
				Vect3f v1 = v0 + (vv[i1] - v0)*(d0 + dd[i1] > FLT_EPS ? d0/(d0 + dd[i1]) : 0);
				Vect3f v2 = v0 + (vv[i2] - v0)*(d0 + dd[i2] > FLT_EPS ? d0/(d0 + dd[i2]) : 0);
				Vect3f vv2[3];
				if(v1.distance2(vv[i2]) < v2.distance2(vv[i1])){ // ��������� ����� v1 - vv[i2]
					vv2[0] = v1;
					vv2[1] = vv[i2];
					vv2[2] = v2;
					clip_triangle(vv2, j + 1);
					vv[out] = v1;
					}
				else{  // ��������� ����� v2 - vv[i1]
					vv2[0] = v1;
					vv2[1] = vv[i1];
					vv2[2] = v2;
					clip_triangle(vv2, j + 1);
					vv[out] = v2;
					}
				}
				break;
			case 2: { // 1 ����� ������ (in)
				int i1 = (in + 1) % 3;
				int i2 = (in + 2) % 3;
				float d0 = dd[in];
				Vect3f& v0 = vv[in];
				//xassert(d0 - dd[i1] > FLT_EPS);
				//xassert(d0 - dd[i2] > FLT_EPS);
				vv[i1] = v0 + (vv[i1] - v0)*(d0 - dd[i1] > FLT_EPS ? d0/(d0 - dd[i1]) : 0);
				vv[i2] = v0 + (vv[i2] - v0)*(d0 - dd[i2] > FLT_EPS ? d0/(d0 - dd[i2]) : 0);
				}
				break;
			case 3: // ���� �������
				return;
			}
		}

	start_autostop_timer(clip_triangle1, STAT_CAMERA);

	for(int i = 0; i < 3; i++){
		if(vv[i].z <= 0)
			return;
		assert(vv[i].z > 0);
		vv[i].x *= x_focus/vv[i].z;
		vv[i].y *= y_focus/vv[i].z;
		if(!(vv[i].xi() >= x_min && vv[i].xi() <= x_max && vv[i].yi() >= y_min && vv[i].yi() <= y_max))
			return;
		assert(vv[i].xi() >= x_min && vv[i].xi() <= x_max && vv[i].yi() >= y_min && vv[i].yi() <= y_max);
		}

	// ����� ������� �������
	if(vv[0].y > vv[1].y){
		Vect3f v = vv[0]; vv[0] = vv[1]; vv[1] = vv[2]; vv[2] = v;
		if(vv[0].y > vv[1].y){
			Vect3f v = vv[0]; vv[0] = vv[1]; vv[1] = vv[2]; vv[2] = v;
			}
		}
	else
		if(vv[0].y > vv[2].y){
			Vect3f v = vv[0]; vv[0] = vv[2]; vv[2] = vv[1]; vv[1] = v;
			}

	// ������������ �����������
	int cross_z = (vv[1].x - vv[0].x)*(vv[2].y - vv[0].y) - (vv[1].y - vv[0].y)*(vv[2].x - vv[0].x);
	if(fabs((float)cross_z) < 1e-5f)
		return;
	if(cross_z < 0)
		swap(vv[1], vv[2]);

	int exit_log = 0; // Two passes

	start_autostop_timer(clip_triangle2, STAT_CAMERA);

	#define DIV(a, b)	(((a) << 16)/(b))

	int al, ar, d, where;
	int zkl, zkr;
	int Y = vv[0].yi(); 
	if(Y >= y_max)
		return;
	int xl = vv[0].xi();
	int zl = vv[0].zi();
	int bl = vv[2].yi() - Y;
	int br = vv[1].yi() - Y;
	if(bl){
		al = DIV(vv[2].xi() - xl, bl);
		zkl = DIV(vv[2].zi() - zl, bl);
		}
	if(br){
		ar = DIV(vv[1].xi() - xl, br);
		zkr = DIV(vv[1].zi() - zl, br);
		}
	int xr = xl = (xl << 16) + (1 << 15);
	int zr = zl = (zl << 16) + (1 << 15);

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
			if(Y >= y_max)
				return;
			if(Y < y_min){
				Y++; continue;
				}

			int x1 = xl >> 16;
			int x2 = xr >> 16;

//			assert(x2 - x1 >= -1);
//			if(x1 > x2)
//				swap(x1, x2);

			if(x1 > x2 || x2 < x_min || x1 >= x_max){
				xl += al; xr += ar; continue;
				}

			int len = x2 - x1;
			int zfk = len ? (zr - zl)/len : 0;
			int zf = zl;

			if(x1 < x_min){
				zf += zfk*(x_min - x1);
				x1 = x_min;
				}
			if(x2 >= x_max)
				x2 = x_max;

			while(x1 < x2){
				int z = zf >> 16;
				if(Z(x1, Y) > z)
					Z(x1, Y) = z;
				x1++;
				zf += zfk;
				}
			Y++;
			xl += al;
			xr += ar;

			zl += zkl;
			zr += zkr;
			}

		if(where){
			if(exit_log) 
				return;
			exit_log = 1;

			br -= bl;
			bl = vv[1].yi() - Y;
			if(bl){
				xl = vv[2].xi();
				al = DIV(vv[1].xi() - xl, bl);
				xl = (xl << 16) + (1 << 15);

				zl = vv[2].zi();
				zkl = DIV(vv[1].zi() - zl, bl);
				zl = (zl << 16) + (1 << 15);
				}
			}
		else {
			if(exit_log) 
				return;
			exit_log = 1;

			bl -= br;
			br = vv[2].yi() - Y;
			if(br){
				xr = vv[1].xi();
				ar = DIV(vv[2].xi() - xr, br);
				xr = (xr << 16) + (1 << 15);

				zr = vv[1].zi();
				zkr = DIV(vv[2].zi() - zr, br);
				zr = (zr << 16) + (1 << 15);
				}
			}
		}

	#undef DIV
}


