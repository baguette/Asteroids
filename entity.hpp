#ifndef _ENTITY_H
#define _ENTITY_H

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
	float x, y, z;
	struct {
		float x, y, z;
	} scale;
	struct {
		float x, y, z;
	} rotate;
	
	Entity();
	virtual ~Entity();
	
	/* Apply the transformations for this entity. */
	virtual void transform();
	
	/* Draw a wireframe version of this entity. */
	virtual void wire() = 0;
	
	/* Draw a solid version of this entity. */
	virtual void solid() = 0;
};

#endif