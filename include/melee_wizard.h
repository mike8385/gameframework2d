
#ifndef __MELEE_WIZARD_H__
#define __MELEE_WIZARD_H__
#include "gfc_vector.h"

#include "entity.h"



/**
* @brief spawn a new melee_wizard entity
* @param position where to spawn it
* @return NULL on error, or a pointer to the spawned melee_wizard entity
*/

Entity* melee_wizard_new_entity(GFC_Vector2D position);

/**
* @brief Controls the melee_wizard with the wsad keys

*/
void melee_wizard_move(Entity* self);

/**
* @brief run the think function for melee_wizard
*/
void melee_wizard_think(Entity* self);

/**
* @brief Update melee_wizard
* @param The entity itself
*/
void melee_wizard_update(Entity* self);

/**
* @brief damage done to the melee_wizard
* @param The entity itself
*/
void melee_wizard_damage(Entity* self);

/**
* @brief Tracks the player position
* @param The entity itself
* Never used
*/
void melee_wizard_track_player(Entity* self);

/**
* @brief Checks to see if melee_wizard can find player
* @param The entity itself
* @return 1 if true, 0 if false
*/
Uint8 melee_wizard_see_player(Entity* self);

/**
* @brief The way the melee_wizard attacks
* @param The entity itself
*/
void melee_wizard_attack(Entity* self);


/**
* @brief Update melee_wizard status condition
* @param The entity itself
*/
void melee_wizard_status(Entity* self);


#endif
#pragma once
