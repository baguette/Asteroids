#include "common.h"

#include <math.h>

#include "entity.hpp"

Entity::Entity()
{
	s();
	rotate();
	scale(1, 1, 1);
	dir(-1, 0, 0);
	r();
	a = 0;
	v = 0;
	mass = 0;
}

Entity::~Entity() {}

void Entity::transform()
{
	glTranslatef(s.x, s.y, s.z);
	
	glRotatef(rotate.z, 0.0, 0.0, 1.0);
	glRotatef(rotate.x, 1.0, 0.0, 0.0);
	glRotatef(rotate.y, 0.0, 1.0, 0.0);
	
	
	glScalef(scale.x, scale.y, scale.z);
}

void Entity::update()
{
	rotate += r;
	
	s += dir * v;
	v += a;
	
	/* wrap around the edges of the screen ("toroid space") */
	if (s.x >= 5.0)
		s.x = -4.9;
	else if (s.x <= -5.0)
		s.x = 4.9;
	
	if (s.y >= 5.0)
		s.y = -4.9;
	else if (s.y <= -5.0)
		s.y = 4.9;
	
	/* TODO: compensate for friction */
	/* TODO: gravity */
}

void Entity::behavior() {}
