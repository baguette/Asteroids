#ifndef _SCENE_HPP
#define _SCENE_HPP

#include "entity.hpp"

class Scene {
protected:
	Entity **entity;
public:
	bool active;

	Scene() {}
	virtual ~Scene() {}

	virtual Scene *next_scene() = 0;

	virtual void display() = 0;
	virtual void timer(int value) = 0;
	virtual void keyboard(unsigned char key, int x, int y) = 0;
	virtual void unkeyboard(unsigned char key, int x, int y) = 0;
	virtual void special(int key, int x, int y) = 0;
	virtual void unspecial(int key, int x, int y) = 0;
	virtual void menu(int id) = 0;
	virtual void mouse(int b, int state, int x, int y) = 0;
	virtual void motion(int x, int y) = 0;
};

#endif
