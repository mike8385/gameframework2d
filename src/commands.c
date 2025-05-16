
#include "simple_logger.h"

#include "gfc_vector.h"

#include "SDL_scancode.h"
#include "commands.h"
#include "player.h"
#include "items.h"
#include "inventory.h"



void cmds()
{
	player = player_get_player();
	const Uint8* cmd_keys;
	cmd_keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

	if (cmd_keys[SDL_SCANCODE_7])
	{
		if (!player) return;
		cmd_get_player_location(player);
	}

	if (cmd_keys[SDL_SCANCODE_8])
	{
		if (!player) return;
		cmd_get_player_inven(player);
	}
}


void cmd_get_player_location(Entity* player)
{
	slog("Player position: [%f, %f]  X: %f Y: %f", player->position.x, player->position.y, player->position.x, player->position.y);
}


void cmd_get_player_inven(Entity* player)
{
	if (!player) return;

	Item* item;
	int count = gfc_list_get_count(player->inventory.itemsList);

	slog("Player Inventory (%d items):", count);

	for (int i = 0; i < gfc_list_count(player->inventory.itemsList); i++)
	{
		item = gfc_list_get_nth(player->inventory.itemsList, i);
		if (item)
		{
			slog("%d: %s (x%d)", i, item->name, item->count);
		}
	}

	slog("Coins: %d", player->inventory.coins);

}