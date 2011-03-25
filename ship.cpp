#include "common.h"
#include "ship.hpp"

static float ship[][3] = {
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

static float square[][3] = {
	{0.5, -0.3, 0.3},
	{0.5, -0.3, -0.3},
	{0.5, 0.3, -0.3},
	{0.5, 0.3, 0.3},
	{0.5, -0.3, 0.3}
};

Ship::Ship() {}
Ship::~Ship() {}

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
