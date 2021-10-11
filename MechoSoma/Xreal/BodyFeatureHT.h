///////////////////////////////////////////
//	BodyFeatureHT for 
// 	   cash support
///////////////////////////////////////////
#ifndef __BodyFeatureHT_H__
#define __BodyFeatureHT_H__

struct BodyPair
{
	int first;
	int second;
	BodyPair(){}
	BodyPair(const Body& b1, const Body& b2) : first(b1.polyhedrons.ID()), second(b2.polyhedrons.ID()) {}
	int operator==(const BodyPair &p) const { return first == p.first && second == p.second; }
};

struct BodyPairHasher : unary_function<BodyPair, size_t> 
{
	size_t operator() (const BodyPair &p) const
	{
		return ((size_t) p.first) ^ (((unsigned int) p.second) << 16)
						^ (((unsigned int) p.second) >> 16);
	}
};

class BodyFeatureHT : public hash_map<BodyPair, FeaturesList , BodyPairHasher> 
{
public:
	BodyFeatureHT(int size) : hash_map<BodyPair, FeaturesList , BodyPairHasher>(size) {}
	FeaturesList& operator()(const Body& b1, const Body& b2);
	void check_to_clear();
};
		 
#endif // __BodyFeatureHT_H__
