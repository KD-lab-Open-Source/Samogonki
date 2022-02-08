//////////////////////////////////////////////////////////////////
//	Точечные Источники
//////////////////////////////////////////////////////////////////
#ifndef __FORCE_FIELDS_H__
#define __FORCE_FIELDS_H__

#include "OwnerProtection.h"
#include "ArcaneStatistics.h"
#include "BaseObject.h"
#include "Grid2D.h"

//////////////////////////////////////////////
//	Базовый источник
class FieldSource : public OwnerProtection, virtual public BaseObject, public Grid2DElement
{
public:
	enum Type {
		General = 1,
		Attraction = 1 << 1,
		Magnetic = 1 << 2,
		BubbleField = 1 << 3,
		Vortex = 1 << 4,
		Stream = 1 << 5,
		Wind = 1 << 6,

		WILD_THICKET = 1 << 7,	// Дикая плесень
		RAGE_OF_SLIME = 1 << 8,	// Ярость слизи
		RED_TRACK = 1 << 9,		// Красные следы
		GREEN_SLIME = 1 << 10,	// Просто слизь, не окрашивает сущность
		YELLOW_SLIME = 1 << 11,	// Просто слизь, не окрашивает сущность
		BLUE_SLIME = 1 << 12,	// Просто слизь, не окрашивает сущность

		Sensor = 1 << 17	// Для отслеживания столкновений с местами
		};

	FieldSource() : OwnerProtection(0) {}
	FieldSource(const Vect3f& r_, float radius_, int owner_ID_ = 0) 
		: OwnerProtection(owner_ID_)
		{ position = r_; radius = radius_; type_ = General; }

	virtual ~FieldSource(){}
	
	Type type() const { return type_; }

	virtual void affect(Body& b) const  = 0;
	virtual void show() const;
	
	int is_affecting(Body& b) const { return b.ID != owner() && b.enabled_fields & type(); }
	void set_affection(Body& b) const { b.affecting_fields_ |= type(); }

	struct iRectangle getBound() const;

protected:
	Type type_;

	Vect3f position;
	float radius;

private:
	friend class BodyAffectOp;
	friend class FieldGrid;
	friend class FieldGridBase;
};

#endif // __FORCE_FIELDS_H__

	
