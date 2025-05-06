#include <SDL.h>
#include "simple_logger.h"

#include "end_boss.h"
#include "player.h"
#include "magic.h"
#include "status.h"




Entity* end_boss_new_entity(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a end_boss entity");
		return NULL;
	}
	gfc_vector2d_copy(self->position, position);
	self->think = end_boss_think;
	self->update = end_boss_update;
	self->sprite = gf2d_sprite_load_all(
		"images/players/wizardSprites/PNG/wizard/wizard_idle3.png",
		128,
		128,
		16,
		0);
	self->collidedType = ETC_monster;
	self->magicCooldown = 0;
	self->meleeCooldown = 0;
	self->lastAttackTime = 0;
	self->health = 100.0f;
	new_status_assign(self);
	self->velocity.x = -2;
	self->velocity.y = 0;

	return self;



}

void end_boss_move(Entity* self)
{
	if (!self) return;

	GFC_Rect playerBounds = get_player_bounds();

	// Move the entity
	gfc_vector2d_add(self->position, self->position, self->velocity);

	//
	if (end_boss_see_player(self))
	{
		self->velocity.x = -2;
	}

	if (self->position.x <= (playerBounds.x + playerBounds.w + (double)300))
	{
		if (self->position.y <= 200)
		{
			self->velocity.y = 2;  // Move right
		}
		else if (self->position.y >= 450)
		{
			self->velocity.y = -2; // Move left
		}
		self->velocity.x = 0;
	}

	self->velocity.x = 0;

	//slog("Wizard Position: %f, Velocity: %f", self->position.x, self->velocity.x);
}


void end_boss_think(Entity* self)
{
	if (!self) return;
	//GFC_Vector2D velocity;
	self->bounds = gfc_rect(self->position.x, self->position.y, 128, 128);
	end_boss_see_player(self);


}

void end_boss_update(Entity* self)
{
	if (!self) return;
	end_boss_move(self);
	self->frame += 0.05f;
	//slog("Frame is: %f", self->frame);
	if (self->frame >= 4) self->frame = 0;
	end_boss_attack(self);
	end_boss_status(self);
	end_boss_damage(self);
	self->worldTime = SDL_GetTicks();



}

void end_boss_damage(Entity* self)
{
	if (!self) return;
	if (self->health <= 0.0f)
	{
		//slog("Health: %d", self->health);

	//	slog("Died");
		currentLevel++;
		entity_free(self);
	}
}

Uint8 end_boss_see_player(Entity* self)
{
	if (!self) return 0;

	GFC_Vector2D playerPos = get_player_position();
	float distance = fabs(self->position.x - playerPos.x);

	if (distance <= 500)  // only "see" player if within 500px
	{
		return 1;
	}

	return 0;
}

void end_boss_track_player(Entity* self)
{
	if (!self) return;
	GFC_Vector2D playerPosition = get_player_position();
}

void end_boss_attack(Entity* self)
{
	if (!self)return;
	GFC_Rect playerBounds = get_player_bounds();
	GFC_Rect worldBounds = get_world_bounds();
	if (end_boss_see_player(self))
	{

		Uint32 currentTime = SDL_GetTicks(); // Get current time in milliseconds
		if (currentTime - self->lastAttackTime >= 1000) { // 3000 ms = 3 seconds
			self->magicCooldown = 0;
		}
		if (currentTime - self->lastAttackTimeMelee >= 3000) { // 3000 ms = 3 seconds
			self->meleeCooldown = 0;
		}
		if (self->magicCooldown == 0)
		{
			Entity* spell = spell_new_entity(gfc_vector2d(self->position.x + 2, self->position.y));

			spell->collidedType = ETC_monster_spell;
			spell->magicType = MT_magic;
			if (self->position.x <= (playerBounds.x + playerBounds.w + (double)400)) spell->magicType = MT_freeze;
			

			spell_move(spell);
			self->magicCooldown = 3;
			self->lastAttackTime = currentTime;	
		}

		if (self->meleeCooldown == 0)
		{
			Entity* spell = spell_new_entity(gfc_vector2d(self->position.x + 2, self->position.y));

			spell->collidedType = ETC_monster_spell;
			spell->magicType = MT_fire;
			spell_move(spell);
			self->meleeCooldown = 3;
			self->lastAttackTimeMelee = currentTime;
		}


	}
}

void end_boss_status(Entity* self)
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