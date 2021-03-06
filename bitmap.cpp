#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "bitmap.hpp"

/* BMPs store pixel data as BGRA, but OpenGL 1.1 only supports RGBA,
 * so this function will read the color components in reverse order.
 */
static int read_rgba(GLubyte *buffer, FILE *fp)
{
	int i, r = 0;
	for (i = 0; i < 4; i++)
		r += fread(buffer+i, 1, 1, fp);
	return r;
}

Bitmap::Bitmap(const char *file)
{
	FILE *fp;
	int size, offset = 0;
	int height = 0;
	
	bitmap.palette = NULL;
	bitmap.data = NULL;
	
	fp = fopen(file, "rb");
	if (!fp) {
		fprintf(stderr, "unable to open %s\n", file);
		return;		/* eventually this should throw an exception... */
	}
	
	/* have to read this separately because of alignment issues... */
	fread(&bitmap.magic, 2, 1, fp);
	
	if (fread(&bitmap.fsize, BITMAP_HEADER_SIZE-2, 1, fp) < 1) {
		fprintf(stderr, "error loading bitmap %s\n", file);
		fclose(fp);
		return;
	}
	
	if (bitmap.size)
		size = bitmap.size * bitmap.bpp / 8;
	else
		size = bitmap.fsize - BITMAP_HEADER_SIZE;
	
	if (bitmap.height < 0)
		offset = size - bitmap.width * bitmap.bpp / 8;
	height = bitmap.height;

//	printf("%d\n", bitmap.height);

	/* no support for palettes right now... */
	if (bitmap.bpp != 32 || bitmap.psize != 0) {
		fprintf(stderr, "unsupported bitmap %s\n", file);
	}
	
	bitmap.data = (GLubyte *)malloc(size);
	if (!bitmap.data) {
		fprintf(stderr, "unable to allocate %d bytes for %s\n", size, file);
		fclose(fp);
		return;
	}
	
	/* if it's stored top to bottom, we have to reverse the rows for OpenGL */
	if (offset) {
		while (height) {
			GLint width;
			for (width = 0; width < bitmap.width; width++) {
				if (read_rgba(bitmap.data+offset+width*bitmap.bpp/8, fp) < 1) {
					fprintf(stderr, "error reading from %s\n", file);
					fclose(fp);
					return;
				}
			}
			height++;
			offset -= bitmap.width * bitmap.bpp / 8;
		}
	} else {
		while (height) {
			GLint width;
			for (width = 0; width < bitmap.width; width++) {
				if (read_rgba(bitmap.data+offset+width*bitmap.bpp/8, fp) < 1) {
					fprintf(stderr, "error reading from %s\n", file);
					fclose(fp);
					return;
				}
			}
			height--;
			offset += bitmap.width * bitmap.bpp / 8;
		}
	}

	fclose(fp);
}

Bitmap::~Bitmap()
{
	free(bitmap.palette);
	free(bitmap.data);
	bitmap.palette = NULL;
	bitmap.data = NULL;
}

void Bitmap::draw(float x, float y)
{	
	int height = bitmap.height;
	height = (height < 0) ? -height : height;
	glRasterPos2f(x, y);
	glDrawPixels(bitmap.width, height, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.data);
}

bool Bitmap::isLoaded()
{
	if (bitmap.data)
		return true;
	return false;
}
