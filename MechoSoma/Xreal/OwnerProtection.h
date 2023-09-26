//////////////////////////////////////////////////////////////////
// База для создания арканных объектов
//	- защита хозяина
//////////////////////////////////////////////////////////////////
#ifndef __OwnerProtection_H__
#define __OwnerProtection_H__

class OwnerProtection
{
	enum { default_duration = 2000 };
	int owner_ID;
	DurationTimer duration_;
public:
	OwnerProtection(int ID = 0) { owner_ID = ID; duration_.start(default_duration); }
	virtual void set_owner(int ID, int duration__ = default_duration) { owner_ID = ID; duration_.start(duration__); }
	void reset_owner() { owner_ID = 0; }
	int owner() const { return duration_() ? owner_ID : 0; }
	time_type duration(){ return duration_(); };
};

#endif //__OwnerProtection_H__
