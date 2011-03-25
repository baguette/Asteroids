#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "asteroid.hpp"

#define random_vertex (0.6 + ((float)rand() / (float)RAND_MAX) / 2.5)

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
}

Asteroid::~Asteroid()
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


/* relic code */
#if 0
void dumbAsteroid()
{
	int i = 0;
	float phi, theta;
	float radius = 1.0;
	float x[4], y[4], z[4];
	
	for (theta = 0; theta < M_PI - subs; theta += subs)
		for (phi = 0; phi < 2*M_PI - subs; phi += subs) {
			printf("Theta: %f, Phi: %f\n", theta, phi);
			/* generate the 4 vertices of this square of the sphere */
			// radius = 0.75 + ((float)rand() / (float)RAND_MAX) / 2.0;
			x[0] = radius * cos(phi) * sin(theta);
			y[0] = radius * sin(phi) * sin(theta);
			z[0] = radius * cos(theta);
			printf("X: %f  Y: %f  Z: %f\n", x[0], y[0], z[0]);
			
			x[1] = radius * cos(phi) * sin(theta + subs);
			y[1] = radius * sin(phi) * sin(theta + subs);
			z[1] = radius * cos(theta + subs);
			printf("X: %f  Y: %f  Z: %f\n", x[1], y[1], z[1]);
			
			x[2] = radius * cos(phi + subs) * sin(theta + subs);
			y[2] = radius * sin(phi + subs) * sin(theta + subs);
			z[2] = radius * cos(theta + subs);
			printf("X: %f  Y: %f  Z: %f\n", x[2], y[2], z[2]);

			x[3] = radius * cos(phi + subs) * sin(theta);
			y[3] = radius * sin(phi + subs) * sin(theta);
			z[3] = radius * cos(theta);
			printf("X: %f  Y: %f  Z: %f\n", x[3], y[3], z[3]);
			printf("\n");
			
			/* Put the vertices in the array in the correct order */
			#define a asteroid[i]
			a[0] = x[0]; a[1] = y[0]; a[2] = z[0]; i++;
			a[0] = x[1]; a[1] = y[1]; a[2] = z[1]; i++;
			a[0] = x[2]; a[1] = y[2]; a[2] = z[2]; i++;
			a[0] = x[3]; a[1] = y[3]; a[2] = z[3]; i++;
			#undef a
		}
}
#endif
