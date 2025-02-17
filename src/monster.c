#include <SDL.h>
#include "simple_logger.h"

#include "entity.h"
#include "monster.h"

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


}

void monster_update(Entity* self)
{
	if (!self) return;
	monster_move(self);
}
