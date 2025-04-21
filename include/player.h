#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "gfc_vector.h"

#include "entity.h"


typedef struct S_stats
{
	Uint8	strength;
	Uint8	magic;
	Uint8	defense;
	Uint8	speed;
	Uint8	luck;
	Uint32	EXP;
}Stats;



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

/**
* @brief Attack for the player
* @param The entity itself
*/
void player_attack(Entity* self);

/**
* @brief Gets the player bounds
* @Note: This is so that anything can get bounds for player
* return GFC_Rect of the bounds box
*/
GFC_Rect get_player_bounds();

/**
* @brief Gets the player position
* @Note: This is so that anything can get position for player
* return GFC_Vector2D of the position
*/
GFC_Vector2D get_player_position();


/**
* @brief frees the player
* @param player itself
*/
void player_free(Entity* self);

/**
* @brief Gets the stats for the player, this allows us to upgrade them
* return Stats
*/
Stats* get_player_stats();

/*
* @Brief Allows the player to level up.
*/
void player_level_up(Entity* self);


float get_player_health();

void player_status(Entity* self);

#endif
#pragma once
