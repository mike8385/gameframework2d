#include <SDL.h>
#include <stdlib.h>
#include "simple_json.h"
#include "simple_logger.h"

#include "gf2d_sprite.h"



#include "entity.h"
#include "world.h"



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

void entity_attack(Entity* self)
{
	if (!self) return;

	if (self->attack)self->attack(self);
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

void entity_bounds_update(Entity* self)
{
	//Make an entity check if they are entering the out of bounds set
//If so make it so they cannot move, if not continue;
/*if (self->bounds.x > world->bounds.x || self->bounds.y )
{
	self->position = world->bounds
}
*/
	if (!self) return;
	GFC_Rect worldBounds = get_world_bounds();
	GFC_Vector2D* poc = { 0 };
	GFC_Vector2D* normal = { 0 };
	gfc_rect_overlap_poc(self->bounds, worldBounds, poc, normal);
	if (!gfc_point_in_rect(gfc_vector2d(self->bounds.x, self->bounds.y), worldBounds) || !gfc_point_in_rect(gfc_vector2d(self->bounds.x + self->bounds.w, self->bounds.y + self->bounds.h), worldBounds))//(gfc_rect_overlap_poc(self->bounds, world->bounds, poc, normal))//
	{
		if (self->bounds.x < worldBounds.x)
		{
			self->position.x = worldBounds.x;
		}

		if (self->bounds.y < worldBounds.y)
		{
			self->position.y = worldBounds.y;
		}

		if (self->bounds.x + self->bounds.w > worldBounds.x + worldBounds.w)
		{
			self->position.x  = worldBounds.x + worldBounds.w - self->bounds.w;
		}

		if (self->bounds.y + self->bounds.h > worldBounds.y + worldBounds.h)
		{
			self->position.y = worldBounds.y + worldBounds.h - self->bounds.h;
		}
		/*if (gfc_point_in_rect(gfc_vector2d(self->bounds.x + self->bounds.w, self->bounds.y + self->bounds.h), world->bounds))
		{
			slog("Crossing");
		}*/

		//if (gfc_rect_overlap_poc(self->bounds, world->bounds, NULL, -1))
		//{gfc_point_in_rect(gfc_vector2d(self->bounds.x, self->bounds.y), world->bounds)
		//	self->acceleration = gfc_vector2d(0.f, 0.f);
		//	self->position = gfc_vector2d(self->position.x + 2, self->position.y);
		//}
		//gfc_rect_overlap_poc(self->bounds, world->bounds, poc, normal);
		/*if ((world->bounds.x <= self->bounds.x) && (world->bounds.x+world->bounds.w) > self->bounds.x)
		{
			
		}*/
		//else if (self->bounds.x == se)
		//slog("POC: %f",poc);
		//slog("NORMAL: %f", normal);

		entity_update(self);
	}

}

void entity_move(Entity* self)
{
	if (!self) return;

	if (self->move)self->move(self);
	gfc_vector2d_add(self->position, self->position, self->velocity);
	gfc_vector2d_add(self->velocity, self->velocity, self->acceleration);


}


void entity_system_move()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse) continue;
		entity_move(&entity_system.entity_list[i]);
	}
}


void entity_collision()
{

}



//Uint8 entity collision check with self and other
/*
* 
* GFC_RECT bounds 1 and 2
* if exist
* if team tests
* if check self otherlayer return 0
* gfc copy bounds 1 and self bounds
* same but with boudns 2 and other
* add bounds 1 with self bounds plus bounds 1
* return oberlap bounds 1 and 2

_______________________________

entity layer check
if exist
return self->layer & layer

------

void entity set collision layer(self, collisionlayers layer

check if exist
set self layer


*/


/*
* entity collide all self
* gfclist *entites
*
* if exist
* entites = list new
* for i  < entity_system_entity_list
* if (!entity_system. exist
* if self == &entitysystemlist
* if check self, list
* gfclistappend entittes, list
* if !listcount(entites)
* delete (entites)
*
*/