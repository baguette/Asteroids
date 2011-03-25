#include "common.h"

#include <stdio.h>
#include <stdlib.h>

#include "vector.hpp"
#include "bitmap.hpp"
#include "entity.hpp"
#include "ship.hpp"
#include "asteroid.hpp"
#include "alien.hpp"
#include "laser.hpp"
#include "game.hpp"
#include "scene.hpp"
#include "title.hpp"

#include <assert.h>

/* shared state */
int window_width, window_height;
Scene *scene;
int menu_id;

/* random number in the range [a, b) */
double rando(double a, double b)
{
	double r = fabs(b - a);
	double s = (a < b) ? a : b;
	return (((double)rand() / (double)RAND_MAX) * r + s);
}

/* draws a string originating at x,y */
void draw_string(float x, float y, const char *txt)
{
	glRasterPos3f(x, y, 2);
	while (*txt != '\0') {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *txt);
		txt++;
	}
}

void breakpoint() {}

void display()
{
	scene->display();
}

void keyboard(unsigned char key, int x, int y)
{
	scene->keyboard(key, x, y);
}

void unkeyboard(unsigned char key, int x, int y)
{
	scene->unkeyboard(key, x, y);
}

void special(int key, int x, int y)
{
	scene->special(key, x, y);
}

void unspecial(int key, int x, int y)
{
	scene->unspecial(key, x, y);
}

void menu(int id)
{
	scene->menu(id);
}

void mouse(int b, int state, int x, int y)
{
	scene->mouse(b, state, x, y);
}

void motion(int x, int y)
{
	scene->motion(x, y);
}

void timer(int value)
{
	if (scene->active)
		scene->timer(value);
	else {
		glutDestroyMenu(menu_id);
		menu_id = glutCreateMenu(menu);
		Scene *next = scene->next_scene();
		delete scene;
		scene = next;
		if (scene == NULL) {
			exit(0);
		}
	}
	glutTimerFunc(30, timer, 1);
	breakpoint();
}

inline void callbacks() {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(unkeyboard);
	glutSpecialFunc(special);
	glutSpecialUpFunc(unspecial);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);
	glutMouseFunc(mouse);
	glutTimerFunc(30, timer, 1);
	menu_id = glutCreateMenu(menu);
}

/* Initialize light source and lighting model. */
void initLighting()
{	
	float light_ambient[] = {0.0, 0.0, 0.0, 0.5},
		light_diffuse[] = {1.0, 1.0, 1.0, 1.0},
	/*and light_specular = (1.0, 1.0, 1.0, 1.0) */
		light_position[] = {-1.0, 1.0, 0.0, 0.0},
		global_ambient[] = {0.75, 0.75, 0.75, 1.0};

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
/*	glLight (GL_LIGHT 0) (Light.GL_SPECULAR light_specular); */
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	glFrontFace(GL_CW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glDepthFunc(GL_LESS);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 640);
	glutCreateWindow("Asteroids!");
	
	window_height = glutGet(GLUT_WINDOW_HEIGHT);
	window_width = glutGet(GLUT_WINDOW_WIDTH);
	
	callbacks();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
/*	gluPerspective(70.0, 1.0, 1.0, 40.0); */
	glOrtho(-5.0, 5.0, -5.0, 5.0, 1.0, 40.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
	
	glShadeModel(GL_SMOOTH);
	
	glClearDepth(1.0);
	glClearColor(0.1, 0.1, 0.1, 0.1);
	glEnable(GL_DEPTH_TEST);
	
	glLineWidth(1.3);
	
	initLighting();
	
	scene = new Title();

	glutMainLoop();
}
