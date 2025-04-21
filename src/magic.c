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
	spell->damageDelt = 0;
	spell->worldTime = SDL_GetTicks();
	spell->spawnTime = SDL_GetTicks();
	spell->lastAttackTime = SDL_GetTicks();
	spell->TTL = 10000;
	return spell;
}

void spell_move(Entity* self)
{
	if (!self) return;
	//slog("CollidedType: %d, MagicType: %d", self->collidedType, self->magicType);

	if ((self->collidedType == ETC_magic) && (self->magicType == MT_magic))
	{
		self->velocity = gfc_vector2d(5.0f, 0.f); //Default magic type

	}

	//Fire SHooting
	if ((self->collidedType == ETC_magic) && (self->magicType == MT_fire))
	{
		self->velocity = gfc_vector2d(2.0f, 0.f); //Default magic type

	}

	//Rapid Fire
	if ((self->collidedType == ETC_magic) && (self->magicType == MT_rapid))
	{
		//slog("Player shoot");
		self->velocity = gfc_vector2d(7.0f, 0.f); //Default magic type
		//slog("World Time: %f", self->worldTime);
		//slog("Spawn Time: %f", self->spawnTime);
		if (self->worldTime - self->spawnTime >= 1050)
		{
			if (self->spawnTime + 1050 <= self->worldTime) // Ensures the delay only occurs once
			{
				Entity* spell2 = spell_new_entity(gfc_vector2d(self->position.x + 50 , self->position.y));
				spell2->magicType = MT_rapid;
				spell2->velocity = gfc_vector2d(7.0f, 0.f); //Default magic type

			}
		}


	}

	//Frozen
	if ((self->collidedType == ETC_magic) && (self->magicType == MT_freeze))
	{
		self->velocity = gfc_vector2d(4.0f, 0.f); 

	}

	//Monster Shooting
	if ((self->collidedType == ETC_monster_spell) && (self->magicType == MT_magic))
	{
		//slog("Monster Normal");
		self->velocity = gfc_vector2d(-5.0f, 0.f);

	}

	if ((self->collidedType == ETC_monster_spell) && (self->magicType == MT_fire))
	{
		//slog("Monster Normal");
		self->velocity = gfc_vector2d(-2.0f, 0.f);

	}
	if ((self->collidedType == ETC_monster_spell) && (self->magicType == MT_rapid))
	{

		self->velocity = gfc_vector2d(-7.0f, 0.f); //Default magic type

		if (self->worldTime - self->spawnTime >= 1050)
		{
			if (self->spawnTime + 1050 <= self->worldTime) // Ensures the delay only occurs once
			{
				slog("Monster Fast");

				Entity* spell2 = spell_new_entity(gfc_vector2d(self->position.x + 50, self->position.y));
				spell2->magicType = MT_rapid;
				spell2->velocity = gfc_vector2d(-7.0f, 0.f); //Default magic type

			}
		}

	}

	if ((self->collidedType == ETC_monster_spell) && (self->magicType == MT_freeze))
	{
		self->velocity = gfc_vector2d(-4.0f, 0.f);

	}

}

void spell_think(Entity* self)
{
	if (!self) return;
	//GFC_Vector2D velocity;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	//spell_attack(self);
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
	spell_type(self);

	spell_world_collision(self);
	update_entity_lifetime(self);
}

void spell_attack(Entity* self)
{
	if (!self) return;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	//self->velocity = gfc_vector2d(1.0f, 0.0f);
}

void spell_world_collision(Entity* self)
{
	//slog("slog before");
	GFC_Rect worldBounds = get_world_bounds();
	//slog("After");


	if (self->bounds.x + self->bounds.w > worldBounds.x + worldBounds.w)
	{
	//	slog("Hit the corner");

		entity_free(self);

	}

	if (self->bounds.x < worldBounds.x)
	{
		//slog("Hit the corner");

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
		if (!other)
		{
			slog("Breoken");
		}

		if ((self->collidedType == ETC_magic) && (other->collidedType == ETC_monster) && (self->magicType == MT_magic))
		{
			
			entity_damage(self, other);
			entity_free(self);
			//entity_free(other);
			Stats* playerStats = get_player_stats();
			playerStats->EXP += 1;
		}
		else if ((self->collidedType == ETC_magic) && (other->collidedType == ETC_monster) && (self->magicType == MT_fire))
		{

			other->statusEffects->fireEffect = 1;
			other->statusEffects->statusDamage = 10.0f;
			other->statusEffects->currentTime = SDL_GetTicks();
			status_give_effect(other);
		}
		else if ((self->collidedType == ETC_magic) && (other->collidedType == ETC_monster) && (self->magicType == MT_freeze))
		{

			other->statusEffects->freezeEffect = 1;
			other->statusEffects->statusDamage = 0.f;
			//other->statusEffects->currentTime = SDL_GetTicks();
			entity_free(self);
			status_give_effect(other);

		}

		if ((self->collidedType == ETC_monster_spell) && (other->collidedType == ETC_entity) && (self->magicType == MT_magic))
		{
			self->damageDelt = 10;
			entity_free(self);
			entity_damage(self, other);
			//entity_free(other);
		}
		else if ((self->collidedType == ETC_monster_spell) && (other->collidedType == ETC_entity) && (self->magicType == MT_freeze))
		{

			other->statusEffects->freezeEffect = 1;
			other->statusEffects->statusDamage = 0.f;
			//other->statusEffects->currentTime = SDL_GetTicks();
			entity_free(self);
			status_give_effect(other);

		}
		else if ((self->collidedType == ETC_monster_spell) && (other->collidedType == ETC_entity) && (self->magicType == MT_fire))
		{

			other->statusEffects->fireEffect = 1;
			other->statusEffects->statusDamage = 10.0f;
			other->statusEffects->currentTime = SDL_GetTicks();
			status_give_effect(other);

		}
	}

}


void spell_type(Entity* self)
{
	if (self->magicType == MT_rapid)
	{
		gf2d_sprite_free(self->sprite);
		self->sprite = gf2d_sprite_load_image("images/players/wizardSprites/PNG/wizard/wizard_ball_pink.png");
		gf2d_sprite_draw_image(self->sprite, self->position);
	}
	if (self->magicType == MT_freeze)
	{
		gf2d_sprite_free(self->sprite);
		self->sprite = gf2d_sprite_load_image("images/players/wizardSprites/PNG/wizard/wizard_ball_ice.png");
		gf2d_sprite_draw_image(self->sprite, self->position);
	}
	if (self->magicType == MT_fire)
	{
		gf2d_sprite_free(self->sprite);
		self->sprite = gf2d_sprite_load_image("images/players/wizardSprites/PNG/wizard/wizard_ball_fire.png");
		gf2d_sprite_draw_image(self->sprite, self->position);
	}

}


void magic_free(Entity* self)
{
	if ((!self) || (!self->data) || (!self->statusEffects)) return;
	Entity* data = self->data;
	//gf2d_sprite_free(data->)
	gf2d_sprite_free(data->sprite);
	free(self->data);
	free(data);
	self->data = NULL;
	free(self);
}