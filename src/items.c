#include <stdlib.h>  // contains NULL and other basic macros
#include "simple_logger.h"
#include "simple_json.h"
#include "items.h"

static SJson* _itemJson = NULL;
static SJson* _itemsDefs = NULL;

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
	SJson* itemDef;
	Item* item;
	itemDef = items_get_def_by_name(name);
	if (!itemDef) return NULL;
	item = gfc_allocate_array(sizeof(item), 1);
	if (!item)return NULL;
	gfc_line_cpy(item->name, name);
	sj_object_get_value_as_int(itemDef, "price", &item->price);
	//Get otehr relevant values (default values)
	//Load sprite here
	item->count = 1;
	return item;
}

void item_free(Item* item)
{
	if (!item)return;
	gf2d_sprite_free(item->sprite);
	free(item);
}

