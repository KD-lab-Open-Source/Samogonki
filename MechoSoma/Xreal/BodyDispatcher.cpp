#include "StdAfx.h"
#include "BodyDispatcher.h"
#include "Mechos.h"
#include "SimpleClip.h"
#include "Params.h"
#include "Mesh3ds.h"
#include "mechosoma.h"
#include "FieldGrid.h"
#include "MultibodyContact.h"
#include "BodyFeatureHT.h"
#include "PolyhedronLibrary.h"
#include "Xreal_utl.h"
#include "CameraDispatcher.h"
#include "IGraph3d.h"
#include "Statistics.h"
#include "DebugPrm.h"
#include "CollisionHandler.hpp"
#include "KEYS.H"
#include <map>

BodyFactory BodyDispatcher::body_factory;

////////////////////////////////////////////////////////////////////////////////
//	Contruction-destruction
////////////////////////////////////////////////////////////////////////////////
BodyDispatcher::BodyDispatcher() 
: contacting_bodies_list(*new ContactingBodiesList),
permanent_fields_grid(*new FieldGrid),
body_hash_table(*new BodyFeatureHT(1200)),
mechos_wheels_to_body_hash_table(*new BodyFeatureHT(1200)),
involve_into_collision_test_global_counter(0)
{
	Body::IDs = 0;
	dynamic_bodies.reserve(300);
	base_objects.reserve(100);
}

BodyDispatcher::~BodyDispatcher()
{
	BodyList::iterator bi;
	FOR_EACH(total_bodies, bi)
		(*bi) -> release_body_handles();
	base_objects.clear();
	total_bodies.clear();
	active_body_ = 0;
	assert(!Body::instancies);

	delete &contacting_bodies_list;
	delete &permanent_fields_grid;
	delete &body_hash_table;
	delete &mechos_wheels_to_body_hash_table;
}

//////////////////////////////////////////////////////////////////////////
//		BaseObject List Utils
//////////////////////////////////////////////////////////////////////////
void BodyDispatcher::attachBaseObject(BaseObject* obj)
{
	obj -> alive_ = 1; 
	base_objects.push_back(obj);
	FieldSource* fs = dynamic_cast<FieldSource*>(obj);
	if(fs)
		attachFieldSource(fs);
}
void BodyDispatcher::killBaseObject(BaseObject* obj)
{
	FieldSource* fs = dynamic_cast<FieldSource*>(obj);
	if(fs)
		deleteFieldSource(fs);
}
void BodyDispatcher::deleteBaseObject(BaseObject* obj)
{
	BaseObjectDispatcher::iterator i;
	FOR_EACH(base_objects, i)
		if(*i == obj){
			killBaseObject(*i);
			base_objects.erase(i); 
			break;
			}
}

void BodyDispatcher::base_objects_quant()
{
	BaseObjectDispatcher::iterator i;
	FOR_EACH(base_objects, i){
		(*i) -> quant();
		if(!(*i) -> alive()){
			killBaseObject(*i);
			i = base_objects.erase(i); 
			--i;
			}
		}
}


//////////////////////////////////////////////////////////////////////////
//		Body List Utils
//////////////////////////////////////////////////////////////////////////
void BodyDispatcher::attachBody(Body* body) 
{ 
	total_bodies.push_back(body); 
	grid.insert(*body, body -> prev_bound = body -> getBound());
	body -> alive_ = 1; 
	body -> completed_ = 1; 
	body -> life_time_.start();
	body -> post_quant(); 
	if(body -> hidden())
		body -> unhide();
}

void BodyDispatcher::killBody(Body* body) 
{ 
	body -> hide(); 
	if(body -> alive())
		body -> kill(); 
	grid.erase(*body, body -> prev_bound);
}

void BodyDispatcher::deleteBody(Body* body) 
{
	killBody(body);
	TmpBodyList::iterator i;
	FOR_EACH(dynamic_bodies, i)
		if(*i == body){
			dynamic_bodies.erase(i); 
			break;
			}
	total_bodies.remove(body); 
}

inline void BodyDispatcher::moveBody(Body& b)
{
	iRectangle bound = b.getBound();
	grid.move(b, b.prev_bound, bound);
	b.prev_bound = bound;
}

Body* BodyDispatcher::searchBody(int ID)
{
	BodyList::iterator bi;
	FOR_EACH(total_bodies, bi)
		if((*bi) -> ID == ID)
			return *bi;
	return 0;
}

Body* BodyDispatcher::searchBody(const Vect3f& R, int body_type, const Body* ignored_body)
{
	float d, dmin = 100000;
	Body* nearest_body = 0;
	BodyList::iterator bi;
	FOR_EACH(total_bodies, bi){
		Body& b = **bi;
		if(b.type() == body_type && &b != ignored_body && b.visible() && (d = getDist(R, b.R()).norm()) < dmin){
			dmin = d;
			nearest_body = &b;
			}
		}
	return nearest_body;
}

Body* BodyDispatcher::searchBody(GetDistanceFunction metric, int body_type)
{
	float d, dmin = 100000;
	Body* nearest_body = 0;
	BodyList::iterator bi;
	FOR_EACH(total_bodies, bi){
		Body& b = **bi;
		if(b.type() == body_type && b.visible() && dmin > (d = metric(b))){
			dmin = d;
			nearest_body = *bi;
			}
		}
	return nearest_body;
}

Body* BodyDispatcher::active_body() 
{ 
	if(!active_body_)
		setActiveBody(total_bodies.front());
	return active_body_; 
}

Mechos* BodyDispatcher::active_mechos()
{
	Body* b = active_body();
	if(b && b -> type() != Body::MECHOS)
		b = searchBody(b -> R(), Body::MECHOS);
	return dynamic_cast<Mechos*>(b);
}

void BodyDispatcher::setActiveBody(Body* b)
{ 
	if(active_body_)
		active_body_ -> set_activity(0);
	active_body_ = b;  
	active_body_ -> set_activity(1);
}

void BodyDispatcher::setNextBody()
{
	active_body() -> set_activity(0);
	BodyList::iterator i;
	FOR_EACH(total_bodies, i)
		if(*i == active_body_)
			break;
	if(++i == total_bodies.end())
		i = total_bodies.begin();
	active_body_ = *i;
	active_body_ -> set_activity(1);
	switch(active_body() -> type()){
		case Body::METEORITE:
			setNextBody();
		}
	camera_dispatcher -> setObserver(active_body());
}

void BodyDispatcher::setPrevBody()
{
	active_body() -> set_activity(0);
	BodyList::iterator i;
	FOR_EACH(total_bodies, i)
		if(*i == active_body_)
			break;
	if(--i == total_bodies.end())
		--i;
	active_body_ = *i;
	active_body_ -> set_activity(1);
	switch(active_body() -> type()){
		case Body::METEORITE:
			setPrevBody();
		}
	camera_dispatcher -> setObserver(active_body());
}

//mchBossRacer* BodyDispatcher::getBoss()
//{
//	BodyList::iterator bi;
//	FOR_EACH(total_bodies, bi)
//		if((*bi) -> type() == Body::BOSS)
//			return dynamic_cast<mchBossRacer*>((*bi)());
//	return 0;
//}

void BodyDispatcher::clearOwner(int ID)
{
	FieldGrid::List::iterator vi;
	FOR_EACH(permanent_fields_grid, vi)
		if((*vi) -> owner() == ID)
			(*vi) -> reset_owner();

	BodyList::iterator bi;
	FOR_EACH(total_bodies, bi){
		OwnerProtection* p = dynamic_cast<OwnerProtection*>((*bi)());
		if(p && p -> owner() == ID)
			p -> reset_owner();
		}
}	

///////////////////////////////////////////////////////////////////
//	Force Fields
///////////////////////////////////////////////////////////////////
void BodyDispatcher::attachFieldSource(FieldSource* source)
{	
#ifndef _FINAL_VERSION_
	if(xreal_log)
		bout < "attachFieldSource: " <= source -> type() < "\n";
#endif
	permanent_fields_grid.add(source); 
} 
void BodyDispatcher::moveFieldSource(FieldSource* source, const Vect3f& r)
{	
#ifndef _FINAL_VERSION_
	if(xreal_log)
		bout < "moveFieldSource: " <= source -> type() < "\n";
#endif
	permanent_fields_grid.move(source, r); 
} 
void BodyDispatcher::changeFieldSource(FieldSource* source, float radius)
{	
#ifndef _FINAL_VERSION_
	if(xreal_log)
		bout < "changeFieldSource1: " <= source -> type() < "\n";
#endif
	permanent_fields_grid.change(source, radius); 
} 
void BodyDispatcher::changeFieldSource(FieldSource* source, const Vect3f& pos, float radius)
{	
#ifndef _FINAL_VERSION_
	if(xreal_log)
		bout < "changeFieldSource2: " <= source -> type() < "\n";
#endif
	permanent_fields_grid.change(source, pos, radius); 
} 
void BodyDispatcher::deleteFieldSource(FieldSource* source)
{	
#ifndef _FINAL_VERSION_
	if(xreal_log)
		bout < "deleteFieldSource: " <= source -> type() < "\n";
#endif
	permanent_fields_grid.remove(source); 
} 
void BodyDispatcher::fieldsAffection(Body& body)
{
	body.affecting_fields_ = 0;
	permanent_fields_grid.affect(body);
}


////////////////////////////////////////////////////////////////
//		Simulation
////////////////////////////////////////////////////////////////
void BodyDispatcher::evolve(float dt)
{
	start_autostop_timer(BD_evolve, STAT_BODYDISP);
#ifndef _FINAL_VERSION_
	msg_buf.init();
#endif
	contacts_vector_size = 0;

	start_timer(Test_single_collision, STAT_BODYDISP);
	BodyList::iterator tbi;
	FOR_EACH(total_bodies, tbi){
		Body& b = **tbi;
		b.in_contact = 0;
		b.body_colliding = 0;
		b.contacts.clear();
		b.contacts2.clear();
		}

	TmpBodyList::iterator bi;
	FOR_EACH(dynamic_bodies, bi)
		(*bi) -> test_collision(1);
	stop_timer(Test_single_collision, STAT_BODYDISP);
				
	start_timer(Test_double_collision, STAT_BODYDISP);
	contacting_bodies_list.clear();
	test_bodies_collisions();
	stop_timer(Test_double_collision, STAT_BODYDISP);


	start_timer(calc_force, STAT_BODYDISP);
	FOR_EACH(dynamic_bodies, bi){
		Body& b = **bi;
		b.set_default_forces();
		fieldsAffection(b);
		b.calc_forces_and_drags();
		}
	stop_timer(calc_force, STAT_BODYDISP);
	   
	start_timer(CollisionHandler, STAT_BODYDISP);
	if(UseCollisionHandler)
		CollisionHandler(contacts_vector, contacts_vector_size, 
			CollisionHandlerRelaxationType, CollisionHandlerRelaxationTime, CollisionHandlerTimeStep, 
			CollisionHandlerVelocityTolerance, CollisionHandlerPenetrationTolerance);
	stop_timer(CollisionHandler, STAT_BODYDISP);

	ContactingBodiesList::iterator cbi;
	FOR_EACH(contacting_bodies_list, cbi)
		cbi -> evolve(dt);

	FOR_EACH(dynamic_bodies, bi){
		if(!(*bi) -> in_contact){
			static ContactingBodies single_body;
			single_body.push_back(*bi);
			single_body.evolve(dt);
			single_body.clear();
			}
		moveBody(**bi);
		}
}

void BodyDispatcher::pre_quant()
{
	start_autostop_timer(BD_pre_quant, STAT_BODYDISPPRE);
	
	base_objects_quant();
	body_hash_table.check_to_clear();
	mechos_wheels_to_body_hash_table.check_to_clear();
	
	dynamic_bodies.clear();
	BodyList::iterator bi;
	FOR_EACH(total_bodies, bi){
		Body& b = **bi;
		start_timer(bodies_quant, STAT_BODYDISPPRE);
		b.quant();
		stop_timer(bodies_quant, STAT_BODYDISPPRE);
		if(b.alive()){
			if(b.is_dynamic())
				dynamic_bodies.push_back(&b);
			}
		else{
			start_timer(bodies_kill, STAT_BODYDISPPRE);
			killBody(*bi);
			bi = --total_bodies.erase(bi); 
			stop_timer(bodies_kill, STAT_BODYDISPPRE);
			}
		}

#ifndef _FINAL_VERSION_
	if(!active_body_){
		FOR_EACH(total_bodies, bi)
			if((*bi) -> type() == Body::MECHOS && (*bi) -> active())
				active_body_ = *bi;
		if(!active_body_)
			active_body_ = total_bodies.front();
		}

	if(xreal_log && bout.tell() > 50000){
		if(xreal_log == 1){
			if(!recorder_logging.GetFileName())
				recorder_logging.open("lst_", XS_OUT);
			recorder_logging.write((char*)bout, bout.tell());
			}
		else{
			static char buf[200000];
			int len = recorder_logging.read(buf, bout.tell());
			if(memcmp(buf, (char*)bout, len)){
				XStream f0("lst0", XS_OUT);
				XStream f1("lst1", XS_OUT);
				f0.write(buf, len);
				f1.write((char*)bout, len);
				append_message_to_file("Non-determinacy encountered", "diff");
				ErrH.Exit();
				}
			if(len < bout.tell()){ // switch to write mode
				xreal_log = 1;
				recorder_logging.close();
				recorder_logging.open("lst_", XS_OUT | XS_APPEND | XS_NOREPLACE);
				recorder_logging.write((char*)bout + len, bout.tell() - len);
				append_message_to_file("Switched from read to write mode", "diff");
				}
			}
		bout.init();
		}

		extern int time_to_restart;
		extern int repeate_times;
		if(time_to_restart && global_time() > time_to_restart){
			time_to_restart = 0x7fffffff;
			if(xreal_log == 1)
				append_message_to_file("Saving has been finished", "diff");
			else
				append_message_to_file("Verifying pass has been finished", "diff");
			if(repeate_times){
				char* c = "RESTART";
				while(*c)		
					KeyBuf -> put(*c++,KBD_CUR_KEY_PRESSED);
				repeate_times--;
				}
			else
				KeyBuf -> put(VK_ESCAPE,KBD_CUR_KEY_PRESSED);
			}
#endif		
}

void BodyDispatcher::evolve_quant()
{
	start_autostop_timer(BD_evolve_quant, STAT_BODYDISP);
	evolve(evolve_time_step);
	
	statistics_add(dynamic_bodies, STAT_NUMERIC, dynamic_bodies.size());
	statistics_add(total_bodies_size, STAT_NUMERIC, total_bodies.size());
	
	start_timer(non_dynamic_evolve, STAT_BODYDISP);
	BodyList::iterator bi;
	FOR_EACH(total_bodies, bi){
		Body& b = **bi;
		if(!b.is_dynamic()){
			b.R_prev = b.R();
			b.non_dynamic_evolve(evolve_time_step);
			b.post_quant();
			moveBody(b);
			}
		}
	stop_timer(non_dynamic_evolve, STAT_BODYDISP);
}

////////////////////////////////////////////////////////////////
//		Utils
////////////////////////////////////////////////////////////////

void BodyDispatcher::showBodies()
{
	if(show_map_borders){
		show_map_line(Vect3f(0,0,0), Vect3f(2048,0,0), XCOL(CYAN, show_map_borders));
		show_map_line(Vect3f(2048,0,0), Vect3f(2048,2048,0), XCOL(CYAN, show_map_borders));
		show_map_line(Vect3f(2048,2048,0), Vect3f(0,2048,0), XCOL(CYAN, show_map_borders));
		show_map_line(Vect3f(0,2048,0), Vect3f(0,0,0), XCOL(CYAN, show_map_borders));
		}
		
	if(show_body_active_only){
		active_body() -> show();
		return;
		}
	BodyList::iterator bi;
	FOR_EACH(total_bodies,bi)
		(*bi) -> show();

	BaseObjectDispatcher::iterator i;
	FOR_EACH(base_objects, i)
		(*i) -> show();
}

void BodyDispatcher::showObjects()
{
	BodyList::iterator bi;
	FOR_EACH(total_bodies, bi){
		Body& b = **bi;
		int color;
		switch(b.type()){
			case Body::PART:
				color = XCOL(RED);
				break;
			case Body::MECHOS:
				if(b.is_dynamic())
					color = XCOL(GREEN);
				else
					color = XCOL(YELLOW);
				break;
			default:
				color = XCOL(BLUE);
			}
		show_map_pixel(b.R(), color);
		}
}		

void BodyDispatcher::print_statistics(XBuffer& buf)
{
#ifndef _FINAL_VERSION_
	buf < "Global time: " <= global_time() < "\n";
	extern int time_to_restart;
	extern int repeate_times;
	if(xreal_log && time_to_restart)
		if(xreal_log == 1)
			buf < "Saving: " <= global_time()*100./time_to_restart < " %\n";
		else
			buf < "Verifying: " <= global_time()*100./time_to_restart < " %,  times left: " <= repeate_times < "\n";

	buf < "Total bodies: " <= total_bodies.size() < "\n";
	buf < "Contacting bodies: " <= contacting_bodies_list.size() < "\n";
	buf < "Permanent fields: " <= permanent_fields_grid.List::size() < "\n";
	buf < "Hash table size: " <= body_hash_table.size() < "\n";
	buf < "Hash table buckets: " <= body_hash_table.bucket_count() < "\n";
	buf < "Mehcos wheels ht: " <= mechos_wheels_to_body_hash_table.bucket_count() < "\n";
	buf < "Objects Grid size: " <= grid.size() < "\n";
	buf < "Dynamics bodies capacity: " <= dynamic_bodies.capacity() < "\n";
	//@caiiiycuk
	buf < "Polyhedron lib size: ???" <= /*polyhedron_lib -> size()*/0 < "\n";
	buf < "Bodies: " <= Body::instancies < "\n";
	buf < "Polyhedrons: " <= Polyhedron::instancies < "\n";
	typedef map<string, int> Map;
	Map map;
	BodyList::iterator bi;
	FOR_EACH(total_bodies, bi)
		map[typeid(**bi).name()]++;
	Map::iterator mi;
	int total = 0;
	buf < "Bodies: \n";
	FOR_EACH(map, mi){
		buf < mi -> first.c_str() < ": " <= mi -> second < "\n";
		total += mi -> second;
		}
	buf < "Body total: " <= total < "\n";
#endif
}

//void BodyDispatcher::save_log(XStream& f)
//{
//	XBuffer buf(200000);
//	BodyList::iterator bi;
//	FOR_EACH(total_bodies, bi)
//		(*bi) -> write_state(buf);
//	f.write
//}

