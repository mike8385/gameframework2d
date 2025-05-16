#include "simple_json.h"
#include "simple_logger.h"
#include "gf2d_graphics.h"

#include "camera.h"

#include "world.h"
#include "entity.h"
#include "fire_wizard.h"
#include "items.h"
#include "pets.h"


size_t maxItems = (size_t)MAX_ITEMS;


typedef struct {
	World* worldData;
	World* nextWorld;
	char nextWorldFile[256];
	Uint8 transitionRequested;  // Add this flag
} WorldSystem;


static WorldSystem world_system = { 0 }; /**<Initalize a LOCAL global entity manager*/

Sprite* world_load_tilesets(const char* filename, Sint32 frameWidth, Sint32 frameHeight, Sint32 framesPerLine, Bool keepSurface, World* self);

World* world_load(const char* filename)
{
	World* world = NULL;
	SJson* json = NULL;
	SJson* wjson = NULL;
	SJson* vertical, * horizontal;
	SJson* item;
	SJson* nextWorldFile;
	int tile;
	int w = 0, h = 0;
	int i, j;
	const char* tileSet;
	const char* itemList;
	const char* background;
	int frame_w, frame_h;
	int frames_per_line;
	const char* nextWorld;


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
	slog("Background: %s", world->background->filepath);

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
	//slog("frame_w: %d, frame_h: %d", frame_w, frame_h);
	//slog("w: %d, h: %d", w, h);
	world->bounds = gfc_rect(0, 500, w * frame_w, 450/*h * frame_h*/);
	//world->tileSet->frame_h = frame_h;


	//slog("After setting, world bounds: %f, %f, %f, %f", world->bounds.x, world->bounds.y, world->bounds.h, world->bounds.w);


	//Works
	world_system.worldData = world;
	world_tile_layer_build(world);
	slog("Here");
	//Pet spawn:
	SJson* petsArray = sj_object_get_value(json, "pets");
	if (petsArray)
	{



		if (petsArray->sjtype != SJVT_Array)
		{
			slog("Pets is not an array!");
			return world;
		}

		int petsCount = sj_array_get_count(petsArray);
		SJson* petsData = NULL;
		for (i = 0; i < petsCount; i++)
		{
			petsData = sj_array_get_nth(petsArray, i);
			if (!petsData) continue;

			const char* petsName = sj_object_get_value_as_string(petsData, "name");
			SJson* positionArray = sj_object_get_value(petsData, "position");
			if (!petsName || !positionArray)
			{
				slog("Pet missing name or position");
				continue;
			}

			GFC_Vector2D position = { 0 };
			sj_get_float_value(sj_array_get_nth(positionArray, 0), &position.x);
			sj_get_float_value(sj_array_get_nth(positionArray, 1), &position.y);
			//slog("Loaded pets %d: position=(%f, %f)", i, position.x, position.y);  // Check if values are correct


			Entity* newPet = pets_new(petsName);
			if (!newPet)
			{
				slog("Failed to create pets: %s", petsName);
				continue;
			}

			newPet->position = position;

			//slog("Name: %s File: %s", newItem->name, newItem->filename);

			//GFC_Vector2D pixel_position;
			//pixel_position.x = position.x * world->tileSize.x;
			//pixel_position.y = position.y * world->tileSize.y;

			gfc_list_append(world->petsList, newPet);
			pets_new_entity_placed(newPet, position);
		}
	}
	else
	{
	slog("No pets array found");
	}

	//Enemy Spawn:


	//Item stuff now:
	SJson* itemsArray = sj_object_get_value(json, "items");
	if (!itemsArray)
	{
		slog("No items array found");
		return world;
	}

	if (itemsArray->sjtype != SJVT_Array)
	{
		slog("Items is not an array!");
		return world;
	}

	int itemCount = sj_array_get_count(itemsArray);
	SJson* itemData = NULL;
	for (i = 0; i < itemCount; i++)
	{
		itemData = sj_array_get_nth(itemsArray, i);
		if (!itemData) continue;

		const char* itemName = sj_object_get_value_as_string(itemData, "name");
		SJson* positionArray = sj_object_get_value(itemData, "position");
		if (!itemName || !positionArray)
		{
			slog("Item missing name or position");
			continue;
		}

		GFC_Vector2D position = { 0 };
		sj_get_float_value(sj_array_get_nth(positionArray, 0), &position.x);
		sj_get_float_value(sj_array_get_nth(positionArray, 1), &position.y);
		//slog("Loaded item %d: position=(%f, %f)", i, position.x, position.y);  // Check if values are correct


		Item* newItem = item_new(itemName);
		if (!newItem)
		{
			slog("Failed to create item: %s", itemName);
			continue;
		}

		newItem->position = position;
		//slog("Name: %s File: %s", newItem->name, newItem->filename);

		//GFC_Vector2D pixel_position;
		//pixel_position.x = position.x * world->tileSize.x;
		//pixel_position.y = position.y * world->tileSize.y;

		gfc_list_append(world->itemList, newItem);
		items_place(newItem, position);
	}

	nextWorld = sj_object_get_value_as_string(wjson, "nextWorld"); // wjson is the "world" object
	if (nextWorld)
	{
		strcpy(world_system.nextWorldFile, nextWorld);
		slog("Next world set to: %s", world_system.nextWorldFile);


	}
	else
	{

		slog("No next world");
		strcpy(world_system.nextWorldFile, "None");
		sj_free(json);
		return world;
	}
		
		


	sj_free(json);
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

	self->tileSize.x = frameWidth;
	self->tileSize.y = frameHeight;
	//slog("Frame Width: %d, Frame Height: %d", frameWidth, frameHeight);

}



//World* world_test_new()
//{
//	//Entity* fire_wizard;
//	
//	int i, j;
//	int width = 500, height = 500;
//	World* world;
//	world = world_new(width,height);
//	if (!world) return NULL;
//	slog("Here");
//	world->background = gf2d_sprite_load_image("images/backgrounds/battlegrounds/PNG/Battleground2/Pale/Battleground2.png");
//	world->tileSet = gf2d_sprite_load_all(
//		"images/backgrounds/tileset1.png",
//		64,
//		64,
//		1,
//		1);
//
//
//	for (i = 0; i < width; i++)
//	{
//		world->tileMap[i] = 1;
//		world->tileMap[i+(height-1)*width] = 1;
//	}
//	for (i = 0; i < height; i++)
//	{
//		world->tileMap[i*width] = 1;
//		world->tileMap[i*width + (height-1)] = 1;
//	}
//	slog("Here");
//	world->bounds = gfc_rect(0, 100, 10000, 500);
//	world_system.worldData = world;
//	slog("Here");
//	//world_tile_layer_build(world);
//	slog("Here");
//	return world;
//}

/**
* @brief allocate a new empty world
* @return NULL on error, or a blank world
*/
World* world_new(Uint32 width, Uint32 height)
{
	World* world;
	
	if ((!width) || (!height))
	{
		slog("Cannot make a world with zero width and height");
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
	world->itemList = gfc_list_new();
	world->petsList = gfc_list_new();
	world->monsterList = gfc_list_new();



	world->worldTime = SDL_GetTicks();
	//slog("World created with width %i, height %i", width, height);
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
	
	UI_health_bar(gfc_rect(15, 15, 300, 75));

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

	if (!world) return;

	if (!world->tileSet) return;

	if (world->tileLayer)
	{
		gf2d_sprite_free(world->tileLayer);
	}
	world->tileLayer = gf2d_sprite_new();
	
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
	//slog("Here");

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
	//slog("Here");
	world->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), world->tileLayer->surface);
	if (!world->tileLayer->texture)
	{
		slog("Failed to convert world tile layer to texture");
		return;
	}
	//slog("TileSet: % s", world->tileSet->filepath);
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
	//slog("Error");
	world->tileLayer->surface = gf2d_graphics_create_surface(
		world->tileWidth * world->tileSet->frame_w,
		world->tileHeight * world->tileSet->frame_h);
	//slog("Error");
	world->tileLayer->frame_w = world->tileWidth * world->tileSet->frame_w;
	world->tileLayer->frame_h = world->tileHeight * world->tileSet->frame_h;
	//slog("Error");
	if (!world->tileLayer->surface)
	{
		slog("failed to create tileLayer surface");
		return;
	}
	//slog("Error");
	//slog("world->tileHeight: %d", world->tileHeight);


	////slog("tileMap size: %d, tileHeight: %d, tileWidth: %d",
	//	world->tileWidth * world->tileHeight,
	//	world->tileHeight,
	//	world->tileWidth);

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
	SJson* world_json;
	SJson* tileMapArray;
	SJson* rowArray;
	SJson* itemsArray;
	SJson* itemObj;
	Item* item;
	int i, j;

	if ((!filename) || (!world)) return;

	json = sj_object_new();        // The full root object
	world_json = sj_object_new();  // The "world" object
	itemsArray = sj_array_new();   // The "items" array (separate)

	// Save general world info
	sj_object_insert(world_json, "name", sj_new_str(world->name));
	if (world->background)
	{
		sj_object_insert(world_json, "background", sj_new_str(world->background->filepath));
	}
	if (world->tileSet)
	{
		sj_object_insert(world_json, "tileSet", sj_new_str(world->tileSet->filepath));
		sj_object_insert(world_json, "frame_w", sj_new_uint32(world->tileSet->frame_w));
		sj_object_insert(world_json, "frame_h", sj_new_uint32(world->tileSet->frame_h));
		sj_object_insert(world_json, "frames_per_line", sj_new_uint32(world->tileSet->frames_per_line));
	}

	// Save tilemap
	tileMapArray = sj_array_new();
	for (j = 0; j < world->tileHeight; j++)
	{
		rowArray = sj_array_new();
		for (i = 0; i < world->tileWidth; i++)
		{
			int tileValue = world->tileMap[i + (j * world->tileWidth)];
			sj_array_append(rowArray, sj_new_int(tileValue));
		}
		sj_array_append(tileMapArray, rowArray);
	}
	sj_object_insert(world_json, "tileMap", tileMapArray);

	// Save each item
	if (world->itemList)
	{
		for (i = 0; i < gfc_list_count(world->itemList); i++)
		{
			item = gfc_list_get_nth(world->itemList, i);
			if (!item) continue;

			itemObj = sj_object_new();
			sj_object_insert(itemObj, "name", sj_new_str(item->name));

			SJson* positionArray = sj_array_new();
			sj_array_append(positionArray, sj_new_float(item->position.x));
			sj_array_append(positionArray, sj_new_float(item->position.y));
			sj_object_insert(itemObj, "position", positionArray);

			sj_array_append(itemsArray, itemObj);
		}
	}

	//Now attach "world" and "items" separately to the root
	sj_object_insert(json, "world", world_json);
	sj_object_insert(json, "items", itemsArray);

	// Save file
	sj_save(json, filename);
	sj_free(json);
}




void world_clear_tile_layer(World* world)
{
	if (!world) return;
	gf2d_sprite_free(world->tileLayer);
	world->tileLayer = NULL;
}

void world_set_tile(World* world, GFC_Vector2D point, Uint8 tile)
{
	GFC_Vector2D location;
	if ((!world) || (!world->tileMap) || (!world->tileSize.x) || (!world->tileSize.y)) return;
	//From world space to tilespace
	location.x = ((float)point.x / world->tileSize.x);
	location.y = ((float)point.y / world->tileSize.y);
	world->tileMap[world_get_tile_index_by_pos(world, location)] = tile;
	world_clear_tile_layer(world);
	world_tile_layer_build(world);

}


Uint8 world_get_tile_index_by_pos(World* world, GFC_Vector2D position)
{
	if ((!world) || (!world->tileMap)) return 0;
	return (Uint32)position.y * (Uint32)world->tileWidth + (Uint32)position.x;
}

Uint8 get_tile_at(World* world, GFC_Vector2D position)
{
	if ((!world) || (!world->tileMap)) return 0;
	return world->tileMap[(Uint32)position.y * (Uint32)world->tileWidth + (Uint32)position.x];
}


void world_set_item(World* world, GFC_Vector2D point, Item* item)
{
	GFC_Vector2D location;
	GFC_Vector2D pixel_position;

	if ((!world) || (!item))
	{
		slog("Dont have world or item to give");

	}
	//From world space to tilespace
	location.x = ((float)point.x / world->tileSize.x);
	location.y = ((float)point.y / world->tileSize.y);
	item->position = location;
	//Save the item name to list, and the location
	if (gfc_list_get_count(world->itemList) >= maxItems)
	{
		slog("Too many items in world");
		return;
	}
	gfc_list_append(world->itemList, item);
	//world->itemList[item_g(world, location, item)] = tile;
	pixel_position.x = location.x * world->tileSize.x;
	pixel_position.y = location.y * world->tileSize.y;

	//items_place(item, pixel_position);  //This fixes drawing it correctly

	world_clear_tile_layer(world);
	world_tile_layer_build(world);
	items_place(item, pixel_position);
}

Uint8 world_get_item_pos(World* world, GFC_Vector2D position, Item* item)
{
	//if ((!world) || (!world->tileMap)) return 0;
	//return (Uint32)position.y * (Uint32)world->tileWidth + (Uint32)position.x;
}


const char* world_get_next_world()
{
	return world_system.nextWorldFile;
}
//Uint8 get_item_at(World* world, GFC_Vector2D position, Item* item)
//{
//
//}

