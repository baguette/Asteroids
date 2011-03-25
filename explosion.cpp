#include "common.h"
#include "explosion.hpp"

Explosion::Explosion()
{
	int i;
	delay = 30;
	n_edges = -1;
	n_points = 80;
	scale(0.1, 0.1, 0.1);
	s(0, 0, 0);
	points = new Vector [n_points];
	for (i = 0; i < n_points; i++)
//		points[i] = Vector(rando(0, 1), rando(0, 1), rando(0, 1));
		points[i] = Vector(rando(0, 1), 0, 0).rotation(rando(0, 360));
}

Explosion::~Explosion()
{
	delete [] points;
}

bool Explosion::isAlive()
{
	return (delay > 0);
}

void Explosion::behavior()
{
	if (scale.norm() >= 1.6) {
		scale();
		s = Vector(-20, -20, 0);
	}
	scale = scale * 1.6;
	delay--;
}

void Explosion::update() {}

void Explosion::solid() {}

void Explosion::wire() {
	int i;
	glBegin(GL_POINTS);
	for (i = 0; i < n_points; i++)
		glVertex3f(points[i].x, points[i].y, points[i].z);
	glEnd();
}
