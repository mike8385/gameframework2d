#include "simple_json.h"
#include "simple_logger.h"


#include "world.h"


World* world_test_new()
{
	int i, j;
	int width = 65, height = 45;
	World* world;
	world = world_new(width,height);
	if (!world) return NULL;
	
	world->background = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
	world->tileSet = gf2d_sprite_load_all(
		"images/backgrounds/tileset1.png",
		16,
		16,
		1,
		1);
	for (i = 0; i < width; i++)
	{
		world->tileMap[i] = 1;
		world->tileMap[i+(height-1)*width] = 1;
	}
	for (i = 0; i < height; i++)
	{
		world->tileMap[i*width] = 1;
		world->tileMap[i*width + (width-1)] = 1;
	}
	return world;
}

/**
* @brief allocate a new empty world
* @return NULL on error, or a blank world
*/
World* world_new(Uint32 width, Uint32 height)
{
	World* world;

	if ((!width) || (!height))
	{
		slog("Cannot make a world with zerp width and height");
		return NULL;
	}

	world = gfc_allocate_array(sizeof(World), 1);	
	if (!world)
	{
		slog("Failed to allocate new world");
		return NULL;
	}
	//all boilerplate code wouold go here
	//all defaults
	world->tileMap = gfc_allocate_array(sizeof(Uint8), height * width);
	world->tileHeight = height;
	world->tileWidth = width;
	world->bounds = gfc_rect(0,0,width,height);


	return world;

}


/**
* @brief free a previously allocated world
* @param world the world to free
*/
void world_free(World* world)
{
	if (!world)return;
	gf2d_sprite_free(world->background);
	gf2d_sprite_free(world->tileSet);
	free(world->tileMap);
	free(world);
}	

/**
* @brief draw the world
* @param world the world to draw
*/
void* world_draw(World* world)
{
	//slog("World Draw");

	int i, j;
	int index;
	int frame;
	GFC_Vector2D position;
	if (!world) return;

	gf2d_sprite_draw_image(world->background, gfc_vector2d(0, 0));
	if (!world->tileSet)return;//Cant draw with no tiles
	for (j = 0; j < world->tileWidth; j++)
	{
		for (i = 0; i < world->tileWidth; i++)
		{
			index = i + (j * world->tileWidth);
			if (world->tileMap[index] == 0)continue;
			position.x = i * world->tileSet->frame_w;
			position.y = j * world->tileSet->frame_h;
			frame = world->tileMap[index] - 1;
				gf2d_sprite_draw(
				world->tileSet,
				position,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				frame);
		}
	}
	
}

