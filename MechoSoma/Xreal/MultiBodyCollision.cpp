#include "StdAfx.h"
#include "Body.h"
#include "BodyDispatcher.h"
#include "SimpleClip.h"
#include "MultibodyContact.h"
#include "BodyFeatureHT.h"
#include <algorithm>

/////////////////////////////////////////////////////////
//	ContactingBodies
/////////////////////////////////////////////////////////

std::vector<ContactingBodies>::iterator ContactingBodiesList::search_group(Body* body)
{
	iterator gi;
	FOR_EACH(*this, gi)
		if(find(gi -> begin(), gi -> end(), body) != gi -> end())
			return gi;
	assert(0);
	return end();
}

void ContactingBodiesList::insert_pair(Body* body1, Body* body2)
{
	switch(body1 -> in_contact + (body2 -> in_contact << 1)){
		case 0: // create new group
			push_back(ContactingBodies());
			back().push_back(body1);
			back().push_back(body2);
			break;
		case 1: // add body2 to body1's group
			search_group(body1)->push_back(body2);
			break;
		case 2: // add body1 to body2's group
			search_group(body2)->push_back(body1);
			break;
		case 3: { // join 2 groups
			auto group1 = search_group(body1);	
			auto group2 = search_group(body2);	
			if(group1 != group2){
				group1->insert(group1->end(), group2->begin(), group2->end());
				erase(group2);
				}
			break;
			}
		default:
			assert(0);
			return;
		}

	body1 -> in_contact = body2 -> in_contact = 1;
}


/////////////////////////////////////////////////////////
//	Body to Body Collision
/////////////////////////////////////////////////////////

int BodyDispatcher::test_body_to_body_collision(Body* body1, Body* body2, const MatXf& X12)
{
	int n = 0;
	if(body1 -> is_dynamic()){
		n += body1 -> test_body_to_body_collision(X12, body2, body_hash_table(*body1, *body2), 1);
		}

	if(body2 -> is_dynamic()){
		MatXf X21;
		X21.invert(X12);
		n += body2 -> test_body_to_body_collision(X21, body1, body_hash_table(*body2, *body1), 1);
		}
	if(n)
		body1 -> body_colliding = body2 -> body_colliding = 1;
	return n;
}

int BodyDispatcher::test_body_to_body_overlaping(Body* body1, Body* body2, const MatXf& X12)
{
	int n = 0;
	if(body1 -> is_dynamic()){
		n += body1 -> test_body_to_body_collision(X12, body2, body_hash_table(*body1, *body2), 0);
		}

	if(body2 -> is_dynamic()){
		MatXf X21;
		X21.invert(X12);
		n += body2 -> test_body_to_body_collision(X21, body1, body_hash_table(*body2, *body1), 0);
		}
//	if(n)
//		body1 -> body_colliding = body2 -> body_colliding = 1;
	return n;
}

	

/////////////////////////////////////////////////////////
//	Test bodies collisions
/////////////////////////////////////////////////////////

class BodyClipOp
{
	Body& b1;
	BodyDispatcher& body_dispatcher;
public:

	BodyClipOp(Body& body, BodyDispatcher& body_dispatcher_) :
		b1(body), body_dispatcher(body_dispatcher_) {}

	void operator()(const Body& body2, float dx, float dy)
	{
		Body& b2 = const_cast<Body&>(body2);
		
		if(body_dispatcher.involve_into_collision_test_global_counter != b2.involve_into_collision_test_counter)
			return;

		if(!b1.alive() || !b2.alive())
			return;

		if(b1.disable_collision_mask & b2.disable_collision_mask)
			return;
			
		if(!(b1.enable_collision_mask & b2.enable_collision_mask))
			return;

		MatXf X12 = b2.pose();
		X12.trans().x -= dx; X12.trans().y -= dy; // расцикливаем body2 по body1
		if(b1.R().distance(X12.trans()) < b1.radius() + b2.radius()){
			X12.invert();
			X12.postmult(b1.pose());  

			if(b1.is_colliding_with(b2) && b2.is_colliding_with(b1)){
				if(body_dispatcher.test_body_to_body_collision(&b1, &b2, X12)){
					b1.body_overlap_reaction(b2);
					b2.body_overlap_reaction(b1);
					if(b1.is_dynamic() && b2.is_dynamic())
						body_dispatcher.contacting_bodies_list.insert_pair(&b1, &b2);
					}
				}
			else
				if(body_dispatcher.test_body_to_body_overlaping(&b1, &b2, X12)){
					b1.body_overlap_reaction(b2);
					b2.body_overlap_reaction(b1);
					}
			}
	}
};

void BodyDispatcher::test_bodies_collisions()
{
	involve_into_collision_test_global_counter++;
	BodyList::iterator bi;
	FOR_EACH(total_bodies, bi){
		Body& b = **bi;
		BodyClipOp op(b, *this);
		grid.scan(b.getBound(), op);
		b.involve_into_collision_test_counter = involve_into_collision_test_global_counter;
		}
}

FeaturesList& BodyFeatureHT::operator()(const Body& b1, const Body& b2)
{
	BodyPair p(b1, b2);
	iterator i = find(p);
	if(i != end())
		return i -> second;

	FeaturesList& features = insert(std::pair<const BodyPair, FeaturesList> (p, FeaturesList()) ).first -> second;
	features.insert(features.end(), b1.spheres.size()*b2.polyhedrons.size(), 0);
	return features;
}

void BodyFeatureHT::check_to_clear()
{
	if(size()*2 > bucket_count())
		clear();
}


/////////////////////////////////////////////////////////////////
//	Mechos wheels to bodies collisions
////////////////////////////////////////////////////////////////

class MechosWheelsBodyClipOp
{
	Body& b1;
	BodyFeatureHT& hash_table;
public:

	MechosWheelsBodyClipOp(Body& body, BodyFeatureHT& hash_table_) :
		b1(body), hash_table(hash_table_) {}

	void operator()(const Body& body2, float dx, float dy)
	{
		Body& b2 = const_cast<Body&>(body2);

		if(&b1 == &b2)
			return;
		
		if(!(b1.alive() & b2.alive()))
			return;

		if(b1.disable_collision_mask & b2.disable_collision_mask)
			return;
			
		if(!(b1.enable_collision_mask & b2.enable_collision_mask))
			return;

		MatXf X12 = b2.pose();
		X12.trans().x -= dx; X12.trans().y -= dy; // расцикливаем body2 по body1
		if(b1.R().distance(X12.trans()) < b1.radius() + b2.radius()){
			X12.invert();
			X12.postmult(b1.pose());  

			if(b1.is_colliding_with(b2) && b2.is_colliding_with(b1) && b1.test_body_to_body_collision(X12, &b2, hash_table(b1, b2), 2)){
				b1.body_overlap_reaction(b2);
				b2.body_overlap_reaction(b1);
				}
			}
	}
};

void BodyDispatcher::test_mechos_wheels_to_bodies_collisions(Body& mechos)
{
	MechosWheelsBodyClipOp op(mechos, mechos_wheels_to_body_hash_table);
	grid.scan(mechos.getBound(), op);
}

