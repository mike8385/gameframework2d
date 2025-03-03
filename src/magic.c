#include <SDL.h>
#include <gfc_input.h>
#include "simple_logger.h"
#include "gfc_shape.h"
#include "gf2d_draw.h"

#include "entity.h"
#include "player.h"
#include "magic.h"
#include "world.h"

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
	return spell;
}

void spell_move(Entity* self)
{
	if (!self) return;
	self->velocity = gfc_vector2d(5.0f, 0.f);


}

void spell_think(Entity* self)
{
	if (!self) return;
	//GFC_Vector2D velocity;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	spell_attack(self);



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