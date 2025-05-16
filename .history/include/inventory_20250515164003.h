#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "gfc_list.h"

// Forward declaration to break circular dependency
typedef struct Item_s Item;

#define MAX_ITEMS 128

typedef enum
{
	II_HealPotion,
	II_MAX
}ItemIndices;

typedef struct
{
	//int				itemslist[II_MAX];			// Count of that item
	int				weight, weightLimit;
	GFC_List*		itemsList;
	Uint32			coins;
	Item*			selectedItem;
	int				itemIndex;
}Inventory;

void inventory_init(Inventory* inventory);

void inventory_cleanup(Inventory* inventory);

/*
* 
*/
void inventory_add_item(Inventory* inventory, const char* name);

#endif

#pragma once
