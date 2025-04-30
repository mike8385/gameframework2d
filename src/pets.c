#include <SDL.h>
#include "simple_logger.h"

#include "pets.h"
#include "player.h"
#include "magic.h"
#include "status.h"

Entity* pets_new_entity(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a pets entity");
		return NULL;
	}
	gfc_vector2d_copy(self->position, position);
	self->think = pets_think;
	self->update = pets_update;
	self->sprite = gf2d_sprite_load_all(
		"images/pets/Pink_Monster/Pink_Monster_Idle_4.png",
		32,
		32,
		4,
		0);
	self->collidedType = ETC_pets;
	self->magicCooldown = 0;
	self->lastAttackTime = 0;
	self->health = 100.0f;
	new_status_assign(self);



}

void pets_move(Entity* self)
{
	if (!self) return;
	GFC_Rect playerBounds = get_player_bounds();

	gfc_vector2d_add(self->position, self->velocity, self->position);

	

	if (pets_see_player(self))
	{

		self->velocity.x = -3;
		
		if (self->position.x <= (playerBounds.x + playerBounds.w))
		{
			self->velocity.x = 0;
		}
		if ((playerBounds.x >= self->position.x))
		{
			self->velocity.x = 3;
		}
		
	}
	//slog("Position is: %f", self->position);


}

void pets_think(Entity* self)
{
	if (!self) return;
	//GFC_Vector2D velocity;
	self->bounds = gfc_rect(self->position.x, self->position.y, 128, 128);
	pets_see_player(self);


}

void pets_update(Entity* self)
{
	if (!self) return;
	//pets_damage(self);
	//pets_move(self);
	pets_collision(self);
	//self->frame += 0.05f;
	////slog("Frame is: %f", self->frame);
	//if (self->frame >= 4) self->frame = 0;
	//pets_attack(self);
	//pets_status(self);
	//self->worldTime = SDL_GetTicks();



}

void pets_damage(Entity* self)
{
	if (!self) return;

	if (self->health <= 0.0f)
	{
		gf2d_sprite_free(self->sprite);
		entity_free(self);
	}
}

Uint8 pets_see_player(Entity* self)
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

void pets_track_player(Entity* self)
{
	if (!self) return;
	GFC_Vector2D playerPosition = get_player_position();
}

void pets_attack(Entity* self)
{
	if (!self)return;
	GFC_Rect playerBounds = get_player_bounds();
	if (pets_see_player(self))
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

				spell->collidedType = ETC_pets_spell;


				spell_move(spell);
				self->magicCooldown = 3;
				self->lastAttackTime = currentTime;
			}
		}



	}
}

void pets_status(Entity* self)
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

void pets_collision(Entity* self)
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

		if ((self->collidedType == ETC_pets) && (other->collidedType == ETC_entity))//And isFollowing is off
		{

			pets_move(self);
		}

	}

}

void pets_free(Entity* self)
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