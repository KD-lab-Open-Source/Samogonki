#ifndef _MULTIBODY_CONTACT_
#define _MULTIBODY_CONTACT_

class ContactingBodies : public vector<class Body*> 
{
	TotalContactList total_contacts;

public:
	ContactingBodies();
	void clear();
	void evolve(float dt);
};

class ContactingBodiesList : public vector<ContactingBodies> 
{
	ContactingBodies& search_group(Body* body);

public:
	ContactingBodiesList(){}
	void insert_pair(Body* body1, Body* body2);
};

#endif // _MULTIBODY_CONTACT_

