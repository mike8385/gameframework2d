#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "gfc_vector.h"

#include "entity.h"


/**
* @brief spawn a new player entity
* @param position where to spawn it
* @return NULL on error, or a pointer to the spawned player entity
*/

Entity* player_new_entity(GFC_Vector2D position);

/**
* @brief Controls the player with the wsad keys

*/
void player_move(Entity *self);

/**
* @brief run the think function for player
*/
void player_think(Entity *self);

/**
* @brief Update player
*/
void player_update(Entity *self);

void player_attack(Entity* self);

GFC_Rect get_player_bounds();


GFC_Vector2D get_player_position();


#endif
#pragma once
