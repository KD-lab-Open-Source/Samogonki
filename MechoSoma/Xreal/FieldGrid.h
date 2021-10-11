
#ifndef __FIELD_GRID__
#define __FIELD_GRID__

#include "Body.h"
#include "Grid2D.h"
#include "FieldSource.h"

class FieldGridBase : public Grid2D<FieldSource, 5> {};

class FieldGrid : public slist<ShareHandle<FieldSource> >, public FieldGridBase
{
public:
	typedef slist<ShareHandle<FieldSource> > List;
	typedef FieldGridBase Grid;

	void add(FieldSource* source);
	void move(FieldSource* source, const Vect3f& pos);
	void change(FieldSource* source, float rad);
	void change(FieldSource* source, const Vect3f& pos, float rad);
	void remove(FieldSource* source);
	void affect(Body& body) const;
};

#endif // __FIELD_GRID__
