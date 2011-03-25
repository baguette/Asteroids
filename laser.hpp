#ifndef _LASER_HPP
#define _LASER_HPP

#include "common.h"
#include "entity.hpp"
#include "vector.hpp"

class Laser : public Entity {
private:
	int lifetime;
public:
	Laser(Vector s, Vector d, double v);
	~Laser();
	bool isAlive();
	void behavior();
	void wire();
	void solid();
};

#endif
