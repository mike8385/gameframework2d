#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_text.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

typedef struct Entity_S
{
	Uint8			_inuse;				/**<Memory management flag*/
	GFC_TextLine	name;				/**<Name of the entity for debugging*/
	Sprite*			sprite;				/**<Graphical representation of entity*/
	float			frame;				/**<for drawing the sprite*/
	GFC_Vector2D	position;			/**<Were to draw it*/
	GFC_Vector2D	velocity;			/**<how we are moving*/
	GFC_Vector2D	acceleration;
}Entity;

/**
* @brief initialize the entity sub entity system_init
* @param maxEnts upper limit for how many entities can exist at once
*/
void entity_system_init(Uint32 maxEnts);

/*
* @brief free all entities in the manager
*/
void entity_system_free_all();

/*
* 
*/
Entity* entity_new();

#endif