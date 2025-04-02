#include <SDL.h>
#include <gfc_input.h>
#include "simple_logger.h"



#include "status.h"

typedef struct {
	Uint32 effect_max;
	Effects* effect_list;
}EffectSystem;


static EffectSystem effect_system = { 0 }; /**<Initalize a LOCAL global entity manager*/

Effects* effect_new()
{
	int i = 0;
	for (i = 0; i < effect_system.effect_max; i++)
	{
		if (effect_system.effect_list[i]._inuse) continue;	//Skip any active entities
		memset(&effect_system.effect_list[i], 0, sizeof(Effects));
		effect_system.effect_list[i]._inuse = 1;
		//Setting the default color
		//Setting the default scale
		return &effect_system.effect_list[i];
	}
	slog("No more available effects");
	return NULL;
}




void effect_system_clear_all()
{
	int i;
	for (i = 0; i < effect_system.effect_max; i++)
	{
		if (!effect_system.effect_list[i]._inuse) continue;	//Skip this iteration of the loop
		effect_free(&effect_system.effect_list[i]);

	}
}

void effect_free(Effects* self)
{
	if (!self) return;
	self->_inuse--;
}

void effect_system_close()
{
	effect_system_clear_all();
	if (effect_system.effect_list != NULL)
	{
		free(effect_system.effect_list);
	}

	/*effect_system.effect_list = NULL;
	effect_system.effect_max = 0;
	slog("Entity system closed");*/
	memset(&effect_system, 0, sizeof(EffectSystem));
}


void effect_system_init(Uint32 maxEnts)
{
	if (effect_system.effect_list)
	{
		slog("Cannot have two instances of effect manager, one is already active");
		return;
	}
	if (!maxEnts)
	{
		slog("cannot intialize an Entity manager for Zero entities!");
		return;
	}
	effect_system.effect_list = gfc_allocate_array(sizeof(Effects), maxEnts);
	if (!effect_system.effect_list)
	{
		slog("Failed to allocate global effect list");
		return;
	}
	effect_system.effect_max = maxEnts;
	atexit(effect_system_close);
}


void status_give_effect(Entity* self)// , Effects* status)
{
	if (!self) return;

	slog("Applying status effect: Fire: %d, Freeze: %d, Damage: %f",
		self->statusEffects->fireEffect,
		self->statusEffects->freezeEffect,
		self->statusEffects->statusDamage);

	if (self->statusEffects->fireEffect)
	{
		self->statusEffects->statusStart = SDL_GetTicks();
		//self->statusEffects->currentTime = SDL_GetTicks();
	}

	if (self->statusEffects->freezeEffect)
	{
		self->statusEffects->statusStart = SDL_GetTicks();
		//self->statusEffects->currentTime = SDL_GetTicks();
	}

}

Effects* new_status_assign(Entity* self)
{
	Effects* statusEffect;
	statusEffect = gfc_allocate_array(sizeof(Effects), 1);
	if (statusEffect)
	{
		statusEffect->_inuse = 0;						/**<Memory management flag*/
		statusEffect->statusDamage = 0.0f;
		statusEffect->statusStart = 0;
		//statusEffect->endTime = 0;


		statusEffect->fireEffect = 0;
		statusEffect->TTL_fire = 2000;

		statusEffect->freezeEffect = 0;
		statusEffect->TTL_freeze = 2000;


	}
	self->statusEffects = statusEffect;
}