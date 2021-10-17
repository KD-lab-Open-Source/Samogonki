// Author: @caiiiycuk
#ifndef OWNER_PROTECTION
#define OWNER_PROTECTION

class OwnerProtection {
public:
    int ID;
	int time;
    OwnerProtection(int ID): ID(ID) {}
    OwnerProtection(): ID(0) {}
    int owner() const {
        return ID;
    }
    void reset_owner() {
        ID = 0;
    }
    void set_owner(int ID, int time) {
		this->ID = ID;
		this->time = time;
    }
	int duration() {
		return time;
	}
};

#endif