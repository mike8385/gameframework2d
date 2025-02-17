#include <SDL.h>
#include <stdlib.h>
#include "simple_json.h"
#include "simple_logger.h"

#include "gf2d_sprite.h"



#include "entity.h"



typedef struct {
	Uint32 entity_max;
	Entity* entity_list;
}EntitySystem;

static EntitySystem entity_system = { 0 }; /**<Initalize a LOCAL global entity manager*/


//void entity_system_close()
//{
//	entity_system_clear_all();
//	if (entity_system.entity_list)
//	{
//		entity_system_free_all();
//		free(entity_system.entity_list);
//	}
//}

void entity_system_close()
{
	entity_system_clear_all();
	if (entity_system.entity_list != NULL)
	{
		free(entity_system.entity_list);
	}

	/*entity_system.entity_list = NULL;
	entity_system.entity_max = 0;
	slog("Entity system closed");*/
	memset(&entity_system, 0, sizeof(EntitySystem));
}


void entity_system_init(Uint32 maxEnts)
{
	if (entity_system.entity_list)
	{
		slog("Cannot have two instances of entity manager, one is already active");
		return;
	}
	if (!maxEnts)
	{
		slog("cannot intialize an Entity manager for Zero entities!");
		return;
	}
	entity_system.entity_list = gfc_allocate_array(sizeof(Entity), maxEnts);
	if (!entity_system.entity_list)
	{
		slog("Failed to allocate global entity list");
		return;
	}
	entity_system.entity_max = maxEnts;
	atexit(entity_system_close);
}

void entity_draw(Entity* self)
{
	if (!self) return;
	if (!self->sprite) return;

	gf2d_sprite_draw(
		self->sprite,
		self->position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(Uint32)self->frame
	);

}

void entity_system_draw()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse) continue;
		entity_draw(&entity_system.entity_list[i]);
	}

}

void entity_free(Entity* self)
{
	if (!self) return;
	self->_inuse--;
}

void entity_system_clear_all()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse) continue;	//Skip this iteration of the loop
		entity_free(&entity_system.entity_list[i]);

	}
}

Entity* entity_new()
{
	int i = 0;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (entity_system.entity_list[i]._inuse) continue;	//Skip any active entities
		memset(&entity_system.entity_list[i], 0, sizeof(Entity));
		entity_system.entity_list[i]._inuse = 1;
		//Setting the default color
		//Setting the default scale
		return &entity_system.entity_list[i];
	}
	slog("No more available entities");
	return NULL;
}

void entity_think(Entity* self)
{
	if (!self) return;
	//Any boilerplate think stuff here
	
	//Make an entity check if they are entering the out of bounds set
	//If so make it so they cannot move, if not continue;
	/*if (self->bounds.x > world->bounds.x || self->bounds.y )
	{
		self->position = world->bounds
	}
	*/

	if (self->think)self->think(self);
}

void entity_system_think()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse) continue;
		entity_think(&entity_system.entity_list[i]);
	}
}

void entity_update(Entity* self)
{
	if (!self) return;
	
	if (self->update)self->update(self);
}

void entity_system_update()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse) continue;
		entity_update(&entity_system.entity_list[i]);
	}
}

void entity_configure(Entity* self, SJson *json)
{
	GFC_Vector2D frameSize = { 0 };
	Uint32 framesPerLine = 0;
	const char* sprite = NULL;
	if ((!self) || (!json)) return;
	sprite = sj_object_get_string(json, "sprite");
	if (sprite)
	{
		sj_object_get_vector2d(json, "spriteSize", &frameSize);
		sj_object_get_uint32(json, "spriteFPL", &framesPerLine);
		self->sprite = gf2d_sprite_load_all(
			sprite,
			(Uint32)frameSize.x,
			(Uint32)frameSize.y,
			framesPerLine,
			0);

	}

}

//void entity_system_think()