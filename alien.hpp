#ifndef _ALIEN_H
#define _ALIEN_H

#include "common.h"
#include "entity.hpp"

class Alien : public Entity {
private:
	int changetime;
	int firetime;
	int deletion_position;
	double Pa, Ps, Pr;
public:
	Alien(bool small=false);
	~Alien();
	
	void wire();
	void solid();
	bool isAlive();
	
	std::vector<Vector> vertices();
	
	void transform();
	void update();
	int behavior();
};

#endif
