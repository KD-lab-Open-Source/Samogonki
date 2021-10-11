#include "SST_Reader.h"
#include "SimpleClip.h"
#pragma once

char* GetTargetName(char* name);

///////////////////////////////////////////////////////////////////////////////////////////
//		SimpleTrack
///////////////////////////////////////////////////////////////////////////////////////////
class SimpleTrack : public vector<Vect3f>
{
	iterator current;
public:
	SimpleTrack(char* sst_name)
	{
		typedef SST_Reader<UsualRecord> Reader;
		Reader reader(GetTargetName(sst_name));
		reserve(reader.size());
		Reader::iterator ti;
		FOR_EACH(reader, ti)
			push_back(Vect3f(ti -> x, ti -> y, tri_map -> H(ti -> x, ti -> y)));

		current = begin();
	}
	SimpleTrack& operator++ () { if(++current == end()) current = begin(); return *this; }
	SimpleTrack& operator-- () { if(--current == end()) current = end() - 1; return *this; }
	Vect3f operator () () const
	{
		if(empty())
			return Vect3f::ZERO;
		return *current;
	}
};
