#pragma once
#pragma once
#pragma once
#ifndef __END_BOSS_H__
#define __END_BOSS_H__
#include "gfc_vector.h"

#include "entity.h"

extern int process;
extern int setup;
extern int currentLevel;
/**
* @brief spawn a new end_boss entity
* @param position where to spawn it
* @return NULL on error, or a pointer to the spawned end_boss entity
*/

Entity* end_boss_new_entity(GFC_Vector2D position);

/**
* @brief Controls the end_boss with the wsad keys

*/
void end_boss_move(Entity* self);

/**
* @brief run the think function for end_boss
*/
void end_boss_think(Entity* self);

/**
* @brief Update end_boss
* @param The entity itself
*/
void end_boss_update(Entity* self);

/**
* @brief damage done to the end_boss
* @param The entity itself
*/
void end_boss_damage(Entity* self);

/**
* @brief Tracks the player position
* @param The entity itself
* Never used
*/
void end_boss_track_player(Entity* self);

/**
* @brief Checks to see if end_boss can find player
* @param The entity itself
* @return 1 if true, 0 if false
*/
Uint8 end_boss_see_player(Entity* self);

/**
* @brief The way the end_boss attacks
* @param The entity itself
*/
void end_boss_attack(Entity* self);


/**
* @brief Update end_boss status condition
* @param The entity itself
*/
void end_boss_status(Entity* self);

void end_boss_free(Entity* self);



#endif
#pragma once
