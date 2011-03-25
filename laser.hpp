#ifndef _LASER_HPP
#define _LASER_HPP

#include "common.h"
#include "entity.hpp"
#include "vector.hpp"

class Laser : public Entity {
public:
	int lifetime;
	Laser(Vector s, Vector d, double v);
	~Laser();
	
	bool isAlive();
 	int behavior();
	void wire();
	void solid();

	void update();
	
	virtual std::vector<Vector> vertices() {};
};

#endif
