#include <SDL.h>
#include "simple_logger.h"

#include "basic_wizard.h"
#include "player.h"
#include "magic.h"
#include "status.h"

Entity* basic_wizard_new_entity(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a basic_wizard entity");
		return NULL;
	}
	gfc_vector2d_copy(self->position, position);
	self->think = basic_wizard_think;
	self->update = basic_wizard_update;
	self->sprite = gf2d_sprite_load_all(
		"images/monsters/basic_wizard_run.png",
		128,
		128,
		16,
		0);
	self->collidedType = ETC_monster;
	self->magicCooldown = 0;
	self->lastAttackTime = 0;
	self->health = 100.0f;
	new_status_assign(self);



}

void basic_wizard_move(Entity* self)
{
	if (!self) return;
	GFC_Rect playerBounds = get_player_bounds();

	gfc_vector2d_add(self->position, self->velocity, self->position);
	if ((basic_wizard_see_player(self)) && (self->statusEffects->freezeEffect == 0))
	{
		self->velocity.x = -1;

		if (self->position.x <= (playerBounds.x + playerBounds.w + (double)50))
		{
			self->velocity.x = 0;
		}
		if ((playerBounds.x + playerBounds.w + (double)50) >= self->position.x)
		{
			self->velocity.x = 1;
		}
	}
	//slog("Position is: %f", self->position);


}

void basic_wizard_think(Entity* self)
{
	if (!self) return;
	//GFC_Vector2D velocity;
	self->bounds = gfc_rect(self->position.x, self->position.y, 128, 128);
	basic_wizard_see_player(self);


}

void basic_wizard_update(Entity* self)
{
	if (!self) return;
	basic_wizard_damage(self);
	basic_wizard_move(self);
	self->frame += 0.05f;
	//slog("Frame is: %f", self->frame);
	if (self->frame >= 4) self->frame = 0;
	basic_wizard_attack(self);
	basic_wizard_status(self);
	self->worldTime = SDL_GetTicks();



}

void basic_wizard_damage(Entity* self)
{
	if (!self) return;

	if (self->health <= 0.0f)
	{
		gf2d_sprite_free(self->sprite);
		entity_free(self);
	}
}

Uint8 basic_wizard_see_player(Entity* self)
{
	if (!self) return;
	GFC_Vector2D playerPosition = get_player_position();
	GFC_Rect playerBounds = get_player_bounds();

	if (self->position.x <= (playerBounds.x + playerBounds.w + (double)400))
	{
		//slog("I can see you!!");
		return 1;
	}

	if (self->position.x <= (playerBounds.x - playerBounds.w - (double)400))
	{
		//slog("I can see you!!");
		return 1;
	}

	return 0;
}

void basic_wizard_track_player(Entity* self)
{
	if (!self) return;
	GFC_Vector2D playerPosition = get_player_position();
}

void basic_wizard_attack(Entity* self)
{
	if (!self)return;
	GFC_Rect playerBounds = get_player_bounds();
	if (basic_wizard_see_player(self))
	{
		if (self->statusEffects->freezeEffect == 0)
		{
			Uint32 currentTime = SDL_GetTicks(); // Get current time in milliseconds
			if (currentTime - self->lastAttackTime >= 1000) { // 3000 ms = 3 seconds
				self->magicCooldown = 0;
			}
			if (self->magicCooldown == 0)
			{
				Entity* spell = spell_new_entity(gfc_vector2d(self->position.x + 2, self->position.y));

				spell->collidedType = ETC_monster_spell;


				spell_move(spell);
				self->magicCooldown = 3;
				self->lastAttackTime = currentTime;
			}
		}



	}
}

void basic_wizard_status(Entity* self)
{
	if (!self) return;

	if (!self->statusEffects) return;




	//slog("Freeze Effect: %d", self->statusEffects->freezeEffect);

	if (self->statusEffects->fireEffect)
	{
		float currentTime = SDL_GetTicks();
		if (currentTime - self->lastDamageTime >= 200) // 1 second cooldown
		{
			slog("Burning");
			self->health -= self->statusEffects->statusDamage;
			self->lastDamageTime = currentTime;
			slog("HEalth: %f", self->health);
		}

		if (currentTime - self->statusEffects->statusStart >= self->statusEffects->TTL_fire)
		{
			slog("Unburn");
			self->statusEffects->fireEffect = 0;
			slog("HEalth: %f", self->health);

		}
	}


	if (self->statusEffects->freezeEffect) // If frozen
	{

		if (self->worldTime - self->statusEffects->statusStart >= self->statusEffects->TTL_freeze)
		{
			self->statusEffects->freezeEffect = 0;
			self->velocity.x = -1; // Stay frozen
			slog("Unfrozen");


		}
		else
		{
			self->velocity.x = 0; // Stay frozen
			slog("Frozen");

		}

	}

}

void basic_wizard_free(Entity* self)
{
	if ((!self) || (!self->data) || (!self->statusEffects)) return;
	Entity* data = self->data;
	//gf2d_sprite_free(data->)
	gf2d_sprite_free(data->sprite);
	free(self->data);
	free(data);
	self->data = NULL;
	slog("Freed");
	free(self);
}