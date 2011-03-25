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
	
	Ship hud;

	/* entity ranges */
	static const int ship = 0;		/* just one ship */
	static const int laser = 1;		/* maximum of (ast - laser) lasers... 4 in this case */
	static const int ast = 5;		/* maximum of (end - ast) asteroids */
	int end;			/* end of entities... one past the last entity */

	int n_ast;		/* number of initial asteroids */
	std::stringstream level_msg;
	
	bool done;

	void fire();
public:
	Game(int n);
	Game(int n, int l);
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
