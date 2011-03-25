#include "common.h"
#include "entity.hpp"

Entity::Entity()
{
	x = 0; y = 0; z = 0;
	rotate.x = 0; rotate.y = 0; rotate.z = 0;
	scale.x = 1; scale.y = 1; scale.z = 1;
}

Entity::~Entity() {}

void Entity::transform()
{
	glTranslatef(x, y, z);
	
	glRotatef(rotate.x, 1.0, 0.0, 0.0);
	glRotatef(rotate.y, 0.0, 1.0, 0.0);
	glRotatef(rotate.z, 0.0, 0.0, 1.0);
	
	glScalef(scale.x, scale.y, scale.z);
}
