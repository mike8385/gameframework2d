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

#endif
#pragma once
