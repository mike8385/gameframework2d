#include <SDL.h>
#include <gfc_input.h>
#include "simple_logger.h"
#include "gfc_shape.h"
#include "gf2d_draw.h"

#include "camera.h"
#include "entity.h"
#include "player.h"
#include "magic.h"
#include "melee.h"

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
	self->burstMagicCooldown = 0;
	self->meleeCooldown = 0;
	self->lastAttackTime = 0;
	self->lastAttackTimeBurst = 0;
	self->lastAttackTimeMelee = 0;
	self->collidedType = ETC_entity;
	self->health = 100;
	self->worldTime = SDL_GetTicks();

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
	}


	//if (keys[SDL_SCANCODE_R])
	//{
	//	self->velocity = gfc_vector2d(1.0f, 0.f);
	//}


	
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
	self->worldTime = SDL_GetTicks();

	camera_center_on(self->position);
}

void player_attack(Entity* self)
{
	if (!self) return;

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	Uint32 currentTime = SDL_GetTicks(); // Get current time in milliseconds
	Uint32 currentTime2 = SDL_GetTicks(); // Get current time in milliseconds

	if (self->worldTime - self->lastAttackTime >= 1000) { // 3000 ms = 3 seconds
		self->magicCooldown = 0;
	}

	if (self->worldTime - self->lastAttackTimeBurst >= 5000) { // 5000 ms = 5 seconds
		self->burstMagicCooldown = 0;
	}

	if (self->worldTime - self->lastAttackTimeMelee >= 1000) { // 3000 ms = 3 seconds
		self->meleeCooldown = 0;
	}
	if (self->worldTime - self->lastAttackTimeWall >= 8000) { // 3000 ms = 3 seconds
		self->wallMagicCooldown = 0;
	}


	if (keys[SDL_SCANCODE_Q] && (self->magicCooldown == 0))
	{

			Entity* spell = spell_new_entity(gfc_vector2d(self->position.x, self->position.y));

			//spell->acceleration = gfc_vector2d(0.1f, 0.f);
			//spell->position = gfc_vector2d(self->position.x + 3, self->position.y);


			spell_move(spell);
			self->magicCooldown = 3;
			self->lastAttackTime = self->worldTime;
	}

	if (keys[SDL_SCANCODE_X] && (self->meleeCooldown == 0))
	{	

		Entity* melee = melee_new_entity(gfc_vector2d(self->position.x + self->bounds.w + 3, self->position.y));
		//spell->position = gfc_vector2d(spell->position.x + 3, self->position.y);

		//spell->acceleration = gfc_vector2d(0.1f, 0.f);
		//spell->position = gfc_vector2d(self->position.x + 3, self->position.y);


		melee_move(melee);
		self->meleeCooldown = 3;
		self->lastAttackTimeMelee = self->worldTime;
		
	}

	if (keys[SDL_SCANCODE_Z] && (self->magicCooldown == 0))
	{

		Entity* spell = spell_new_entity(gfc_vector2d(self->position.x + 2, self->position.y));
		spell->magicType = MT_fire;



		spell_move(spell);
		self->magicCooldown = 3;
		self->lastAttackTime = self->worldTime;
	}

	if (keys[SDL_SCANCODE_R] && (self->burstMagicCooldown == 0))
	{

		Entity* spell = spell_new_entity(gfc_vector2d(self->position.x + 2, self->position.y));
		spell->worldTime = self->worldTime;
		spell->magicType = MT_rapid;
		spell_move(spell);

		self->burstMagicCooldown = 3;
		self->lastAttackTimeBurst = self->worldTime;
	}

	if (keys[SDL_SCANCODE_C] && (self->wallMagicCooldown == 0))
	{

		Entity* spell = spell_new_entity(gfc_vector2d(self->position.x + 2, self->position.y));
		spell->worldTime = self->worldTime;
		spell->magicType = MT_wall;
		spell_move(spell);
		Entity* spell2 = spell_new_entity(gfc_vector2d(self->position.x + 2, self->position.y-50));
		spell2->worldTime = self->worldTime;
		spell2->magicType = MT_wall;
		spell_move(spell2);
		Entity* spell3 = spell_new_entity(gfc_vector2d(self->position.x + 2, self->position.y+50));
		spell3->worldTime = self->worldTime;
		spell3->magicType = MT_wall;
		spell_move(spell3);

		self->wallMagicCooldown = 10;
		self->lastAttackTimeWall = self->worldTime;
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