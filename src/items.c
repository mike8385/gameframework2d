#include <stdlib.h>  // contains NULL and other basic macros
#include "simple_logger.h"
#include "simple_json.h"
#include "items.h"
#include "gf2d_sprite.h"
#include "gfc_vector.h"
#include "camera.h"

static SJson* _itemJson = NULL;
static SJson* _itemsDefs = NULL;

#define MAX_ITEMS 128
static Item* placedItems[MAX_ITEMS] = { 0 };


void items_close();

void items_initalize(const char* filename)
{
	if (!filename)
	{
		slog("No filename provided for item initalization");
		return;
	}
	_itemJson = sj_load(filename);
	if (!_itemJson)
	{
		slog("Failed to load the json for item definition");
	}
	_itemsDefs = sj_object_get_value(_itemJson, "items");
	if (!_itemsDefs)
	{
		slog("Item definition file %s does not contain items list", filename);
		sj_free(_itemJson);
		_itemJson = NULL;
		return;
	}
	atexit(items_close);
}

void items_close()
{
	if (_itemJson)
	{
		sj_free(_itemJson);
		_itemJson = NULL;
		_itemsDefs = NULL;
	}
}

SJson* items_get_def_by_name(const char* name)
{
	int i, c;
	SJson* item;
	const char* itemName = NULL;
	if (!name) return NULL;
	if (!_itemsDefs)
	{
		slog("No item definitions loaded");
		return NULL;
	}

	c = sj_array_get_count(_itemsDefs);
	for (i=0;i<c;i++)
	{
		item = sj_array_get_nth(_itemsDefs, i);
		if (!item)continue;
		itemName = sj_object_get_value_as_string(item, "name");
		if (!itemName)continue;
		if (gfc_strlcmp(name, itemName) == 0)
		{
			//Found it
			return item;
		}
	}
	slog("No item found by name %s", name);
	return NULL;
}

Item* item_new(const char* name)
{
	Item* item;
	SJson* itemDef;
	SJson* spriteObj;
	const char* spriteFile = NULL;

	itemDef = items_get_def_by_name(name);
	if (!itemDef)
	{
		slog("Item definition not found: %s", name);
		return NULL;
	}

	item = gfc_allocate_array(sizeof(Item), 1);
	if (!item) return NULL;
	gfc_line_cpy(item->name, name);
	sj_object_get_value_as_int(itemDef, "price", &item->price);

	spriteObj = sj_object_get_value(itemDef, "sprite");
	if (spriteObj)
	{
		item->filename = sj_object_get_value_as_string(spriteObj, "filename");
		sj_object_get_value_as_int(spriteObj, "frame", &item->frame);

		if (item->filename)
		{
			item->sprite = gf2d_sprite_load_all(item->filename, 64, 64, 1, 0);
		}
		else
		{
			slog("Sprite filename missing for item: %s", name);
		}
	}
	else
	{
		slog("No sprite object found for item: %s", name);
	}

	item->count = 1;
	return item;
}


void item_free(Item* item)
{
	if (!item)return;
	gf2d_sprite_free(item->sprite);
	free(item);
}



void items_place(Item* self, GFC_Vector2D position)
{
	if (!self) return;

	self->position = position;

	// Add to global list
	for (int i = 0; i < MAX_ITEMS; i++)
	{
		if (!placedItems[i])
		{
			placedItems[i] = self;
			break;
		}
	}
}

void items_draw_all()
{
	for (int i = 0; i < MAX_ITEMS; i++)
	{
		if (placedItems[i])
		{
			item_draw(placedItems[i]);
		}
	}
}


void item_draw(Item* self)
{
	if (!self || !self->sprite) return;
	GFC_Vector2D offset, position;
	offset = camera_get_offset();

	gfc_vector2d_add(position, self->position, offset);
	gf2d_sprite_draw(
		self->sprite,
		position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(int)self->frame);

	//slog("Item->position: %f, %f", self->position.x, self->position.y);
}


void items_clear_all()
{
	for (int i = 0; i < MAX_ITEMS; i++)
	{
		if (placedItems[i])
		{
			item_free(placedItems[i]);  // Free the item memory
			placedItems[i] = NULL;      // Clear the slot
		}
	}
}

//size_t item_get_max()
//{
//	return MAX_ITEMS;
//}
