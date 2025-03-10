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

void player_attack(Entity* self);

GFC_Rect get_player_bounds();


GFC_Vector2D get_player_position();

void player_free(Entity* self);


Stats* get_player_stats();

void player_level_up(Entity* self);


#endif
#pragma once
