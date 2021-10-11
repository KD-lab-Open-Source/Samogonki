
#ifndef __STONE_H__
#define __STONE_H__


class Stone : public Body
{
	DelayTimer disable_collision_timer;
public:
	Stone(const Vect3f& R_, const Vect3f& finish_);
	void quant(); 
	void calc_forces_and_drags();
	void body_hit_reaction(const Body& body, const Contact& c);
	void ground_hit_reaction(const Contact& c);
	void damage(float decr,int damage_style = 0);
	void kill();
};


class StoneGun : public Vect3f, public BaseObject 
{
	Vect3f finish;
	DurationTimer fire_latency;
	ShareHandle<Body> stone;
public:
	StoneGun(const Vect3f& A, const Vect3f& B);
	void quant();
};

#endif // __STONE_H__
