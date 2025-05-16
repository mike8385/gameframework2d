#pragma once
#pragma once
#ifndef __ICE_WIZARD_H__
#define __ICE_WIZARD_H__
#include "gfc_vector.h"

#include "entity.h"






/**
* @brief spawn a new ice_wizard entity
* @param position where to spawn it
* @return NULL on error, or a pointer to the spawned ice_wizard entity
*/

Entity* ice_wizard_new_entity(GFC_Vector2D position);

/**
* @brief Controls the ice_wizard with the wsad keys

*/
void ice_wizard_move(Entity* self);

/**
* @brief run the think function for ice_wizard
*/
void ice_wizard_think(Entity* self);

/**
* @brief Update ice_wizard
* @param The entity itself
*/
void ice_wizard_update(Entity* self);

/**
* @brief damage done to the ice_wizard
* @param The entity itself
*/
void ice_wizard_damage(Entity* self);

/**
* @brief Tracks the player position
* @param The entity itself
* Never used
*/
void ice_wizard_track_player(Entity* self);

/**
* @brief Checks to see if ice_wizard can find player
* @param The entity itself
* @return 1 if true, 0 if false
*/
Uint8 ice_wizard_see_player(Entity* self);

/**
* @brief The way the ice_wizard attacks
* @param The entity itself
*/
void ice_wizard_attack(Entity* self);


/**
* @brief Update ice_wizard status condition
* @param The entity itself
*/
void ice_wizard_status(Entity* self);

void ice_wizard_free(Entity* self);



#endif
#pragma once