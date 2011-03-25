#ifndef _ASTEROID_H
#define _ASTEROID_H

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
	
	float vertices[nverts][3];
	/* The final asteroid has repeated vertices along shared edges */
	float asteroid[nverts*4][3];
	
	void genericAsteroid(GLenum mode);
	
public:
	Asteroid();
	~Asteroid();
	void behavior();
	void wire();
	void solid();
	bool isAlive();
};

#endif
