///////////////////////////////////////////////////////////////////////////////
//	Contact 
///////////////////////////////////////////////////////////////////////////////
#ifndef __CONTACT_H__
#define __CONTACT_H__

#include "xmath.h"

struct Sphere;
class DynamicBody;
class Triangle;

struct ContactProperty 
{
	float restitution_plus_one;
	float dynamic_friction;
	float impact_friction;
	float static_friction;

	ContactProperty();
	void combination(const ContactProperty& prop);
};

struct Contact : ContactProperty
{
	enum State {
		General,
		BodyOfMechos
		} state;

	int ID;
	float dist;
	int normal_index; // Index in normal-force matrix [1..n]
	int tangent_index; // Index in tangent-force matrix [1..n]
	Vect3f cp1; // collision point of body1 in body1's local frame
	Vect3f cp2; // collision point of body2 in body2's local frame
	Vect3f cp1g; // collision point of body1 in global frame
	Vect3f cp2g; // collision point of body2 in global frame
	Vect3f normal; // normal points towards body1 expressed in global frame
	Vect3f tangent;
	float u_n, u_t; // u_n < 0 - penetration, u_n > 0 - disjoining for single and double contacts both
	float a_n, a_t; // acceleration
	float j_n, j_t; // contraint impulse calculated
	float f_n, f_t; // contraint force calculated
	unsigned is_frictional : 1;
	unsigned is_dynamic_frictional : 1;

	DynamicBody *body1, *body2;
	Sphere* sphere;
	const Triangle* triangle;

	Contact();
	void set(float dist_, const Vect3f& cp1_, const Vect3f& cp2_, const Vect3f& normal_, 
			Sphere* sphere_, const Triangle* triangle_,
			DynamicBody* body1_, DynamicBody* body2_);

	float calc_normal_velocity(); 
	float calc_normal_acceleration(); 

	void prepare_to_impulse_calculation();
	void prepare_to_force_calculation(int enable_dynamic_friction);

	int log_resolving(ostream& out);

	void show() const;
};

class ContactList : public std::vector<Contact> {};
class TotalContactList : public std::vector<Contact*> {};

// In order to avoid vector<> reallocation problem
class ContactRef
{
	ContactList* contacts;
	int index;
public:
	ContactRef(int index_, ContactList& contacts_) : index(index_), contacts(&contacts_) {}
	Contact* operator->() { return contacts -> data() + index; }
};

typedef std::vector<ContactRef> ContactList2;

#endif //__CONTACT_H__
