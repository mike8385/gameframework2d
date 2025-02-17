#ifndef __WORLD_H__
#define __WORLD_H_

#include "gf2d_sprite.h"
#include "gfc_shape.h"
#include "gf2d_draw.h"

typedef struct
{
	Sprite*			background;				/**<Background image for the world*/
	GFC_Rect		bounds;					/**<Bounds for the map*/
	Sprite*			tileSet;				/**Sprite containing tiles for the world<*/
	Uint8*			tileMap;				/**<The tiles that make up the world*/
	Uint32			tileHeight;				/**<How many tiles tall the map is*/
	Uint32			tileWidth;				/**<How many tiles long the map is*/

}World;

/**
* @brief Test function to see if world is working;
*/
World* world_test_new();

/**
* @brief allocate a new empty world
* @return NULL on error, or a blank world
*/
World* world_new(Uint32 width, Uint32 height);


/**
* @brief free a previously allocated world
* @param world the world to free
*/
void world_free(World *world);

/**
* @brief draw the world
* @param world the world to draw
*/
void* world_draw(World* world);


#endif

#pragma once
