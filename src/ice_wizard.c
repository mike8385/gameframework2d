#include <SDL.h>
#include "simple_logger.h"


#include "player.h"
#include "magic.h"
#include "status.h"
#include "ice_wizard.h"

Entity* ice_wizard_new_entity(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a ice_wizard entity");
		return NULL;
	}
	gfc_vector2d_copy(self->position, position);
	self->think = ice_wizard_think;
	self->update = ice_wizard_update;
	self->sprite = gf2d_sprite_load_all(
		"images/players/wizardSprites/PNG/wizard_ice/ice_wizard_walk.png",
		128,
		128,
		16,
		0);
	self->collidedType = ETC_monster;
	self->magicCooldown = 0;
	self->lastAttackTime = 0;
	self->health = 100.0f;
	new_status_assign(self);

	return self;

}

void ice_wizard_move(Entity* self)
{
	if (!self) return;

	gfc_vector2d_add(self->position, self->velocity, self->position);
	////slog("Position is: %f", self->position);


}

void ice_wizard_think(Entity* self)
{
	if (!self) return;
	//GFC_Vector2D velocity;
	self->bounds = gfc_rect(self->position.x, self->position.y, 128, 128);
	ice_wizard_see_player(self);


}

void ice_wizard_update(Entity* self)
{
	if (!self) return;
	ice_wizard_move(self);
	self->frame += 0.05f;
	////slog("Frame is: %f", self->frame);
	if (self->frame >= 4) self->frame = 0;
	ice_wizard_attack(self);
	ice_wizard_status(self);
	ice_wizard_damage(self);



}

void ice_wizard_damage(Entity* self)
{
	if (!self) return;
	if (self->health <= 0.0f)
	{
		//slog("Health: %d", self->health);

		//slog("Died");
		entity_free(self);
	}
}

Uint8 ice_wizard_see_player(Entity* self)
{
	if (!self) return;
	GFC_Vector2D playerPosition = get_player_position();
	GFC_Rect playerBounds = get_player_bounds();

	if (self->position.x <= (playerBounds.x + playerBounds.w + (double)100))
	{
		////slog("I can see you!!");
		return 1;
	}

	if (self->position.x <= (playerBounds.x - playerBounds.w - (double)400))
	{
		////slog("I can see you!!");
		return 1;
	}

	return 0;
}

void ice_wizard_track_player(Entity* self)
{
	if (!self) return;
	GFC_Vector2D playerPosition = get_player_position();
}

void ice_wizard_attack(Entity* self)
{
	if (!self)return;
	GFC_Rect playerBounds = get_player_bounds();
	if (ice_wizard_see_player(self))
	{
		self->velocity.x = 2;

		Uint32 currentTime = SDL_GetTicks(); // Get current time in milliseconds
		if (currentTime - self->lastAttackTime >= 3000) { // 3000 ms = 3 seconds
			self->magicCooldown = 0;
		}
		if (self->magicCooldown == 0)
		{
			Entity* spell = spell_new_entity(gfc_vector2d(self->position.x + 2, self->position.y));

			spell->collidedType = ETC_monster_spell;
			spell->magicType = MT_freeze;

			spell_move(spell);
			Entity* spell2 = spell_new_entity(gfc_vector2d(self->position.x + 2, self->position.y - 50));
			spell2->worldTime = self->worldTime;
			spell2->magicType = MT_freeze;
			spell2->collidedType = ETC_monster_spell;

			spell_move(spell2);

			Entity* spell3 = spell_new_entity(gfc_vector2d(self->position.x + 2, self->position.y + 50));
			spell3->worldTime = self->worldTime;
			spell3->magicType = MT_freeze;
			spell3->collidedType = ETC_monster_spell;

			spell_move(spell3);

			self->magicCooldown = 5;
			self->lastAttackTime = currentTime;
		}
		//}



	}
	else
	{
		self->velocity.x = 0;

	}
}

void ice_wizard_status(Entity* self)
{
	if (!self) return;

	if (!self->statusEffects) return;




	////slog("Freeze Effect: %d", self->statusEffects->freezeEffect);

	if (self->statusEffects->fireEffect)
	{
		float currentTime = SDL_GetTicks();
		if (currentTime - self->lastDamageTime >= 200) // 1 second cooldown
		{
			////slog("Burning");
			self->health -= self->statusEffects->statusDamage;
			self->lastDamageTime = currentTime;
			////slog("HEalth: %f", self->health);
		}

		if (currentTime - self->statusEffects->statusStart >= self->statusEffects->TTL_fire)
		{
			//slog("Unburn");
			self->statusEffects->fireEffect = 0;
			//slog("HEalth: %f", self->health);

		}
	}


	if (self->statusEffects->freezeEffect) // If frozen
	{

		if (self->worldTime - self->statusEffects->statusStart >= self->statusEffects->TTL_freeze)
		{
			self->statusEffects->freezeEffect = 0;
			self->velocity.x = -1; // Stay frozen
			//slog("Unfrozen");


		}
		else
		{
			self->velocity.x = 0; // Stay frozen
			////slog("Frozen");

		}

	}

}
