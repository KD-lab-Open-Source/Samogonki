#ifndef __DEBRIS__
#define __DEBRIS__

class Debris : public Body {
	MeasurementTimer timer;
public:
	Debris(cMesh* mesh, const Body& parent);
	void quant(); 
	int is_colliding_with(const Body& body) { return type() != body.type(); }
	void calc_forces_and_drags(){}
};

#endif
