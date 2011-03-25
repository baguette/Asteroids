#ifndef _SHIP_H
#define _SHIP_H

#include "entity.hpp"

class Ship : public Entity {
public:
	Ship();
	~Ship();
	void behavior();
	void wire();
	void solid();
	bool isAlive();
};

#endif
