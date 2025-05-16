#ifndef __PLAYER2_H__
#define __PLAYER2_H__
#include "gfc_vector.h"

#include "entity.h"

#include "player.h"


/**
* @brief spawn a new player22 entity
* @param position where to spawn it
* @return NULL on error, or a pointer to the spawned player2 entity
*/

Entity* player2_new_entity(GFC_Vector2D position);

/**
* @brief Controls the player2 with the wsad keys

*/
void player2_move(Entity* self);

/**
* @brief run the think function for player2
*/
void player2_think(Entity* self);

/**
* @brief Update player2
*/
void player2_update(Entity* self);

/**
* @brief Attack for the player2
* @param The entity itself
*/
void player2_attack(Entity* self);

/**
* @brief Gets the player2 bounds
* @Note: This is so that anything can get bounds for player2
* return GFC_Rect of the bounds box
*/
GFC_Rect get_player2_bounds();

/**
* @brief Gets the player2 position
* @Note: This is so that anything can get position for player2
* return GFC_Vector2D of the position
*/
GFC_Vector2D get_player2_position();


/**
* @brief frees the player2
* @param player2 itself
*/
void player2_free(Entity* self);

/**
* @brief Gets the stats for the player2, this allows us to upgrade them
* return Stats
*/
Stats* get_player2_stats();

/*
* @Brief Allows the player2 to level up.
*/
void player2_level_up(Entity* self);


float get_player2_health();

void player2_status(Entity* self);

/*
* @brief Gets the player2s data
*/
Entity* player2_get_player2();



Uint8 get_player2_pet();

void player2_update_pet(Uint8 bool);

void player2_use_items(Entity* self);

#endif
#pragma once


#pragma once
