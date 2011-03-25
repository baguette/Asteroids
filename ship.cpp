#include "common.h"
#include "ship.hpp"
#include <stdio.h>
#include <math.h>
#include <vector>

namespace {
	const int nverts = 6;
	const int nfaces = 4;
	
	const double xvertices[][3] = {
		{-0.5, 0.0, 0.0},
		{0.5, -0.3, 0.3},
		{0.5, 0.3, 0.3},
		{0.5, 0.3, -0.3},
		{0.5, -0.3, -0.3},
		{-0.5, 0.0, 0.0}
	};
	
	const float ship[][3] = {
		{0, 0, 1},
		{-0.5, 0.0, 0.0},
		{0.5, -0.3, 0.3},
		{0.5, 0.3, 0.3},
	
		{0, 1, 0},
		{-0.5, 0.0, 0.0},
		{0.5, 0.3, 0.3},
		{0.5, 0.3, -0.3},
	
		{0, 0, -1},
		{-0.5, 0.0, 0.0},
		{0.5, 0.3, -0.3},
		{0.5, -0.3, -0.3},
	
		{0, -1, 0},
		{-0.5, 0.0, 0.0},
		{0.5, -0.3, -0.3},
		{0.5, -0.3, 0.3}
	};

	const float square[][3] = {
		{0.5, -0.3, 0.3},
		{0.5, -0.3, -0.3},
		{0.5, 0.3, -0.3},
		{0.5, 0.3, 0.3},
		{0.5, -0.3, 0.3}
	};
}

Ship::Ship() {
	shape = Polygon;
	s(0, 0, 0);
	r(8, 0, 0);
	scale(0.5, 0.5, 0.5);
}
Ship::~Ship() {}

int Ship::behavior()
{
	dir(-1, 0, 0);
	dir.rotate(rotate.z);
	
	a = 0;
	v /= 1.3;
	
	return 0;
}

std::vector<Vector> Ship::vertices()
{
	int i;
	std::vector<Vector> verts;
	Vector max1, max2;
	
	for (i = 0; i < nverts; i++) {
		Vector u = Vector(xvertices[i]);
		u.componentwise_scale(scale);
		u.rotate(rotate.z);
		u += s;
		u.z = 0;
		verts.push_back(u);
	}
	
	return verts;
}

void Ship::wire()
{
	int i;
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < sizeof(ship) / sizeof(ship[0]); i++)
		if (i % 4 == 0)
			glNormal3fv(ship[i]);
		else
			glVertex3fv(ship[i]);
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < sizeof(square) / sizeof(square[0]); i++)
		glVertex3fv(square[i]);
	glEnd();
}

void Ship::solid()
{
	int i;
	glBegin(GL_TRIANGLES);
	for (i = 0; i < sizeof(ship) / sizeof(ship[0]); i++)
		if (i % 4 == 0)
			glNormal3fv(ship[i]);
		else
			glVertex3fv(ship[i]);
	glEnd();
	glBegin(GL_QUADS);
	for (i = 0; i < sizeof(square) / sizeof(square[0]) - 1; i++)
		glVertex3fv(square[i]);
	glEnd();
}

bool Ship::isAlive()
{
	return true;
}
