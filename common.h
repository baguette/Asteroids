#ifndef _COMMON_H
#define _COMMON_H

#ifdef _M_IX86
/* I honestly don't care if fopen() isn't "safe"... */
#pragma warning ( disable : 4996 )
#include <windows.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>

#ifdef _M_IX86
#include <gl/glut.h>
#define M_PI 3.141592654
#else
#include <glut/glut.h>
#endif

extern double rando(double a, double b);
extern void draw_string(float x, float y, const char *txt);
extern int window_height, window_width;

#endif
