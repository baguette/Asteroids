#include "common.h"

#include <stdio.h>
#include <stdlib.h>

#include "bitmap.hpp"
#include "entity.hpp"
#include "ship.hpp"
#include "asteroid.hpp"
#include "alien.hpp"

/* shared state */
Bitmap *title;
int btn[3] = {0};
int mouse_x, mouse_y;
double world_mouse[3] = {0};
int xold, yold = 0;
int ttime = 0;
int spin = 1;
float color[] = {0.2f, 0.2f, 0.2f, 0.9f};

Entity *entity[3];

/* draws a string originating at x,y */
void draw_string(float x, float y, const char *txt)
{
	glRasterPos2f(x, y);
	while (*txt != '\0') {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *txt);
		txt++;
	}
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

/* callback to display stuff */
void display()
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
	draw_string (-0.775, -3.5, "INSERT COIN");
	glPolygonMode(GL_FRONT, GL_LINE);
	glRectf(-1.2, 1.2, 1.2, -1.2);
	glTranslatef(-3.0, 0.0, 0.0);
	glRectf(-1.2, 1.2, 1.2, -1.2);
	glTranslatef(6.0, 0.0, 0.0);
	glRectf(-1.2, 1.2, 1.2, -1.2);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	
	for (i = 0; i < sizeof(entity) / sizeof(entity[0]); i++) {
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

void cleanup()
{
	int i;
	for (i = 0; i < sizeof(entity) / sizeof(entity[0]); i++)
		delete entity[i];
	exit(0);
}

void initEntities()
{
	entity[0] = new Asteroid();
	entity[1] = new Ship();
	entity[2] = new Alien();
	
	entity[1]->x = -3;
	entity[2]->x = 3;
}

void resetAsteroid()
{
	delete entity[0];
	entity[0] = new Asteroid();
	glutPostRedisplay();
}

void timer(int value)
{
	int i;
	ttime = ttime + 5;
	for (i = 0; i < sizeof(entity) / sizeof(entity[0]); i++) {
		entity[i]->rotate.x = ttime;
	}
	glutPostRedisplay();
	if (spin)
		glutTimerFunc(30, timer, 1);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'p':
		spin = spin ? 0 : 1;
		if (spin)
			glutTimerFunc(30, timer, 1);
		break;
	case 'n':
		resetAsteroid();
		break;
	case 27: 
		cleanup();
	}
}

void menu(int id)
{
	switch (id) {
	case 0:
		cleanup();
	case 1:
		resetAsteroid();
		break;
	case 2:
		spin = spin ? 0 : 1;
		if (spin)
			glutTimerFunc(30, timer, 1);
		break;
	}
}

void mouse(int b, int state, int x, int y)
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
			&world_mouse[0], &world_mouse[1], &world_mouse[2]
		);
	} else {
		btn[b] = 0;
	}
	mouse_x = x;
	mouse_y = glutGet(GLUT_WINDOW_HEIGHT) - y;
}

void motion(int x, int y)
{
	if (btn[0]) {
		/* what did we click on? make sure we rotate the right thing */
		int i;
		for (i = 0; i < sizeof(entity) / sizeof(entity[0]); i++) {
			if (world_mouse[0] > entity[i]->x - entity[i]->scale.x
			&&  world_mouse[0] < entity[i]->x + entity[i]->scale.x
			&&  world_mouse[1] < entity[i]->y + entity[i]->scale.y
			&&  world_mouse[1] > entity[i]->y - entity[i]->scale.y) {
				entity[i]->rotate.y = entity[i]->rotate.y + xold - x;
				entity[i]->rotate.z = entity[i]->rotate.z + yold - y;
			}
		}
	}
	xold = x;
	yold = y;
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 640);
	glutCreateWindow("Asteroids!");
	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	glutTimerFunc(30, timer, 1);
	
	glutCreateMenu(menu);
	glutAddMenuEntry("Toggle spin", 2);
	glutAddMenuEntry("New asteroid", 1);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
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

	title = new Bitmap("asteroids.bmp");
	
	initLighting();
	initEntities();
	
	glutMainLoop();
}
