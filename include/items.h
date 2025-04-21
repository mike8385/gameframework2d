#ifndef __ITEMS_H__
#define __ITEMS_H__

#include "simple_json.h"

#include "gf2d_sprite.h"
#include "gfc_text.h"

typedef struct
{
	GFC_TextLine	name;
	Sprite*			sprite;
	int				count;
	int				price;
}Item;

/**
* @Brief initialize and load item definitions
* @params filename the json file containing item data
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


#endif
