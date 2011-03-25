#ifndef _SHIP_HPP
#define _SHIP_HPP

#include "entity.hpp"

class Ship : public Entity {
public:
	Ship();
	~Ship();
	
	int behavior();
	void wire();
	void solid();
	bool isAlive();
	
	virtual std::vector<Vector> vertices();
};

#endif
