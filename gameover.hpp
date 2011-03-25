#ifndef _GAMEOVER_HPP
#define _GAMEOVER_HPP
#include "common.h"
#include "entity.hpp"
#include "scene.hpp"

class Gameover : public Scene {
private:
	bool done;
	int delay;
public:

	Gameover();
	~Gameover() { active = false; }

	Scene *next_scene();
    
	void display();
	void timer(int value);
	void keyboard(unsigned char key, int x, int y);
	void unkeyboard(unsigned char key, int x, int y);
	void special(int key, int x, int y);
	void unspecial(int key, int x, int y);
	void menu(int id);
	void mouse(int b, int state, int x, int y);
	void motion(int x, int y);
};

#endif
