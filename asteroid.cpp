#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "asteroid.hpp"

#define random_vertex (0.6 + ((float)rand() / (float)RAND_MAX) / 2.5)

#define TOP 0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3

const float Asteroid::subs = M_PI/(psubs-1);

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

Asteroid::Asteroid()
{
	int i = 0;
	float phi, theta;	/* azimuth, inclination */
	float radius = 1.0;

	/* use the same radius for all of the pole vertices */
	float zr = random_vertex;
	float nzr = random_vertex;
	
	/* use for placement of the asteroid */
	int x, y;
	int edge;

	/* build a set of all the vertices in the asteroid */
	for (theta = 0; theta < M_PI; theta += subs)
		for (phi = 0; phi < 2*M_PI;  phi += subs) {
			assert(i < nverts);
			/* the poles are special cases, since many polygons share
			 * a vertex there
			 */
			if (theta == 0) {
				vertices[i][0] = 0;
				vertices[i][1] = 0;
				vertices[i][2] = zr;
			} else if (theta > M_PI-subs) {
				vertices[i][0] = 0;
				vertices[i][1] = 0;
				vertices[i][2] = -nzr;
			} else {
				/* randomly perturb each vertex */
				radius = random_vertex;
				vertices[i][0] = radius * cos(phi) * sin(theta);
				vertices[i][1] = radius * sin(phi) * sin(theta);
				vertices[i][2] = radius * cos(theta);
			}
			/* The last phi values are special cases, too.
			 * Without this, the sphere is composed of a spiral
			 * rather than stacks.
			 * This code just sort of cleans up the values computed
			 * in the last conditional.
			 */
			if (phi > 2*M_PI - subs) {
				vertices[i][0] = vertices[i-tsubs+1][0];
				vertices[i][1] = vertices[i-tsubs+1][1];
				vertices[i][2] = vertices[i-tsubs+1][2];
			}
			i++;
		}

	/* build the actual model of the asteroid by linking all the vertices
	 * in counter-clockwise order; the first vertex of each group is also
	 * the normal of that face
	 */
	for (i = 0; i < nverts; i++) {
		asteroid[i*4][0] = vertices[i][0];
		asteroid[i*4][1] = vertices[i][1];
		asteroid[i*4][2] = vertices[i][2];
	
		asteroid[i*4+1][0] = vertices[i+tsubs][0];
		asteroid[i*4+1][1] = vertices[i+tsubs][1];
		asteroid[i*4+1][2] = vertices[i+tsubs][2];

		asteroid[i*4+2][0] = vertices[i+tsubs+1][0];
		asteroid[i*4+2][1] = vertices[i+tsubs+1][1];
		asteroid[i*4+2][2] = vertices[i+tsubs+1][2];
	
		asteroid[i*4+3][0] = vertices[i+1][0];
		asteroid[i*4+3][1] = vertices[i+1][1];
		asteroid[i*4+3][2] = vertices[i+1][2];
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
	this->dir(rando(-1, 1), rando(-1, 1), 0);
	this->dir.normalize();
}

Asteroid::~Asteroid() {}

void Asteroid::behavior()
{
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
	return true;
}
