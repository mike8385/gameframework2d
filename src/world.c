#include "simple_json.h"
#include "simple_logger.h"
#include "gf2d_graphics.h"

#include "camera.h"

#include "world.h"
#include "entity.h"
#include "fire_wizard.h"
#include "items.h"


typedef struct {
	World* worldData;
	//U///int32 entity_max;
	//Entity* entity_list;
}WorldSystem;

static WorldSystem world_system = { 0 }; /**<Initalize a LOCAL global entity manager*/

Sprite* world_load_tilesets(const char* filename, Sint32 frameWidth, Sint32 frameHeight, Sint32 framesPerLine, Bool keepSurface, World* self);

World* world_load(const char* filename)
{
	World* world = NULL;
	SJson* json = NULL;
	SJson* wjson = NULL;
	SJson* vertical, * horizontal;
	SJson* item;
	int tile;
	int w = 0, h = 0;
	int i, j;
	const char* tileSet;
	const char* background;
	int frame_w, frame_h;
	int frames_per_line;


	if (!filename)
	{
		slog("No filename provided for world_load");
		return NULL;
	}

//Breaks here
	json = sj_load(filename);
	//Broke

	if (!json)
	{
		slog("Failed to load world file %s", filename);
		return NULL;
	}


	wjson = sj_object_get_value(json, "world");
	if (!wjson)
	{
		slog("%s missing 'world' object", filename);
		sj_free(json);
		return NULL;
	}

	vertical = sj_object_get_value(wjson, "tileMap");
	if (!vertical)
	{
		slog("%s missing 'tileMap' object", filename);
		sj_free(json);
		return NULL;
	}

	h = sj_array_get_count(vertical);
	horizontal = sj_array_get_nth(vertical, 0);
	//if (!horizontal)
	//{
	//			slog("%s missing 'tileMap' object", filename);
	//	sj_free(json);
	//	return NULL;
	//}
	w = sj_array_get_count(horizontal);	
	if (!w || !h)
	{
		slog("ERROR: Cannot find w or h");
		return NULL;
	}
	world = world_new(w, h);
	if (!world)
	{
		slog("Failed to create space for a new world for file %s", filename);
		sj_free(json);
		return NULL;
	}

	for (j = 0; j < h; j++)
	{
		horizontal = sj_array_get_nth(vertical, j);
		if (!horizontal) continue; //This might be worth erroring over but for now continue
		for (i = 0; i < w; i++)
		{
			item = sj_array_get_nth(horizontal, i);
			if (!item) continue;
			tile = 0;
			sj_get_integer_value(item, &tile);
			world->tileMap[i + (j * w)] = tile;

		}
	}

	background = sj_object_get_value_as_string(wjson, "background");
	if (!background)
	{
		slog("No background found");
		return NULL;
	}
	world->background = gf2d_sprite_load_image(background);

	tileSet = sj_object_get_value_as_string(wjson, "tileSet");
	if (!tileSet)
	{
		slog("Cannot find tileSet");
		return NULL;
	}
	sj_object_get_value_as_int(wjson, "frame_w", &frame_w);
	sj_object_get_value_as_int(wjson, "frame_h", &frame_h);
	sj_object_get_value_as_int(wjson, "frames_per_line", &frames_per_line);
	world_load_tilesets(tileSet, frame_w, frame_h, frames_per_line, 1, world);
	slog("frame_w: %d, frame_h: %d", frame_w, frame_h);
	slog("w: %d, h: %d", w, h);
	world->bounds = gfc_rect(0, 0, w * frame_w, h * frame_h);

	slog("After setting, world bounds: %f, %f, %f, %f", world->bounds.x, world->bounds.y, world->bounds.h, world->bounds.w);


	//Works
	world_system.worldData = world;
	world_tile_layer_build(world);
	//item = item_new("waffle");
	//if (!item)
	//{
	//	slog("No item found");
	//	return;
	//}
	//slog("%s", item);
	sj_free(json);

	slog("After setting, world bounds: %f, %f", world_system.worldData->bounds.h, world_system.worldData->bounds.w);

	return world;

}

Sprite* world_load_tilesets(const char* filename, Sint32 frameWidth, Sint32 frameHeight, Sint32 framesPerLine, Bool keepSurface, World* self)

{
	if (!filename) return;
	if (!self) return;
	self->tileSet = gf2d_sprite_load_all(
		filename,
		frameWidth,
		frameHeight,
		framesPerLine,
		1);
}



World* world_test_new()
{
	//Entity* fire_wizard;
	
	int i, j;
	int width = 500, height = 500;
	World* world;
	world = world_new(width,height);
	if (!world) return NULL;
	slog("Here");
	world->background = gf2d_sprite_load_image("images/backgrounds/battlegrounds/PNG/Battleground2/Pale/Battleground2.png");
	world->tileSet = gf2d_sprite_load_all(
		"images/backgrounds/tileset1.png",
		64,
		64,
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
		world->tileMap[i*width + (height-1)] = 1;
	}
	slog("Here");
	world->bounds = gfc_rect(0, 100, 10000, 500);
	world_system.worldData = world;
	slog("Here");
	//world_tile_layer_build(world);
	slog("Here");
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
	world->worldTime = SDL_GetTicks();
	slog("World created with width %i, height %i", width, height);
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
	GFC_Vector2D offset;
	//int i, j;
	//int index;
	//int frame;
	//GFC_Vector2D position;
	if (!world) return;
	offset = camera_get_offset();

	gf2d_sprite_draw_image(world->background, offset);//gfc_vector2d(0, 0));
	if (!world->tileSet)return;//Cant draw with no tiles

	gf2d_sprite_draw_image(world->tileLayer, offset);//gfc_vector2d(0, 0));

	/*for (j = 0; j < world->tileWidth; j++)
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
	}*/
	
	
}


GFC_Rect get_world_bounds()
{
	if (!world_system.worldData)
	{
		slog("World system data is NULL");
		return gfc_rect(0, 0, 1200, 720);  // Return safe default bounds
	}
	//slog("Test: %f, %f", world_system.worldData->bounds.h, world_system.worldData->bounds.h);
	return world_system.worldData->bounds;
}

void world_setup_camera(World* world)
{
	if (!world) return;
	if ((!world->tileLayer) || (!world->tileLayer->surface))
	{
		slog("No tile layer set for world");
		return;
	}
	camera_set_bounds(gfc_rect(0, 0, world->tileLayer->surface->w,world->tileLayer->surface->h));
	camera_bounds_check();
}

Uint32 get_world_time()
{
	return world_system.worldData->worldTime;
}


void world_tile_layer_build(World* world)
{

	int i, j;
	Uint32 index;
	Uint32 frame;
	GFC_Vector2D position;

	slog("Here");
	if (!world) return;
	slog("Here");
	if (!world->tileSet) return;
	slog("Here");
	if (world->tileLayer)
	{
		gf2d_sprite_free(world->tileLayer);
	}
	world->tileLayer = gf2d_sprite_new();
	slog("Here");
	world->tileLayer->surface = gf2d_graphics_create_surface(
		world->tileWidth * world->tileSet->frame_w, 
		world->tileHeight * world->tileSet->frame_h);
	

	world->tileLayer->frame_w = world->tileWidth * world->tileSet->frame_w;
	world->tileLayer->frame_h = world->tileHeight * world->tileSet->frame_h;

	if (!world->tileLayer->surface)
	{
		slog("Failed to create tileLayer Surface");
		return;
	}
	slog("Here");

	for (j = 0; j < world->tileHeight; j++)
	{
		for (i = 0; i < world->tileWidth; i++)
		{

			index = i + (j * world->tileWidth);
			if (world->tileMap[index] == 0) continue;

			position.x = i * world->tileSet->frame_w;
			position.y = j * world->tileSet->frame_h;
			frame = world->tileMap[index] - 1;

			gf2d_sprite_draw_to_surface(
				world->tileSet,
				position,
				NULL,
				NULL,
				frame,
				world->tileLayer->surface);
		}
	}
	slog("Here");
	world->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), world->tileLayer->surface);
	if (!world->tileLayer->texture)
	{
		slog("Failed to convert world tile layer to texture");
		return;
	}
	slog("TileSet: % s", world->tileSet->filepath);
}


void world_tile_layer(World* world)
{
	int i, j;
	Uint32 index;
	Uint32 frame;
	GFC_Vector2D position;
	if (!world)
	{
		slog("No world found for tilestuff");
		return;
	}
	
	if (!world->tileSet) return;
	
	if (!world->tileMap) {
		slog("ERROR: world->tileMap is NULL before rendering tiles.");
		return;
	}
	slog("Error");
	if (world->tileLayer)
	{
		gf2d_sprite_free(world->tileLayer);
	}
	world->tileLayer = gf2d_sprite_new();
	if (!world->tileLayer)
	{
		slog("Cannot  make new tileLayer");
		return;
	}
	slog("Error");
	world->tileLayer->surface = gf2d_graphics_create_surface(
		world->tileWidth * world->tileSet->frame_w,
		world->tileHeight * world->tileSet->frame_h);
	slog("Error");
	world->tileLayer->frame_w = world->tileWidth * world->tileSet->frame_w;
	world->tileLayer->frame_h = world->tileHeight * world->tileSet->frame_h;
	slog("Error");
	if (!world->tileLayer->surface)
	{
		slog("failed to create tileLayer surface");
		return;
	}
	slog("Error");
	slog("world->tileHeight: %d", world->tileHeight);


	slog("tileMap size: %d, tileHeight: %d, tileWidth: %d",
		world->tileWidth * world->tileHeight,
		world->tileHeight,
		world->tileWidth);

	for (j = 0; j < world->tileHeight; j++)
	{
		for (i = 0; i < world->tileWidth; i++)
		{
			index = i + (j * world->tileWidth);
			if (world->tileMap[index] == 0) continue;
			position.x = i * world->tileSet->frame_w;
			position.y = j * world->tileSet->frame_h;
			frame = world->tileMap[index] - 1;
			gf2d_sprite_draw_to_surface(
				world->tileSet,
				position,
				NULL,
				NULL,
				frame,
				world->tileLayer->surface);
		}
	}
	world->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), world->tileLayer->surface);
	if (!world->tileLayer->texture)
	{
		slog("failed to convert world tile layer to texture");
		return;
	}

	
	slog("End");
}

void world_save(World* world, const char* filename)
{
	SJson* json;
	if ((!filename) || (!world)) return;
	json = sj_object_new();
	if (!json)
	{
		slog("Failed to create new JSon");
		return;
	}
	sj_object_insert(json, "name", sj_new_str(world->name));
	if (world->background)
	{
		sj_object_insert(json, "background", sj_new_str(world->background->filepath));
		//sj_object_insert(json, "backgroundFileFrameW", sj_new_Uint32(world->background->frame_w));
		//sj_object_insert(json, "backgroundFileFrameH", sj_new_Uint32(world->background->frame_h));
	}
	/*
	Add the rest (has to be same name
	*/




	sj_save(json, filename);
	sj_free(json);
}



void world_clear_tileset(World* world)
{
	if (!world) return;
	gf2d_sprite_free(world->tileLayer);
	world->tileLayer = NULL;
}

//void world_set_tile(World* world, GFC_Vector2D point, Uint8 tile)
//{
//	GFC_Vector2D location;
//	if ((!world) || (!world->tileMap) || (!world->tileSize.x) || (!world->tileSize.y)) return;
//	//From world space to tilespace
//	location.x = ((float)point.x / world->tileSize.x);
//	location.y = ((float)point.y / world->tileSize.y);
//	world_get_tile_at(world, location);
//
//
//}

//Uint8 world_get_tile_index_by_pos(World* world, GFC_Vector2D position)
//{
//	if ((!world) || (!world->tileMap)) return 0;
//	return 
//}
//{
//	if not world or tileMap
//		return tile
//}
//
//get_tile_at