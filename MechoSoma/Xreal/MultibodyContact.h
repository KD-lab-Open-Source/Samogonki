#ifndef _MULTIBODY_CONTACT_
#define _MULTIBODY_CONTACT_

class ContactingBodies : public std::vector<class Body*> 
{
	TotalContactList total_contacts;

public:
	ContactingBodies();
	void clear();
	void evolve(float dt);
};

class ContactingBodiesList : public std::vector<ContactingBodies> 
{
	std::vector<ContactingBodies>::iterator search_group(Body* body);

public:
	ContactingBodiesList(){}
	void insert_pair(Body* body1, Body* body2);
};

#endif // _MULTIBODY_CONTACT_

