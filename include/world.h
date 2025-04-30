#ifndef __WORLD_H__
#define __WORLD_H_

#include "gf2d_sprite.h"
#include "gfc_shape.h"
#include "gf2d_draw.h"
#include "gfc_text.h"

typedef struct
{
	GFC_TextLine name;
	Sprite*			background;				/**<Background image for the world*/
	Sprite*			tileLayer;
	GFC_Rect		bounds;					/**<Bounds for the map*/
	Sprite*			tileSet;				/**Sprite containing tiles for the world<*/
	Uint8*			tileMap;				/**<The tiles that make up the world*/
	Uint32			tileHeight;				/**<How many tiles tall the map is*/
	Uint32			tileWidth;				/**<How many tiles long the map is*/
	Uint32			worldTime;
	GFC_Vector2D	tileMapSize;
	GFC_Vector2D	tileSize;

	GFC_List		monsterList;
	GFC_List*		itemList;

	GFC_Rect		ground;
	
}World;


/**
* @Brief load a new world from a config file
* @Param filename the name of the world file to load
* @return NULL on error, or a usable world otherwise
*/
World* world_load(const char* filename);

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

void world_tile_layer(World* world);

void world_tile_layer_build(World* world);

/*
* @brief save a world file
*/
void world_save(World* world, const char* filename);

void world_clear_tile_layer(World* world);

void world_set_tile(World* world, GFC_Vector2D point, Uint8 tile);

Uint8 world_get_tile_index_by_pos(World* world, GFC_Vector2D position);

Uint8 get_tile_at(World* world, GFC_Vector2D position);

#endif

#pragma once