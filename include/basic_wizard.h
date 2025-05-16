#ifndef __BASIC_WIZARD_H__
#define __BASIC_WIZARD_H__
#include "gfc_vector.h"

#include "entity.h"



/**
* @brief spawn a new basic_wizard entity
* @param position where to spawn it
* @return NULL on error, or a pointer to the spawned basic_wizard entity
*/

Entity* basic_wizard_new_entity(GFC_Vector2D position);

/**
* @brief Controls the basic_wizard with the wsad keys

*/
void basic_wizard_move(Entity* self);

/**
* @brief run the think function for basic_wizard
*/
void basic_wizard_think(Entity* self);

/**
* @brief Update basic_wizard
*/
void basic_wizard_update(Entity* self);

/**
* @Brief Allows the basic_wizard to take damage and despawn (free)
* when health is zero
*/
void basic_wizard_damage(Entity* self);

/**
* @Brief Allows the basic_wizard track where the players location is at all times.
* @Note: It calls a function in player that has a global playerData. This allows us to
* always have access to the player specific data when needed.
*/
void basic_wizard_track_player(Entity* self);

/**
* @Brief Allows the basic_wizard to shoot projecticles based on certain conditions
* @Param The entity itself
* Return 1 if True, 0 if False
*/
Uint8 basic_wizard_see_player(Entity* self);


/**
* @Brief Allows the basic_wizard to shoot projecticles based on certain conditions
*
*/
void basic_wizard_attack(Entity* self);

/**
* @Brief Allows the basic_wizard to Have status effects
*/
void basic_wizard_status(Entity* self);

/**
* @Brief Frees the basic_wizard
*/
void basic_wizard_free(Entity* self);



#endif
#pragma once
