/////////////////////////////////////////////////////////////////////////////////////////////////
//	Поддержка сетки полей
/////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "FieldGrid.h"

iRectangle FieldSource::getBound() const
{ 
	return iRectangle(round(position.x - radius), round(position.y - radius), 
			round(position.x + radius), round(position.y + radius)); 
}

void FieldGrid::add(FieldSource* source)
{
	push_front(source); 
	Grid::insert(*source, source -> getBound()); 
}

void FieldGrid::remove(FieldSource* source)
{
	Grid::erase(*source, source -> getBound());
	List::remove(source);
}

void FieldGrid::move(FieldSource* source, const Vect3f& pos)
{
	Grid::erase(*source, source -> getBound());
	source -> position = pos; 
	Grid::insert(*source, source -> getBound()); 
}	

void FieldGrid::change(FieldSource* source, float rad)
{
	Grid::erase(*source, source -> getBound());
	source -> radius = rad; 
	Grid::insert(*source, source -> getBound()); 
}	

void FieldGrid::change(FieldSource* source, const Vect3f& pos, float rad)
{
	Grid::erase(*source, source -> getBound());
	source -> position = pos; 
	source -> radius = rad; 
	Grid::insert(*source, source -> getBound()); 
}	

	
class BodyAffectOp
{
	Body& body;
public:

	BodyAffectOp(Body& body_) :
		body(body_) {}

	void operator()(const FieldSource& source, float dx, float dy)
	{
		body.Xlg.trans().x += dx; body.Xlg.trans().y += dy;
		source.affect(body);
		body.Xlg.trans().x -= dx; body.Xlg.trans().y -= dy;
	}
};

void FieldGrid::affect(Body& body) const
{
	BodyAffectOp op(body);
	scan(body.getBound(), op);
}
	

