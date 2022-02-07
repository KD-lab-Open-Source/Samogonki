//////////////////////////////////////////////////////////////////
//	Диспетчер 
//	динамических объектов
//////////////////////////////////////////////////////////////////
#ifndef __BODY_DISPATCHER__
#define __BODY_DISPATCHER__

#include "Body.h"
#include "BodyFactory.h"

class Mechos;
class Part;
class FieldSource;

class BodyDispatcher 
{
public:
	// Types
	typedef float (*GetDistanceFunction)(const Body& body);
	

	BodyDispatcher();
	virtual ~BodyDispatcher();

	// Базовые обекты, получающие ресурс
	void attachBaseObject(BaseObject* obj);
	void deleteBaseObject(BaseObject* obj);

	// Создание и удаление тел
	void attachBody(Body* body);
	void deleteBody(Body* body);
	Body* createBody(const char* type_name, const Vect3f& pose, int angle_degree = -1000);

	// Поиск тел
	Body* searchBody(int ID);
	Body* searchBody(const Vect3f& R, int body_type, const Body* ignored_body = 0);
	Body* searchBody(GetDistanceFunction metric, int body_type);
	Part* searchPart(const Mechos& mechos, int m3dType, int m3dKind = 0); // default - any Kind
	void test_mechos_wheels_to_bodies_collisions(Body& mechos);

	// Работа с источниками поля
	void attachFieldSource(FieldSource* source); 
	void moveFieldSource(FieldSource* source, const Vect3f& pos); 
	void changeFieldSource(FieldSource* source, float radius); 
	void changeFieldSource(FieldSource* source, const Vect3f& pos, float radius); 
	void deleteFieldSource(FieldSource* source); // удаление, в том числе самого источника
	void clearOwner(int ID);

	// Кванты
	virtual void pre_quant(); // Управления, 15 Гц
	void evolve_quant(); // Эволюции, 75 Гц

	// Active Body
	void setActiveBody(Body* b);
	Body* active_body();
	Mechos* active_mechos();

	// Debug
	void showBodies();
	void showObjects();

	void setNextBody();
	void setPrevBody();

	void key_handler(int& key);
	void draw_handler();
	void print_statistics(XBuffer& buf);

protected: 
	// Types
	typedef list<ShareHandle<Body> > BodyList;
	typedef vector<Body*> TmpBodyList;
	typedef vector<ShareHandle<BaseObject> > BaseObjectDispatcher;

	// Data
	BodyList total_bodies;
	TmpBodyList dynamic_bodies;
	class ContactingBodiesList& contacting_bodies_list;

	ShareHandle<Body> active_body_;

	class FieldGrid& permanent_fields_grid;

	int involve_into_collision_test_global_counter;

	Grid2D<Body, 5> grid;

	class BodyFeatureHT& body_hash_table;
	class BodyFeatureHT& mechos_wheels_to_body_hash_table;

	BaseObjectDispatcher base_objects;

	static BodyFactory body_factory;
	
	// Protected members
	void evolve(float dt);
	void fieldsAffection(Body& body);
	void test_bodies_collisions();
	int test_body_to_body_collision(Body* body1, Body* body2, const MatXf& X12);
	int test_body_to_body_overlaping(Body* body1, Body* body2, const MatXf& X12);
	void killBody(Body* body);
	void moveBody(Body& b);
	void base_objects_quant();
	void killBaseObject(BaseObject* obj);

	// Friends
	friend class BodyClipOp;
};


class WorldDispatcher : public BodyDispatcher
{
public:
	WorldDispatcher(int track);
	int track() const { return track_; }
	int isSnow() const { return is_snow; }
	int isBossTrack() const { return boss_track; }
	float waterDamage() const { return water_damage; }
	float maxTraction() const { return max_traction; }

	void createBonusMen(char* str_cfg);
	void loadGears(class cM3D& M3D);
	
	void beep(const Vect3f& pos);

protected:
	int track_;
	int is_snow;
	int boss_track;
	float water_damage;
	float max_traction;
};

extern WorldDispatcher* Mdisp;

#endif  // __BODY_DISPATCHER__
