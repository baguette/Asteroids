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
#define GL_BGRA GL_RGBA
#define GL_UNSIGNED_INT_8_8_8_8_REV GL_UNSIGNED_INT
#else
#include <glut/glut.h>
#endif
