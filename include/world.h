#ifndef __WORLD_H__
#define __WORLD_H_

#include "gf2d_sprite.h"
#include "gfc_shape.h"
#include "gf2d_draw.h"

typedef struct
{
	Sprite* background;				/**<Background image for the world*/
	Sprite* tileLayer;
	GFC_Rect		bounds;					/**<Bounds for the map*/
	Sprite* tileSet;				/**Sprite containing tiles for the world<*/
	Uint8* tileMap;				/**<The tiles that make up the world*/
	Uint32			tileHeight;				/**<How many tiles tall the map is*/
	Uint32			tileWidth;				/**<How many tiles long the map is*/
	Uint32			worldTime;
	GFC_Rect		ground;
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
void world_free(World* world);

/**
* @brief draw the world
* @param world the world to draw
*/
void* world_draw(World* world);


/*
* @brief gets world bounds
*/
GFC_Rect get_world_bounds();

void world_setup_camera(World* world);

Uint32 get_world_time();

#endif

#pragma once