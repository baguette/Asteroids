#ifndef _ENTITY_HPP
#define _ENTITY_HPP

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
	int		n_edges;
	
	Entity();
	virtual ~Entity();
	
	/* Apply the transformations for this entity. */
	virtual void transform();
	
	/* Update callback. Each entity should be able to update itself. */
	virtual void update();
	
	virtual void behavior();
	
	virtual bool isAlive() = 0;
	
	/* Draw a wireframe version of this entity. */
	virtual void wire() = 0;
	
	/* Draw a solid version of this entity. */
	virtual void solid() = 0;
	
	/* Get the projection of this entity using edge v */
	virtual Range shadow(Vector v) = 0;
	
	/* Get the vector representing edge n */
	virtual Range edge(int n) = 0;
};

#endif
