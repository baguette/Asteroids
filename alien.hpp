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
};

#endif
