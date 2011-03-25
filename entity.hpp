#ifndef _ENTITY_H
#define _ENTITY_H

#include "vector.hpp"

/*
 * Defines an interface for all game objects to implement.
 * All game objects have a location and other transformation properties.
 * They also all two methods to draw themselves.
 * They may share or override this transform(), which by default applies
 * a translation, a rotation, and a scale based on the transform properties.
 */
class Entity {
public:
	/* Some public properties. */
	Vector	s,	/* displacement (position) */
			dir,	/* direction relative to z axis */
			r,	/* rotational velocity */
			scale,
			rotate;
	double 	v,	/* velocity */
			a,	/* acceleration */
			mass;
	
	Entity();
	virtual ~Entity();
	
	/* Apply the transformations for this entity. */
	void transform();
	
	/* Update callback. Each entity should be able to update itself. */
	void update();
	
	virtual void behavior();
	
	virtual bool isAlive() = 0;
	
	/* Draw a wireframe version of this entity. */
	virtual void wire() = 0;
	
	/* Draw a solid version of this entity. */
	virtual void solid() = 0;
};

#endif
