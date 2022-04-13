#include "StdAfx.h"
#include "aci_parser.h"
#include "IVisGeneric.h"
#include "M3d_effects.h"
#include "PolyMgr.h"
#include "TERRA.H"
#include "sound.h"
#include "race.h"


#include "fxlabID.h"
#include "fxlabGeneral.h"
#include "fxlabResource.h"
#include "fxlabInterface.h"
#include "fxlabApplication.h"
#include "XTList.h"
#include "fxlabClientGeneral.h"
#include "fxlabClientEffect.h"
#include "Hermit.h"
#include "CameraDispatcher.h"

void fxlabSwarmDomain::Load(fxlabResourceObject* key,float time)
{
	float buffer[FXLAB_SWARM_DOMAIN_DATA_MAX];

	if(key->GetKeyNum() != FXLAB_SWARM_DOMAIN_DATA_MAX)
		ErrH.Abort("Bad Key Size for Domain in fxlabClientEffect");

	key->GetKey(buffer,time);

	switch(int(round(buffer[FXLAB_SWARM_DOMAIN_TYPE]))){
		case FXLAB_SWARM_DOMAIN_POINT:
			Point(buffer[FXLAB_SWARM_DOMAIN_DATA_X],buffer[FXLAB_SWARM_DOMAIN_DATA_Y],buffer[FXLAB_SWARM_DOMAIN_DATA_Z]);
			break;
		case FXLAB_SWARM_DOMAIN_LINE:
			Line(buffer[FXLAB_SWARM_DOMAIN_DATA_X],buffer[FXLAB_SWARM_DOMAIN_DATA_Y],buffer[FXLAB_SWARM_DOMAIN_DATA_Z],buffer[FXLAB_SWARM_DOMAIN_DATA_A1],buffer[FXLAB_SWARM_DOMAIN_DATA_B1],buffer[FXLAB_SWARM_DOMAIN_DATA_C1]);
			break;
		case FXLAB_SWARM_DOMAIN_SPHERE:
			Sphere(buffer[FXLAB_SWARM_DOMAIN_DATA_X],buffer[FXLAB_SWARM_DOMAIN_DATA_Y],buffer[FXLAB_SWARM_DOMAIN_DATA_Z],buffer[FXLAB_SWARM_DOMAIN_DATA_A1],buffer[FXLAB_SWARM_DOMAIN_DATA_B1]);
			break;
		case FXLAB_SWARM_DOMAIN_CYLINDER:
			Cylinder(buffer[FXLAB_SWARM_DOMAIN_DATA_X],buffer[FXLAB_SWARM_DOMAIN_DATA_Y],buffer[FXLAB_SWARM_DOMAIN_DATA_Z],buffer[FXLAB_SWARM_DOMAIN_DATA_A1],buffer[FXLAB_SWARM_DOMAIN_DATA_B1],buffer[FXLAB_SWARM_DOMAIN_DATA_C1],buffer[FXLAB_SWARM_DOMAIN_DATA_A2],buffer[FXLAB_SWARM_DOMAIN_DATA_B2]);
			break;
		case FXLAB_SWARM_DOMAIN_CONE:
			Cone(buffer[FXLAB_SWARM_DOMAIN_DATA_X],buffer[FXLAB_SWARM_DOMAIN_DATA_Y],buffer[FXLAB_SWARM_DOMAIN_DATA_Z],buffer[FXLAB_SWARM_DOMAIN_DATA_A1],buffer[FXLAB_SWARM_DOMAIN_DATA_B1],buffer[FXLAB_SWARM_DOMAIN_DATA_C1],buffer[FXLAB_SWARM_DOMAIN_DATA_A2],buffer[FXLAB_SWARM_DOMAIN_DATA_B2]);
			break;
		case FXLAB_SWARM_DOMAIN_CIRCLE:
			Circle(buffer[FXLAB_SWARM_DOMAIN_DATA_X],buffer[FXLAB_SWARM_DOMAIN_DATA_Y],buffer[FXLAB_SWARM_DOMAIN_DATA_Z],buffer[FXLAB_SWARM_DOMAIN_DATA_A1],buffer[FXLAB_SWARM_DOMAIN_DATA_B1]);
			break;
		case FXLAB_SWARM_DOMAIN_POLAR_SPHERE:
			PolarSphere(buffer[FXLAB_SWARM_DOMAIN_DATA_X],buffer[FXLAB_SWARM_DOMAIN_DATA_Y],buffer[FXLAB_SWARM_DOMAIN_DATA_Z],buffer[FXLAB_SWARM_DOMAIN_DATA_A1],buffer[FXLAB_SWARM_DOMAIN_DATA_B1],buffer[FXLAB_SWARM_DOMAIN_DATA_C1],buffer[FXLAB_SWARM_DOMAIN_DATA_A2],buffer[FXLAB_SWARM_DOMAIN_DATA_B2]);
			break;
		case FXLAB_SWARM_DOMAIN_ISO_SPHERE:
			IsoSphere(buffer[FXLAB_SWARM_DOMAIN_DATA_X],buffer[FXLAB_SWARM_DOMAIN_DATA_Y],buffer[FXLAB_SWARM_DOMAIN_DATA_Z],buffer[FXLAB_SWARM_DOMAIN_DATA_A1],buffer[FXLAB_SWARM_DOMAIN_DATA_B1]);
			break;
		case FXLAB_SWARM_DOMAIN_BOX:
			Box(buffer[FXLAB_SWARM_DOMAIN_DATA_X],buffer[FXLAB_SWARM_DOMAIN_DATA_Y],buffer[FXLAB_SWARM_DOMAIN_DATA_Z],buffer[FXLAB_SWARM_DOMAIN_DATA_A1],buffer[FXLAB_SWARM_DOMAIN_DATA_B1],buffer[FXLAB_SWARM_DOMAIN_DATA_C1]);
			break;
	};
};

void fxlabSwarmDomain::Point(float x,float y,float z)
{
	Type = FXLAB_SWARM_DOMAIN_POINT;
	P1 = Vect3f(x,y,z);
};

void fxlabSwarmDomain::Box(float x1,float y1,float z1,float x2,float y2,float z2)
{
	Type = FXLAB_SWARM_DOMAIN_BOX;
	P1 = Vect3f(x1,y1,z1);
	P2 = Vect3f(x2 - x1,y2 - y1,z2 - z1);
};

void fxlabSwarmDomain::Line(float x1,float y1,float z1,float x2,float y2,float z2)
{
	Type = FXLAB_SWARM_DOMAIN_LINE;
	P1 = Vect3f(x1, y1, z1);
	P2 = Vect3f(x2, y2, z2);
	P2 -= P1;
};

void fxlabSwarmDomain::Sphere(float x,float y,float z,float r1,float r2)
{
	Type = FXLAB_SWARM_DOMAIN_SPHERE;

	P1 = Vect3f(x,y,z);
	if(r1 > r2){
		Radius1 = r1;
		Radius2 = r2;
	}else{
		Radius1 = r2;
		Radius2 = r1;
	};
};

void fxlabSwarmDomain::IsoSphere(float x,float y,float z,float r1,float r2)
{
	Type = FXLAB_SWARM_DOMAIN_ISO_SPHERE;

	P1 = Vect3f(x,y,z);
	Radius1 = r1;
	Num = r2;
	Radius2 = 2.0f * M_PI / Num;
};

void fxlabSwarmDomain::PolarSphere(float x1,float y1,float z1,float r,float a,float p,float r1,float r2)
{
	float x,y,z;

	Type = FXLAB_SWARM_DOMAIN_POLAR_SPHERE;
	x = r*sinf(p);
	y = x*sinf(a);
	x = x*cosf(a);
	z = r*cosf(p);

	P1 = Vect3f(x1 + x,y1 + y,z1 + z);
	if(r1 > r2){
		Radius1 = r1;
		Radius2 = r2;
	}else{
		Radius1 = r2;
		Radius2 = r1;
	};
};

void fxlabSwarmDomain::Circle(float x1,float y1,float z1,float r1,float r2)
{
	Type = FXLAB_SWARM_DOMAIN_CIRCLE;

	P1 = Vect3f(x1,y1,z1);

	if(r1 > r2){
		Radius1 = r1;
		Radius2 = r2;
	}else{
		Radius1 = r2;
		Radius2 = r1;
	};
};

void fxlabSwarmDomain::Cone(float x1,float y1,float z1,float x2,float y2,float z2,float r1,float r2)
{
	Vect3f n;	
	Vect3f basis;
	float p2l2;

	Type = FXLAB_SWARM_DOMAIN_CONE;

	P1 = Vect3f(x1,y1,z1);
	P2 = Vect3f(x2,y2,z2);

	if(r1 > r2){
		Radius1 = r1;
		Radius2 = r2;
	}else{
		Radius1 = r2;
		Radius2 = r1;
	};

	n = P2;
	p2l2 = n.norm2(); // Optimize this.
	n.normalize();	
	
	// Find a vector with an orthogonal component to n.
	basis = Vect3f(1.0f, 0.0f, 0.0f);
	if(1.0f - fabs(basis.dot(n)) < 1e-5f)
		basis = Vect3f(0.0f, 1.0f, 0.0f);
	
	// Project away N component, normalize and cross to get
	// second orthonormal vector.
	U = basis - n * (basis * n);
	U.normalize();
//	V = n ^ U;
	V.cross(n,U);
};

void fxlabSwarmDomain::Cylinder(float x1,float y1,float z1,float x2,float y2,float z2,float r1,float r2)
{
	Vect3f n;
	Vect3f basis;
	float p2l2;

	Type = FXLAB_SWARM_DOMAIN_CYLINDER;

	P1 = Vect3f(x1,y1,z1);
	P2 = Vect3f(x2,y2,z2);

	if(r1 > r2){
		Radius1 = r1;
		Radius2 = r2;
	}else{
		Radius1 = r2;
		Radius2 = r1;
	};

	n = P2;
	p2l2 = n.norm2(); // Optimize this.
	n.normalize();	

	// Find a vector with an orthogonal component to n.
	basis = Vect3f(1.0f, 0.0f, 0.0f);
	if(1.0f - fabs(basis.dot(n)) < 1e-5f)
		basis = Vect3f(0.0f, 1.0f, 0.0f);
	
	// Project away N component, normalize and cross to get
	// second orthonormal vector.
	U = basis - n * (basis * n);
	U.normalize();
//	V = n ^ U;
	V.cross(n,U);
};

void fxlabSwarmDomain::Get(Vect3f& d)
{
	float r1;
	float dist;
	float theta;
	Mat3f mat;

	switch (Type){
		case FXLAB_SWARM_DOMAIN_POINT:
			d = P1;
			break;
		case FXLAB_SWARM_DOMAIN_LINE:
			d = P1 + P2 * fxlabClientRND.UnitRND();
			break;
		case FXLAB_SWARM_DOMAIN_SPHERE:
		case FXLAB_SWARM_DOMAIN_POLAR_SPHERE:
			d = Vect3f(fxlabClientRND.UnitRND(),fxlabClientRND.UnitRND(),fxlabClientRND.UnitRND()) - Vect3f(0.5f,0.5f,0.5f);
			d.normalize();
			if(Radius1 == Radius2)
				d *= Radius1;
			else
				d *= Radius2 + fxlabClientRND.UnitRND() * (Radius1 - Radius2);
			d += P1;
			break;
		case FXLAB_SWARM_DOMAIN_CYLINDER:
			dist = fxlabClientRND.UnitRND();
			theta = fxlabClientRND.UnitRND() * 2.0f * float(M_PI);
			r1 = Radius2 + fxlabClientRND.UnitRND() * (Radius1 - Radius2);
			d = P1 + P2 * dist + U * (r1 * cosf(theta)) + V * (r1 * sinf(theta));
			break;
		case FXLAB_SWARM_DOMAIN_CIRCLE:
			theta = fxlabClientRND.UnitRND() * 2.0f * float(M_PI);
			r1 = Radius2 + fxlabClientRND.UnitRND() * (Radius1 - Radius2);
			d.x = P1.x + r1 * cosf(theta);
			d.y = P1.y + r1 * sinf(theta);
			d.z = P1.z;
			break;
		case FXLAB_SWARM_DOMAIN_CONE:
			dist = fxlabClientRND.UnitRND();
			theta = fxlabClientRND.UnitRND() * 2.0f * float(M_PI);
			r1 = (Radius2 + fxlabClientRND.UnitRND() * (Radius1 - Radius2))*dist;
			d = P1 + P2 * dist + U * (r1 * cosf(theta)) + V * (r1 * sinf(theta));
			break;
		case FXLAB_SWARM_DOMAIN_ISO_SPHERE:
			mat = Mat3f(2.0f * fxlabClientRND.UnitRND() * M_PI,Y_AXIS) * Mat3f((float)(fxlabClientRND.RND(round(Num))) * Radius2,Z_AXIS);
			d = P1 + Vect3f(Radius1,0,0) * mat;
			break;
		case FXLAB_SWARM_DOMAIN_BOX:
			d = P1 + Vect3f(P2.x * fxlabClientRND.UnitRND(),P2.y * fxlabClientRND.UnitRND(),P2.z * fxlabClientRND.UnitRND());
			break;
		default:
			d = Vect3f(0,0,0);
			break;
	};
};

//-------------------------------------------------------------

void fxlabParticleFear::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_FEAR_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabParticleFear");
};

void fxlabParticleFear::KeyUpdate(void)
{
	fxlabParticleCore::KeyUpdate();
	ImageType = round(KeyData[FXLAB_FEAR_DATA_IMAGE_TYPE]);
	ImageID = round(KeyData[FXLAB_FEAR_DATA_IMAGE_ID]);
	ImageNoise = round(KeyData[FXLAB_FEAR_DATA_IMAGE_NOISE]);
};

#include "VisGeneric.h"
#include "Camera.h"
#include "Scene.h"

void fxlabParticleFear::Show(void)
{
	int i,n;
	cInterfaceVisGeneric *IVisGeneric;
	cScene* scene;
	fxlabParticleType* p;
	
	n = 1;
	if(Visibility && ParticleList.first() != ParticlePoint){
		IVisGeneric=CreateIVisGeneric();
		scene = ((cVisGeneric*)(IVisGeneric))->GetActiveScene();
		assert(scene);
		MatXf mat(Mat3f::ID,Vect3f(Position));
		for(i = 0;i < scene->GetNumberCamera();i++){
			if(Visibility & n){
				p = ParticleList.first();
				P3D->BeginList(scene->GetCamera(i),p->ImagePoint->GetSpriteID(),mat);
				while(p != ParticlePoint){
					P3D->AttachCenter(p->Position,p->ImagePoint->GetSpriteData(p->Phase),p->ImagePoint->GetSpriteID());
					p = p->next;
				};
				P3D->EndList();
			};
			n <<= 1;
		};
		IVisGeneric->Release();
	};
};

//-------------------------------------------

void fxlabFearSpring::Open(void)
{
	fxlabParticleFear::Open();
	AddRateFactor = 0;
};

void fxlabFearSpring::CoreGenerate(void)
{
	int n,i;
	float num;
	float delta;
	fxlabParticleType* p;
	float r;
	
	if(Rate >= 0.0001){
		if(Summoning)
			r = Rate * fxlabParticleSystemScale;
		else
			r = Rate * DeltaTime * fxlabParticleSystemScale;

		num = floor(/*Rate*/r);
		AddRateFactor += r - num;
		if(AddRateFactor > 1.0f){
			AddRateFactor -= 1.0f;
			num += 1.0f;	
		};

//		if(fxlabClientRND.UnitRND() < /*Rate*/r - num)
//			num++;
		n = round(num);
		delta = (EndPower - StartPower) / Age;

		for(i = 0;i < n;i++){
			p = AddParticle();
			EmitterPosition.Get(p->Position);
			EmitterVelocity.Get(p->Velocity);

			p->Age = 0;
			p->LifeTime = Age;
			p->Phase = StartPower;
			p->DeltaPhase = delta;
			p->ImagePoint = fxlabResourceD->GetResource(ImageType,ImageID + fxlabClientRND.RND(ImageNoise));
		};
	};
};

void fxlabFearSpring::KeyUpdate(void)
{
	fxlabParticleFear::KeyUpdate();
	Rate = KeyData[FXLAB_FEAR_SPRING_DATA_RATE];
	Age = KeyData[FXLAB_FEAR_SPRING_DATA_AGE];
	StartPower = KeyData[FXLAB_FEAR_SPRING_DATA_START_POWER];
	EndPower = KeyData[FXLAB_FEAR_SPRING_DATA_END_POWER];

	EmitterPosition.Load(fxlabResourceD->GetResource(FXLAB_SCRIPT_SECTION_KEY,round(KeyData[FXLAB_FEAR_SPRING_DATA_POSITION_KEY_ID])),KeyTime);
	EmitterVelocity.Load(fxlabResourceD->GetResource(FXLAB_SCRIPT_SECTION_KEY,round(KeyData[FXLAB_FEAR_SPRING_DATA_VELOCITY_KEY_ID])),KeyTime);
};

void fxlabFearSpring::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_FEAR_SPRING_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabFearSpring");
};

//--------------------------------------------------------------------

void fxlabCrazyTwister::Open(void)
{
	fxlabParticleFear::Open();
	OffsetAngle = fxlabClientRND.UnitRND() * M_PI;
};

void fxlabCrazyTwister::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_FEAR_TWISTER_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabCrazyTwister");
};

void fxlabCrazyTwister::KeyUpdate(void)
{
	fxlabParticleFear::KeyUpdate();
	NumBranch = round(KeyData[FXLAB_FEAR_TWISTER_DATA_NUM_BRANCH]);
	NumWing = round(KeyData[FXLAB_FEAR_TWISTER_DATA_NUM_WING]);

	StartHeight = KeyData[FXLAB_FEAR_TWISTER_DATA_START_HEIGHT];
	EndHeight = KeyData[FXLAB_FEAR_TWISTER_DATA_END_HEIGHT];

	StartRadius = KeyData[FXLAB_FEAR_TWISTER_DATA_START_RADIUS];
	EndRadius = KeyData[FXLAB_FEAR_TWISTER_DATA_END_RADIUS];

	Age = KeyData[FXLAB_FEAR_TWISTER_DATA_AGE];
	Noise = KeyData[FXLAB_FEAR_TWISTER_DATA_NOISE];

	EmitterSpeed = KeyData[FXLAB_FEAR_TWISTER_DATA_EMITTER_SPEED];
	EmitterAngle = KeyData[FXLAB_FEAR_TWISTER_DATA_EMITTER_ANGLE];

	StartPowerBranch = KeyData[FXLAB_FEAR_TWISTER_DATA_START_BRANCH_POWER];
	EndPowerBranch = KeyData[FXLAB_FEAR_TWISTER_DATA_END_BRANCH_POWER];

	StartPower = KeyData[FXLAB_FEAR_TWISTER_DATA_START_POWER];
	EndPower = KeyData[FXLAB_FEAR_TWISTER_DATA_END_POWER];
};

void fxlabCrazyTwister::Show(void)
{
	int i,n;
	cInterfaceVisGeneric *IVisGeneric;
	cScene* scene;
	fxlabParticleType* p;
	
	n = 1;
	if(Visibility && ParticleList.first() != ParticlePoint){
		IVisGeneric=CreateIVisGeneric();
		scene = ((cVisGeneric*)(IVisGeneric))->GetActiveScene();
		assert(scene);
		MatXf mat(Mat3f::ID,Vect3f(Position));
		for(i = 0;i < scene->GetNumberCamera();i++){
			if(Visibility & n){
				p = ParticleList.first();
				P3D->BeginList(scene->GetCamera(i),p->ImagePoint->GetSpriteID(),mat);
				while(p != ParticlePoint){
					P3D->AttachCenter(Vect3f(p->Position.x * cosf(p->Position.y + OffsetAngle),p->Position.x * sinf(p->Position.y + OffsetAngle),p->Position.z),p->ImagePoint->GetSpriteData(p->Phase),p->ImagePoint->GetSpriteID());
					p = p->next;
				};
				P3D->EndList();
			};
			n <<= 1;
		};
		IVisGeneric->Release();
	};
};

void fxlabCrazyTwister::CoreProcess(void)
{
	Vect3f v;
 	fxlabParticleType* p;
	fxlabParticleType* pp;

	FXLAB_FEAR_BORDER_DEFINE;

	p = ParticleList.first();

	while(p != ParticlePoint){
		pp = p->next;
		p->Age += DeltaTime;
		if(p->Age > p->LifeTime){
			DeleteParticle(p);
		}else{
			v = p->Velocity;
			v *= DeltaTime;
			p->Position += v;
			p->Phase += p->DeltaPhase*DeltaTime;
			FXLAB_FEAR_BORDER_PROCESS;
		};
		p = pp;
	};
	OffsetAngle += EmitterSpeed * DeltaTime;
//	ConvertPosition();
};

void fxlabCrazyTwister::CoreGenerate(void)
{
	fxlabParticleType* p;
	float dz,dr,da;
	float r,h;
	float angle,step_angle;
	float pow,step_pow;
	int i,j;

	if(fxlabClientRND.UnitRND() * fxlabInvParticleSystemScale < DeltaTime || Summoning){
		dz = (EndHeight - StartHeight) / Age;
		dr = (EndRadius - StartRadius) / Age;
		da = EmitterAngle / Age;
		step_angle = 2 * M_PI / NumBranch;
		step_pow = (EndPowerBranch - StartPowerBranch) / NumBranch;

		r = StartRadius;
		h = StartHeight;

		for(j = 0;j < NumWing;j++){
			angle = 0;
			pow = StartPowerBranch;
			for(i = 0;i < NumBranch;i++){
				p = AddParticle();
				p->Position.x = r + fxlabClientRND.UnitRND() * Noise;
				p->Position.y = angle;
				p->Position.z = h;

				p->Velocity.x = dr - dr*Noise * fxlabClientRND.UnitRND();
				p->Velocity.y = da;
				p->Velocity.z = dz;

				p->Age = 0;
				p->LifeTime = Age;

				p->Phase = pow * StartPower;
				p->DeltaPhase = (pow * EndPower - p->Phase) / Age;
				p->ImagePoint = fxlabResourceD->GetResource(ImageType,ImageID + fxlabClientRND.RND(ImageNoise));

				angle += step_angle;
				pow += step_pow;
			};
		};
	};
};


void fxlabCrazyTwister::ConvertPosition(void)
{	
	if(ProcessMaxX < 0) ProcessMaxX = -ProcessMaxX;
	if(ProcessMinX < 0) ProcessMinX = -ProcessMinX;

	if(ProcessMaxX > ProcessMinX)
		VsS = Vect3f(ProcessMaxX,ProcessMaxX,ProcessMaxZ - ProcessMinZ);
	else
		VsS = Vect3f(ProcessMinX,ProcessMinX,ProcessMaxZ - ProcessMinZ);

	VsL.x = XCYCL(int(round(Position.x - VsS.x)));
	VsL.y = YCYCL(int(round(Position.y - VsS.y)));
	VsL.z = Position.z - VsS.z;

	VsS *= 2.0f;
};

//--------------------------------------------------------------------

void fxlabRubberSnake::Open(void)
{
	fxlabParticleFear::Open();
	Spline = NULL;
	Conversion = 0;
};

void fxlabRubberSnake::CoreGenerate(void)
{
	float pow;
	double d;
	Vect3f pos,vel;
	fxlabParticleType* p;
	HermitIterator<Vect3f> warp;
	list<Vect3f>::iterator n;
	int cnt1,cnt2;
	Vect3f vd,vp;

	FXLAB_FEAR_BORDER_DEFINE;

	DeleteAllParticle();
	pow = Conversion;

	if(Spline){
		cnt1 = 0;
		cnt2 = 0;
		FOR_EACH((*Spline),n){
			if(cnt1 > 0){
				vd = vp;
				vd -= (*n);
				if(vd.sumAbs() > 0.0001f)
					cnt2++;
			};
			cnt1++;
			vp = *n;
		};

		if(cnt1 >= 4 && cnt2 >= 3){
			warp.set(Spline,(*Spline).begin(),0.0);

			while(!warp.isOut()){
				pos = warp();
				vel = warp.deriv();
				p = AddParticle();
				p->Position = pos;
				p->Phase = pow;

				p->ImagePoint = fxlabResourceD->GetResource(ImageType,ImageID + fxlabClientRND.RND(ImageNoise));

				FXLAB_FEAR_BORDER_PROCESS;
				d = vel.norm();
				if(d < 0.001)
					warp += 0.001f;
				else
					warp += 4.0 / d;
			};
		};
	};
//	ConvertPosition();
};

void fxlabRubberSnake::SetConversion(int conv)
{
	Conversion = (float)(conv) / 255.0f;
};

void fxlabRubberSnake::SetSplineOwner(void* point)
{
	Spline = (list<Vect3f>*)(point);
};


//--------------------------------------------------------

void fxlabCrazyExplode::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_FEAR_EXPLODE_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabCrazyExplode");
};

void fxlabCrazyExplode::KeyUpdate(void)
{
	fxlabFearSpring::KeyUpdate();
	PointPower1 = KeyData[FXLAB_FEAR_EXPLODE_DATA_POINT_POWER1];
	PointEpsilon1 = KeyData[FXLAB_FEAR_EXPLODE_DATA_POINT_EPSILON1];
	PointPower2 = KeyData[FXLAB_FEAR_EXPLODE_DATA_POINT_POWER2];
	PointEpsilon2 = KeyData[FXLAB_FEAR_EXPLODE_DATA_POINT_EPSILON2];
};

void fxlabCrazyExplode::CoreProcess(void)
{
	Vect3f v;
 	fxlabParticleType* p;
	fxlabParticleType* pp;
	float r;
	float power1,power2;

	FXLAB_FEAR_BORDER_DEFINE;

	p = ParticleList.first();

	power1 = PointPower1 * DeltaTime;
	power2 = PointPower2 * DeltaTime;

	while(p != ParticlePoint){
		pp = p->next;
		p->Age += DeltaTime;
		if(p->Age > p->LifeTime){
			DeleteParticle(p);
		}else{
			v.x = -p->Position.x;
			v.y = -p->Position.y;
			v.z = -p->Position.z;

			r = v.norm2();
			v *= (power1 / (r + PointEpsilon1) + power2 / (r + PointEpsilon2));

			v += p->Velocity;
			p->Velocity = v;
			v *= DeltaTime;
			p->Position += v;
			p->Phase += p->DeltaPhase * DeltaTime;
			FXLAB_FEAR_BORDER_PROCESS;
		};
		p = pp;
	};
//	ConvertPosition();
};

//--------------------------------------------------------

void fxlabCrazyTail::Open(void)
{
	fxlabFearSpring::Open();
	Velocity = Vect3f::ZERO;	
};

void fxlabCrazyTail::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_FEAR_TAIL_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabCrazyTail");
};

void fxlabCrazyTail::KeyUpdate(void)
{
	fxlabFearSpring::KeyUpdate();
	PointPower = KeyData[FXLAB_FEAR_TAIL_DATA_POINT_POWER];
	PointEpsilon = KeyData[FXLAB_FEAR_TAIL_DATA_POINT_EPSILON];
	PointRadius = KeyData[FXLAB_FEAR_TAIL_DATA_POINT_RADIUS];
};

void fxlabCrazyTail::CoreProcess(void)
{
	float r;
	Vect3f pos;
	Vect3f v;
 	fxlabParticleType* p;
	fxlabParticleType* pp;
	float power;

	FXLAB_FEAR_BORDER_DEFINE;

	p = ParticleList.first();

	pos = Velocity;
	pos.Normalize();
	pos *= PointRadius;

	power = PointPower * DeltaTime;
	while(p != ParticlePoint){
		pp = p->next;
		p->Age += DeltaTime;
		if(p->Age > p->LifeTime){
			DeleteParticle(p);
		}else{
			v = pos - p->Position;

			r = v.norm2();
			v *= power / (r + PointEpsilon);

			v += p->Velocity;
			p->Velocity = v;
			v *= DeltaTime;
			p->Position += v;
			p->Phase += p->DeltaPhase * DeltaTime;

			FXLAB_FEAR_BORDER_PROCESS;
		};
		p = pp;
	};
//	ConvertPosition();
};

void fxlabCrazyTail::SetVelocity(const Vect3f& v)
{
	Velocity = v;
};

//---------------------------------------------

void fxlabCrazyDust::Open(void)
{
	fxlabFearSpring::Open();
	Velocity = Vect3f::ZERO;
};

void fxlabCrazyDust::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_FEAR_DUST_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabCrazyDust");
};

void fxlabCrazyDust::KeyUpdate(void)
{
	fxlabFearSpring::KeyUpdate();
	Damp = KeyData[FXLAB_FEAR_DUST_DATA_DAMP];
};

void fxlabCrazyDust::CoreProcess(void)
{
	Vect3f v;
	float damp;
 	fxlabParticleType* p;
	fxlabParticleType* pp;

	FXLAB_FEAR_BORDER_DEFINE;

	damp = 1.0f - ((1.0f - Damp) * DeltaTime);
	p = ParticleList.first();
	while(p != ParticlePoint){
		pp = p->next;
		p->Age += DeltaTime;
		if(p->Age > p->LifeTime){
			DeleteParticle(p);
		}else{
			v = p->Velocity;
			v *= damp;
			p->Velocity = v;
			v *= DeltaTime;
			p->Position += v;
			p->Phase += p->DeltaPhase * DeltaTime;

			FXLAB_FEAR_BORDER_PROCESS;
		};
		p = pp;
	};
//	ConvertPosition();
};

void fxlabCrazyDust::CoreGenerate(void)
{
	fxlabParticleType* p;
	float num;
	int n,i;
	float delta;
	float r;

	if(Summoning)
		r = Rate * fxlabParticleSystemScale;
	else
		r = Rate * DeltaTime * fxlabParticleSystemScale;

	num = floor(/*Rate*/r);
	AddRateFactor += r - num;
	if(AddRateFactor > 1.0f){
		AddRateFactor -= 1.0f;
		num += 1.0f;	
	};
//	num = floor(/*Rate*/r);
//	if(fxlabClientRND.UnitRND() < /*Rate*/r - num)
//		num++;
	n = round(num);

	delta = (EndPower - StartPower) / Age;

	for(i = 0;i < n;i++){
		p = AddParticle();
		EmitterPosition.Get(p->Position);
		EmitterVelocity.Get(p->Velocity);
		p->Velocity += Velocity;

		p->Age = 0;
		p->LifeTime = Age;
		p->ImagePoint = fxlabResourceD->GetResource(ImageType,ImageID + fxlabClientRND.RND(ImageNoise));

		p->Phase = StartPower;
		p->DeltaPhase = delta;
	};	
};

void fxlabCrazyDust::SetVelocity(const Vect3f& v)
{
	Velocity = v;
};

//-----------------------------------------------------------

void fxlabFearFire::Open(void)
{
	fxlabFearSpring::Open();
	Velocity = Vect3f::ZERO;
};

void fxlabFearFire::CoreProcess(void)
{
	float r;
	Vect3f vd;
	Vect3f v,s,n;
	Vect3f pos;
	Vect3f dir;
 	fxlabParticleType* p;
	fxlabParticleType* pp;
	float power;

	FXLAB_FEAR_BORDER_DEFINE;

	p = ParticleList.first();
	pos = Vect3f::ZERO;
	dir = Velocity;
	dir += Direction;

	power = Power * DeltaTime;
	while(p != ParticlePoint){
		pp = p->next;
		p->Age += DeltaTime;
		if(p->Age > p->LifeTime){
			DeleteParticle(p);
		}else{
			v = pos - p->Position;
			s = (Axis * v.dot(Axis));
			n = v - s;
			r = n.norm2();

			vd = (n + dir) * (power / (r + Epsilon));
			vd += p->Velocity;

			p->Velocity = vd;
			vd *= DeltaTime;
			p->Position += vd;
			p->Phase += p->DeltaPhase * DeltaTime;
			FXLAB_FEAR_BORDER_PROCESS;
		};
		p = pp;
	};	
//	ConvertPosition();
};

void fxlabFearFire::SetVelocity(const Vect3f& v)
{
	Velocity = v;
};

void fxlabFearFire::KeyUpdate(void)
{
	fxlabFearSpring::KeyUpdate();

	Power = KeyData[FXLAB_FEAR_FIRE_DATA_POWER];
	Epsilon = KeyData[FXLAB_FEAR_FIRE_DATA_EPSILON];

	Direction = Vect3f(KeyData[FXLAB_FEAR_FIRE_DATA_DIRECTION_X],KeyData[FXLAB_FEAR_FIRE_DATA_DIRECTION_Y],KeyData[FXLAB_FEAR_FIRE_DATA_DIRECTION_Z]);
	Axis = Vect3f(KeyData[FXLAB_FEAR_FIRE_DATA_AXIS_X],KeyData[FXLAB_FEAR_FIRE_DATA_AXIS_Y],KeyData[FXLAB_FEAR_FIRE_DATA_AXIS_Z]);
	Axis.normalize();
};

void fxlabFearFire::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_FEAR_FIRE_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabFearFire");
};

//---------------------------------------------------------------------

void fxlabSparkGenerator::CoreProcess(void)
{
	Vect3f v;
 	fxlabParticleType* p;
	fxlabParticleType* pp;	

	FXLAB_FEAR_BORDER_DEFINE;
	p = ParticleList.first();
	while(p != ParticlePoint){
		pp = p->next;
		p->Age += DeltaTime;
		if(p->Age > p->LifeTime){
			DeleteParticle(p);
		}else{
			v = p->Velocity;
			v *= DeltaTime;
			p->Position += v;
			p->Phase += p->DeltaPhase * DeltaTime;
			FXLAB_FEAR_BORDER_PROCESS;
		};
		p = pp;
	};
//	ConvertPosition();
};


//----------------------------------------------------------------

void fxlabFearStream::Open(void)
{
	fxlabFearSpring::Open();

	Velocity = Vect3f::ZERO;
	ScaleRate = 1.0f;
};

void fxlabFearStream::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_FEAR_STREAM_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabFearStream");
};

void fxlabFearStream::KeyUpdate(void)
{
	fxlabFearSpring::KeyUpdate();

	Force = Vect3f(KeyData[FXLAB_FEAR_STREAM_DATA_FORCE_X],KeyData[FXLAB_FEAR_STREAM_DATA_FORCE_Y],KeyData[FXLAB_FEAR_STREAM_DATA_FORCE_Z]);
	ReflectionImpulse = KeyData[FXLAB_FEAR_STREAM_DATA_REFLECTION_IMPULSE];
	VelocityImpulse = KeyData[FXLAB_FEAR_STREAM_DATA_VELOCITY_IMPULSE];
};

void fxlabFearStream::CoreGenerate(void)
{
	float num;
	int n,i;
	float delta,r;
	fxlabParticleType* p;
	Vect3f v;

	v = Velocity;
	v *= VelocityImpulse;

	if(Summoning)
		r = Rate * ScaleRate * fxlabParticleSystemScale;
	else
		r = Rate * ScaleRate * DeltaTime * fxlabParticleSystemScale;

	num = floor(/*Rate*/r);
	AddRateFactor += r - num;
	if(AddRateFactor > 1.0f){
		AddRateFactor -= 1.0f;
		num += 1.0f;	
	};

//	num = floor(r);
//	if(fxlabClientRND.UnitRND() < r - num)
//		num++;
	n = round(num);

	delta = (EndPower - StartPower) / Age;

	for(i = 0;i < n;i++){
		p = AddParticle();
		EmitterPosition.Get(p->Position);
		EmitterVelocity.Get(p->Velocity);
		p->Velocity += v;

		p->Age = 0;
		p->LifeTime = Age;

		p->Phase = StartPower;
		p->DeltaPhase = delta;
		p->ImagePoint = fxlabResourceD->GetResource(ImageType,ImageID + fxlabClientRND.RND(ImageNoise));
	};
};

void fxlabFearStream::CoreProcess(void)
{
	int x,y,z;
	Vect3f v;
 	fxlabParticleType* p;
	fxlabParticleType* pp;

	FXLAB_FEAR_BORDER_DEFINE;

	p = ParticleList.first();

	x = round(Position.x);
	y = round(Position.y);
	z = round(Position.z);

	while(p != ParticlePoint){
		pp = p->next;
		p->Age += DeltaTime;
		if(p->Age > p->LifeTime){
			DeleteParticle(p);
		}else{
			v = p->Velocity;
			if((round(p->Position.z) + z) <= ZV(XCYCL(int(round(p->Position.x)) + x),YCYCL(int(round(p->Position.y)) + y))){
				if(v.z > 0)
					v.z *= ReflectionImpulse;
				else
					v.z *= -ReflectionImpulse;
			}else
				v += Force;
			p->Velocity = v;
			v *= DeltaTime;
			p->Position += v;
			p->Phase += p->DeltaPhase * DeltaTime;
			FXLAB_FEAR_BORDER_PROCESS;
		};
		p = pp;
	};
//	ConvertPosition();
};

void fxlabFearStream::SetVelocity(const Vect3f& v)
{
	Velocity = v;
};

void fxlabFearStream::SetScale(float scale)
{
	ScaleRate = scale;
};

//----------------------------------------------------------


void fxlabCrazyTailWaterClip::CoreProcess(void)
{
	Vect3f pos;
	Vect3f v;
 	fxlabParticleType* p;
	fxlabParticleType* pp;
	float r;
	float z;
	float power;

	FXLAB_FEAR_BORDER_DEFINE;

	p = ParticleList.first();

	pos = Velocity;
	pos.normalize();
	pos *= PointRadius;

//	z = vMap->LevelH2O;
	z = vMap->LevelH2O - Position.z;

	power = PointPower * DeltaTime;

	while(p != ParticlePoint){
		pp = p->next;
		p->Age += DeltaTime;
		if(p->Age > p->LifeTime || p->Position.z >= z){
			DeleteParticle(p);
		}else{
			v = pos - p->Position;

			r = v.norm2();
			v *= power / (r + PointEpsilon);

			v += p->Velocity;
			p->Velocity = v;
			v *= DeltaTime;
			p->Position += v;
			p->Phase += p->DeltaPhase * DeltaTime;

			FXLAB_FEAR_BORDER_PROCESS;
		};
		p = pp;
	};
//	ConvertPosition();
};

void fxlabCrazyDustWaterClip::CoreProcess(void)
{
	float z;
	Vect3f v;
 	fxlabParticleType* p;
	fxlabParticleType* pp;
	FXLAB_FEAR_BORDER_DEFINE;
	float damp;

	p = ParticleList.first();
	
//	z = vMap->LevelH2O;
	z = vMap->LevelH2O - Position.z;

	damp = 1.0f - (1.0f - Damp) * DeltaTime;
	while(p != ParticlePoint){
		pp = p->next;
		p->Age += DeltaTime;
		if(p->Age > p->LifeTime || p->Position.z >= z){
			DeleteParticle(p);
		}else{
			v = p->Velocity;
			v.x *= damp;
			v.y *= damp;
			p->Velocity = v;
			v *= DeltaTime;
			p->Position += v;
			p->Phase += p->DeltaPhase * DeltaTime;

			FXLAB_FEAR_BORDER_PROCESS;
		};
		p = pp;
	};
//	ConvertPosition();
};


void fxlabSparkWaterClip::CoreProcess(void)
{
	float z;
	Vect3f v;
 	fxlabParticleType* p;
	fxlabParticleType* pp;	

	FXLAB_FEAR_BORDER_DEFINE;
	p = ParticleList.first();

	z = vMap->LevelH2O - Position.z;
	while(p != ParticlePoint){
		pp = p->next;
		p->Age += DeltaTime;
		if(p->Age > p->LifeTime){
			DeleteParticle(p);
		}else{
			v = p->Velocity;
			v *= DeltaTime;
			
			p->Position += v;
			if(p->Position.z > z)
				p->Position.z = z;
			
			p->Phase += p->DeltaPhase * DeltaTime;
			FXLAB_FEAR_BORDER_PROCESS;
		};
		p = pp;
	};
//	ConvertPosition();
};

//--------------------------------------

void fxlabSparkWind::Open(void)
{
	fxlabSparkGenerator::Open();
	Velocity = Vect3f::ZERO;
};

void fxlabSparkWind::CoreGenerate(void)
{
	float num;
	int n,i;
	float delta;
	fxlabParticleType* p;
	Vect3f vel;
	float r;

	vel = Velocity;
	vel *= KeyData[FXLAB_SPARK_WIND_DATA_SPEED];	

	if(Summoning)
		r = Rate * fxlabParticleSystemScale;
	else
		r = Rate * DeltaTime * fxlabParticleSystemScale;

	num = floor(/*Rate*/r);
	AddRateFactor += r - num;
	if(AddRateFactor > 1.0f){
		AddRateFactor -= 1.0f;
		num += 1.0f;	
	};
//	num = floor(/*Rate*/r);
//	if(fxlabClientRND.UnitRND() < /*Rate*/r - num)
//		num++;
	n = round(num);

	delta = (EndPower - StartPower) / Age;

	for(i = 0;i < n;i++){
		p = AddParticle();
		EmitterPosition.Get(p->Position);
		EmitterVelocity.Get(p->Velocity);
		p->Velocity += vel;

		p->Age = 0;
		p->LifeTime = Age;

		p->Phase = StartPower;
		p->DeltaPhase = delta;
		p->ImagePoint = fxlabResourceD->GetResource(ImageType,ImageID + fxlabClientRND.RND(ImageNoise));
	};
};

void fxlabSparkWind::KeyCheck(void)
{
	if(KeyPoint->GetKeyNum() != FXLAB_SPARK_WIND_DATA_MAX)
		ErrH.Abort("Bad Key of fxlabSparkWind");
};


//---------------------

void fxlabSparkWindWaterClip::CoreProcess(void)
{
	float z;
	Vect3f v;
 	fxlabParticleType* p;
	fxlabParticleType* pp;	

	FXLAB_FEAR_BORDER_DEFINE;
	p = ParticleList.first();

	z = vMap->LevelH2O;
	while(p != ParticlePoint){
		pp = p->next;
		p->Age += DeltaTime;
		if(p->Age > p->LifeTime){
			DeleteParticle(p);
		}else{
			v = p->Velocity;
			v *= DeltaTime;
			
			p->Position += v;
			if(p->Position.z > z)
				p->Position.z = z;
			
			p->Phase += p->DeltaPhase * DeltaTime;
			FXLAB_FEAR_BORDER_PROCESS;
		};
		p = pp;
	};
//	ConvertPosition();
};

//-----------------------------------

void fxlabSparkGeneratorExternal::CoreGenerate(void)
{
	int n,i;
	float num;
	float delta;
	fxlabParticleType* p;
	Vect3f add_p;
	Vect3f add_v;
	float r;

	if(Rate >= 0.0001){
		if(Summoning)
			r = Rate * fxlabParticleSystemScale;
		else
			r = Rate * DeltaTime * fxlabParticleSystemScale;

		num = floor(/*Rate*/r);
		AddRateFactor += r - num;
		if(AddRateFactor > 1.0f){
			AddRateFactor -= 1.0f;
			num += 1.0f;	
		};

//		num = floor(/*Rate*/r);
//		if(fxlabClientRND.UnitRND() < /*Rate*/r - num)
//			num++;

		n = round(num);
		delta = (EndPower - StartPower) / Age;

		for(i = 0;i < n;i++){
			p = AddParticle();
			EmitterPosition.Get(p->Position);
			EmitterVelocity.Get(p->Velocity);

			ExternalPosition.Get(add_p);
			ExternalVelocity.Get(add_v);

			p->Position += add_p;
			p->Velocity += add_v;

			p->Age = 0;
			p->LifeTime = Age;
			p->Phase = StartPower;
			p->DeltaPhase = delta;
			p->ImagePoint = fxlabResourceD->GetResource(ImageType,ImageID + fxlabClientRND.RND(ImageNoise));
		};
	};
};

void fxlabSparkGeneratorExternal::SetPositionDomain(fxlabSwarmDomain* p)
{
	ExternalPosition.Copy(p);
};

void fxlabSparkGeneratorExternal::SetVelocityDomain(fxlabSwarmDomain* p)
{
	ExternalVelocity.Copy(p);
};

//--------------------------------------------------

void fxlabSparkLine::Open(void)
{
	fxlabSparkGenerator::Open();
	Velocity = Vect3f::ZERO;
};

void fxlabSparkLine::CoreGenerate(void)
{
	int n,i;
	float num;
	float delta;
	fxlabParticleType* p;
	float r;
	Vect3f tv;
	float k;
	
	if(Rate >= 0.0001){
		if(Summoning)
			r = Rate * fxlabParticleSystemScale;
		else
			r = Rate * DeltaTime * fxlabParticleSystemScale;

		num = floor(/*Rate*/r);
		AddRateFactor += r - num;
		if(AddRateFactor > 1.0f){
			AddRateFactor -= 1.0f;
			num += 1.0f;	
		};

//		num = floor(/*Rate*/r);
//		if(fxlabClientRND.UnitRND() < /*Rate*/r - num)
//			num++;
		n = round(num);
		delta = (EndPower - StartPower) / Age;

		for(i = 0;i < n;i++){
			p = AddParticle();
			EmitterPosition.Get(tv);
			EmitterVelocity.Get(p->Velocity);

			k = fxlabClientRND.UnitRND();
			tv.x += Velocity.x * k;
			tv.y += Velocity.y * k;
			tv.z += Velocity.z * k;
			p->Position = tv;

			p->Age = 0;
			p->LifeTime = Age;
			p->Phase = StartPower;
			p->DeltaPhase = delta;
			p->ImagePoint = fxlabResourceD->GetResource(ImageType,ImageID + fxlabClientRND.RND(ImageNoise));
		};
	};
};

//----------------------------------------------

/*
extern int hCamera;
extern float mchCameraX;
extern float mchCameraY;

void fxlabSnowLayer::Show(void)
{
	int i,n;
	cInterfaceVisGeneric *IVisGeneric;
	cScene* scene;
	fxlabParticleType* p;
	Vect3f v,g;
	cCamera* cp;

	n = 1;
	if(Visibility && ParticleList.first() != ParticlePoint){
		IVisGeneric=CreateIVisGeneric();
		scene = ((cVisGeneric*)(IVisGeneric))->GetActiveScene();
		assert(scene);
		for(i = 0;i < scene->GetNumberCamera();i++){
			if(Visibility & n){
				p = ParticleList.first();

				cp = static_cast<cCamera*>(scene->GetCamera(i));
				cp->GetMatrix().invXformVect(cp->GetMatrix().trans(),v);
				v *= -1.0f;
				v.z += 20.0f;

				MatXf mat(Mat3f::ID,v);
				P3D->BeginList(scene->GetCamera(i),p->ImagePoint->GetSpriteID(),mat);
				while(p != ParticlePoint){
					P3D->AttachCenter(p->Position,p->ImagePoint->GetSpriteData(p->Phase),p->ImagePoint->GetSpriteID());
					p = p->next;
				};
				P3D->EndList();
			};
			n <<= 1;
		};
		IVisGeneric->Release();
	};
};


void fxlabSnowLayer::CheckVisibility(void)
{
	Visibility = 0xffffffff;
};

void fxlabSnowLayer::StopTimeCheckVisibility(void)
{
	Visibility = 0xffffffff;
};

void fxlabSnowLayer::ConvertPosition(void)
{
};

void fxlabSnowLayer::CoreProcess(void)
{
	Vect3f v;
 	fxlabParticleType* p;
	fxlabParticleType* pp;	

	p = ParticleList.first();
	while(p != ParticlePoint){
		pp = p->next;
		p->Age += DeltaTime;
		if(p->Age > p->LifeTime){
			DeleteParticle(p);
		}else{
			v = p->Velocity;
			v *= DeltaTime;
			p->Position += v;
			p->Phase += p->DeltaPhase * DeltaTime;
		};
		p = pp;
	};
};
*/

#include "TileMap.h"

void fxlabSnowLayer::Open(void)
{
	fxlabSparkGenerator::Open();
	TilePoint = NULL;
};

void fxlabSnowLayer::CheckVisibility(void)
{	
	if(TilePoint->GetVisible())
		Visibility = 0xffffffff;
	else{
		Visibility = 0;
		if(RecallTime > 0) RecallTime--;
	};
};

void fxlabSnowLayer::StopTimeCheckVisibility(void)
{
	if(TilePoint->GetVisible())
		Visibility = 0xffffffff;
	else{
		Visibility = 0;
	};
};

void fxlabSnowLayer::ConvertPosition(void)
{
};

void fxlabSnowLayer::CoreProcess(void)
{
	Vect3f v;
 	fxlabParticleType* p;
	fxlabParticleType* pp;	

	p = ParticleList.first();
	while(p != ParticlePoint){
		pp = p->next;
		p->Age += DeltaTime;
		if(p->Age > p->LifeTime){
			DeleteParticle(p);
		}else{
			v = p->Velocity;
			v *= DeltaTime;
			p->Position += v;
			p->Phase += p->DeltaPhase * DeltaTime;
		};
		p = pp;
	};
};

void fxlabSnowLayer::SetTilePoint(void* pTileMap)
{ 
	TilePoint = (sTileMap*)pTileMap;
};


//-------------------------------------------------------

extern int mchSplitScreenGame;

const double FXLAB_SPLINE_WAY_DELTA = 0.01f;
const double FXLAB_SPLINE_WAY_SPEED = 0.1;

void fxlabSplineWay::Open(void)
{
	fxlabClientEvolutionType::Open();
	ImagePoint = fxlabResourceD->GetResource(FXLAB_SCRIPT_SECTION_SPRITE,FXLAB_ID_SPRITE_SPLINE_WAY);
	Offset = 0;
	Owner = NULL;
	Phase = 0;
};

extern int mchFreezeTime;
extern int mchTurnBasedGame;
void fxlabSplineWay::Show(void)
{
	cInterfaceVisGeneric *IVisGeneric;
	Vect3f pos;
	Vect3f vc,vp,vl;
	list<Vect3f>::iterator p;
	HermitIterator<Vect3f> warp;
	list<Vect3f> spline;
	mchControlPoint* s;

	if(mchTurnBasedGame && mchFreezeTime && Owner && (Owner->flags & MCH_ACTIVE_RACER)){
		s = Owner->seedLst.first();
		if(s){
			vl = vc = Owner->R();
			spline.push_back(Vect3f(0,0,0));
			vp = Vect3f(0,0,0);
			while(s){
				vp.x += getDistX((float)s->PosX,vl.x);
				vp.y += getDistY((float)s->PosY,vl.y);
				vp.z += s->PosZ - vl.z;
				vl = Vect3f(s->PosX,s->PosY,s->PosZ);

				spline.push_back(vp);
				s = s->next;
			};

			IVisGeneric=CreateIVisGeneric();
			p = spline.begin();
			warp.set(&spline,p,Offset);

			MatXf mat(Mat3f::ID,Vect3f::ZERO);

			if(mchSplitScreenGame){
				if(Owner->flags & MCH_ACTIVE_RACER2)
					P3D->BeginListShare(camera_dispatcher2->camera(),ImagePoint->GetSpriteID(),mat);
				else
					P3D->BeginListShare(camera_dispatcher->camera(),ImagePoint->GetSpriteID(),mat);
			}else
				P3D->BeginListShare(camera_dispatcher->camera(),ImagePoint->GetSpriteID(),mat);

			while(!warp.isOut()){
				pos = warp();
				pos += vc;
				pos = CYCLE(pos);
				pos.z = tri_map -> H(pos.xi(), pos.yi()) + 10.0f;
				P3D->AttachCenterShare(pos,ImagePoint->GetSpriteData(Phase),ImagePoint->GetSpriteID());

				warp += FXLAB_SPLINE_WAY_SPEED;
			};

			P3D->EndList();

			IVisGeneric->Release();

			Offset += FXLAB_SPLINE_WAY_DELTA;
			if(Offset >= FXLAB_SPLINE_WAY_SPEED) Offset = 0;
		};
		Phase += 0.1 * DeltaTime;
		if(Phase >= 1.0f)
			Phase = 0;
	};
};

