#include "StdAfx.h"
#include "TrackIterator.h"
#include "track.h"
#include "race.h"
#include "SimpleClip.h"

#undef assert
#define assert(x)

const int TrackIterator::track_radius = 80;

TrackIterator& TrackIterator::set(int x, int y)
{
	int size = 8;
	while((element = mch_trackMap -> GetTrackEl(x, y, size, size)) == 0)
		if((size <<= 1) > 1024)
			break;

	if(element)
		position.set(element -> Cx, element -> Cy, element -> Cz);
	else{
		assert(0);
		position.set(x, y, 0);
		}
	return *this;
}	

TrackIterator& TrackIterator::set(int check_point)
{
	mchCheckpoint* cp = mch_raceD -> getCP(check_point + 1); 
	if(cp){
		mchTrackBranch* br = cp -> getTrack();
		if(br){ 
			element = br -> pointLst -> first();
			position.set(element -> Cx, element -> Cy, element -> Cz);
			return *this;
			}
		}
	assert(0);
	position.set(0.f);
	return *this;
}

TrackIterator& TrackIterator::set(mchTrackElement* el)
{
	if(element = el)
		position.set(element -> Cx, element -> Cy, element -> Cz);
	else{
		assert(0);
		position.set(0.f);
		}
	return *this;
}

TrackIterator& TrackIterator::operator ++ ()
{
	if(!element)
		return *this;
	mchTrackElement* el = element -> next;
	if(!el){
		mchTrackBranch* br = element -> owner;
		mchCheckpoint* cp = br -> owner;
		if((cp = cp -> next) == 0)
			cp = mch_raceD -> cpLst -> first();
		assert(cp);
		br = cp -> getTrack();
		xassert("������ ����!!!" && br);
		el = br -> pointLst -> first();
		}
	element = el;
	if(element)
		position.set(element -> Cx, element -> Cy, element -> Cz);
	else
		assert(0);
	return *this;
}

TrackIterator& TrackIterator::operator -- ()
{
	if(!element)
		return *this;
	mchTrackElement* el = element -> prev;
	if(el == element -> owner -> pointLst -> last()){
		mchTrackBranch* br = element -> owner;
		mchCheckpoint* cp = br -> owner;
		cp = cp -> prev;
		assert(cp);
		br = cp -> getTrack();
		xassert("������ ����!!!" && br);
		el = br -> pointLst -> last();
		}
	element = el;
	if(element)
		position.set(element -> Cx, element -> Cy, element -> Cz);
	else
		assert(0);
	return *this;
}

TrackIterator& TrackIterator::operator += (int n)
{
	if(n > 0)
		while(n--)
			++*this;
	else
		if(n < 0)
			while(n++)
				--*this;
	return *this;
}

TrackIterator& TrackIterator::operator -= (int n)
{
	if(n > 0)
		while(n--)
			--*this;
	else
		if(n < 0)
			while(n++)
				++*this;
	return *this;
}

Vect3f TrackIterator::operator () (float tx, float ty) const
{
	TrackIterator p0 = *this;
	TrackIterator p1 = *this;
	--p0;
	++p1;
	Vect3f P0 = p0;
	Vect3f P1 = p1;
	Vect3f Y = P1 - P0;
	Vect3f X(-Y.y,Y.x, 0);
	X.Normalize(track_radius*tx);
	Y *= ty;
	return Vect3f(*this) + X + Y;
}