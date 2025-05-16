#ifndef __ITEMS_H__
#define __ITEMS_H__

#include "simple_json.h"

#include "gf2d_sprite.h"
#include "gfc_text.h"
#include "gfc_shape.h"
#include "entity.h"




typedef enum
{
	item_default,
	potion,
	food,
	coins,
	MAX

}ItemTypes;

ItemTypes string_to_item_type(const char* typeStr);


typedef struct item_S
{
	GFC_TextLine	name;
	const char*		filename;
	Sprite*			sprite;
	int				count;
	int				price;
	GFC_Vector2D	position;
	int				frame;
	ItemTypes		type;
	GFC_Rect		bounds;
}Item;




/**
* @brief initialize and load item definitions
* @param filename the json file containing item data
*/
void items_initalize(const char* filename);

/**
* @Brief get an item definition by its name
* @params name the search criteria
* @return NULL if not found, otherwise the definition information of the item
* @note DO NOT FREE THAT DATA
*/
SJson* items_get_def_by_name(const char* name);

Item* item_new(const char* name);

void item_free(Item* item);

void items_place(Item* self, GFC_Vector2D position);

void item_draw(Item* self);

void items_draw_all();

void items_clear_all();

void item_collision(Item* self);

Uint8 item_collision_check(Item* item, Entity* entity);

void items_collide_all();

void item_pickup(Item* item, Entity* player);

#endif
