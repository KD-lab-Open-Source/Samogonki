
#ifndef __CRAWLING_OBJECT__
#define __CRAWLING_OBJECT__

class CrawlingObject : virtual public ControlledObject 
{
	const class Triangle* current_tri;
protected:
	int search_closest_triangle_counter;
	int search_closest_triangle_delay;
public:
	CrawlingObject();
	void non_dynamic_evolve(float dt);
	void show() const;
};


#endif  // __CRAWLING_OBJECT__
