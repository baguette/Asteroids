#ifndef _EXPLOSION_HPP
#define _EXPLOSION_HPP

#include "common.h"
#include "entity.hpp"

class Explosion : public Entity {
private:
	Vector *points;
	int n_points;
	int delay;
public:
	Explosion();
	~Explosion();
	
	bool isAlive();
	void behavior();
	void wire();
	void solid();
	void update();

	Range edge(int n) { return Range(); }
	Range shadow(Vector v) { return Range(); }
};

#endif
