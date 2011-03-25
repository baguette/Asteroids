#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include <typeinfo>

#include "entity.hpp"
#include "laser.hpp"
#include "asteroid.hpp"

/* max vertex is 1 */
#define random_vertex (0.6 + ((float)rand() / (float)RAND_MAX) / 2.5)

#define TOP 0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3

const float Asteroid::subs = M_PI/(psubs-1);
const double Asteroid::min_scale = 0.4;

void Asteroid::genericAsteroid(GLenum mode)
{
	int i;
	glBegin(mode);
	for (i = 0; i < sizeof(asteroid) / sizeof(asteroid[0]); i++) {
		/* every fourth vertex is also the normal of this face */
		if (i % 4 == 0)
			glNormal3fv(asteroid[i]);
		glVertex3fv(asteroid[i]);
	}
	glEnd();
}

void Asteroid::init()
{
	int i = 0;
	float phi, theta;	/* azimuth, inclination */
	float radius = 1.0;

	/* use the same radius for all of the pole xvertices */
	float zr = random_vertex;
	float nzr = random_vertex;
	
	/* use for placement of the asteroid */
	int x, y;
	int edge;
	
	shape = Circle;
	points = 20;

	/* build a set of all the xvertices in the asteroid */
	for (theta = 0; theta < M_PI; theta += subs)
		for (phi = 0; phi < 2*M_PI;  phi += subs) {
			assert(i < nverts);
			/* the poles are special cases, since many polygons share
			 * a vertex there
			 */
			if (theta == 0) {
				xvertices[i][0] = 0;
				xvertices[i][1] = 0;
				xvertices[i][2] = zr;
			} else if (theta > M_PI-subs) {
				xvertices[i][0] = 0;
				xvertices[i][1] = 0;
				xvertices[i][2] = -nzr;
			} else {
				/* randomly perturb each vertex */
				radius = random_vertex;
				xvertices[i][0] = radius * cos(phi) * sin(theta);
				xvertices[i][1] = radius * sin(phi) * sin(theta);
				xvertices[i][2] = radius * cos(theta);
			}
			/* The last phi values are special cases, too.
			 * Without this, the sphere is composed of a spiral
			 * rather than stacks.
			 * This code just sort of cleans up the values computed
			 * in the last conditional.
			 */
			if (phi > 2*M_PI - subs) {
				xvertices[i][0] = xvertices[i-tsubs+1][0];
				xvertices[i][1] = xvertices[i-tsubs+1][1];
				xvertices[i][2] = xvertices[i-tsubs+1][2];
			}
			i++;
		}

	/* build the actual model of the asteroid by linking all the xvertices
	 * in counter-clockwise order; the first vertex of each group is also
	 * the normal of that face
	 */
	for (i = 0; i < nverts; i++) {
		asteroid[i*4][0] = xvertices[i][0];
		asteroid[i*4][1] = xvertices[i][1];
		asteroid[i*4][2] = xvertices[i][2];
	
		asteroid[i*4+1][0] = xvertices[i+tsubs][0];
		asteroid[i*4+1][1] = xvertices[i+tsubs][1];
		asteroid[i*4+1][2] = xvertices[i+tsubs][2];

		asteroid[i*4+2][0] = xvertices[i+tsubs+1][0];
		asteroid[i*4+2][1] = xvertices[i+tsubs+1][1];
		asteroid[i*4+2][2] = xvertices[i+tsubs+1][2];
	
		asteroid[i*4+3][0] = xvertices[i+1][0];
		asteroid[i*4+3][1] = xvertices[i+1][1];
		asteroid[i*4+3][2] = xvertices[i+1][2];
	}
	
	/* randomly determine which edge of the screen to appear on */
	edge = rando(0, 4);		// should be an integer between 0 and 3
	switch (edge) {
		case TOP:
			y = 4.4;
			x = rando(0, 5);
			break;
		case BOTTOM:
			y = -4.4;
			x = rando(0, 5);
			break;
		case LEFT:
			x = -4.4;
			y = rando(0, 5);
			break;
		case RIGHT:
			x = 4.4;
			y = rando(0, 5);
			break;
		default: printf("invalid edge: %d\n", edge);
	}
	this->s.x = x;
	this->s.y = y;
	
	this->a = 0;
	this->v = rando(0.02, 0.06);
	this->r(rando(0, 5), rando(0, 5), rando(0, 5));
	this->scale(1, 1, 1);
	this->dir(rando(-1, 1), rando(-1, 1), 0);
	this->dir.normalize();
}

Asteroid::Asteroid()
{
	init();
}

Asteroid::Asteroid(const Entity &old, bool second)
{
	init();
	
	this->scale = old.scale / 1.6;
	this->s = second ?	// this is sort of hackish... make sure asteroids don't appear in the same place
		  old.s + Vector(old.scale.x, 0, 0)
		: old.s - Vector(old.scale.x, 0, 0);
	switch (old.points) {
		case 20:
			this->points = 50;
			break;
		case 50:
			this->points = 100;
			break;
		default:
			this->points = 20;
	}
}

Asteroid::~Asteroid() {}

int Asteroid::behavior()
{
	return 0;
}

void Asteroid::wire()
{
	genericAsteroid(GL_LINE_LOOP);
}

void Asteroid::solid()
{
	genericAsteroid(GL_QUADS);
}

bool Asteroid::isAlive()
{
	return scale.norm() >= min_scale;
}
