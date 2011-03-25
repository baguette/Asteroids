#ifndef _GAME_HPP
#define _GAME_HPP
#include "common.h"

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

#include <sstream>

class Game : public Scene {
	int btn[3];
	int keys[3];
	int ttime;
	int recharge;
	int lives;
	bool gc, mouse_control;

	int mouse_x, mouse_y;
	Vector world_mouse;
	int xold, yold;
	
	static const int RECHARGE = 7;
	float color[4];
	
	bool small_alien_is_fair_game;
	unsigned int countdown;
	
	Ship hud;

	/* entity ranges */
	static const int ship = 0;		/* just one ship */
	static const int alien = 1;
	static const int laser = 2;		/* maximum of (aLaser - laser) lasers... 4 in this case */
	static const int aLaser = 6;
	static const int ast = 7;		/* maximum of (end - ast) asteroids */
	int end;			/* end of entities... one past the last entity */

	int n_ast;		/* number of initial asteroids */
	int level;
	long score;
	std::stringstream level_msg;
	std::stringstream score_msg;
	std::stringstream lives_msg;
	
	bool done;

	void fire();
	void alienFire(int);
public:
	Game(int level, int lives, long score);
	~Game();

	virtual Scene *next_scene();

	virtual void display();
	virtual void timer(int value);
	virtual void keyboard(unsigned char key, int x, int y);
	virtual void unkeyboard(unsigned char key, int x, int y);
	virtual void special(int key, int x, int y);
	virtual void unspecial(int key, int x, int y);
	virtual void menu(int id);
	virtual void mouse(int b, int state, int x, int y);
	virtual void motion(int x, int y);
	
	virtual void collision(int i, int j);
};

#endif
