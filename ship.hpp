#ifndef _SHIP_HPP
#define _SHIP_HPP

#include "entity.hpp"

class Ship : public Entity {
public:
	Ship();
	~Ship();
	
	void behavior();
	void wire();
	void solid();
	bool isAlive();
	
	Range edge(int n);
	Range shadow(Vector v);
};

#endif
