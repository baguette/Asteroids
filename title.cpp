#include "title.hpp"
#include "asteroid.hpp"
#include "alien.hpp"
#include "ship.hpp"
#include "laser.hpp"
#include "scene.hpp"
#include "game.hpp"
#include "bitmap.hpp"

#include <stdlib.h> 	/* for NULL ... */

Title::Title()
{
	int i;

	done = false;
	
	for (i = 0; i < 4; i++) {
		keys[i] = 0;
		btn[i] = 0;
		color[i] = 0.2f;
	}
	color[i] = 0.9f;
	ttime = 0;
	recharge = 0;
	gc = false;
	mouse_control = false;

	title = new Bitmap("asteroids.bmp");
	entity = new Entity *[3];
	end = 3;
	entity[0] = new Asteroid();
	entity[1] = new Ship();
	entity[2] = new Alien();
	
	entity[0]->s(0, 0, 0);
	entity[1]->s(-3, 0, 0);
	entity[2]->s(3, 0, 0);

	active = true;

	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

Title::~Title()
{
	int i;
	for (i = 0; i < end; i++)
		delete entity[i];
	delete entity;
	delete title;
	active = false;
}

Scene *Title::next_scene()
{
	if (done)
		return NULL;
	return new Game(1, 4, 0);
}

void Title::display()
{
	int i;
	
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* decorations */
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
	if (title->isLoaded())
		title->draw(-3.0, 2.5);
	draw_string(-4.8, 4.6, "2010");
	draw_string (-0.775, -3.5, "PRESS ENTER");
	glPolygonMode(GL_FRONT, GL_LINE);
	glRectf(-1.2, 1.2, 1.2, -1.2);
	glTranslatef(-3.0, 0.0, 0.0);
	glRectf(-1.2, 1.2, 1.2, -1.2);
	glTranslatef(6.0, 0.0, 0.0);
	glRectf(-1.2, 1.2, 1.2, -1.2);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	
	for (i = 0; i < end; i++) {
		glPushMatrix();
		
		entity[i]->transform();
	
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		entity[i]->solid();
	
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 1.0);
		glScalef(1.01, 1.01, 1.01);
		entity[i]->wire();
		glEnable(GL_LIGHTING);
		
		glPopMatrix();
	}

	glutSwapBuffers();
}

void Title::timer(int value)
{
	int i;
	ttime = ttime + 5;
	for (i = 0; i < end; i++) {
		entity[i]->rotate.x = ttime;
	}
	glutPostRedisplay();
}

void Title::keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		done = true;
	case '\n':
	case '\r':
		active = false;
	}
}

void Title::unkeyboard(unsigned char key, int x, int y) {}

void Title::special(int key, int x, int y) {}

void Title::unspecial(int key, int x, int y) {}

void Title::menu(int id)
{
	switch (id) {
	case 0:
		active = false;
		done = true;
	}
}

void Title::mouse(int b, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		GLdouble model[16], proj[16];
		GLint view[4];
		
		btn[b] = 1;
		xold = x;
		yold = y;
		
		/* unproject screen coordinates onto world coordinates */
		glGetDoublev(GL_MODELVIEW_MATRIX, model);
		glGetDoublev(GL_PROJECTION_MATRIX, proj);
		glGetIntegerv(GL_VIEWPORT, view);
		gluUnProject(
			x, glutGet(GLUT_WINDOW_HEIGHT) - y, 0,
			model, proj, view,
			&world_mouse.x, &world_mouse.y, &world_mouse.z
		);
	} else {
		btn[b] = 0;
	}
	mouse_x = x;
	mouse_y = glutGet(GLUT_WINDOW_HEIGHT) - y;
}

void Title::motion(int x, int y)
{
	if (btn[0]) {
		/* what did we click on? make sure we rotate the right thing */
		int i;
		for (i = 0; i < end; i++) {
			if (world_mouse.x > entity[i]->s.x - entity[i]->scale.x
			&&  world_mouse.x < entity[i]->s.x + entity[i]->scale.x
			&&  world_mouse.y < entity[i]->s.y + entity[i]->scale.y
			&&  world_mouse.y > entity[i]->s.y - entity[i]->scale.y) {
				entity[i]->rotate.y = entity[i]->rotate.y + xold - x;
				entity[i]->rotate.z = entity[i]->rotate.z + yold - y;
			}
		}
	}
	xold = x;
	yold = y;
	glutPostRedisplay();
}

