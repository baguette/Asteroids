#include "common.h"

#include <stdio.h>

#include "laser.hpp"
#include "vector.hpp"

Laser::Laser(Vector s, Vector dir, double v)
{
	this->s = s;
	this->dir = dir;
	this->v = v + 0.1;
	this->lifetime = 45;
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
	glBegin(GL_POINTS);
	glVertex3f(0, 0, 0);
	glEnd();
}

void Laser::solid()
{
}
