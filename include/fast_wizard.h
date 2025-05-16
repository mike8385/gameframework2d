#pragma once
#pragma once
#ifndef __FAST_WIZARD_H__
#define __FAST_WIZARD_H__
#include "gfc_vector.h"

#include "entity.h"



/**
* @brief spawn a new fast_wizard entity
* @param position where to spawn it
* @return NULL on error, or a pointer to the spawned fast_wizard entity
*/

Entity* fast_wizard_new_entity(GFC_Vector2D position);

/**
* @brief Controls the fast_wizard with the wsad keys

*/
void fast_wizard_move(Entity* self);

/**
* @brief run the think function for fast_wizard
*/
void fast_wizard_think(Entity* self);

/**
* @brief Update fast_wizard
* @param The entity itself
*/
void fast_wizard_update(Entity* self);

/**
* @brief damage done to the fast_wizard
* @param The entity itself
*/
void fast_wizard_damage(Entity* self);

/**
* @brief Tracks the player position
* @param The entity itself
* Never used
*/
void fast_wizard_track_player(Entity* self);

/**
* @brief Checks to see if fast_wizard can find player
* @param The entity itself
* @return 1 if true, 0 if false
*/
Uint8 fast_wizard_see_player(Entity* self);

/**
* @brief The way the fast_wizard attacks
* @param The entity itself
*/
void fast_wizard_attack(Entity* self);


/**
* @brief Update fast_wizard status condition
* @param The entity itself
*/
void fast_wizard_status(Entity* self);


//void fast_wizard_free(Entity* self);


#endif
#pragma once
