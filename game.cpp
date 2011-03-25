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

namespace {
	bool new_game = false;
}

Game::Game(int level, int lives, long score)
{
	int i;

	new_game = false;

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
	small_alien_is_fair_game = false;
	countdown = 600;
	
	hud = Ship();
	hud.scale(0.22, 0.22, 0.22);
	hud.rotate(0, 0, -90);

	n_ast = (level <= 4) ? level + 2 : 6;	/* 6 maximum asteroids */
	end = ast + n_ast * 5;		/* a large asteroid can break into 4 small, plus explosion */
	entity = new Entity *[end];
	
	this->level = level;
	level_msg << "Level " << level;
	this->lives = lives;
	this->score = score;
	
	entity[ship] = new Ship();
	entity[alien] = NULL;
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
	
	active = true;
	
	glutAddMenuEntry("Toggle mouse control", 1);
	glutAddMenuEntry("New Game", 2);
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
	if (new_game)
		return new Game(1, 4, 0);
	return new Game(level+1, lives, score);
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

void Game::alienFire(int at)
{
	Vector target;
	int i = aLaser, j, ok = 0;
	
	if (entity[alien] == NULL)
		return;

	if (entity[aLaser] != NULL && entity[aLaser]->isAlive() == false) {
		delete entity[aLaser];
		entity[aLaser] = NULL;
	}
	if (entity[aLaser] == NULL)
		ok = 1;
	if (ok) {
		switch (at) {
			case 1:
				target = entity[ship]->s - entity[alien]->s;
				break;
			case 2:
				for (j = ast; j < end; j++)
					if (entity[j] && entity[j]->isAlive()) {
						target = entity[j]->s - entity[alien]->s;
						break;
					} else
						target = Vector(1, 0, 0).rotation(rando(0,360));
			break;
			case 3:
				target = Vector(1, 0, 0).rotation(rando(0,360));
		}
		entity[i] = new Laser(entity[alien]->s, target.normal(), 0.19);
	}
}

/* callback to display stuff */
void Game::display()
{
	int i; int lives = (this->lives > 4) ? 2 : this->lives;

	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glDisable(GL_LIGHTING);
	draw_string(-4.8, 4.6, level_msg.str().c_str());
	draw_string(3.2, 4.6, "Lives");
	draw_string(-4.8, -4.75, score_msg.str().c_str());
	if (this->lives > 4)
		draw_string(4.1, 4.6, lives_msg.str().c_str());		// FIX THIS
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
	if (countdown-- <= 0) {
		if (entity[alien] == NULL) {
			entity[alien] = new Alien(small_alien_is_fair_game);
			small_alien_is_fair_game = true;
			countdown = 400;		// more frequent after the first time
		}
	}

	for (i = 0; i < end; i++) {
		if (entity[i]) {
			int k; int msg;
			entity[i]->update();
			msg = entity[i]->behavior();
			if (msg)
				alienFire(msg);		// this is the only message right now
			for (k = i+1; k < end; k++) {
				if (entity[k] && entity[i]) {
					collision(i, k);
				}
			}
		}
	}
	
	score_msg.seekp(0);
	score_msg << score;
	
	lives_msg.seekp(0);
	lives_msg << "                       ";
	lives_msg.seekp(0);
	lives_msg << (this->lives - 1) << "x";
	
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
/*
		case 'a':
			if (entity[alien] == NULL)
				entity[alien] = new Alien();
			break;
		case 'q':
			if (entity[alien] == NULL)
				entity[alien] = new Alien(true);
			break;
*/
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
		break;
	case 1:
		mouse_control = mouse_control ? false : true;
		break;
	case 2:
		active = false;
		new_game = true;
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

/*
 * This is super-awful. I came up with a much better way to do this
 * half-way through, but I don't have time to refactor :(
 */
void Game::collision(int i, int j)
{	
	int n = 0;
	bool collide = false;
	Entity *a = entity[i], *b = entity[j];
	std::vector<Vector>::iterator it, ir;
	std::vector<Vector> av, bv;
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
	if (a->shape == Entity::None || b->shape == Entity::None)	// don't collide with special effects
		collide = false;
	else if (a->shape == Entity::Polygon && b->shape == Entity::Circle) {	// ship and asteroid
		av = a->vertices();
		for (it = av.begin(); it < av.end(); it++) {
		//	double d1 = (b->s - *it).norm();		// this is technically correct but frustrating in gameplay
			double d1 = (b->s - a->s).norm();		// this is technically incorrect but much less frustrating
			double r2 = (b->scale / 1.25).norm();	// average radius is more like 0.8 due to perturbations
			if (d1 < r2) {
				collide = true;
				break;
			}
		}
	} else if (a->shape == Entity::Point && b->shape == Entity::Circle) {	// laser and asteroid
		Vector v = b->s - a->s;
		collide = v.norm() < b->scale.norm();
	} else if (a->shape == Entity::Polygon && b->shape == Entity::Point) {
		av = a->vertices();
		int c = 0;
		Range r2(b->s, Vector(10, 0, 0));
		for (it = av.begin(); it < av.end()-1; it++) {
			Range r1(*it, *(it+1));
			double den = Range::den(r2, r1);
			double t1 = Range::t1(r2, r1);
			double t2 = Range::t2(r2, r1);
	
			if (den != 0.0) {
				t1 /= den; t2 /= den;
				if (t1 > 0.0 && t1 < 1.0 && t2 > 0.0 && t2 < 1.0)
					c++;
			}
		}
		if ((c % 2) != 0) {
			collide = true;
		}
	} else if (a->shape == Entity::Polygon && b->shape == Entity::Polygon) {
		av = a->vertices();
		bv = b->vertices();
		int c = 0;
		for (ir = bv.begin(); ir < bv.end()-1; ir++) {
			Range r2(*ir, *(ir+1));
			for (it = av.begin(); it < av.end()-1; it++) {
				Range r1(*it, *(it+1));
				double den = Range::den(r2, r1);
				double t1 = Range::t1(r2, r1);
				double t2 = Range::t2(r2, r1);

				if (den != 0.0) {
					t1 /= den; t2 /= den;
					if (t1 > 0.0 && t1 < 1.0 && t2 > 0.0 && t2 < 1.0) {
						collide = true; break;
					}
				}
			}
		}
	}
	if (collide) {
		if (i >= laser && i < aLaser && j >= ast && j < end) {
			// we have a collision
			int k;
			Vector s = b->s;
			
			if (((score + b->points) / 10000) > (score / 10000)) {
				this->lives++;
			}
			score += b->points;
			delete a;
			entity[i] = NULL;

			for (k = ast; k < end; k++)
				if (entity[k] == NULL)
					break;
			entity[k] = new Asteroid(*b);
			for (k = ast; k < end; k++)
				if (entity[k] == NULL)
					break;
			entity[k] = new Asteroid(*b, true);
			
			delete b;
			entity[j] = new Explosion();
			entity[j]->s = s;
		} else if (i == ship && j >= ast && j < end) {
			int k;
			Vector s = b->s;
			Vector r = a->s;
			
//			score += b->points;
			delete a;
			entity[i] = new Explosion();
			entity[i]->s = r;

			for (k = ast; k < end; k++)
				if (entity[k] == NULL)
					break;
			entity[k] = new Asteroid(*b);
			for (k = ast; k < end; k++)
				if (entity[k] == NULL)
					break;
			entity[k] = new Asteroid(*b, true);
			
			delete b;
			entity[j] = new Explosion();
			entity[j]->s = s;
		} else if (i == ship && j == aLaser) {
			Vector s = a->s;
			delete a;
			entity[i] = new Explosion();
			entity[i]->s = s;
			
			delete b;
			entity[j] = NULL;
		} else if (i == alien && j >= laser && j < aLaser) {
			Vector s = a->s;
			score += a->points;
			delete a;
			entity[i] = new Explosion();
			entity[i]->s = s;
			delete b;
			entity[j] = NULL;
		} else if (i == ship && j == alien) {
			Vector s = a->s;
			Vector r = b->s;
			delete a;
			delete b;
			entity[i] = new Explosion();
			entity[i]->s = a->s;
			entity[j] = new Explosion();
			entity[j]->s = b->s;
		}
	}
}
