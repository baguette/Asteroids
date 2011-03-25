#include "common.h"
#include "ship.hpp"
#include <stdio.h>
#include <math.h>

namespace {
	const int nverts = 5;
	const int nfaces = 4;
	
	const double vertices[][3] = {
		{-0.5, 0.0, 0.0},
		{0.5, -0.3, 0.3},
		{0.5, 0.3, 0.3},
		{0.5, 0.3, -0.3},
		{0.5, -0.3, -0.3}
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

Range Ship::edge(int n)
{
	int i;
	Vector verts[nverts];
	Vector max1, max2;
	
	for (i = 0; i < nverts; i++) {
		verts[i] = Vector(vertices[i]);
		verts[i].componentwise_scale(scale);
//		verts[i] = Vector(verts[i].z, verts[i].x, verts[i].y).rotation(rotate.x);
//		verts[i] = Vector(verts[i].y, verts[i].z, verts[i].x);	// blah
		verts[i].rotate(rotate.z);
		verts[i] = verts[i] + s;
		verts[i].z = 0;
		if (verts[i].norm() > max1.norm()) { max2 = max1; max1 = verts[i]; }
	}
	
	return (n == 0) ? Range(verts[0], max1) :
		   (n == 1) ? Range(max1, max2) :
					  Range(max2, verts[0]);
}

Range Ship::shadow(Vector v)
{
	int i;
	Vector verts[nverts];
	Vector edges[3];
	Range r;
	Vector max1, max2;
	
	for (i = 0; i < nverts; i++) {
		verts[i] = Vector(vertices[i]);
		verts[i].componentwise_scale(scale);
		verts[i] = Vector(verts[i].z, verts[i].x, verts[i].y).rotation(rotate.x);
		verts[i] = Vector(verts[i].y, verts[i].z, verts[i].x);	// blah
		verts[i].rotate(-rotate.z);
		verts[i] = verts[i] + s;
		verts[i].z = 0;
		if (verts[i].norm() > max1.norm()) { max2 = max1; max1 = verts[i]; }
	}

	r = Range(verts[0].projection(v), max1.projection(v)) + r;
	r = Range(verts[0].projection(v), max2.projection(v)) + r;
	r = Range(max2.projection(v), max1.projection(v)) + r;
	
	return r;
}

Ship::Ship() {
	n_edges = 3;
	s(0, 0, 0);
	r(8, 0, 0);
	scale(0.5, 0.5, 0.5);
}
Ship::~Ship() {}

void Ship::behavior()
{
	dir(-1, 0, 0);
	dir.rotate(rotate.z);
	
	a = 0;
	v /= 1.3;
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
