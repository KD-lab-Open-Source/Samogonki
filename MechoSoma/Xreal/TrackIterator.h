#ifndef  __TRACK_ITERATOR__
#define  __TRACK_ITERATOR__

class Vect3f;

class TrackIterator
{
	class mchTrackElement* element;
	Vect3f position;

	static const int track_radius;

public:
	TrackIterator() : position((float)0) { element = 0; }
	TrackIterator& set(int x, int y);
	TrackIterator& set(int check_point);
	TrackIterator& set(mchTrackElement* el);
	TrackIterator& operator ++ ();
	TrackIterator& operator -- ();
	TrackIterator& operator += (int n);
	TrackIterator& operator -= (int n);
	int operator == (const TrackIterator& ti) const { return element == ti.element; }
	int operator != (const TrackIterator& ti) const { return !(*this == ti); }
	int x() const { return position.xi(); }
	int y() const { return position.yi(); }
	operator Vect3f () const { return position; }
	Vect3f operator () (float x, float y) const; // point in track-qudrangle: x - right, y - forward. [-1..1]
};

#endif //  __TRACK_ITERATOR__
