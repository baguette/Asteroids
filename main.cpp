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

#include <assert.h>

#define RECHARGE 7

/* entity ranges */
#define ship 0		/* just one ship */
#define laser 1		/* maximum of (ast - laser) lasers... 4 in this case */
#define ast 5		/* maximum of (end - ast) asteroids */
#define end 13		/* end of entities... one past the last entity */

/* shared state */
int window_width, window_height;
int btn[3] = {0};
int keys[3] = {0};
int mouse_x, mouse_y;
Vector world_mouse;
int xold, yold = 0;
int ttime = 0;
int spin = 1;
float color[] = {0.2f, 0.2f, 0.2f, 0.9f};
int recharge = 0;
bool gc = false, mouse_control = false;

/*
Entity *ship;
Entity *laser[4];
Entity *ast[12];
*/
// Entity *ufo;
Entity *entity[end];

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
	glRasterPos2f(x, y);
	while (*txt != '\0') {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *txt);
		txt++;
	}
}

void breakpoint() {}

void fire()
{
	int i, ok = 0;
	
	if (recharge <= 0 && !gc) {
		recharge = RECHARGE;
		for (i = laser; i < ast; i++) {
			if (entity[i]) {
				if (!entity[i]->isAlive()) {
					delete entity[i];
					entity[i] = NULL;
					ok = 1;
					break;
				}
			} else {
				ok = 1;
				break;
			}
		}
		if (ok) {
			entity[i] = new Laser(entity[ship]->s, entity[ship]->dir, entity[ship]->v);
		}
	} else
		recharge--;
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
	
	for (i = 0; i < end; i++) {
		if (entity[i] && !gc) {
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
	}

	glutSwapBuffers();
}

void cleanup()
{
	int i;
	
	for (i = 0; i < end; i++)
		if (entity[i])
			delete entity[i];
	
	exit(0);
}

void initEntities()
{
	int i;
	unsigned int t;
	
	entity[ship] = new Ship();
	for (i = ast; i < end; i++) {
		entity[i] = NULL;
	}
	for (i = laser; i < ast; i++) {
		entity[i] = NULL;
	}
	entity[ast] = new Asteroid();
	entity[ast+1] = new Asteroid();
	entity[ast+2] = new Asteroid();
}

void timer(int value)
{
	int i;
	/* garbage collect dead entities */
	gc = true;
	for (i = 0; i < end; i++) {
		if (entity[i]) {
			if (!entity[i]->isAlive()) {
				delete entity[i];
				entity[i] = NULL;
			}
		}
	}
	gc = false;
	for (i = 0; i < end; i++) {
		if (entity[i]) {
			entity[i]->update();
			entity[i]->behavior();
//			printf("Entity %d updated.\n", i);
		}
	}
	if (keys[0])
		entity[ship]->rotate.z += 8;
	if (keys[1])
		entity[ship]->rotate.z -= 8;
	if (keys[2])
		entity[ship]->a = 0.06;
	if (btn[0])
		fire();
	glutPostRedisplay();
	if (spin)
		glutTimerFunc(30, timer, 1);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'x':
		keys[2] = 1;
		break;
	case 'z':
		btn[0] = 1;
		break;
	case 'p':
		spin = spin ? 0 : 1;
		if (spin)
			glutTimerFunc(30, timer, 1);
		break;
	case 27: 
		cleanup();
	}
}

void unkeyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'x':
			keys[2] = 0;
			break;
		case 'z':
			btn[0] = 0;
			break;
	}
}

void special(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_LEFT:
			keys[0] = 1;
			break;
		case GLUT_KEY_RIGHT:
			keys[1] = 1;
			break;
		case GLUT_KEY_UP:
			keys[2] = 1;
			break;
	}
}

void unspecial(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_LEFT:
			keys[0] = 0;
			break;
		case GLUT_KEY_RIGHT:
			keys[1] = 0;
			break;
		case GLUT_KEY_UP:
			keys[2] = 0;
			break;
	}
}

void menu(int id)
{
	switch (id) {
	case 0:
		cleanup();
	case 1:
		mouse_control = mouse_control ? false : true;
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
		btn[b] = 1;
		xold = x;
		yold = y;
	} else {
		btn[b] = 0;
	}
	mouse_x = x;
	mouse_y = glutGet(GLUT_WINDOW_HEIGHT) - y;
}

void motion(int x, int y)
{
	if (mouse_control) {
		GLdouble model[16], proj[16];
		GLint view[4];
		Vector start(-1, 0, 0);
	
		/* unproject screen coordinates onto world coordinates */
		glGetDoublev(GL_MODELVIEW_MATRIX, model);
		glGetDoublev(GL_PROJECTION_MATRIX, proj);
		glGetIntegerv(GL_VIEWPORT, view);
		gluUnProject(
			x, glutGet(GLUT_WINDOW_HEIGHT) - y, 0,
			model, proj, view,
			&world_mouse.x, &world_mouse.y, &world_mouse.z
		);
	
		world_mouse.z = 0;
	
		/*
		 * calculate the angle between where the ship is currently
		 * pointing and the direction between the mouse and ship.
		 */
		entity[ship]->dir = (world_mouse - entity[ship]->s);
		entity[ship]->dir.normalize();
		entity[ship]->rotate.z = start.angleTo(entity[ship]->dir);
	
		xold = x;
		yold = y;
		glutPostRedisplay();
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 640);
	glutCreateWindow("Asteroids!");
	
	window_height = glutGet(GLUT_WINDOW_HEIGHT);
	window_width = glutGet(GLUT_WINDOW_WIDTH);
	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(unkeyboard);
	glutSpecialFunc(special);
	glutSpecialUpFunc(unspecial);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);
	glutMouseFunc(mouse);
	glutTimerFunc(30, timer, 1);
	
	glutCreateMenu(menu);
	glutAddMenuEntry("Pause", 2);
	glutAddMenuEntry("Toggle mouse control", 1);
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
	glPointSize(3.0);
	
	initLighting();
	initEntities();
	
	glutMainLoop();
}
