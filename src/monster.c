#include <SDL.h>
#include "simple_logger.h"

#include "monster.h"
#include "player.h"
#include "magic.h"

Entity* monster_new_entity(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a monster entity");
		return NULL;
	}
	gfc_vector2d_copy(self->position, position);
	self->think = monster_think;
	self->update = monster_update;
	self->sprite = gf2d_sprite_load_all(
		"images/ed210.png",
		128,
		128,
		16,
		0);
	self->collidedType = ETC_monster;
	self->magicCooldown = 0;
	self->lastAttackTime = 0;
	self->health = 100;


}

void monster_move(Entity* self)
{
	if (!self) return;

	gfc_vector2d_add(self->position, self->velocity, self->position);
	//slog("Position is: %f", self->position);


}

void monster_think(Entity* self)
{
	if (!self) return;
	//GFC_Vector2D velocity;
	self->bounds = gfc_rect(self->position.x, self->position.y, 128, 128);
	monster_see_player(self);
	monster_damage(self);


}

void monster_update(Entity* self)
{
	if (!self) return;
	monster_move(self);
	self->frame += 0.05f;
	//slog("Frame is: %f", self->frame);
	if (self->frame >= 4) self->frame = 0;
	monster_attack(self);
}

void monster_damage(Entity* self)
{
	if (self->health == 0)
	{
		slog("Died");
		entity_free(self);
	}
}

Uint8 monster_see_player(Entity* self)
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

void monster_track_player(Entity* self)
{
	if (!self) return;
	GFC_Vector2D playerPosition = get_player_position();
}

void monster_attack(Entity* self)
{
	if (!self)return;
	GFC_Rect playerBounds = get_player_bounds();
	if (monster_see_player(self))
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
		//}

		

	}
}