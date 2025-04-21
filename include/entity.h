#ifndef __ENTITY_H__
#define __ENTITY_H__


#include "simple_json.h"
#include "gfc_text.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"
#include "gfc_types.h"
#include "gfc_shape.h"
#include "world.h"
#include "inventory.h";



typedef enum
{
	ETC_entity = 1,
	ETC_world = 2,
	ETC_other = 3,
	ETC_magic = 4,
	ETC_monster = 5,
	ETC_monster_spell = 6,
	ETC_MAX = 15
} EntityTypeCollide;

typedef enum
{
	MT_magic = 1,
	MT_fire = 2,
	MT_rapid = 3,
	MT_freeze = 4,
	MT_melee = 5,
	MT_MAX = 6
} MagicType;

typedef struct s_Effect Effects;


typedef struct Entity_S
{
	Uint8					_inuse;						/**<Memory management flag*/
	GFC_TextLine			name;						/**<Name of the entity for debugging*/
	GFC_Rect				bounds;						/**<Entity Bounds*/
	GFC_Vector2D			ground;
	Sprite*					sprite;						/**<Graphical representation of entity*/
	GFC_List				*sprites;
	float					frame;						/**<for drawing the sprite*/
	GFC_Vector2D			position;					/**<Were to draw it*/
	GFC_Vector2D			velocity;					/**<how we are moving*/
	GFC_Vector2D			acceleration;
	EntityTypeCollide		collidedType;
	float					gravity;
	Uint8					isPlayer;
	Inventory				inventory;


	float					magicCooldown;
	float					fireMagicCooldown;
	float					burstMagicCooldown;
	float					wallMagicCooldown;
	float					lastAttackTime;
	float					lastAttackTimeBurst;
	float					lastAttackTimeWall;


	//Code for melee handling
	Uint32					lastAttackTimeMelee;
	float					meleeCooldown;

	float					worldTime;
	MagicType				magicType; /**<Type of magic thats gettting shot*/
	Effects*					statusEffects;

	float					TTL;
	float					spawnTime;

	float					lastJumpTime;

	float					jumpCooldown;
	float					health;
	float					maxHealth;
	float					damageDelt;
	float					lastDamageTime;

	float					onGround;


	void (*think)(struct Entity_S* self);		/**<Function to call to make decisions*/
	void (*update)(struct Entity_S* self);		/**<Function to call to execute those decisions*/
	void (*attack)(struct Entity_S* self);		/**<Function to call to attack*/
	void (*damage)(struct Entity_S* self, struct Entity_S *other, int damage);		/**<Function to call damage*/
	void (*move)(struct Entity_S* self);		/**<Function to call to move*/
	void (*free)(struct Entity_S* self);		/**<Function to clean up any custom allocated data*/
	void (*collision)(struct Entity_S* self);		/**<Function to call damage*/
	void* data;									/**<For ad hoc addition data for the entity*/
	void* inven;								
	//int			(*collide)(struct Entity_S, *self, struct Entity_S *other, EntityCollisionType type);	

	
	
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

/*
* @brief Check the bounds of an entity and not allow it to leave the screen/world space
*@param the entity itself
**/
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
GFC_List* entity_collide_all(Entity* self);

/**
* @brief Entity Collision check to see if the two entities collide
* @param The entity itself, the other entity.
* @return true or false (1 or 0)
*/
Uint8 entity_collision_check(Entity* self, Entity* other);

/**
* @brief What to do when collision happens
* @param self
*/
void collision(Entity* self);

void update_entity_lifetime(Entity* self);


void entity_damage(Entity* self, Entity* other);



#endif