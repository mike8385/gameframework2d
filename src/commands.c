
#include "simple_logger.h"

#include "gfc_vector.h"

#include "SDL_scancode.h"
#include "commands.h"
#include "player.h"




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
}


void cmd_get_player_location(Entity* player)
{
	slog("Player position: [%f, %f]  X: %f Y: %f", player->position.x, player->position.y, player->position.x, player->position.y);
}