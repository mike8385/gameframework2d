#ifndef __MONSTER_H__
#define __MONSTER_H__
#include "gfc_vector.h"

#include "entity.h"



/**
* @brief spawn a new monster entity
* @param position where to spawn it
* @return NULL on error, or a pointer to the spawned monster entity
*/

Entity* monster_new_entity(GFC_Vector2D position);

/**
* @brief Controls the monster with the wsad keys

*/
void monster_move(Entity* self);

/**
* @brief run the think function for monster
*/
void monster_think(Entity* self);

/**
* @brief Update monster
*/
void monster_update(Entity* self);

/**
* @Brief Allows the monster to take damage and despawn (free)
* when health is zero
*/
void monster_damage(Entity* self);

/**
* @Brief Allows the monster track where the players location is at all times.
* @Note: It calls a function in player that has a global playerData. This allows us to
* always have access to the player specific data when needed.
*/
void monster_track_player(Entity* self);

/**
* @Brief Allows the monster to shoot projecticles based on certain conditions
* @Param The entity itself
* Return 1 if True, 0 if False
*/
Uint8 monster_see_player(Entity* self);


/**
* @Brief Allows the monster to shoot projecticles based on certain conditions
* 
*/
void monster_attack(Entity* self);

/**
* @Brief Allows the monster to Have status effects
*/
void monster_status(Entity* self);

/**
* @Brief Frees the monster
*/
void monster_free(Entity* self);



#endif
#pragma once
