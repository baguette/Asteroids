#ifndef _ALIEN_H
#define _ALIEN_H

#include "common.h"
#include "entity.hpp"

class Alien : public Entity {
public:
	Alien();
	~Alien();
	
	void wire();
	void solid();
	bool isAlive();
	
	Range edge(int n) { return Range(Vector(), Vector()); };
	Range shadow(Vector v) { return Range(Vector(), Vector()); };
};

#endif
