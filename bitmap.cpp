#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "bitmap.hpp"

/*
 * Constructor.
 * Loads bitmap data from file into a new Bitmap object.
 */
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
	}
	
	/* have to read this separately because of alignment issues... */
	fread(&bitmap.magic, 2, 1, fp);
	
	if (fread(&bitmap.fsize, BITMAP_HEADER_SIZE-2, 1, fp) < 1) {
		fprintf(stderr, "error loading bitmap %s\n", file);
	}
	
	if (bitmap.size)
		size = bitmap.size * bitmap.bpp / 8;
	else
		size = bitmap.fsize - BITMAP_HEADER_SIZE;
	
	if (bitmap.height < 0)
		offset = size - bitmap.width * bitmap.bpp / 8;
	height = bitmap.height;

	/* no support for palettes right now... */
	if (bitmap.bpp != 32 || bitmap.psize != 0) {
		fprintf(stderr, "unsupported bitmap %s\n", file);
	}
	
	bitmap.data = (GLubyte *)malloc(size);
	if (!bitmap.data) {
		fprintf(stderr, "unable to allocate %d bytes for %s\n", size, file);
		fclose(fp);
	}
	
	/* if it's stored top to bottom, we have to reverse the rows for OpenGL */
	if (offset) {
		while (height) {
			if (fread(bitmap.data+offset, 1, bitmap.width * bitmap.bpp / 8, fp) < 1) {
				fprintf(stderr, "error reading from %s\n", file);
				fclose(fp);
			}
			height++;
			offset -= bitmap.width * bitmap.bpp / 8;
		}
	} else {
		if (fread(bitmap.data, 1, size, fp) < 1) {
			fprintf(stderr, "error reading from %s\n", file);
			fclose(fp);
		}
	}

	fclose(fp);
}

/*
 * Destructor.
 * Frees data that was allocated in the constructor.
 */
Bitmap::~Bitmap()
{
	free(bitmap.palette);
	free(bitmap.data);
	bitmap.palette = NULL;
	bitmap.data = NULL;
}

/*
 * Draw this Bitmap at x, y (OpenGL world coordinates).
 */
void Bitmap::draw(float x, float y)
{	
	int height = bitmap.height;
	height = (height < 0) ? -height : height;
	glRasterPos2f(x, y);
	glDrawPixels(bitmap.width, height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, bitmap.data);
}

/*
 * Determine if this Bitmap has valid data associated with it.
 * This is just a hackish way to handle errors. It'd be much better
 * to throw an exception, but I didn't feel like it.
 */
bool Bitmap::isLoaded()
{
	if (bitmap.data)
		return true;
	return false;
}
