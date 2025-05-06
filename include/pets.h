#ifndef __PETS_H__
#define __PETS_H__
#include "gfc_vector.h"

#include "entity.h"





/*
* @brief Gets a pets definition by their name
* @param The name of the pet to get the definition
* @return a SJson* of the pet in the def
* @note: Gets a pets definition by searching the list of objects
* and returning the pet if found
* @note: pet_new() calls this function
*/
SJson* pets_get_def_by_name(const char* name);

/*
* @brief Creates a new pet based off the pet name
* @param The name of pet to be created
* @return The pet entity
*/
Entity* pets_new(const char* name);


/*
* @brief Initializes all the pets to be gathered from the def file
* @param The filename
*/
void pets_initalize(const char* filename);

/**
* @brief spawn a new pets entity
* @param position where to spawn it
* @return NULL on error, or a pointer to the spawned pets entity
*/
Entity* pets_new_entity(GFC_Vector2D position);

/**
* @brief spawn a new pets entity
* @param position where to spawn it
* @return NULL on error, or a pointer to the spawned pets entity
* @note: Difference between this and 'pets_new_entity' is this one
* uses the data from the JSON loading it. Didnt want to delete the
* old function just in case.
*/
void pets_new_entity_placed(Entity* self, GFC_Vector2D position);


/**
* @brief Controls the pets with the wsad keys

*/
void pets_move(Entity* self);

/**
* @brief run the think function for pets
*/
void pets_think(Entity* self);

/**
* @brief Update pets
*/
void pets_update(Entity* self);

/**
* @Brief Allows the pets to take damage and despawn (free)
* when health is zero
*/
void pets_damage(Entity* self);

/**
* @brief Allows the pets track where the players location is at all times.
* @note: It calls a function in player that has a global playerData. This allows us to
* always have access to the player specific data when needed.
*/
void pets_track_player(Entity* self);

/**
* @Brief Allows the pets to shoot projecticles based on certain conditions
* @Param The entity itself
* Return 1 if True, 0 if False
*/
Uint8 pets_see_player(Entity* self);


/**
* @Brief Allows the pets to shoot projecticles based on certain conditions
*
*/
void pets_attack(Entity* self);

/**
* @Brief Allows the pets to Have status effects
*/
void pets_status(Entity* self);



void pets_collision(Entity* self);


/**
* @Brief Frees the pets
*/
void pets_free(Entity* self);





#endif
#pragma once
