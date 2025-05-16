#ifndef __FIRE_WIZARD_H__
#define __FIRE_WIZARD_H__
#include "gfc_vector.h"

#include "entity.h"



/**
* @brief spawn a new fire_wizard entity
* @param position where to spawn it
* @return NULL on error, or a pointer to the spawned fire_wizard entity
*/

Entity* fire_wizard_new_entity(GFC_Vector2D position);

/**
* @brief Controls the fire_wizard with the wsad keys

*/
void fire_wizard_move(Entity* self);

/**
* @brief run the think function for fire_wizard
*/
void fire_wizard_think(Entity* self);

/**
* @brief Update fire_wizard
* @param The entity itself
*/
void fire_wizard_update(Entity* self);

/**
* @brief damage done to the fire_wizard
* @param The entity itself
*/
void fire_wizard_damage(Entity* self);

/**
* @brief Tracks the player position
* @param The entity itself
* Never used
*/
void fire_wizard_track_player(Entity* self);

/**
* @brief Checks to see if fire_wizard can find player
* @param The entity itself
* @return 1 if true, 0 if false
*/
Uint8 fire_wizard_see_player(Entity* self);

/**
* @brief The way the fire_wizard attacks
* @param The entity itself
*/
void fire_wizard_attack(Entity* self);


/**
* @brief Update fire_wizard status condition
* @param The entity itself
*/
void fire_wizard_status(Entity* self);

void fire_wizard_free(Entity* self);

#endif
#pragma once
