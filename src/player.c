#include <SDL.h>
#include <gfc_input.h>
#include "simple_logger.h"
#include "gfc_shape.h"
#include "gf2d_draw.h"

#include "entity.h"
#include "player.h"
#include "magic.h"
#include "melee.h"


typedef struct {
	Entity* playerData;

}PlayerSystem;

static PlayerSystem player_system = { 0 }; /**<Initalize a LOCAL global entity manager*/

Entity* player_new_entity(GFC_Vector2D position)
{
	//gfc_input_init("config/my_input.cfg")
	Entity *self;
	//Pointer in data
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
	self->free;
	self->sprite = gf2d_sprite_load_all(
		"images/players/wizardSprites/PNG/wizard/wizard_Idle3.png",
		128,
		128,
		4,
		0);
	self->magicCooldown = 0;
	self->meleeCooldown = 0;
	self->lastAttackTime = 0;
	self->lastAttackTimeMelee = 0;
	self->lastJumpTime = 0;
	self->jumpCooldown = 0;
	self->collidedType = ETC_entity;
	self->health = 100;

	self->position = position;
	self->damage = 10;

	player_system.playerData = self;


	return self;
}

void player_move(Entity *self)
{
	if (!self) return; 
	
	//gfc_vector2d_add(self->position, self->velocity, self->position);


}

void player_think(Entity* self)
{

	if (!self) return;
	//GFC_Vector2D velocity;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_D])		//gfc_input_command_down("right")
	{
		self->velocity.x = 3;
		//self->sprite =gf2d_sprite_load_all(
		//	"images/players/wizardSprites/PNG/wizard/wizard_run.png",
		//	128,
		//	128,
		//	4,
		//	0);
		//self->sprite = gf2d_sprite_load_image("images/players/wizardSprites/PNG/wizard/wizard_run.png");
		//self->sprite->frame_w = 128;
		//self->sprite->frame_h = 128;

		//slog("Clicked D");

	}
	else if (keys[SDL_SCANCODE_A])
	{
		self->velocity.x = -3;
	}
	else
	{
		self->velocity.x = 0;
	}
	if (keys[SDL_SCANCODE_W])
	{
		self->velocity.y = -3;
		//slog("Clicked D");
	}
	else if (keys[SDL_SCANCODE_S])
	{
		self->velocity.y = 3;
		//slog("Clicked D");
	}
	else
	{
		self->velocity.y = 0;
		//self->sprite = gf2d_sprite_load_all(
		//	"images/players/wizardSprites/PNG/wizard/wizard_run.png",
		//	128,
		//	128,
		//	4,
		//	0);
		//self->sprite = gf2d_sprite_load_image("images/players/wizardSprites/PNG/wizard/wizard_run.png");
		//self->sprite->frame_w = 128;
		//self->sprite->frame_h = 128;


	}


	if (keys[SDL_SCANCODE_SPACE])
	{
		Uint8 currentTime = SDL_GetTicks();

		if (currentTime - self->lastJumpTime >= 1000)
		{ // 3000 ms = 3 seconds
			self->jumpCooldown = 0;

		}


		if (self->jumpCooldown == 0)
		{
			slog("test");
			self->velocity.y = -3;
			self->acceleration.y = 4;
			self->lastJumpTime = 3;
		}
		else
		{
			self->velocity.y = -3;
		}
	}

}

void player_update(Entity* self)
{
	if (!self) return;
	self->frame += 0.05f;
	//slog("Frame is: %f", self->frame);
	if (self->frame >= 4) self->frame = 0;
	self->ground = gfc_vector2d(self->position.x + (128 / 2), self->position.y + 128);
	self->bounds = gfc_rect(self->position.x, self->position.y, 128, 128);
	player_attack(self);

}

void player_attack(Entity* self)
{
	if (!self) return;

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	Uint32 currentTime = SDL_GetTicks(); // Get current time in milliseconds
	Uint32 currentTime2 = SDL_GetTicks(); // Get current time in milliseconds

	if (currentTime - self->lastAttackTime >= 1000) { // 3000 ms = 3 seconds
		self->magicCooldown = 0;
	}

	if (currentTime2 - self->lastAttackTimeMelee >= 1000) { // 3000 ms = 3 seconds
		self->meleeCooldown = 0;
	}

	if (keys[SDL_SCANCODE_Q] && (self->magicCooldown == 0))
	{

		Entity* spell = spell_new_entity(gfc_vector2d(self->position.x + 2, self->position.y));

		//spell->acceleration = gfc_vector2d(0.1f, 0.f);
		//spell->position = gfc_vector2d(self->position.x + 3, self->position.y);


		spell_move(spell);
		self->magicCooldown = 3;
		self->lastAttackTime = currentTime;
	}

	if (keys[SDL_SCANCODE_X] && (self->meleeCooldown == 0))
	{


		Entity* melee = melee_new_entity(gfc_vector2d(self->position.x + self->bounds.w + 3, self->position.y));
		//spell->position = gfc_vector2d(spell->position.x + 3, self->position.y);

		//spell->acceleration = gfc_vector2d(0.1f, 0.f);
		//spell->position = gfc_vector2d(self->position.x + 3, self->position.y);


		melee_move(melee, self);
		self->meleeCooldown = 3;
		self->lastAttackTimeMelee = currentTime2;
	}




}


//Void player free
/*
* make PLayerEntityData
IF !self or !self->data
data = self->data
gf2d_sprite_free(data->profilepicture)
//other cleanup
free(self->data)
free(data)
self->data =NULL
*/

void player_free(Entity* self)
{
	if ((!self) || (!self->data)) return;
	Entity* data = self->data;
	//gf2d_sprite_free(data->)
	gf2d_sprite_free(data->sprite);
	free(self->data);
	free(data);
	self->data = NULL;
	free(self);

	
}

GFC_Rect get_player_bounds()
{
	return player_system.playerData->bounds;
}

GFC_Vector2D get_player_position()
{
	return player_system.playerData->position;
}


