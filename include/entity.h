#ifndef __ENTITY_H__
#define __ENTITY_H__


#include "simple_json.h"
#include "gfc_text.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"
#include "gfc_types.h"
#include "gfc_shape.h"
#include "world.h"

//typedef enum
//{
//
//} EntityTeamType;
//
//typedef enum
//{
//
//}EntityCollisionLayers;


typedef enum
{
	ETC_entity = 1,
	ETC_world = 2,
	ETC_other = 3,
	ETC_magic = 4,
	ETC_monster = 5,
	ETC_MAX = 15
} EntityTypeCollide;

typedef struct Entity_S
{
	Uint8					_inuse;						/**<Memory management flag*/
	GFC_TextLine			name;						/**<Name of the entity for debugging*/
	GFC_Rect				bounds;						/**<Entity Bounds*/
	GFC_Vector2D			ground;
	Sprite*					sprite;						/**<Graphical representation of entity*/
	float					frame;						/**<for drawing the sprite*/
	GFC_Vector2D			position;					/**<Were to draw it*/
	GFC_Vector2D			velocity;					/**<how we are moving*/
	GFC_Vector2D			acceleration;
	EntityTypeCollide		collidedType;
	float					magicCooldown;
	Uint32					lastAttackTime;


	void (*think)(struct Entity_S* self);		/**<Function to call to make decisions*/
	void (*update)(struct Entity_S* self);		/**<Function to call to execute those decisions*/
	void (*attack)(struct Entity_S* self);		/**<Function to call to attack*/
	void (*damage)(struct Entity_S* self, struct Entity_S *other, EntityTypeCollide type);		/**<Function to call to attack*/
	void (*move)(struct Entity_S* self);		/**<Function to call to move*/
	void (*free)(struct Entity_S* self);		/**<Function to clean up any custom allocated data*/
	void* data;									/**<For ad hoc addition data for the entity*/
	//int			(*collide)(struct Entity_S, *self, struct Entity_S *other, EntityCollisionType type);	
	int health;

	
	//


}Entity;

/**
* @brief initialize the entity sub entity system_init
* @param maxEnts upper limit for how many entities can exist at once
*/
void entity_system_init(Uint32 maxEnts);

/*
* @brief free all entities in the manager
* @Param ignore do not clean up this entity
*/
void entity_system_clear_all();

/*
* @brief get new empty entity to work with
* @return NULL if out of entities, or a blank entity otherwise
*/
Entity* entity_new();

/*
* @brief free a previously created entity
*/
void entity_free(Entity* self);

/*
* @brief free all entities in the manager
* @Param ignore do not clean up this entity
*/
void entity_system_free_all();


/**
* @brief run the think function for all active entites
*/
void entity_system_think();

/**
* @brief Update all active entites
*/
void entity_system_update();

/**
* @brief draw all active entites
*/
void entity_system_draw();

/**
* @brief configure an entity based on provided json config
* 
**/
void entity_configure(Entity* self, SJson* json);


void entity_bounds_update(Entity* self);

/**
* @brief run an attack function for an Entity
* 
* 
*/
void entity_attack(Entity* self);

/**
* @brief move function for an Entity
* @param The entity itself
*/
void entity_move(Entity* self);


/**
* @brief Entity Collision System that allows it to check the type that collides with it, and if its correct typ destroy
* @param The entity itself, the other entity.
*/
void entity_collision(Entity* self, Entity* other);

#endif