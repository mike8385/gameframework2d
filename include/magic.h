#ifndef __MAGIC_H__
#define __MAGIC_H__


#include "simple_json.h"

#include "world.h"
#include "entity.h"


Entity* spell_new_entity(GFC_Vector2D position);

/**
* @brief Move the spell depending on certian requirements in the code
* @params The entity itself
*/
void spell_move(Entity* self);

void spell_think(Entity* self);

void spell_update(Entity* self);

/**
* @brief run an attack function for an Entity
*/
void spell_attack(Entity* self);


/**
* @brief Checks if the spell collides with world bounds
* @param The entity itself
* @return If it gets to the end of the world bounds/screen, free
*/
void spell_world_collision(Entity* self);

/**
* @brief Checks if the spell collides with ANYTHING
* If so, check the type and depending on type flag, you free, damage, or ignore it
* @param The entity itself
*/
void spell_collision(Entity* self);


/**
* @brief being able to set the different spell types
* @param The entity itself
*/
void spell_type(Entity* self);

void magic_free(Entity* self);


#pragma once
#endif