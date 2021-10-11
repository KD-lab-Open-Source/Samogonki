#ifndef __FIXED_PATH_BODY__
#define __FIXED_PATH_BODY__

class FixedPathBody : public Body
{
	int soundID;
	int prev_move;

public: 
	FixedPathBody(cMesh* mesh, int damaging, BodyDispatcher* body_dispatcher);
	~FixedPathBody();

protected:
	void non_dynamic_evolve(float dt);
	void createBound();
	void quant();
	void post_quant(){}
	float hit_damage_ability(const class Body& b, float velocity) const;
};


class FixedPathRoot : public FixedPathBody
{
	float speed, phase;
	void non_dynamic_evolve(float dt);
public:
	FixedPathRoot(cMesh* mesh, int damaging, BodyDispatcher* body_dispatcher);
};


#endif  // __FIXED_PATH_BODY__
		