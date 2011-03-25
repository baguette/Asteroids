#include "common.h"

#include <stdio.h>

#include "laser.hpp"
#include "vector.hpp"

Laser::Laser(Vector s, Vector dir, double v)
{
	//n_edges = 1;
	n_edges = 0;
	this->s = s;
	this->dir = dir;
	this->v = v + 0.1;
	this->lifetime = 20;
	this->scale(1, 1, 1);
//	printf("I'm a new laser!\n");
}

Laser::~Laser() {}

bool Laser::isAlive()
{
	return (this->lifetime > 0);
}

void Laser::behavior()
{
	--lifetime;
}

void Laser::wire()
{
	glPointSize(3.0);
	glBegin(GL_POINTS);
	glVertex3f(0, 0, 0);
	glEnd();
	glPointSize(1.0);
}

void Laser::solid()
{
}

Range Laser::edge(int n)
{
	Vector u = dir*-1.0, w = dir;
	if (n == 2) { u = Vector(u.y*-1.0, u.x, 0); w = Vector(w.y*-1.0, w.x, 0); }
	u += s; w += s;
	return Range(u, w);
}

Range Laser::shadow(Vector v)
{
	Vector u = s;
	u.z = 0;
	return Range(u.projection(v), u.projection(v));
}

void Laser::update()
{
	rotate += r;
	
	s += dir * v;
	v += a;
	
	/* wrap around the edges of the screen ("toroid space") */
	if (s.x >= 5.0)
		dir.x = -dir.x;
	else if (s.x <= -5.0)
		dir.x = -dir.x;
	
	if (s.y >= 5.0)
		dir.y = -dir.y;
	else if (s.y <= -5.0)
		dir.y = -dir.y;
	
	/* TODO: compensate for friction */
	/* TODO: gravity */
}
