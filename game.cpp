#include "game.hpp"
#include "asteroid.hpp"
#include "alien.hpp"
#include "ship.hpp"
#include "laser.hpp"
#include "scene.hpp"
#include "explosion.hpp"
#include "title.hpp"
#include "gameover.hpp"

#include <stdlib.h> 	/* for NULL ... */
#include <string>
#include <sstream>

Game::Game(int n_ast, int l)
{
	int i;

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
	done = false;
	
	hud = Ship();
	hud.scale(0.22, 0.22, 0.22);
	hud.rotate(0, 0, -90);

	this->n_ast = n_ast;
	end = ast + n_ast * 5;		/* a large asteroid can break into 4 small, plus explosion */
	entity = new Entity *[end];
	
	level_msg << "Level " << n_ast-2;
	
	entity[ship] = new Ship();
	for (i = laser; i < ast; i++) {
		entity[i] = NULL;
	}
	for (i = ast; i < ast + n_ast; i++) {
		entity[i] = new Asteroid();
		entity[i]->scale = Vector(0.8, 0.8, 0.8);
	}
	for (i = ast + n_ast; i < end; i++) {
		entity[i] = NULL;
	}
	
	lives = l;
	
	active = true;

	glutAddMenuEntry("Toggle mouse control", 1);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

Game::Game(int n_ast)
{
//	Game::Game(n_ast, 4);
	int i;

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
	done = false;
	
	hud = Ship();
	hud.scale(0.22, 0.22, 0.22);
	hud.rotate(0, 0, -90);

	this->n_ast = n_ast;
	end = ast + n_ast * 5;		/* a large asteroid can break into 4 small, plus explosion */
	entity = new Entity *[end];
	
	level_msg << "Level " << n_ast-2;
	
	entity[ship] = new Ship();
	for (i = laser; i < ast; i++) {
		entity[i] = NULL;
	}
	for (i = ast; i < ast + n_ast; i++) {
		entity[i] = new Asteroid();
		entity[i]->scale = Vector(0.8, 0.8, 0.8);
	}
	for (i = ast + n_ast; i < end; i++) {
		entity[i] = NULL;
	}
	
	lives = 4;
	
	active = true;

	glutAddMenuEntry("Toggle mouse control", 1);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

Game::~Game()
{
	int i;
	
	for (i = 0; i < end; i++)
		if (entity[i])
			delete entity[i];
	delete [] entity;
	
	active = false;
}

Scene *Game::next_scene()
{
	if (done)
		return NULL;
	if (lives <= 0)
		return new Gameover();
	if (n_ast < 6)
		return new Game(n_ast + 1, lives);
	else
		return new Game(n_ast, lives);
}

void Game::fire()
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
			entity[i] = new Laser(entity[ship]->s, entity[ship]->dir, 0.19);
		}
	} else
		recharge--;
}

/* callback to display stuff */
void Game::display()
{
	int i;

	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glDisable(GL_LIGHTING);
	draw_string(-4.8, 4.6, level_msg.str().c_str());
	draw_string(3.2, 4.6, "Lives");
	glEnable(GL_LIGHTING);
	
	for (i = 1; i < lives; i++) {
		glPushMatrix();
		
		hud.s(4.8 - i*0.22, 4.65, 2);
		
		hud.transform();
		
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		hud.solid();
		
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 1.0);
		glScalef(1.01, 1.01, 1.01);
		hud.wire();
		glEnable(GL_LIGHTING);
		
		glPopMatrix();
	}

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

void Game::timer(int value)
{
	int i, j = 0;
	
	/* garbage collect dead entities */
	gc = true;
	for (i = 0; i < end; i++) {
		if (entity[i]) {
			if (!entity[i]->isAlive()) {
				delete entity[i];
				entity[i] = NULL;
				j++;
			}
		} else {
			j++;
		}
	}
	gc = false;
	if (!entity[ship]) {
		lives--;
		if (lives <= 0)
			active = false;
		else
			entity[ship] = new Ship();
	}
	if (j >= end-1)
		active = false;

	for (i = 0; i < end; i++) {
		if (entity[i]) {
			int k;
			entity[i]->update();
			entity[i]->behavior();
			for (k = i+1; k < end; k++) {
				if (entity[k] && entity[i]) {
					collision(i, k);
				}
			}
		}
	}
	
	if (entity[ship]) {
		if (keys[0])
			entity[ship]->rotate.z += 8;
		if (keys[1])
			entity[ship]->rotate.z -= 8;
		if (keys[2])
			entity[ship]->a = 0.06;
		if (btn[0])
			fire();
	}
	glutPostRedisplay();
}

void Game::keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'x':
		keys[2] = 1;
		break;
	case 'z':
		btn[0] = 1;
		break;
	case 27: 
		active = false;
		done = true;
	}
}

void Game::unkeyboard(unsigned char key, int x, int y)
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

void Game::special(int key, int x, int y)
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

void Game::unspecial(int key, int x, int y)
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

void Game::menu(int id)
{
	switch (id) {
	case 0:
		active = false;
		done = true;
	case 1:
		mouse_control = mouse_control ? false : true;
		break;
	}
}

void Game::mouse(int b, int state, int x, int y)
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

void Game::motion(int x, int y)
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

void Game::collision(int i, int j)
{	
	int k, n = 0;
	bool collide = false;
	Entity *a = entity[i], *b = entity[j];
/*	
	for (k = 0; k < a->n_edges + b->n_edges; k++) {
		Vector v = (k < a->n_edges) ? a->edge(k).flat().normal() : b->edge(k - a->n_edges).flat().normal();
		v(v.y*-1.0, v.x, 0);
		Range r = a->shadow(v);
		Range s = b->shadow(v);
		if (r.overlaps(s))
			n++;
	}
	collide = (n == a->n_edges + b->n_edges);
*/
	if (a->n_edges == -1 || b->n_edges == -1)	// don't collide with special effects
		collide = false;
	else if (a->n_edges > 0 && b->n_edges == 0) {
		for (k = 0; k < a->n_edges; k++) {
			Range r = a->edge(k);
			double d1 = (b->s - a->s).norm();
			double d2 = (b->s - a->s).norm();
			double r2 = (b->scale / 1.25).norm();	// average radius is more like 0.8 due to perturbations
			if (d1 < r2 || d2 < r2) {
				collide = true;
				break;
			}
		}
	} else if (a->n_edges == 0 && b->n_edges == 0) {
		Vector v = b->s - a->s;
		collide = v.norm() < b->scale.norm();
	}
	if (collide) {
		if (i >= laser && i < ast && j >= ast && j < end) {
			// we have a collision
			int k;
			Vector scale = b->scale / 1.6;
			Vector s = b->s;
			delete a;
			entity[i] = NULL;
			delete b;
			entity[j] = new Explosion();
			entity[j]->s = s;

			for (k = ast; k < end; k++)
				if (entity[k] == NULL)
					break;
			entity[k] = new Asteroid();
			entity[k]->scale = scale;
			entity[k]->s = s - Vector(scale.x, 0, 0);
			for (k = ast; k < end; k++)
				if (entity[k] == NULL)
					break;
			entity[k] = new Asteroid();
			entity[k]->scale = scale;
			entity[k]->s = s - Vector(scale.x, 0, 0);
		} else if (i == ship && j >= ast && j < end) {
			int k;
			Vector scale = b->scale / 1.6;
			Vector s = b->s;
			Vector r = a->s;
			delete a;
			entity[i] = new Explosion();
			entity[i]->s = r;
			delete b;
			entity[j] = new Explosion();
			entity[j]->s = s;

			for (k = ast; k < end; k++)
				if (entity[k] == NULL)
					break;
			entity[k] = new Asteroid();
			entity[k]->scale = scale;
			entity[k]->s = s - Vector(scale.x, 0, 0);
			for (k = ast; k < end; k++)
				if (entity[k] == NULL)
					break;
			entity[k] = new Asteroid();
			entity[k]->scale = scale;
			entity[k]->s = s - Vector(scale.x, 0, 0);
		}
	}
}
