#ifndef _ENTITY_HPP
#define _ENTITY_HPP

#include "vector.hpp"
#include <vector>

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
	long	points;
	// Shape used for collision detection
	enum Shape {
		None = 0, Point, Circle, Polygon
	} shape;
	// Vertices used for collision detection
	virtual std::vector<Vector> vertices() {};
	
	Entity();
	virtual ~Entity();
	
	/* Apply the transformations for this entity. */
	virtual void transform();
	
	/* Update callback. Each entity should be able to update itself. */
	virtual void update();
	
	virtual int behavior();
	
	virtual bool isAlive() = 0;
	
	/* Draw a wireframe version of this entity. */
	virtual void wire() = 0;
	
	/* Draw a solid version of this entity. */
	virtual void solid() = 0;
};

#endif
