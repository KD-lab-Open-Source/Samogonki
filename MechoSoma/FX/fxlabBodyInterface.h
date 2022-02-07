#include "Meteorite.h"

template<class ObjectType>
void fxlabCreateBolide(int model,int noise,const Vect3f& pos,const Vect3f& vel,float damage,int client_key_type,ArcaneStatisticsInfo* stat,fxlabProtectionType* protect,ObjectType* stuff)
{
	Body* p;
	OwnerProtection* op;
	fxlabGeneralObjectType* ct;

	if(stat) ArcaneStatisticsService::setCurrentArcane(*stat);
	p = new ObjectType(M3D_TOTAL_TYPE(model + fxlabServerRND.RND(noise),M3D_XREAL),pos,vel,damage,-1);
	if(protect){
		op = dynamic_cast<OwnerProtection*>(p);
		if(op) op->set_owner(protect->ProtectionID,protect->ProtectionTime);
	};
	Mdisp -> attachBody(p);

	if(client_key_type >= 0){
		ct = fxlabClientD->CreateObject(client_key_type);
		ct->SetStartTime(fxlabClientD->GetTime());
		ct->SetPosition(pos);
		ct->SetVelocity(vel);
		ct->SetBody(p);
		ct->Start();
	};
};

template<class ObjectType>
void fxlabCreateSplinter(int model,int noise,Vect3f& pos,float rate,float start_psi,float thetta,float delta_thetta,float speed,float damage,int client_key_type,class ArcaneStatisticsInfo* stat,struct fxlabProtectionType* protect,ObjectType* stuff)
{
	int i,n;
	float psi,dpsi,f;
	Vect3f v;

	f = floor(rate);
	if(fxlabServerRND.UnitRND() < rate - f)
		n = round(f) + 1;
	else
		n = round(f);

	psi = start_psi;
	dpsi = 2.0f * M_PI / n;

	for(i = 0;i < n;i++){
		v.setSpherical(psi,thetta + fxlabServerRND.UnitRND() * delta_thetta,speed);
		fxlabCreateBolide<ObjectType>(model,noise,pos,v,damage,client_key_type,stat,protect,NULL);

		psi += dpsi;
	};
};


//Bomb
class fxlabCannonBomb : public Meteorite
{
public:
	fxlabCannonBomb(int m3d_model, const Vect3f& R_, const Vect3f& V_, double damage__, int owner_ID = 0)
		: Meteorite( m3d_model, R_, V_, damage__, owner_ID) {}
	void kill();
};

//VolcanicBomb
class fxlabVolcanicBomb : public Meteorite
{
public:
	fxlabVolcanicBomb(int m3d_model, const Vect3f& R_, const Vect3f& V_, double damage__, int owner_ID = 0)
		: Meteorite( m3d_model, R_, V_, damage__, owner_ID) {}
	void kill();
};

//FireBolide
class fxlabFireBolide : public StatisticalMeteorite
{
public:
	fxlabFireBolide(int m3d_model, const Vect3f& R_, const Vect3f& V_, double damage__, int owner_ID = 0)
		: StatisticalMeteorite( m3d_model, R_, V_, damage__, owner_ID) { scaleRadius(2); }
	void kill();
};

//FireBolide
class fxlabSnowBall : public StatisticalMeteorite
{
public:
	fxlabSnowBall(int m3d_model, const Vect3f& R_, const Vect3f& V_, double damage__, int owner_ID = 0);
	void body_overlap_reaction(Body& body);
	void calc_forces_and_drags();
	void kill();
};

//Mortar
class fxlabSleepyMortar : public BonusMeteorite
{
	MeasurementTimer color_time;
public:
	fxlabSleepyMortar(int m3d_model, const Vect3f& R_, const Vect3f& V_, double damage__, int owner_ID = 0);
	void kill();
	void quant();
};

//MinorBonus
class fxlabMinorBonus : public BonusMeteorite
{
public:
	fxlabMinorBonus(int m3d_model, const Vect3f& R_, const Vect3f& V_, double damage__, int owner_ID = 0)
		: BonusMeteorite(m3d_model, R_, V_, damage__, owner_ID) {}
	void body_overlap_reaction(Body& body);
	void kill();
};

//MajorBonus
class fxlabMajorBonus : public BonusMeteorite
{
public:
	fxlabMajorBonus(int m3d_model, const Vect3f& R_, const Vect3f& V_, double damage__, int owner_ID)
		: BonusMeteorite(m3d_model,R_,V_,damage__,owner_ID){ type_ = REAL_BONUS_METEORITE; }
	void body_overlap_reaction(Body& body);
	void kill();
};
