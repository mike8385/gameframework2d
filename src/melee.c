#include <SDL.h>
#include <gfc_input.h>
#include "simple_logger.h"
#include "gfc_shape.h"
#include "gf2d_draw.h"

#include "player.h"
#include "melee.h"

Entity* melee_new_entity(GFC_Vector2D position)
{

	Entity* melee;
	melee = entity_new();
	if (!melee)
	{
		slog("failed to spawn a melee entity");
	}
	gfc_vector2d_copy(melee->position, position);

	melee->sprite = gf2d_sprite_load_all(
		"images/players/wizardSprites/PNG/wizard/wizard_ball.png",
		50,
		50,
		1,
		0);
	//melee->position = gfc_vector2d(melee->position.x + 3, self->position.y);
	melee->bounds = gfc_rect(50, 50, 50, 50);
	melee->update = melee_update;
	melee->think = melee_think;
	melee->collidedType = ETC_magic;
	melee->collision = melee_collision;
	melee->TTL = 20;
	melee->spawnTime = SDL_GetTicks();  // Set the current time

	return melee;
}

void melee_move(Entity* self, Entity* other)
{
	if (!self) return;
	//self->velocity = gfc_vector2d(5.0f, 0.f);
	//Uint32 currentTimeAttack = SDL_GetTicks(); // Get current time in milliseconds

	//if (currentTimeAttack - self->lastAttackTimeMelee <= 2000) { // 3000 ms = 3 seconds
	//	slog("Hello");
//	}

//	self->lastAttackTime = currentTimeAttack;
	self->position = other->position;



}

void melee_think(Entity* self)
{
	if (!self) return;
	//GFC_Vector2D velocity;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	melee_attack(self);
	melee_collision(self);



}

void melee_update(Entity* self)
{
	if (!self) return;
	//magic_move(self);
	//self->frame += 0.05f;
	//slog("Frame is: %f", self->frame);
	//if (self->frame >= 4) self->frame = 0;
	self->ground = gfc_vector2d(self->position.x + (128 / 2), self->position.y + 128);
	self->bounds = gfc_rect(self->position.x, self->position.y, 50, 50);

	melee_world_collision(self);
	//melee_collision(self);
	// In the game loop or entity update function
	update_entity_lifetime(self);  // Check if the melee entity has expired

}

void melee_attack(Entity* self)
{
	if (!self) return;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	//self->velocity = gfc_vector2d(1.0f, 0.0f);
}

void melee_world_collision(Entity* self)
{
	GFC_Rect worldBounds = get_world_bounds();


	if (self->bounds.x + self->bounds.w > worldBounds.x + worldBounds.w)
	{

		entity_free(self);

	}
}

void melee_collision(Entity* self)
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

		if ((self->collidedType == ETC_magic) && (other->collidedType == ETC_monster))
		{
			entity_free(self);
			//entity_damage(other);
			entity_free(other);
		}
	}
}

