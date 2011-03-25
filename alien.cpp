#include "common.h"
#include "alien.hpp"

Alien::Alien() {}

Alien::~Alien() {}

void Alien::wire()
{
	glutWireTorus(0.4, 0.66, 6, 6);
	glutWireSphere(0.6, 8, 8);
}

void Alien::solid()
{
	glutSolidTorus(0.39, 0.65, 6, 6);
	glutSolidSphere(0.6, 8, 8);
}

bool Alien::isAlive()
{
	return true;
}
