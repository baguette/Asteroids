#ifndef _ASTEROID_HPP
#define _ASTEROID_HPP

#include "common.h"

#include <math.h>

#include "entity.hpp"

class Asteroid : public Entity {
private:
	static const int psubs = 7;		/* subdivisions per phi */
	static const int tsubs = 13;	/* subdivisions (phis) per theta */
	static const int nverts = psubs * tsubs;	/* number of vertices */
	/* increments of this value will be used when calculating vertices */
	static const float subs;	/* VS won't let me initiliaze this within the class definition... */

	static const double min_scale;

	float xvertices[nverts][3];
	/* The final asteroid has repeated vertices along shared edges */
	float asteroid[nverts*4][3];
	
	void init();
	void genericAsteroid(GLenum mode);
	
public:
	Asteroid();
	Asteroid(const Entity &old, bool second=false);
	~Asteroid();
	
	int behavior();
	void wire();
	void solid();
	bool isAlive();
	
	std::vector<Vector> vertices() {};
};

#endif
