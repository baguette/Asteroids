#include "common.h"
#include "alien.hpp"

namespace {
	const int nverts = 5;
	const double xvertices[][3] = {
		{-1.04, 0.39, 0},
		{1.04, 0.39, 0},
		{1.04, -0.39, 0},
		{-1.04, -0.39, 0},
		{-1.04, 0.39, 0}
	};
	const int FIRETIME = 9;
}

std::vector<Vector> Alien::vertices()
{
	int i;
	std::vector<Vector> verts;

	for (i = 0; i < nverts; i++) {
		Vector u(xvertices[i]);
		u.componentwise_scale(scale);
		u += s;
		u.z = 0;
		verts.push_back(u);
	}

	return verts;
}

Alien::Alien(bool small)
{
	int side;
	double y;
	shape = Polygon;
	
	s(0, 0, 0);
	a = 0;
	r(0, 0, 8);
	rotate(90, 0, 0);
	
	if (!small) {
		points = 200;
		v = 0.1;
		scale = Vector(0.7, 0.7, 0.7);
		Pa = 0.3;
		Ps = 0.2;
		Pr = 0.5;
	} else {
		points = 1000;
		v = 0.17;
		scale = Vector(0.4, 0.4, 0.4);
		Pa = 0.4;
		Ps = 0.5;
		Pr = 0.1;
	}
	
	changetime = rando(6, 15);
	firetime = FIRETIME;
	
	side = rando(0, 2);
	y = rando(-5, 5);
	
	if (side == 0) {
		s.x = -5;
		deletion_position = 5;
		dir(1, 0, 0);
	} else {
		s.x = 5;
		deletion_position = -5;
		dir(-1, 0, 0);
	}
	
	s.y = y;
}

Alien::~Alien() {}

void Alien::transform()
{
	glTranslatef(s.x, s.y, s.z);
	
	glRotatef(rotate.x, 1.0, 0.0, 0.0);
	glRotatef(rotate.y, 0.0, 1.0, 0.0);
	glRotatef(rotate.z, 0.0, 0.0, 1.0);
	
	glScalef(scale.x, scale.y, scale.z);
}

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
//	return true;
	if (deletion_position > 0)
		return (s.x < deletion_position);
	else
		return (s.x > deletion_position);
}

void Alien::update()
{
	rotate += r;	
	s += dir * v;
	
	if (s.y >= 5.0)
		s.y = -4.9;
	else if (s.y <= -5.0)
		s.y = 4.9;
		
	if (s.x >= 5.5)
		dir.x = -dir.x;
	else if (s.x <= -5.5)
		dir.x = -dir.x;
}

int Alien::behavior()
{
	if (changetime <= 0) {
		dir.rotate(rando(-30, 30));
		changetime = rando(6, 15);
	}
	if (firetime <= 0) {
		double todo = rando(0, 1);
		firetime = FIRETIME;
		if (todo < Pa)
			return 2;
		if (todo < Pa+Ps)
			return 1;
		return 3;
	}
	changetime--;
	firetime--;
	return 0;
}
