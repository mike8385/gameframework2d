#include <SDL.h>
#include <gfc_input.h>
#include "simple_logger.h"
#include "gfc_shape.h"
#include "gf2d_draw.h"

#include "entity.h"
#include "player.h"

Entity* player_new_entity(GFC_Vector2D position)
{
	Entity *self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a player entity");
		return NULL;
	}
	gfc_vector2d_copy(self->position,position);
	self->think = player_think;
	self->update = player_update;
	self->frame = 0;
	self->sprite = gf2d_sprite_load_all(
		"images/players/wizardSprites/PNG/wizard/wizard_Idle3.png",
		128,
		128,
		4,
		0);
	//Set ground for player (Center)
	
	//self->bounds = gfc_rect(self->position.x, self->position.y, 128, 128);

	//gf2d_draw_rect(self->bounds, GFC_COLOR_RED);
	return self;
}

void player_move(Entity *self)
{
	if (!self) return; 
	
	gfc_vector2d_add(self->position, self->velocity, self->position);
	//slog("Position is: %f", self->position);


}

void player_think(Entity* self)
{
	if (!self) return;
	//GFC_Vector2D velocity;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_D])
	{
		self->velocity = gfc_vector2d(1.0f,0.0f);
		//slog("Clicked D");
	}
	else if (keys[SDL_SCANCODE_A])
	{
		self->velocity = gfc_vector2d(-1.0f, 0.0f);
		//slog("Clicked D");
	}
	else if (keys[SDL_SCANCODE_W])
	{
		self->velocity = gfc_vector2d(0.0f, -1.0f);
		//slog("Clicked D");
	}
	else if (keys[SDL_SCANCODE_S])
	{
		self->velocity = gfc_vector2d(0.0f, 1.0f);
		//slog("Clicked D");
	}
	else
	{
		self->velocity = gfc_vector2d(0.0f, 0.0f);
	}



	
}

void player_update(Entity* self)
{
	if (!self) return;
	player_move(self);
	self->frame += 0.05f;
	//slog("Frame is: %f", self->frame);
	if (self->frame >= 4) self->frame = 0;
	self->ground = gfc_vector2d(self->position.x + (128 / 2), self->position.y + 128);
	self->bounds = gfc_rect(self->position.x, self->position.y, 128, 128);
}
