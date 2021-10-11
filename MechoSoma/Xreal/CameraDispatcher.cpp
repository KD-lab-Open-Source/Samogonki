#include "StdAfx.h"

/* ---------------------------- INCLUDE SECTION ----------------------------- */

#include "mechosoma.h"
#include "race.h"
#include "controls.h"
#include "RenderDevice.h"

#include "CameraDispatcher.h"
#include "mesh3ds.h"
#include "params.h"
#include "SimpleClip.h"
#include "CameraPrm.h"
#include "Statistics.h"
#include "MultibodyContact.h"
#include "DebugPrm.h"

/* ----------------------------- EXTERN SECTION ----------------------------- */
extern cInterfaceVisGeneric	*gb_IVisGeneric;


class FreeFlyBody : public Body 
{
	Vect3f angles;
public:
	FreeFlyBody();
	void start(CameraCoords coords);
	void evolve(CameraCoords& coords, float dt);
	void project_coords(CameraCoords& coords);
};

/* --------------------------- DEFINITION SECTION --------------------------- */

char* mch_cameraINI = "RESOURCE\\ISCREEN\\camera.ini";

char* mch_cameraModeID[CAM_MODE_MAX] = 
{
	"flat",
	"fixed_3d",
	"free_3d",
	"rotate_3d",
	"slope_rotate_3d",
	"stop_time"
};

CameraDispatcher* camera_dispatcher = 0;
CameraDispatcher* camera_dispatcher2 = 0;

// Camera's params
float hCamera=512;
float mchCameraAX = 0.0f;
float mchCameraAY = 0.0f;
float mchCameraAZ = 0.0f;

int allow_transparency_of_bridges_by_camera = 1;

float realtive_mouse_factor = 1;

void clear_mouse_movement()
{
	XGR_MouseObj.SetPos(XGR_MAXX/2, XGR_MAXY/2);
}

Vect3f retrieve_mouse_movement()
{
	Vect3f pos(XGR_MAXX/2, XGR_MAXY/2, 0);
	Vect3f m_pos(XGR_MouseObj.PosX, XGR_MouseObj.PosY, 0);
	XGR_MouseObj.SetPos(pos.x, pos.y);
	return (m_pos - pos)*realtive_mouse_factor;
}

////////////////////////////////////////////////////////////////////////////////////
//	CameraModeInfo
////////////////////////////////////////////////////////////////////////////////////

void CameraModeInfo::init(char* iniFile,char* id)
{
	mapMode = atoi(getIniKey(iniFile, id, "map_mode"));

	zPrm.init(iniFile, id, "z");

	slopeMode = atoi(getIniKey(iniFile, id, "slope"));
	slopePrm.init(iniFile,id,"slope");

	max_slopeDelta = 0.0f;

	rotateMode = atoi(getIniKey(iniFile, id, "rotate"));
	rotatePrm.init(iniFile, id, "rotate");
}

void CameraModeInfo::save(char* iniFile,char* id)
{
	zPrm.save(iniFile, id, "z");
	if(slopeMode)
		slopePrm.save(iniFile,id,"slope");
}

static float init_prm(char* iniFile,char* section,char* id, char* postfix)
{
	XBuffer XBuf;
	XBuf < id < postfix;
	char* str = getIniKey(iniFile,section,XBuf.address());
	if(strlen(str))
		return atof(str);
	else
		return atof(getIniKey(iniFile, "general" ,XBuf.address()));
}

void CameraPrm::init(char* iniFile,char* section,char* id)
{
	Max = init_prm(iniFile,section,id, "_max");
	Min = init_prm(iniFile,section,id, "_min");
	Delta = init_prm(iniFile,section,id, "_delta");
	Default = init_prm(iniFile,section,id, "_default");
}

void CameraPrm::save(char* iniFile,char* section,char* id)
{
	if(init_prm(iniFile,section,id, "_default") == Default)
		return;
	XBuffer XBuf;
	XBuf < id < "_default"; 
	XBuffer Val;
	Val <= Default;
	putIniKey(iniFile,section,XBuf.address(), Val);
}


////////////////////////////////////////////////////////////////////////
//		CameraDispatcher
////////////////////////////////////////////////////////////////////////

CameraDispatcher::CameraDispatcher(cUnknownClass* ivsRenderDevice_)
{
	ivsRenderDevice = ivsRenderDevice_;
	ivsCamera = (cCamera*)gb_IVisGeneric->CreateCamera();
	gb_IVisGeneric->AttachCamera(ivsCamera);		// присоединение камеры к активной сцене
	gb_IVisGeneric->SetCameraAttribute(ivsCamera,ATTRIBUTE_CAMERA_PERSPECTIVE_WORLD_SHARE_CUTTING); // сферичность вывода и перспектива
	gb_IVisGeneric->SetCameraPosition(ivsCamera,&Vect3f(0,0,512),&Vect3f(0,0,0));
	gb_IVisGeneric->SetCameraFrustum(ivsCamera,	// устанавливаетс€ пирамида видимости
		&Vect2f(0.5f,0.5f),						// центр камеры
		&sRectangle4f(-0.499f,-0.499f,0.499f,0.499f),		// видима€ область камеры
		&Vect2f(1.0f,1.0f),						// фокус камеры
		&Vect2f(10.0f,3000.0f),					// ближайший и дальний z-плоскости отсечени€
		&Vect2f(0.2f,0.90f));						// zNear и zFar дл€ мапировани€ в zBuffer
	gb_IVisGeneric->AttachCameraViewPort(ivsCamera, ivsRenderDevice);

	centeringDeltaFactor = 1;

	for(int i = 0; i < CAM_MODE_MAX; i ++)
		modes[i].init(mch_cameraINI,mch_cameraModeID[i]);
	
	free_fly_body = 0;
	
	clear();
}

void CameraDispatcher::activate()
{
	gb_IVisGeneric->AttachCamera(ivsCamera);	
}
void CameraDispatcher::deactivate()
{
	gb_IVisGeneric->DetachCamera(ivsCamera);	
}

void CameraDispatcher::setWindow(const mchGameWindow* gWnd)
{
	Vect2f Center(((float)gWnd -> CenterX)/XGR_MAXX,((float)gWnd->CenterY)/XGR_MAXY);
	Vect2f Size((float)gWnd->SizeX2/XGR_MAXX,(float)gWnd->SizeY2/XGR_MAXX);
	gb_IVisGeneric->SetCameraFrustum(ivsCamera, &Center, &sRectangle4f(-Size.x,-Size.y,+Size.x,+Size.y), 0, 0, 0);
	centeringDeltaFactor = (float)gWnd->SizeY/XGR_MAXY;
}

CameraDispatcher::~CameraDispatcher()
{
	if(free_fly_body)
		delete free_fly_body;
	gb_IVisGeneric->ReleaseCamera(ivsCamera); // удаление ненужной камеры
}

void CameraDispatcher::clear(void)
{
	for(int i = 0; i < CAM_MODE_MAX; i ++)
		modes[i].save(mch_cameraINI, mch_cameraModeID[i]);

	observer = 0;
	advanced_observer = 0;
	fly_around_angle =  fly_around_angle_incr = 0;
	observer_transparent = 0;
	clipping_to_reuse_calculated = 0;
	mouse_3D_position.set(0.f);
	
	interpolate_position_timer.stop();
	rotation_switch_timer.stop();
	interpolate_position_timer.stop();

	flags_ = EnableControl;
	input_mode = ConstInput;
	clip_mode = InitClipping;

	realtive_mouse_factor = 1024.f/XGR_MAXX;

	if(free_fly_body){
		delete free_fly_body;
		free_fly_body = 0;
		}
	time_speed = 1;
	
	setMode(CAM_MODE_SLOPE_ROTATE);
}

void CameraDispatcher::saveControls(XBuffer& buf)
{
	for(int i = 0; i < CAM_MODE_MAX; i ++)
		buf < modes[i].zPrm.Default < modes[i].slopePrm.Default;
}

void CameraDispatcher::loadControls(XBuffer& buf)
{
	for(int i = 0; i < CAM_MODE_MAX; i ++)
		buf > modes[i].zPrm.Default > modes[i].slopePrm.Default;
}

void CameraDispatcher::ConvertorWorldToViewPort(const Vect3f& vw, Vect3f& pv, Vect3f& pe)
{
	gb_IVisGeneric->ConvertorWorldToViewPort(ivsCamera, &vw, &pv, &pe);
}

float CameraDispatcher::getScale(const Vect3f& vw)
{
	Vect3f pv, pe;
	ConvertorWorldToViewPort(vw, pv, pe);
	return pv.z > 0 ? ivsCamera -> GetFocus().x*640.0f/pv.z : 1;
}

Vect3f CameraDispatcher::Screen2World(int xs, int ys)
{
	S2G_last_result = ivsCamera -> Screen2World(xs - XGR_MAXX/2, ys - XGR_MAXY/2);
	return CYCLE(S2G_last_result);
}


void CameraDispatcher::setMode(int m, int interpolate)
{
	camera_mode = (CameraMode)m;

	if(interpolate)
		is_interpolating_timer.start(camera_setMode_interpolation_time);
	else
		is_interpolating_timer.stop();

	targetCoords() = input();
	
	if(!interpolate)
		currentCoords = targetCoords();

	velocity = CameraCoords();
	
	flags_ &= ~(Perspective | EnableRotate | EnableSlope | EnableRoll | Centering);
	switch(m){
		case CAM_MODE_FLAT:
			flags_ |= Centering;
			break;
		case CAM_MODE_FIXED_3D:
			flags_ |= Perspective | EnableRotate;
			break;
		case CAM_MODE_ROTATE:
		case CAM_MODE_SLOPE_ROTATE:
			flags_ |= Perspective | EnableRotate | EnableSlope | EnableRoll;
			break;
		case CAM_MODE_STOP_TIME:
			flags_ |= Perspective | EnableRotate | Centering;
			break;
		}

	if(flags_ & Perspective) 
		gb_IVisGeneric->SetCameraAttribute(ivsCamera, ATTRIBUTE_CAMERA_PERSPECTIVE);
	else 
		gb_IVisGeneric->ClearCameraAttribute(ivsCamera, ATTRIBUTE_CAMERA_PERSPECTIVE);
	
	setIvsCamera(currentCoords);
}


///////////////////////////////////////////////////////////////////////////////////////
//		Camera Clipping
///////////////////////////////////////////////////////////////////////////////////////
class RayCaster
{
	Vect3f observer;
	Vect3f observer_offset;
	float psi, theta, roll, z_distance, CD;
	float dist_min;
	cCamera* camera;
	Vect3f vector_min;

public:

	RayCaster(const Vect3f& observer_, const Vect3f& observer_offset_, float psi_, float theta_, float roll_, float z_distance_, float CD_, cCamera* camera_) 
	: observer(observer_ + observer_offset_), observer_offset(observer_offset_), psi(psi_), theta(theta_), roll(roll_), z_distance(z_distance_), CD(CD_), camera(camera_)
	{
		init();
		CYCLE(observer);
	}
	
	void init()
	{
		dist_min = 1e+10f;
		vector_min.set(psi, theta, z_distance);
	}

	float metric(float ps, float th, float rad)
	{
//		return (fabs(getDist(ps, psi, 2*M_PI)) + fabs(th - theta))*z_distance + fabs(rad - z_distance);
		return (camera_theta_dominance*sqr(getDist(ps, psi, 2*M_PI)) + sqr(th - theta))*sqr(z_distance) + sqr(rad - z_distance);
	}

	int cast(float ps, float th)
	{
		if(th < 0)
			return 1;
		Mat3f R = Mat3f(th, X_AXIS)*Mat3f(roll, Y_AXIS)*Mat3f(ps, Z_AXIS);
		Vect3f offset(0, -CD*z_distance, z_distance);
		R.invXform(offset);
		offset -= observer_offset;
		start_timer(cast_ray, STAT_CAMERA);
		int cast_log = tri_map -> cast_ray(observer, offset, clip_ray_radius, camera);
		stop_timer(cast_ray, STAT_CAMERA);
		offset += observer_offset;
		R.xform(offset);
		float rad = offset.z;
		float d = metric(ps, th, rad);
		if(cast_log)
			d += 100000.f;
		if(d < dist_min){
			dist_min = d;
			vector_min.set(ps, th, rad);
			}
		return cast_log;
	}
	
	float best_psi() const { return vector_min.x; }
	float best_theta() const { return vector_min.y; }
	float best_radius() const { return vector_min.z; }
	float best_factor() const { return dist_min; }
};
	
int CameraDispatcher::clip(CameraCoords& coords)
{
	camera_metric = 0;
	if(!camera_enable_clipping || input_mode != BodyInput)
		return 0;
    
	float psi_desired = G2R(coords.angles.z);
	float theta_desired = G2R(coords.angles.x);
	float radius_desired = coords.cameraDistance;
	float psi = G2R(coords.angles.z);
	float theta = G2R(clipCoords.angles.x);

	RayCaster caster( coords.position, Vect3f::ZERO, psi_desired, theta_desired, G2R(coords.angles.y), radius_desired, coords.CenteringDelta, ivsCamera); 

	switch(clip_mode){
		case ToDesiredByTheta:
			if(fabs(theta_desired - theta) > delta_theta)
				theta += SIGN(theta_desired - theta)*delta_theta;
			else
				theta = theta_desired;
			if(!caster.cast(psi, theta)){
				if(clip_counter++ && (delta_theta *= 2) > camera_angle_step)
					delta_theta = camera_angle_step;
				if(theta == theta_desired)
					return 0;
				}
			else{
				clip_mode = FromDesiredByTheta;
				delta_theta = clip_counter > 5 ? camera_angle_step : delta_theta / 2;
				clip_counter = 0;
				}
			break;

		case FromDesiredByTheta:
			if(caster.cast(psi, theta - SIGN(theta_desired - theta)*delta_theta)){
				if(clip_counter++){
					if((delta_theta *= 2) > camera_angle_step)
						delta_theta = camera_angle_step;
					if(clip_counter > from_desired_search_attempts){
						clip_mode = SearchHole;
						search_hole_delta = 0;			
						}
					}
				}
			else{
				clip_mode = ToDesiredByTheta;
				clip_counter = 0;
				delta_theta /= 2;
				}
			break;

		case SearchHole:
			if((search_hole_delta += camera_angle_step) > M_PI/2)
				search_hole_delta = 0;			
			if(!caster.cast(psi, theta - search_hole_delta) || !caster.cast(psi, theta + search_hole_delta)){
				clip_mode = ToDesiredByTheta;
				delta_theta = camera_angle_step/2;
				clip_counter = 0;
				}
			else{
				caster.init();
				caster.cast(psi_desired, theta_desired);
				coords.cameraDistance = caster.best_radius();
				}
			break;

		case InitClipping:
			clipCoords = coords;
			clip_mode = ToDesiredByTheta;
			delta_psi = delta_theta = camera_angle_step;
			clip_counter = 0;
			return 0;
		}

	coords.angles.z = R2G(caster.best_psi());
	coords.angles.x = R2G(caster.best_theta());
	camera_metric = caster.best_factor();
	restriction(coords);
	if(clip_mode != SearchHole)
		clipCoords = coords;

	return 1;
}


////////////////////////////////////////////////////////////////////////
//	CameraCoords
////////////////////////////////////////////////////////////////////////
CameraCoords& CameraCoords::operator += (const CameraCoords& c)
{
	position += c.position;
	angles += c.angles;
	cameraDistance += c.cameraDistance;
	CenteringDelta += c.CenteringDelta;
	time += c.time;
	return *this;
}

CameraCoords& CameraCoords::operator -= (const CameraCoords& c)
{
	position -= c.position;
	angles -= c.angles;
	cameraDistance -= c.cameraDistance;
	CenteringDelta -= c.CenteringDelta;
	time -= c.time;
	return *this;
}

CameraCoords CameraCoords::operator + (const CameraCoords& c) const
{
	return CameraCoords(position + c.position, angles + c.angles, cameraDistance + c.cameraDistance, CenteringDelta + c.CenteringDelta, time + c.time);
}

CameraCoords CameraCoords::operator - (const CameraCoords& c) const
{
	return CameraCoords(position - c.position, angles - c.angles, cameraDistance - c.cameraDistance, CenteringDelta - c.CenteringDelta, time - c.time);
}

CameraCoords CameraCoords::operator * (const CameraCoords& c) const
{
	return CameraCoords(position * c.position, angles * c.angles, cameraDistance * c.cameraDistance, CenteringDelta * c.CenteringDelta, time * c.time);
}

CameraCoords CameraCoords::operator * (float t) const
{
	return CameraCoords(position * t, angles * t, cameraDistance * t, CenteringDelta * t, time * t);
}

float CameraCoords::distance(const CameraCoords& c) const 
{
	return ::sqrt(getDist(position, c.position).norm2() + (sqr(G2R(angles.x - c.angles.x)) + sqr(G2R(angles.y - c.angles.y)) + sqr(G2R(getDist(angles.z, c.angles.z, 360))))*cameraDistance*c.cameraDistance + sqr(cameraDistance - c.cameraDistance));
}

CameraCoords& CameraCoords::cycle()
{
	CYCLE(position);
	::cycle(angles.z, 360.f);
	return *this;
}

CameraCoords& CameraCoords::sqrt()
{
	position.x = ::sqrt(position.x);
	position.y = ::sqrt(position.y);
	position.z = ::sqrt(position.z);
	angles.x = ::sqrt(angles.x);
	angles.y = ::sqrt(angles.y);
	angles.z = ::sqrt(angles.z);
	cameraDistance = ::sqrt(cameraDistance);
	CenteringDelta = ::sqrt(CenteringDelta);
	time = ::sqrt(time);
	return *this;
}	

CameraCoords& CameraCoords::point2eye()
{
	Vect3f delta(0, -CenteringDelta*cameraDistance, cameraDistance);
	rot().invXform(delta);
	position += delta;
	return *this;
}

CameraCoords& CameraCoords::eye2point()
{
	Vect3f delta(0, -CenteringDelta*cameraDistance, cameraDistance);
	rot().invXform(delta);
	position -= delta;
	return *this;
}

CameraCoords getDist(const CameraCoords& c1, const CameraCoords& c0)
{
	CameraCoords coords;
	coords.position = getDist(c1.position, c0.position);
	coords.angles.x = c1.angles.x - c0.angles.x;
	coords.angles.y = c1.angles.y - c0.angles.y;
	coords.angles.z = getDist(c1.angles.z, c0.angles.z, 360.f);
	coords.cameraDistance = c1.cameraDistance - c0.cameraDistance;
	coords.CenteringDelta = c1.CenteringDelta - c0.CenteringDelta;
	coords.time = c1.time - c0.time;
	return coords;
}

////////////////////////////////////////////////////////////////////////////////////
//			Control
////////////////////////////////////////////////////////////////////////////////////
void CameraDispatcher::control(CameraCoords& coords)
{
	static int PosZ;
	int config = advanced_observer ? advanced_observer -> get_control_config() : 0;
	if(flags_ & EnableControl){
		if(mchKeyPressed(MCH_KEY_ZOOM_IN, config))
			if(!mchKeyPressed(MCH_KEY_SLOPE_CAM, config)){
				if((coords.cameraDistance = cameraMode().zPrm.Default -= (cameraMode().zPrm.Default + 1)*camera_zoom_sensitivity) < cameraMode().zPrm.Min)
					coords.cameraDistance = cameraMode().zPrm.Default = cameraMode().zPrm.Min;
				}
			else 
				if((coords.angles.x = cameraMode().slopePrm.Default += (cameraMode().zPrm.Default + 1)*camera_zoom_sensitivity) > cameraMode().slopePrm.Max)
					coords.angles.x = cameraMode().slopePrm.Default = cameraMode().slopePrm.Max;

		if(mchKeyPressed(MCH_KEY_ZOOM_OUT, config))
			if(!mchKeyPressed(MCH_KEY_SLOPE_CAM, config)){
				if((coords.cameraDistance = cameraMode().zPrm.Default += cameraMode().zPrm.Delta) > cameraMode().zPrm.Max)
					coords.cameraDistance = cameraMode().zPrm.Default = cameraMode().zPrm.Max;
				}
			else 
				if((coords.angles.x = cameraMode().slopePrm.Default -= cameraMode().slopePrm.Delta) < cameraMode().slopePrm.Min)
					coords.angles.x = cameraMode().slopePrm.Default = cameraMode().slopePrm.Min;
		if(enable_zoom_by_wheel && PosZ != XGR_MouseObj.PosZ){
			coords.cameraDistance = cameraMode().zPrm.Default -= (XGR_MouseObj.PosZ - PosZ)*camera_mouse_wheel_sensitivity*(cameraMode().zPrm.Default + 1);
			if(coords.cameraDistance > cameraMode().zPrm.Max)
				coords.cameraDistance = cameraMode().zPrm.Default = cameraMode().zPrm.Max;
			if(coords.cameraDistance < cameraMode().zPrm.Min)
				coords.cameraDistance = cameraMode().zPrm.Default = cameraMode().zPrm.Min;
			}
		}
	PosZ = XGR_MouseObj.PosZ;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Body  Input
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CameraDispatcher::setObserver(const Body* obs, int interpolate)
{
	if(obs){
		if(advanced_observer && observer_transparent){
			if(observer -> geometry)
				observer -> geometry -> ClearAttribute(MESH_FIRST_DRAW);
			const_cast<Mechos*>(advanced_observer) -> setPermanentAlpha(1.f);
			observer_transparent = 0;
			}
		observer = obs;
		advanced_observer = dynamic_cast<const Mechos*>(observer);
		}

	if(observer -> geometry)
		observer -> geometry -> SetAttribute(MESH_FIRST_DRAW);
	allow_transparency_of_bridges_by_camera = 1;

	CameraCoords prev_coords = targetCoords();
	input_mode = observer ? BodyInput : ConstInput;
	if(interpolate){
		targetCoords() = prev_coords;
		is_interpolating_timer.start(interpolate == 1 ? camera_setObserver_interpolation_time : interpolate);
		}
	else
		is_interpolating_timer.stop();
	
	targetCoords() = input();
	if(!interpolate){
		currentCoords = targetCoords();
		setIvsCamera(currentCoords);
		}
}

CameraCoords CameraDispatcher::bodyInput()
{
	control(targetCoords());
	targetCoords().angles.x = cameraMode().slopePrm.Default;
	targetCoords().cameraDistance = cameraMode().zPrm.Default;
	targetCoords().CenteringDelta = centering_delta_default*centeringDeltaFactor;

	if(!advanced_observer || !(advanced_observer -> features() & Mechos::ROTATING))
		targetCoords().angles.z = 180.0f + R2G(observer -> psi()) + fly_around_angle + camera_turn_angle_delta;
	fly_around_angle += fly_around_angle_incr;

	Vect3f position = observer -> Rgeometry() - observer -> R();
	observer -> Alg().invXform(position);
	position.y = 0;
	position *= Mat3f(M_PI - G2R(currentCoords.angles.z), Z_AXIS);
	position += observer -> R();

	if(!is_interpolating_timer){
		targetCoords().position = position;
		if(input_mode == BodyInput){
			if(interpolate_position_timer() && !fly_around_angle_incr){ // отставание на споте
				float t = spot_acc_time - interpolate_position_timer(); // 0 - spot_acc_time
				t = t < spot_acc_front ? t/spot_acc_front : 1.f - (t - spot_acc_front)/(spot_acc_time - spot_acc_front); // 0 - 1 - 0
				targetCoords().position += getDist(currentCoords.position, targetCoords().position)*t;
				}
			currentCoords.position = targetCoords().position;
			}
		}
	else
		currentCoords.position = position = currentCoords.position + getDist(position, currentCoords.position)*(evolve_time_step_ms/is_interpolating_timer());

	CameraCoords coords = targetCoords();
	coords.position = position;

	if(advanced_observer){
		//coords.angles.x += R2G(advanced_observer -> pitch());
		//coords.angles.y = -R2G(advanced_observer -> roll());

		if(!camera_disable_zoom_by_speed){
			float v = observer -> averageSpeed() / zoom_by_speed_velocity_max;
			if(v < 0.0f) v = 0.0f;
			if(v > 1.0f) v = 1.0f;
			v = zoom_by_speed*v + 1.f;
			coords.cameraDistance *= v;
			}

		if(!interpolate_position_timer && advanced_observer -> features() & Mechos::ACCELERATED_BY_SPOT)
			interpolate_position_timer.start(spot_acc_time);

		if(advanced_observer -> isFlyingByArcane()){
			if(coords.angles.x > fly_slope)
				coords.angles.x = fly_slope;
			}

		// ѕрозрачность при приближении
		if(currentCoords.cameraDistance < camera_transparensy_distance){
			const_cast<Mechos*>(advanced_observer) -> setPermanentAlpha(pow(currentCoords.cameraDistance/camera_transparensy_distance, camera_transparensy_power));
			observer_transparent = 1;
			}
		else
			if(observer_transparent){
				const_cast<Mechos*>(advanced_observer) -> setPermanentAlpha(1.f);
				observer_transparent = 0;
				}
		}

	return coords;
}	

void CameraDispatcher::runStartScenario()
{
	setMode(CAM_MODE_SLOPE_ROTATE);
	setObserver(0);

	//currentCoords = input();
	
	// 0
	CameraCoords coords = currentCoords;
	coords.angles.x = start_scenario_slope0;
	coords.cameraDistance = start_scenario_distance0;
	coords.time = 0;
	coords_spline.push_back(coords);

	// 0.5
	coords = currentCoords;
	coords.time = start_scenario_time1;
	coords_spline.push_back(coords);

	// 1
	coords = currentCoords;
	coords.angles.z += 90;
	coords.time = start_scenario_time2;
	coords_spline.push_back(coords);
	
	// 2
	coords = currentCoords;
	coords.angles.z += 180;
	coords.time = start_scenario_time2;
	coords_spline.push_back(coords);

	// 3
	coords = currentCoords;
	coords.angles.z += 270;
	coords.time = start_scenario_time2;
	coords_spline.push_back(coords);

	// 4
	coords = currentCoords;
	coords.time = start_scenario_time2;
	coords_spline.push_back(coords);

	// 5
	coords = currentCoords;
	coords.time = start_scenario_time2;
	coords_spline.push_back(coords);

	input_mode = SplineInput;
	next_input_mode = BodyInput;
	targetCoords() = currentCoords = coords_spline();
	setIvsCamera(currentCoords);
}

CameraCoords CameraDispatcher::splineInput()
{
	coords_spline += evolve_time_step_ms;
	if(isInterpolationCompleted())
		switch(next_input_mode){
			case BodyInput:
				setObserver(0, 1);
				break;
			case StopTimeInput:
				setStopTime();
				break;
			default:
				assert(0);
			}
	return targetCoords() = coords_spline();
}	

void CameraDispatcher::runFinishScenario()
{
	setObserver(0);
	fly_around_angle = 0;
	fly_around_angle_incr = finish_fly_angle_increment;
}

void CameraDispatcher::stopFinishScenario()
{
	fly_around_angle = 0;
	fly_around_angle_incr = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//				Const  Input
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CameraDispatcher::setStaticObserver(const Vect3f& point, const Vect3f& angles, float distance, float centering_delta, int interpolation_time)
{
	CameraCoords coords(point, angles, distance, centering_delta, interpolation_time);
//	coords_spline.push_back(coords);

	input_mode = ConstInput;
	targetCoords() = coords;
	if(!interpolation_time){
		currentCoords = targetCoords();// =coords_spline();
		is_interpolating_timer.stop();
		}
	else
		is_interpolating_timer.start(interpolation_time);
	setIvsCamera(currentCoords);
}

void CameraDispatcher::setStaticObserver()
{
	input_mode = ConstInput;
	targetCoords() = currentCoords;
	is_interpolating_timer.stop();
	setIvsCamera(currentCoords);
}


CameraCoords CameraDispatcher::constInput()
{
	if(static_observer_rotation_timer()){
		targetCoords().angles.z += static_observer_rotation_speed;
		currentCoords.angles.z += static_observer_rotation_speed;
		}

	if(is_interpolating_timer())
		currentCoords += getDist(targetCoords(), currentCoords)*(evolve_time_step_ms/is_interpolating_timer());
	else
		currentCoords = targetCoords();
	return currentCoords;
}	

//////////////////////////////////////////////////////////////////////////////////////////
//			Track  Input
//////////////////////////////////////////////////////////////////////////////////////////
void CameraDispatcher::setTrackObserver(mchTrackElement* el, float slope, float distance, float centering_delta)
{
	input_mode = TrackInput;

	if(observer){
		observer -> geometry -> ClearAttribute(MESH_FIRST_DRAW);
		allow_transparency_of_bridges_by_camera = 0;
		}

	TrackIterator ti;
	if(!el)
		ti.set(0);
	else
		ti.set(el);
	track_spline.set(ti);

	setMode(CAM_MODE_SLOPE_ROTATE);

	targetCoords().angles.x = slope;
	targetCoords().cameraDistance = distance;
	targetCoords().CenteringDelta = centering_delta;

	currentCoords = targetCoords() = input();
	setIvsCamera(currentCoords);
}

 CameraCoords CameraDispatcher::trackInput()
{
	targetCoords().position = track_spline();
	targetCoords().position.z += track_fly_z_offset;
	targetCoords().angles.z = R2G(-M_PI/2 - track_spline.deriv().psi());

	float t = 0;
	if(track_fly_enable_binary_search){
		float dt = track_fly_increment_initial;
		CameraCoords coords = targetCoords();
		while(dt > track_fly_increment_epsilon){
			TrackSpline spline = track_spline;
			spline += t + dt;
			coords.position = spline();
			coords.position.z += track_fly_z_offset;
			coords.angles.z = R2G(-M_PI/2 - spline.deriv().psi());
			if(targetCoords().distance(coords) < track_fly_distance)
				t += dt;
			dt /= 2.f;
			}
		statistics_add(track_dt, STAT_NUMERIC, t);
		t += track_fly_increment_epsilon;
		}
	else
		t = track_fly_increment;
	track_spline += t;
	return targetCoords();
}

//////////////////////////////////////////////////////////////////////////////////////////
//	Common function
//////////////////////////////////////////////////////////////////////////////////////////
CameraCoords CameraDispatcher::input()
{
	CameraCoords coords;
	switch(input_mode){
		case BodyInput:
			coords = bodyInput();
			restriction(coords);
			break;
		case ConstInput:
			coords = constInput();
			break;
		case TrackInput:
			coords = trackInput();
			break;
		case SplineInput:
			coords = splineInput();
			break;
		case StopTimeInput:
			coords = stopTimeInput();
			break;
		case MakeTurnInvitationInput:
			coords = makeTurnInvitationInput();
			break;
		case FlyWhenStopTimeInput: 
			coords = flyWhenStopTimeInput();
			break;
		case StartTimeInput:
			coords = startTimeInput();
			break;
		case FreeDirectionFly:
			coords = freeDirectionFlyInput();
			break;
		default:
			assert(0);
		}
	CYCLE(coords.position);
	return coords;
}

int CameraDispatcher::isInterpolationCompleted()  const 
{
	if(!coords_spline.isFinished())
		return 0;
	return !is_interpolating_timer;
}
	
void CameraDispatcher::restriction(CameraCoords& coords)
{
	CYCLE(coords.position);

	if(!(flags_ & EnableRotate))
		coords.angles.z = 0;

	if(!(flags_ & EnableSlope))
		coords.angles.x = 0;
	else
		if(coords.angles.x > cameraMode().slopePrm.Max)
			coords.angles.x = cameraMode().slopePrm.Max;
		else
			if(coords.angles.x < cameraMode().slopePrm.Min)
				coords.angles.x = cameraMode().slopePrm.Min;

	if(!(flags_ & EnableRoll))
		coords.angles.y = 0;
	else
		if(fabs(coords.angles.y) > cameraMode().slopePrm.Max)
			coords.angles.y = SIGN(coords.angles.y)*cameraMode().slopePrm.Max;

	if(coords.cameraDistance > cameraMode().zPrm.Max)
		coords.cameraDistance = cameraMode().zPrm.Max;
	if(coords.cameraDistance < 0)
		coords.cameraDistance = 0;
//	if(coords.cameraDistance < cameraMode().zPrm.Min)
//		coords.cameraDistance = cameraMode().zPrm.Min;

	if(flags_ & Centering)
		coords.CenteringDelta = 0;
	else
		if(coords.CenteringDelta > centering_delta_max*centeringDeltaFactor)
			coords.CenteringDelta = centering_delta_max*centeringDeltaFactor;
}

Vect3f mi_angles;

void CameraDispatcher::integration(CameraCoords& current, const CameraCoords& input, const CameraCoords& cliped_input, float dt)
{
	start_autostop_timer(integration, STAT_CAMERA);

	CameraCoords target;
	CameraCoords mass_inv(0);
	CameraCoords damping;

	if(!memcmp(&input, &cliped_input, sizeof(CameraCoords))){
		target = input;
		float slope_mass_inv = observer && observer -> submersion() < 0.1 ? camera_slope_mass_inv : camera_water_slope_mass_inv;

		mass_inv = input_mode == BodyInput ? 
			CameraCoords(Vect3f::ZERO, Vect3f(slope_mass_inv, slope_mass_inv, camera_turn_mass_inv), camera_distance_mass_inv, camera_centering_mass_inv) :
			CameraCoords(camera_const_input_mass_inv);
		
		damping = mass_inv;
		damping.sqrt();
		damping = damping*camera_damping_factor;
		}
	else{
		target = cliped_input;
		mass_inv.position = Vect3f(camera_position_mass_inv_cliped);
		mass_inv.angles = Vect3f(camera_slope_mass_inv, camera_slope_mass_inv, camera_turn_mass_inv);
		mass_inv.cameraDistance = camera_distance_mass_inv;
		if(current.cameraDistance > cliped_input.cameraDistance){
			mass_inv.cameraDistance = camera_distance_mass_inv_cliped;
			mass_inv.angles.x = cliped_input.angles.x > input.angles.x ? camera_slope_mass_inv_cliped_up : camera_slope_mass_inv_cliped_down;
			mass_inv.angles.z = camera_turn_mass_inv_cliped;
			}
		else{
			if(fabs(cliped_input.angles.x - current.angles.x) + 0.01f < fabs(cliped_input.angles.x - input.angles.x) + fabs(current.angles.x - input.angles.x))
				mass_inv.angles.x = cliped_input.angles.x > input.angles.x ? camera_slope_mass_inv_cliped_up : camera_slope_mass_inv_cliped_down;
			if(fabs(getDist(cliped_input.angles.z, current.angles.z, 360)) + 0.01f < fabs(getDist(cliped_input.angles.z, input.angles.z, 360)) + fabs(getDist(current.angles.z, input.angles.z, 360)))
				mass_inv.angles.z = camera_turn_mass_inv_cliped;
			}
			
		mass_inv.CenteringDelta = camera_centering_mass_inv;
		
		damping = mass_inv;
		damping.sqrt();
		damping = damping*camera_damping_factor_cliped;
		}

	mi_angles = mass_inv.angles;

	// Initialization
	CameraCoords R0 = current;
	CameraCoords V0 = velocity;
	CameraCoords R1 = R0;
	CameraCoords V1 = V0;
	CameraCoords R = R0;
	CameraCoords V = V0;
	CameraCoords dV;

	// Constants
	const float h = dt;
	const float h2 = h/2.f;
	const float h3 = h/3.f;
	const float h6 = h/6.f;

	// First iteration
	dV = getDist(target, R)*mass_inv - V*damping;
	R1 += V*h6;
	V1 += dV*h6;

	// Second iteration
	R = R0 + V*h2;
	V = V0 + dV*h2;
	dV = getDist(target, R)*mass_inv - V*damping;
	R1 += V*h3;
	V1 += dV*h3;

	// Third iteration
	R = R0 + V*h2;
	V = V0 + dV*h2;
	dV = getDist(target, R)*mass_inv - V*damping;
	R1 += V*h3;
	V1 += dV*h3;

	// Fourth iteration
	R = R0 + V*h;
	V = V0 + dV*h;
	dV = getDist(target, R)*mass_inv - V*damping;
	R1 += V*h6;
	V1 += dV*h6;

	current = R1;
	velocity = V1;

	current.cycle();
	if(current.cameraDistance < camera_distance_min)
		current.cameraDistance = camera_distance_min;
}

Vect3f matrix2Euler(const Mat3f& m)
{
	// m = Rx(x)*Rz(z);
	float z = atan2(-m[0][1], m[0][0]);
	float x = atan2(-m[1][2], m[2][2]);
	return Vect3f(x, 0, z);

//	// m = Rx(x)*Ry(y)*Rz(z);
//  	float sin_y = m[0][2];      
//	float t1 = m[1][2];
//	float t2 = m[1][1];
//	float t5 = m[0][1];
//	float t7 = m[2][2];
//	float t8 = m[0][0];
//	float t12 = sqrtf(t2*(-t1*sin_y*t5+t7*t8));
//	float t13 = 1.f/t12;
//
//	float sin_x = -t1*t2*t13;
//	float cos_x = t7*t2*t13;
//	float cos_y = 1.f/t2*t12;
//	float sin_z = -t5*t2*t13;
//	float cos_z = t8*t2*t13;
//
//	float x = atan2(sin_x, cos_x);
//	if(fabsf(x) > M_PI/2){
//		x = atan2(-sin_x, -cos_x);		
//		cos_y = -cos_y;
//		sin_z = -sin_z;
//		cos_z = -cos_z;
//		}
//	float y = atan2(sin_y, cos_y);
//	float z = atan2(sin_z, cos_z);
//
//	return Vect3f(x, y, z);
}

void CameraDispatcher::setCoords(const Vect3f& vCamera_, const Vect3f& vTarget_)
{
	Vect3f vCamera = vCamera_;
	Vect3f vTarget = vTarget_;
	CYCLE(vTarget);
	vCamera = UNCYCLE(vCamera, vTarget);

	input_mode = MAX_Input;
	currentCoords.position = vTarget;
	currentCoords.CenteringDelta = 0;

	Vect3f z_axis = vCamera - vTarget; // —мотрим наоборот, т.к. Mat3f::ID смотрит вниз
	currentCoords.cameraDistance = z_axis.norm();
	z_axis /= currentCoords.cameraDistance;
	Vect3f x_axis = z_axis % Vect3f::K;
	x_axis.normalize();
	Vect3f y_axis = z_axis % x_axis;

	Mat3f R(
		Vect3f::I, Vect3f::J, Vect3f::K, // from_basis
		x_axis, y_axis, z_axis // to_basis
		);
	
	currentCoords.angles = matrix2Euler(R)*R2G(1.f);
		
	Vect3f camera_position = R*vTarget + Vect3f(0, 0, currentCoords.cameraDistance);
	camera_position.negate();

	MatXf MC(R, camera_position);
	gb_IVisGeneric->SetCameraPosition(ivsCamera, &MC, &vTarget);
	targetCoords() = currentCoords;
}

void CameraDispatcher::setIvsCamera(const CameraCoords& coords)
{
	Vect3f vg = coords.position; // “очка, котора€ будет находитьс€ на экране [0, CenteringDelta*SizeY/2, H]

	Mat3f R = coords.rot();

	Vect3f camera_position = R*vg + Vect3f(0, -coords.CenteringDelta*coords.cameraDistance, coords.cameraDistance);
	camera_position.negate();

	MatXf MC(R, camera_position);
	gb_IVisGeneric->SetCameraPosition(ivsCamera, &MC, &vg);

#ifndef _FINAL_VERSION_	
	if(camera_perspective) 
		gb_IVisGeneric->SetCameraAttribute(ivsCamera, ATTRIBUTE_CAMERA_PERSPECTIVE);
	else 
		gb_IVisGeneric->ClearCameraAttribute(ivsCamera, ATTRIBUTE_CAMERA_PERSPECTIVE);
	if(camera_world_share) 
		gb_IVisGeneric->SetCameraAttribute(ivsCamera, ATTRIBUTE_CAMERA_WORLD_SHARE);
	else 
		gb_IVisGeneric->ClearCameraAttribute(ivsCamera, ATTRIBUTE_CAMERA_WORLD_SHARE);
#endif

	// “олько дл€ чтени€
	mchCameraAX = coords.angles.x;
	mchCameraAY = coords.angles.y;
	mchCameraAZ = coords.angles.z;

	MC.invert();
	hCamera = MC.trans().z;
}


//void CameraDispatcher::show()
//{
//	if(observer){
//		show_vector(observer -> R(), XCOL(GREEN));
//		show_map_circle(observer -> R(), 4, XCOL(GREEN));
//		}
//	show_vector(currentCoords.position, YELLOW);
//	show_map_circle(currentCoords.position, 4, YELLOW);
//	show_vector(targetCoords().position, XCOL(CYAN));
//	show_map_circle(targetCoords().position, 4, XCOL(CYAN));
 
/*
	TrackIterator ti;
	TrackSpline spline;
	spline.set(ti.set(mch_raceD -> getCP(1) -> getTrack() -> pointLst -> first()));
	for(int i = 0;i < 5000; i++){
		show_vector(spline(), XCOL(MAGENTA));
		spline += 0.1f;
		}
	for(i = 0;i < 500; i++){
		show_vector((Vect3f)ti, XCOL(CYAN, 150 + 30*(i%4)));
		++ti;
		}
*/
/*
	for(i = 0;i < 500; i++){
		mchCheckpoint* cp = mch_raceD -> getCP(i);
		if(!cp)
			break;

		Vect3f p = (Vect3f(cp -> PosX0, cp -> PosY0, 0) + UNCYCLE(Vect3f(cp -> PosX1, cp -> PosY1, 0), Vect3f(cp -> PosX0, cp -> PosY0, 0)))/2;
		p.z = tri_map -> H(p.x, p.y);
		show_vector(p, XCOL(YELLOW));
		}
*/
//}	
	
void CameraDispatcher::quant()
{
	start_timer(quantCamera, STAT_CAMERA);

	if(input_mode != BodyInput && observer_transparent && advanced_observer){
		const_cast<Mechos*>(advanced_observer) -> setPermanentAlpha(1.f);
		observer_transparent = 0;
		}

//	if(input_mode != FreeDirectionFly && free_fly_body){
//		delete free_fly_body;
//		free_fly_body = 0;
//		}

	if(input_mode == MAX_Input)
		return;
	
	CameraCoords coords = input();

	start_timer(clipCamera, STAT_CAMERA);
	CameraCoords clip_coords = coords;
	if(camera_new_clip)
		new_clip(clip_coords);
	else
		clip(clip_coords);
	stop_timer(clipCamera, STAT_CAMERA);

	for(int i = 0; i < camera_integration_steps; i++)
		integration(currentCoords, coords, clip_coords, 1.f/(float)camera_integration_steps);

	setIvsCamera(currentCoords);

	stop_timer(quantCamera, STAT_CAMERA);
}
		

/////////////////////////////////////////////////////////////////////////////
//	CameraCoordsSpline
/////////////////////////////////////////////////////////////////////////////

CameraCoordsSpline::CameraCoordsSpline()
{
	n = 0;
	weight = 0;
}

int CameraCoordsSpline::isFinished() const
{
	return !weight;
}

void CameraCoordsSpline::push_back(const CameraCoords& coords)
{
	if(!coords.time){
		P[0] = P[1] = P[2] = P[3] = coords;	
		n = 0;
		t = 0;
		weight = 0;
		points.clear();
		return;
		}

	switch(n){
		case 0:
			P[2] = P[3] = coords;
			prepare();
			break;
		case 1:
			P[0] = P[1] = (*this)();
			P[3] = coords;	
			if((P[2].time -= t*weight) < 1e-3){
				P[2] = P[3];
				n = 0;
				}
			prepare();
			break;
		default:
			points.push_back(coords);
		}
	n++;
}

void CameraCoordsSpline::shift()
{
	weight = t = 0;
	P[0] = P[1];
	P[1] = P[2];
	P[2] = P[3];
	if(!n)
		return;
	n--;
	if(!points.empty()){
		P[3] = points.front();
		points.pop_front();
		}
	if(!P[2].time)
		shift();
	else
		prepare();
}

void CameraCoordsSpline::prepare()
{
	P[1].cycle();
	P[0].cycle();
	P[0] = P[1] + getDist(P[0], P[1]);
	P[2].cycle();
	P[2] = P[1] + getDist(P[2], P[1]);
	P[3].cycle();
	P[3] = P[1] + getDist(P[3], P[1]);

	t = 0;
	weight = 1/P[2].time;
}

CameraCoordsSpline& CameraCoordsSpline::operator += (float dtime)
{
	if((t += dtime*weight) > 1.f)
		shift();
	return *this;
}

CameraCoords CameraCoordsSpline::operator()() const 
{
	CameraCoords coords((3.0f/2.0f*P[1]-3.0f/2.0f*P[2]-P[0]/2.0f+P[3]/2.0f)*t*t*t +
		(2.0f*P[2]+P[0]-P[3]/2.0f-5.0f/2.0f*P[1])*t*t + (P[2]/2.0f-P[0]/2.0f)*t + P[1]);
	coords.cycle();
	return coords;
}

CameraCoords CameraCoordsSpline::deriv() const 
{
	return 3.0f*(3.0f/2.0f*P[1]-3.0f/2.0f*P[2]-P[0]/2.0f+P[3]/2.0f)*t*t +
		2.0f*(2.0f*P[2]+P[0]-P[3]/2.0f-5.0f/2.0f*P[1])*t + (P[2]/2.0f-P[0]/2.0f);
}

/////////////////////////////////////////////////////////////////////////////////
//			Stop Time
/////////////////////////////////////////////////////////////////////////////////
void CameraDispatcher::runTurnBasedStartScenario() 
{
	runStartScenario();
	next_input_mode = StopTimeInput;
}

void CameraDispatcher::setStopTime()
{
	V_scroll.set(0.f);
	input_mode = StopTimeInput;
	targetCoords() = currentCoords;
	setMode(CAM_MODE_STOP_TIME, 1);
}

void CameraDispatcher::setStartTime()
{
	input_mode = StartTimeInput;
	setMode(CAM_MODE_SLOPE_ROTATE, 1);
	targetCoords() = currentCoords;
	is_interpolating_timer.start(camera_runStartTimeScenario_interpolation_time);
}

void CameraDispatcher::rotateWhenStopTime()
{
	rotation_switch_timer.start(rotation_when_stop_time_time);
	if(camera_mode != CAM_MODE_SLOPE_ROTATE)
		setMode(CAM_MODE_SLOPE_ROTATE, 1);
}

void CameraDispatcher::centerMechosWhenStopTime()
{
	V_scroll.set(0.f);
	targetCoords().position = observer -> R();
} 

CameraCoords CameraDispatcher::stopTimeInput()
{
	if(!rotation_switch_timer){
		if(camera_mode != CAM_MODE_STOP_TIME)
			setMode(CAM_MODE_STOP_TIME, 1);
		}
	else{
		targetCoords().angles.z += rotation_when_stop_time_speed;
		targetCoords().angles.x = cameraMode().slopePrm.Default;
		}

	control(targetCoords());
	targetCoords().cameraDistance = cameraMode().zPrm.Default;

	if(!XGR_MouseObj.rBt.Pressed){
		int config = advanced_observer ? advanced_observer -> get_control_config() : 0;
		float scroll_force = targetCoords().cameraDistance*camera_scroll_force_of_distance;
	
		Vect3f F_scroll(0);

		if(mchKeyPressed(MCH_KEY_TURN_LEFT, config))
			F_scroll.x = -scroll_force;
		else 
			if(mchKeyPressed(MCH_KEY_TURN_RIGHT, config))
				F_scroll.x = scroll_force;

		if(mchKeyPressed(MCH_KEY_MOVE_FORWARD, config))
			F_scroll.y = -scroll_force;
		else 
			if(mchKeyPressed(MCH_KEY_MOVE_BACKWARD, config))
				F_scroll.y = scroll_force;

		Mat3f(-G2R(targetCoords().angles.z), Z_AXIS).xform(F_scroll);

		targetCoords().position += V_scroll*evolve_time_step;
		V_scroll += (F_scroll*camera_scroll_mass_inv - V_scroll*sqrt(camera_scroll_mass_inv)*camera_scroll_damping_factor)*evolve_time_step;
		mouse_3D_position = Screen2World(XGR_MouseObj.PosX, XGR_MouseObj.PosY);		
		}
	else{
		Vect3f pv, pe, pe0(XGR_MouseObj.PosX, XGR_MouseObj.PosY, 0);
		
		_controlfp(_PC_53, _MCW_PC); 
		if(hCamera > stop_time_hCamera){
			Vect2f ScrSize = ((cRenderDevice*)ivsRenderDevice) -> GetSize();
			Vect2f Center = ivsCamera -> GetCenter()*ScrSize;	
			pe0.x -= Center.x;
			pe0.y -= Center.y;
			if(pe0.norm() > XGR_MAXX*stop_time_share_radius_factor)
				pe0.normalize(XGR_MAXX*stop_time_share_radius_factor);
			pe0.x += Center.x;
			pe0.y += Center.y;
			}

		for(int i = 0; i < stop_time_max_iterations; i++){
			setIvsCamera(currentCoords);
			ConvertorWorldToViewPort(mouse_3D_position, pv, pe); pe.z = 0;
			float f = pe.distance2(pe0);

			if(f < stop_time_precision)
				break;

			currentCoords.position.x += stop_time_dxy;
			CYCLE(currentCoords.position);
			setIvsCamera(currentCoords);
			ConvertorWorldToViewPort(mouse_3D_position, pv, pe); pe.z = 0;
			float df_dx = (pe.distance2(pe0) - f)/stop_time_dxy;
			currentCoords.position.x -= stop_time_dxy;

			currentCoords.position.y += stop_time_dxy;
			CYCLE(currentCoords.position);
			setIvsCamera(currentCoords);
			ConvertorWorldToViewPort(mouse_3D_position, pv, pe); pe.z = 0;
			float df_dy = (pe.distance2(pe0) - f)/stop_time_dxy;
			currentCoords.position.x -= stop_time_dxy;
			
			Vect3f delta(fabs(df_dx) > FLT_EPS ? -(f/df_dx)/2 : 0, fabs(df_dy) > FLT_EPS ? -(f/df_dy)/2 : 0, 0);
			
			if(delta.norm() > stop_time_max_step)
				delta.normalize(stop_time_max_step);

			currentCoords.position += delta;
			}
		CONTROL_FP();

		targetCoords().position = currentCoords.position;
		velocity.position.set(0.f);
		V_scroll.set(0.f);
		}

	targetCoords().position.z = tri_map -> H(targetCoords().position.xi(), targetCoords().position.yi()) + stop_time_z_offset;

	restriction(targetCoords());

	return targetCoords();
}

CameraCoords CameraDispatcher::startTimeInput()
{
	if(isInterpolationCompleted())
		input_mode = BodyInput;
	return bodyInput();
}

////////////////////////////////////////////////////////////////////////////
//		Fly When Stop Time
////////////////////////////////////////////////////////////////////////////
void CameraDispatcher::setFlyWhenStopTime()
{
	V_scroll.set(0.f);
	input_mode = FlyWhenStopTimeInput;
	targetCoords() = currentCoords;
	setMode(CAM_MODE_SLOPE_ROTATE, 1);
	targetCoords().angles.x = cameraMode().slopePrm.Default;
	clear_mouse_movement();
}

CameraCoords CameraDispatcher::flyWhenStopTimeInput()
{
	control(targetCoords());
	targetCoords().cameraDistance = cameraMode().zPrm.Default;

	Vect3f mm = retrieve_mouse_movement();
	targetCoords().angles.z += mm.x*camera_fly_stop_time_scale_x;
	targetCoords().angles.x += mm.y*camera_fly_stop_time_scale_y;

	float scroll_force = targetCoords().cameraDistance*camera_scroll_force_of_distance;
	int config = advanced_observer ? advanced_observer -> get_control_config() : 0;

	Vect3f F_scroll(0);

	if(mchKeyPressed(MCH_KEY_TURN_LEFT, config))
		F_scroll.x -= scroll_force;
	else 
		if(mchKeyPressed(MCH_KEY_TURN_RIGHT, config))
			F_scroll.x += scroll_force;

	if(mchKeyPressed(MCH_KEY_MOVE_FORWARD, config))
		F_scroll.y -= scroll_force;
	else 
		if(mchKeyPressed(MCH_KEY_MOVE_BACKWARD, config))
			F_scroll.y += scroll_force;

	Mat3f(-G2R(targetCoords().angles.z), Z_AXIS).xform(F_scroll);

	targetCoords().position += V_scroll*evolve_time_step;
	targetCoords().position.z = tri_map -> H(targetCoords().position.xi(), targetCoords().position.yi());
	V_scroll += (F_scroll*camera_scroll_mass_inv - V_scroll*sqrt(camera_scroll_mass_inv)*camera_scroll_damping_factor)*evolve_time_step;

	restriction(targetCoords());

	return targetCoords();
}
 
 /////////////////////////////////////////////////////////////////////////////////
 //	Make Turn Invitation
 /////////////////////////////////////////////////////////////////////////////////
void CameraDispatcher::setMakeTurnInvitation()
{
	input_mode = MakeTurnInvitationInput;
	setMode(CAM_MODE_SLOPE_ROTATE, 1);
}
CameraCoords CameraDispatcher::makeTurnInvitationInput()
{
	return bodyInput();
}

////////////////////////////////////////////////////////////////////////////
//		Free Direction Fly
////////////////////////////////////////////////////////////////////////////
void CameraDispatcher::setFreeDirectionFly()
{
	switch(input_mode){
		case FreeDirectionFly:{
			switch(prev_input_mode){
				case BodyInput:
					setObserver(0);
					break;
				case StopTimeInput:
					setStopTime();
					break;
				default:
					xassert(0);
				}
			return;
			}
		case BodyInput:
		case StopTimeInput:
		#ifndef _FINAL_VERSION_
		case ConstInput:
		#endif		
			break;
		default: 
			return;
		}
	
	if(!free_fly_body) 
		free_fly_body = new FreeFlyBody;
	free_fly_body -> start(currentCoords);
	prev_input_mode = input_mode;
	input_mode = FreeDirectionFly;
	targetCoords() = currentCoords;
	setMode(CAM_MODE_SLOPE_ROTATE, 1);
}

CameraCoords CameraDispatcher::freeDirectionFlyInput()
{
	free_fly_body -> evolve(currentCoords, evolve_time_step/time_speed);	
	setIvsCamera(currentCoords);
	return targetCoords() = currentCoords;
}

FreeFlyBody::FreeFlyBody()
{
	dragV = free_fly_dragV;
	spheres.push_back(Sphere(Vect3f::ZERO, free_fly_body_radius));
}

void FreeFlyBody::start(CameraCoords coords)
{
	coords.point2eye();
	clearVelocities();
	setTrans(coords.position);
	make_non_colliding();
	angles = coords.angles*G2R(1.f);
	clear_mouse_movement();
}

void FreeFlyBody::evolve(CameraCoords& coords, float dt)
{
	clearForces();
	test_collision(1);

	Vect3f mm = retrieve_mouse_movement();
	angles.x -= mm.y*free_fly_torque_factor.x;
	if(angles.x < 0)
		angles.x = 0;
	if(angles.x > free_fly_max_climb_angle)
		angles.x = free_fly_max_climb_angle;

	angles.z += mm.x*free_fly_torque_factor.z;

	if(!(XKey.Pressed(VK_PRIOR) | XKey.Pressed(VK_HOME)))
		angles.y *= 1.f - free_fly_roll_angle_decrement_factor;
	if(XKey.Pressed(VK_HOME))
		angles.y += free_fly_torque_factor.y;
	if(XKey.Pressed(VK_PRIOR))
		angles.y -= free_fly_torque_factor.y;

	setRot(xpose(Mat3f(angles.x, X_AXIS)*Mat3f(angles.y, Y_AXIS)*Mat3f(angles.z, Z_AXIS)));

	if(camera_free_fly_by_key && XKey.Pressed(VK_UP) || XGR_MouseObj.lBt.Pressed)
		applyLocalForce(free_fly_force_factor.z, Z_AXIS);
	if(camera_free_fly_by_key && XKey.Pressed(VK_DOWN) || XGR_MouseObj.rBt.Pressed)
		applyLocalForce(-free_fly_force_factor.z, Z_AXIS);
	if(XKey.Pressed(VK_LEFT))
		applyLocalForce(free_fly_force_factor.x, X_AXIS);
	if(XKey.Pressed(VK_RIGHT))
		applyLocalForce(-free_fly_force_factor.x, X_AXIS);
	if(XKey.Pressed(VK_END))
		applyLocalForce(-free_fly_force_factor.y, Y_AXIS);
	if(XKey.Pressed(VK_NEXT))
		applyLocalForce(free_fly_force_factor.y, Y_AXIS);

	applyGlobalForce(Gravity, Z_AXIS);

	ContactingBodies single_body;
	single_body.push_back(this);
	single_body.evolve(dt);
	CYCLE(Xlg.trans());

	coords.position = trans();
	coords.angles = angles*R2G(1.f);
	coords.eye2point();
	CYCLE(coords.position);
}

void FreeFlyBody::project_coords(CameraCoords& coords)
{
	start(coords);

	coords.position = trans();
	coords.angles = angles*R2G(1.f);
	coords.eye2point();
	CYCLE(coords.position);
}
