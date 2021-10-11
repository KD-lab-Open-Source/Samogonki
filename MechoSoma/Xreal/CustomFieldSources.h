#ifndef __CUSTOM_FIELD_SOURCE_H__
#define __CUSTOM_FIELD_SOURCE_H__

#include "FieldSource.h"

/////////////////////////////////////////////////////////////////////
//	�������� ��� ������� �������
class CircleMapSource : public FieldSource, public virtual ArcaneStatisticsService 
{
	float z_up;

public:
	CircleMapSource(float x_, float y_, float r_, Type type__, int owner_ID = 0, float z_down_ = 0, float z_up_ = 800) 
		: FieldSource(Vect3f(x_, y_, z_down_), r_, owner_ID)
		{ type_ = type__; z_up = z_up_; }

	void affect(Body& b) const; // ������� + ground collision
};

/////////////////////////////////////////////////////////////////////
//	�������� ������� ������
#pragma warning( disable : 4250 )  
class RedTrackSource : public CircleMapSource, public BaseObjectTTL
{
public:
	RedTrackSource(const Vect3f& pose, float rad, int time_to_live, int owner_ID, const ArcaneStatisticsService& stat);
};
#pragma warning( default : 4250 )  

////////////////////////////////////////////////////////////////
//	����� �������� ��� �������
class GeneralFieldSource : public FieldSource, protected ArcaneStatisticsService
{
protected:	
	float attraction_force;
	float lift_force;
	Vect3f vortical_force; // normal*force_factor
	Vect3f torque; // normal*torque_factor

public:
	GeneralFieldSource(const Vect3f& r_, float radius_, float attraction_force_, float lift_force_, 
		const Vect3f& vortical_force_, const Vect3f& torque_, int owner_ID_ = 0);
};

// �����
class VortexFieldSource : public GeneralFieldSource 
{
public:
	VortexFieldSource(const Vect3f& r_, float radius_, int owner_ID_ = 0, float factor = 1, int clock_wise = 1);
	void affect(Body& b) const;
};

// ��������� ����
class MagneticFieldSource : public GeneralFieldSource 
{
public:
	MagneticFieldSource(const Vect3f& r_, float radius_, int owner_ID_ = 0);
	void affect(Body& b) const;
};

// ����������
class AttractionFieldSource : public GeneralFieldSource 
{
public:
	AttractionFieldSource(const Vect3f& r_, float radius_, int owner_ID_ = 0);
	void affect(Body& b) const;
};

// ����������, �����
class BubbleFieldSource : public GeneralFieldSource 
{
	mutable ShareHandle<Body> victim;
public:
	BubbleFieldSource(const Vect3f& r_, float radius_, int owner_ID_ = 0);
	void affect(Body& b) const;
	int catch_victim() const { return victim ? 1 : 0; }
};

// ���������� ����
class UniformFieldSource : public FieldSource 
{
	Vect3f force;
public:
	UniformFieldSource(const Vect3f& r_, float radius_, const Vect3f& force_) : FieldSource (r_, radius_) { force = force_; }
	void affect(Body& b) const;
	void setForce(const Vect3f& force_) { force = force_; }
};

// ������� (������� �� ���������� � ����)
class StreamFieldSource : public UniformFieldSource 
{
public:
	StreamFieldSource(const Vect3f& r_, float radius_, const Vect3f& force_) : UniformFieldSource (r_, radius_, force_) { type_ = Stream; }
	void affect(Body& b) const;
};

//  ����� 
class WindFieldSource : public UniformFieldSource
{
public:
	WindFieldSource(const Vect3f& r_, float radius_, const Vect3f& force_)
		: UniformFieldSource(r_, radius_, force_) { type_ = Wind; }
};


// ��� ������������ ������������ � �������
class SensorFieldSource : public FieldSource 
{
	int ID_;
public:
	int ID(void) const { return ID_; }

	SensorFieldSource(const Vect3f& r_, float radius_,int id = 0) : FieldSource (r_, radius_) { type_ = Sensor; ID_ = id; }
	void affect(Body& b) const;
};

#endif // __CUSTOM_FIELD_SOURCE_H__
