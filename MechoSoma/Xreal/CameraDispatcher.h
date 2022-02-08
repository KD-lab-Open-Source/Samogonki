
#ifndef __CAMERADISPATCHER_H__
#define __CAMERADISPATCHER_H__

#include "Timers.h"
#include "TrackSpline.h"

struct CameraPrm
{
	float Max;
	float Min;
	float Delta;
	float Default;

	void init(char* iniFile,char* section,char* id);
	void save(char* iniFile,char* section,char* id);
};

enum CameraMode 
{
	CAM_MODE_FLAT,
	CAM_MODE_FIXED_3D,	// 1
	CAM_MODE_FREE_3D,	// 2
	CAM_MODE_ROTATE,	// 3
	CAM_MODE_SLOPE_ROTATE,	// 4
	CAM_MODE_STOP_TIME,	// 5

	CAM_MODE_MAX
};

struct CameraModeInfo
{
	int mapMode;

	CameraPrm zPrm;
	
	int slopeMode;
	CameraPrm slopePrm;
	float max_slopeDelta;

	int rotateMode;
	CameraPrm rotatePrm;

	void init(char* iniFile,char* id);
	void save(char* iniFile,char* id);
};

struct CameraCoords
{
	Vect3f position;
	Vect3f angles;
	float cameraDistance; // расстояние от камеры до плоскости объекта (z-distance)
	float CenteringDelta; // смещение точки наблюдения от центра экрана: крайняя нижняя точка 0.5
	float time; 

	CameraCoords(float f = 0) :
		position(f), angles(f), cameraDistance(f), CenteringDelta(f), time(f){}
	CameraCoords(const Vect3f& position_, const Vect3f& angles_, float cameraDistance_, float CenteringDelta_, float time_ = 0) :
		position(position_), angles(angles_), cameraDistance(cameraDistance_), CenteringDelta(CenteringDelta_), time(time_){}

	CameraCoords& operator += (const CameraCoords& c);
	CameraCoords& operator -= (const CameraCoords& c);
	CameraCoords operator + (const CameraCoords& c) const;
	CameraCoords operator - (const CameraCoords& c) const;
	CameraCoords operator * (const CameraCoords& c) const;
	CameraCoords operator * (float t) const;
	CameraCoords operator / (float t) const { return *this * (1/t); }

	Mat3f rot() const { return Mat3f(G2R(angles.x), X_AXIS)*Mat3f(G2R(angles.y), Y_AXIS)*Mat3f(G2R(angles.z), Z_AXIS); }

	float distance(const CameraCoords& coords) const;
	
	CameraCoords& cycle();
	CameraCoords& sqrt();
	CameraCoords& point2eye();
	CameraCoords& eye2point();

	friend CameraCoords getDist(const CameraCoords& c1, const CameraCoords& c0);
	friend CameraCoords operator* (float t, const CameraCoords& c) { return c*t; }
};


class CameraCoordsSpline 
{
	int n;
	list<CameraCoords> points;
	CameraCoords P[4];
	float t, weight;

	void shift();
	void prepare();

public:

	CameraCoordsSpline();
	int isFinished() const;
	void push_back(const CameraCoords& coords);
	CameraCoordsSpline& operator += (float dtime);

	CameraCoords operator()() const;
	CameraCoords deriv() const;
};

class CameraDispatcher 
{
	enum InputMode {
		ConstInput, 
		BodyInput,
		TrackInput,
		SplineInput,
		StopTimeInput,
		MakeTurnInvitationInput,
		FlyWhenStopTimeInput,
		StartTimeInput, 
		FreeDirectionFly,
		MAX_Input,
		InputModeMax
		};
		
	enum Flags {
		EnableControl = 1, // разрешен ручной zoom & slope

		Perspective = 64,
		EnableRotate = 128,
		EnableSlope = 256,
		EnableRoll = 512,

		Centering = 1024, // точка наблюдения будет находиться в центре
		};
	
	enum ClipMode {
		InitClipping,
		ToDesiredByTheta,
		FromDesiredByTheta,
		SearchHole
		};


	int flags_;
	InputMode input_mode;

	class cCamera* ivsCamera;
	class cUnknownClass* ivsRenderDevice;

	CameraCoords currentCoords;
	CameraCoords targetCoordsArray[InputModeMax];
	CameraCoords& targetCoords() { return targetCoordsArray[input_mode]; }
	CameraCoords velocity;

	// Body Input
	const class Body* observer;
	const class Mechos* advanced_observer;
	DurationTimer interpolate_position_timer;
	float fly_around_angle, fly_around_angle_incr;

	// Track Spline
	TrackSpline track_spline;

	// Const Input
	CameraCoordsSpline coords_spline;
	InputMode next_input_mode;
	DurationTimer static_observer_rotation_timer;
	
	// Clipping
	ClipMode clip_mode;
	CameraCoords clipCoords;
	float delta_psi;
	float delta_theta;
	float search_hole_delta;
	int clip_counter;
	float camera_metric;
	int observer_transparent;
	float centeringDeltaFactor;
	CameraCoords clipped_coords_to_reuse;
	int clipping_to_reuse_calculated;

	// Scrolling
	Vect3f V_scroll;

	// Stop Time
	DurationNonStopTimer rotation_switch_timer;
	Vect3f mouse_3D_position;

	// isInterpolationCompleted Logic
	DurationNonStopTimer is_interpolating_timer;

	// Free Fly
	float time_speed;
	class FreeFlyBody* free_fly_body;
	InputMode prev_input_mode;

	// Debug 
	Vect3f S2G_last_result;

	CameraMode camera_mode;
	CameraModeInfo modes[CAM_MODE_MAX];

private:
	void control(CameraCoords& coords);
	CameraCoords input();
	CameraCoords bodyInput();
	CameraCoords constInput();
	CameraCoords trackInput();
	CameraCoords splineInput();
	CameraCoords stopTimeInput();
	CameraCoords makeTurnInvitationInput();
	CameraCoords flyWhenStopTimeInput();
	CameraCoords startTimeInput();
	CameraCoords freeDirectionFlyInput();
	void restriction(CameraCoords& coords);
	int clip(CameraCoords& coords);
	int new_clip(CameraCoords& coords);
	void integration(CameraCoords& current, const CameraCoords& input, const CameraCoords& cliped_input, float dt);
	void setIvsCamera(const CameraCoords& coords);

public:
	CameraDispatcher(class cUnknownClass* ivsRenderDevice);
	~CameraDispatcher();
	
	void activate();
	void deactivate();
	void setWindow(const struct mchGameWindow* gWnd);

	cCamera* camera() { return ivsCamera; }

	void clear(void);
	void setMode(int m, int interpolate = 0);
	void quant();

	void setObserver(const Body* observer, int interpolate = 0); // Устанавливает динамич. наблюдателя, центровка согласно текущему режиму, 0 восстанавливает пред. наблюдателя
	void setStaticObserver(const Vect3f& point, const Vect3f& angles, float distance, float centering_delta, int interpolation_time); // Устанавливает стат. точку набл.
	void setStaticObserver(); // Останавливает камеру.
	void setTrackObserver(mchTrackElement* el, float slope, float distance, float centering_delta); // el == 0 - берется CP(1)
	void setCoords(const Vect3f& vCamera, const Vect3f& vTarget);
	void setStopTime();
	void setMakeTurnInvitation();
	void setFlyWhenStopTime();
	void setStartTime();
	void setFreeDirectionFly();

	void runStartScenario(); // assume Body observer has been set
	void runFinishScenario(); // assume Body observer has been set
	void stopFinishScenario();

	void runTurnBasedStartScenario();
	void rotateWhenStopTime();
	void centerMechosWhenStopTime();

	int isInterpolationCompleted() const; 
	int isRotateEnabled() const { return cameraMode().rotateMode; }
	int isStartScenarioRunning() const { return input_mode == SplineInput; }

	CameraMode currentMode() const { return camera_mode; }
	const CameraModeInfo& cameraMode() const { return modes[camera_mode]; }
	CameraModeInfo& cameraMode() { return modes[camera_mode]; }

	const Vect3f& observationPoint() const { return currentCoords.position; }
	Vect3f getObservationDist(const Vect3f& point) const { return getDist(point, currentCoords.position); }
	const CameraCoords& getCurrentCoords() const { return currentCoords; }

	void ConvertorWorldToViewPort(const Vect3f& vw, Vect3f& pv, Vect3f& pe);
	float getScale(const Vect3f& vw);
	Vect3f Screen2World(int xs, int ys);

	void saveControls(XBuffer& buf); // Записывает и читает zoom & slope 
	void loadControls(XBuffer& buf); // для всех режимов камеры

	void setTimeSpeed(float speed){ time_speed = speed; }

	void startStaticObserverRotation(int time) { static_observer_rotation_timer.start(time); }
	
	void show(); // debug
	void write_state(XBuffer& buf);
};

extern CameraDispatcher* camera_dispatcher;
extern CameraDispatcher* camera_dispatcher2;

#endif // __CAMERADISPATCHER_H__
