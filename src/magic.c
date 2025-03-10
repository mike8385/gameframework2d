#include <SDL.h>
#include <gfc_input.h>
#include "simple_logger.h"
#include "gfc_shape.h"
#include "gf2d_draw.h"

#include "player.h"
#include "magic.h"
#include "status.h"


Entity* spell_new_entity(GFC_Vector2D position)
{

	Entity* spell;
	spell = entity_new();
	if (!spell)
	{
		slog("failed to spawn a spell entity");
	}
	gfc_vector2d_copy(spell->position, position);

	spell->sprite = gf2d_sprite_load_all(
		"images/players/wizardSprites/PNG/wizard/wizard_ball.png",
		50,
		50,
		1,
		0);
	//spell->position = gfc_vector2d(spell->position.x + 3, self->position.y);
	spell->bounds = gfc_rect(50, 50, 50, 50);
	spell->update = spell_update;
	spell->think = spell_think;
	spell->collidedType = ETC_magic;
	spell->collision = spell_collision;
	spell->magicType = MT_magic;
	return spell;
}

void spell_move(Entity* self)
{
	if (!self) return;

	self->velocity = gfc_vector2d(5.0f, 0.f); //Default magic type

	if (self->magicType == MT_fire)
	{
		self->velocity = gfc_vector2d(2.0f, 0.f); //Default magic type

	}

	if (self->magicType == MT_rapid)
	{
		self->velocity = gfc_vector2d(7.0f, 0.f); //Default magic type
		//Uint32 currentTime = SDL_GetTicks();
		if (self->worldTime - self->spawnTime >= 1050)
		{
			if (self->spawnTime + 1050 <= self->worldTime) // Ensures the delay only occurs once
			{
				Entity* spell2 = spell_new_entity(gfc_vector2d(self->position.x + 50 , self->position.y));
				spell2->magicType = MT_rapid;
				//spell_move(spell2);
				spell2->velocity = gfc_vector2d(7.0f, 0.f); //Default magic type

			}
		}


	}

	if (self->magicType == MT_wall)
	{
		self->velocity = gfc_vector2d(4.0f, 0.f); //Default magic type
		////Uint32 currentTime = SDL_GetTicks();
		//if (self->worldTime - self->spawnTime >= 1050)
		//{
		//	if (self->spawnTime + 1050 <= self->worldTime) // Ensures the delay only occurs once
		//	{
		//		Entity* spell2 = spell_new_entity(gfc_vector2d(self->position.x + 50, self->position.y));
		//		spell2->magicType = MT_rapid;
		//		//spell_move(spell2);
		//		spell2->velocity = gfc_vector2d(7.0f, 0.f); //Default magic type

		//	}
		//}


	}






}

void spell_think(Entity* self)
{
	if (!self) return;
	//GFC_Vector2D velocity;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	spell_attack(self);
	spell_collision(self);



}

void spell_update(Entity* self)
{
	if (!self) return;
	//magic_move(self);
	//self->frame += 0.05f;
	//slog("Frame is: %f", self->frame);
	//if (self->frame >= 4) self->frame = 0;
	self->ground = gfc_vector2d(self->position.x + (128 / 2), self->position.y + 128);
	self->bounds = gfc_rect(self->position.x, self->position.y, 50, 50);

	spell_world_collision(self);
	//spell_collision(self);
}

void spell_attack(Entity* self)
{
	if (!self) return;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	//self->velocity = gfc_vector2d(1.0f, 0.0f);
}

void spell_world_collision(Entity* self)
{
	GFC_Rect worldBounds = get_world_bounds();


	if (self->bounds.x + self->bounds.w > worldBounds.x + worldBounds.w)
	{

		entity_free(self);

	}
}

void spell_collision(Entity* self)
{
	if (!self) return;

	int i;
	GFC_List* list = entity_collide_all(self);
	if (!gfc_list_count(list))
	{
			gfc_list_delete(list); return;
	}
	for (i = 0; i < gfc_list_count(list); ++i)
	{
		Entity* other = (Entity*)gfc_list_nth(list, i);

		if ((self->collidedType == ETC_magic) && (other->collidedType == ETC_monster) && (self->magicType == MT_magic))
		{
			entity_free(self);
			//entity_damage(other);
			entity_free(other);
		}
		else if ((self->collidedType == ETC_magic) && (other->collidedType == ETC_monster) && (self->magicType == MT_fire))
		{
			Effects* effect = effect_new();  // Allocate a new effect instance
			if (!effect) return;  // Avoid crash if allocation fails

			effect->fireEffect = 1;
			effect->TTL_fire = 4000;
			effect->statusDamage = 10;
			effect->statusStart = SDL_GetTicks();

			status_give_effect(other, effect);
			//Make a file for statusEffects and call give status to other
			//status_give_effect(other, )
			
		}
	}
}


void spell_type(Entity* self)
{
	
}