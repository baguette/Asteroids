#include "gameover.hpp"
#include "title.hpp"
#include <stdlib.h>

Gameover::Gameover()
{
	this->delay = 300;	// about 10 seconds
	this->done = false;
	this->active = true;
	
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

Scene *Gameover::next_scene()
{
	if (done)
		return NULL;
	return new Title();
}

void Gameover::display()
{
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* decorations */
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
	draw_string (-0.775, 1.0, " GAME OVER ");
	glEnable(GL_LIGHTING);
	glPopMatrix();

	glutSwapBuffers();
}

void Gameover::timer(int value)
{
	this->delay--;
	if (this->delay <= 0)
		active = false;
	glutPostRedisplay();
}

void Gameover::keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		done = true;
	default:
		active = false;
	}
}

void Gameover::unkeyboard(unsigned char key, int x, int y) {}

void Gameover::special(int key, int x, int y) {
	active = false;
}

void Gameover::unspecial(int key, int x, int y) {}

void Gameover::menu(int id)
{
	switch (id) {
	case 0:
		done = true;
		active = false;
	}
}

void Gameover::mouse(int b, int state, int x, int y) {}

void Gameover::motion(int x, int y) {}
