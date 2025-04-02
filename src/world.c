#include "simple_json.h"
#include "simple_logger.h"

#include "camera.h"

#include "world.h"
#include "entity.h"
#include "fire_wizard.h"


typedef struct {
	World* worldData;
	//U///int32 entity_max;
	//Entity* entity_list;
}WorldSystem;

static WorldSystem world_system = { 0 }; /**<Initalize a LOCAL global entity manager*/


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
		slog("No filename ;rovided for world_load");
		return NULL;
	}


	json = sj_load(filename);
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
	if (!vertical)
	w = sj_array_get_count(horizontal);
	
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
			item = sj_array_get_nth(vertical, i);
			if (!item) continue;
			tile = 0;
			sj_get_integer_value(item, &tile);
			world->tileMap[i + (j * w)] = 1;

		}
	}

	background = sj_object_get_value_as_string(wjson, "background");
	world->background = gf2d_sprite_load_image(background);
	tileSet = sj_object_get_value_as_string(wjson, "tileSet");
	sj_object_get_value_as_int(wjson, "frame_w", &frame_w);
	sj_object_get_value_as_int(wjson, "frame_h", &frame_h);
	sj_object_get_value_as_int(wjson, "frames_per_line", &frames_per_line);
	world->tileSet = gf2d_sprite_load_all(
		tileSet,
		frame_w,
		frame_h,
		frames_per_line,
		1);

	//world_tile_layer_build(world);

	sj_free(json);
	return world;

}



World* world_test_new()
{
	//Entity* fire_wizard;
	int i, j;
	int width = 500, height = 500;
	World* world;
	world = world_new(width,height);
	if (!world) return NULL;
	
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
		world->tileMap[i*width + (width-1)] = 1;
	}
	world->bounds = gfc_rect(0, 100, 10000, 500);
	world_system.worldData = world;
	//fire_wizard = fire_wizard_new_entity(gfc_vector2d(700, 200));
	//entity_bounds_update(fire_wizard);
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
	int i, j;
	int index;
	int frame;
	GFC_Vector2D position;
	if (!world) return;
	offset = camera_get_offset();

	gf2d_sprite_draw_image(world->background, offset);//gfc_vector2d(0, 0));
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

GFC_Rect get_world_bounds()
{
	return world_system.worldData->bounds;
}

void world_setup_camera(World* world)
{
	if (!world) return;
	//if(!world->tileLayer)
	camera_set_bounds(gfc_rect(0, 0, 1200, 720));
	camera_bounds_check();
}

Uint32 get_world_time()
{
	return world_system.worldData->worldTime;
}

