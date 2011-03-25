#ifndef _BITMAP_H
#define _BITMAP_H

#include "common.h"

#define BITMAP_HEADER_SIZE 54

class Bitmap {
private:
	struct {
		GLushort magic;		/* should be 0x4d42 */
		GLuint fsize;
		GLushort res1;
		GLushort res2;
		GLuint start;
	
		GLuint hsize;		/* should be 40 */
		GLint width;
		GLint height;		/* if negative, rows top to bottom */
		GLushort nplanes;	/* must be 1 */
		GLushort bpp;
		GLuint compression;	/* should be 0 for uncompressed data */
		GLuint size;		/* may be 0 */
		GLint hres;
		GLint vres;
		GLuint psize;		/* 0 is default */
		GLuint ncolors;		/* 0 when all colors are important */
	
		struct {
			GLubyte b, g, r, x;
		} *palette;			/* no palette when bpp >= 16 */
	
		GLubyte *data;
	} bitmap;

public:
	Bitmap(const char *file);
	~Bitmap();
	void draw(float x, float y);
	bool isLoaded();
};

#endif
