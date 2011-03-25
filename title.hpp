#ifndef _TITLE_HPP
#define _TITLE_HPP

#include "common.h"
#include "vector.hpp"
#include "scene.hpp"
#include "bitmap.hpp"

class Title : public Scene {
	int btn[3];
	int keys[3];
	int ttime;
	int recharge;
	bool gc, mouse_control;

	int mouse_x, mouse_y;
	Vector world_mouse;
	int xold, yold;
	bool done;
	static const int RECHARGE = 7;
	float color[4];

	/* entity ranges */
	static const int ship = 0;		/* just one ship */
	static const int laser = 1;		/* maximum of (ast - laser) lasers... 4 in this case */
	static const int ast = 5;		/* maximum of (end - ast) asteroids */
	int end;			/* end of entities... one past the last entity */

	Bitmap *title;
public:
	Title();
	~Title();

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
};

#endif
