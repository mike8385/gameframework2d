#ifndef __PETS_H__
#define __PETS_H__
#include "gfc_vector.h"

#include "entity.h"



/**
* @brief spawn a new pets entity
* @param position where to spawn it
* @return NULL on error, or a pointer to the spawned pets entity
*/

Entity* pets_new_entity(GFC_Vector2D position);

/**
* @brief Controls the pets with the wsad keys

*/
void pets_move(Entity* self);

/**
* @brief run the think function for pets
*/
void pets_think(Entity* self);

/**
* @brief Update pets
*/
void pets_update(Entity* self);

/**
* @Brief Allows the pets to take damage and despawn (free)
* when health is zero
*/
void pets_damage(Entity* self);

/**
* @Brief Allows the pets track where the players location is at all times.
* @Note: It calls a function in player that has a global playerData. This allows us to
* always have access to the player specific data when needed.
*/
void pets_track_player(Entity* self);

/**
* @Brief Allows the pets to shoot projecticles based on certain conditions
* @Param The entity itself
* Return 1 if True, 0 if False
*/
Uint8 pets_see_player(Entity* self);


/**
* @Brief Allows the pets to shoot projecticles based on certain conditions
*
*/
void pets_attack(Entity* self);

/**
* @Brief Allows the pets to Have status effects
*/
void pets_status(Entity* self);



void pets_collision(Entity* self);


/**
* @Brief Frees the pets
*/
void pets_free(Entity* self);





#endif
#pragma once
