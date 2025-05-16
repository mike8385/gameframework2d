#include <stdlib.h>  // contains NULL and other basic macros
#include "simple_logger.h"
#include "simple_json.h"
#include "items.h"
#include "gf2d_sprite.h"
#include "gfc_vector.h"
#include "camera.h"
#include "player.h"
#include "entity.h"

static SJson* _itemJson = NULL;
static SJson* _itemsDefs = NULL;

#define MAX_ITEMS 128
static Item* placedItems[MAX_ITEMS] = { 0 };


ItemTypes string_to_item_type(const char* typeStr) {
	if (!typeStr) return item_default; // Default

	if (strcmp(typeStr, "potion") == 0) return potion;
	if (strcmp(typeStr, "food") == 0) return food;
	if (strcmp(typeStr, "coins") == 0) return coins;


	slog("Unknown collision type: %s", typeStr);
	return item_default; // Default fallback
}


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
	const char* type;

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
	type = sj_object_get_value_as_string(itemDef, "type");
	if (!type)
	{
		slog("%s missing 'type'", name);
		return NULL;
	}
	item->type = string_to_item_type(type);


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



void items_place(Item* item, GFC_Vector2D position)
{
	if (!item) return;

	item->position = position;

	// Add to global list
	for (int i = 0; i < MAX_ITEMS; i++)
	{
		if (!placedItems[i])
		{
			placedItems[i] = item;
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


void item_draw(Item* item)
{
	if (!item || !item->sprite) return;
	GFC_Vector2D offset, position;
	offset = camera_get_offset();

	gfc_vector2d_add(position, item->position, offset);
	gf2d_sprite_draw(
		item->sprite,
		position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(int)item->frame);

	item->bounds = gfc_rect(item->position.x, item->position.y, 64, 64);



	//slog("Item->position: %f, %f", item->position.x, item->position.y);
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

void item_collision(Item* item)
{
	if (!item) return;

	for (int i = 0; i < entity_system_get_max(); i++)
	{
		Entity* other = entity_system_get_entity(i);
		if (!other) continue;
		if (!other->isPlayer) continue;

		Entity* player = other;

		if (item_collision_check(item, player))
		{
			
			//EVERYTHING WITH ITEMS HERE;
			slog("Inside pickup");

			if (item->type == potion) item_pickup(item, player);
			if (item->type == coins) player->inventory.coins += 10;

			// Remove from the world (placedItems array)
			for (int i = 0; i < MAX_ITEMS; i++)
			{
				if (placedItems[i] == item)
				{
					placedItems[i] = NULL;  // Remove reference
					item_free(item);       // Now safely free
					break;
				}
			}
		}



		//}
	}

}


Uint8 item_collision_check(Item* item, Entity* entity)
{
	if (!item) return;
	if (!entity) return;
	if (gfc_rect_overlap(item->bounds, entity->bounds))
	{
		return 1;
	}
	return 0;
}



void items_collide_all()
{
	for (int i = 0; i < MAX_ITEMS; i++)
	{
		if (placedItems[i])
		{
			item_collision(placedItems[i]);

		}
	}
}


void item_pickup(Item* item, Entity* player)
{
	if (!item || !player) return;

	// Add to inventory (creates a new copy)
	inventory_add_item(&player->inventory, item->name);

	
}


//GFC_List* item_collide_all(Item* item)
//{
//
//	if (!item) return;
//	int i;
//	GFC_List* entities;
//	ItemTypes itemType = item->type;
//
//	entities = gfc_list_new();
//	for (i = 0; i < entity_system.entity_max; ++i)
//	{
//		if (!entity_system.entity_list[i]._inuse) continue;	//Skip any active entities
//		if (item == &entity_system.entity_list[i]) continue;	//Skip any active entities
//		if (entity_collision_check(item, &entity_system.entity_list[i]))
//		{
//			gfc_list_append(entities, &entity_system.entity_list[i]);
//		}
//	}
//	if (!gfc_list_count(entities))
//	{
//		gfc_list_delete(entities); return;
//	}
//	return entities;
//}
