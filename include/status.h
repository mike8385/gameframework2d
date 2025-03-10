#ifndef __STATUS_H__
#define __STATUS_H__


#pragma once


#include "entity.h"

typedef struct s_Effect
{
	Uint8					_inuse;						/**<Memory management flag*/
	Uint32		statusDamage;
	Uint32		statusStart;
	Uint8		fireEffect;
	Uint8		TTL_fire;
}Effects;



void effect_system_clear_all();

void effect_free(Effects* self);

Effects* effect_new();

void effect_system_close();

void effect_system_init(Uint32 maxEnts);



void status_give_effect(Entity* self, Effects* status);


#endif

