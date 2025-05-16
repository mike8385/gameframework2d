#ifndef __SHOP_KEEPER_H__
#define __SHOP_KEEPER_H__
#include "gfc_vector.h"

#include "entity.h"



/**
* @brief spawn a new shop_keeper entity
* @param position where to spawn it
* @return NULL on error, or a pointer to the spawned shop_keeper entity
*/

Entity* shop_keeper_new_entity(GFC_Vector2D position);

/**
* @brief Controls the shop_keeper with the wsad keys

*/
void shop_keeper_move(Entity* self);

/**
* @brief run the think function for shop_keeper
*/
void shop_keeper_think(Entity* self);

/**
* @brief Update shop_keeper
*/
void shop_keeper_update(Entity* self);

/**
* @Brief Allows the shop_keeper to take damage and despawn (free)
* when health is zero
*/
void shop_keeper_damage(Entity* self);

/**
* @Brief Allows the shop_keeper track where the players location is at all times.
* @Note: It calls a function in player that has a global playerData. This allows us to
* always have access to the player specific data when needed.
*/
void shop_keeper_track_player(Entity* self);

/**
* @Brief Allows the shop_keeper to shoot projecticles based on certain conditions
* @Param The entity itself
* Return 1 if True, 0 if False
*/
Uint8 shop_keeper_see_player(Entity* self);


/**
* @Brief Allows the shop_keeper to shoot projecticles based on certain conditions
*
*/
void shop_keeper_attack(Entity* self);

/**
* @Brief Allows the shop_keeper to Have status effects
*/
void shop_keeper_status(Entity* self);

/**
* @Brief Frees the shop_keeper
*/
void shop_keeper_free(Entity* self);



#endif
#pragma once
