#ifndef __STATUS_H__
#define __STATUS_H__


#pragma once


#include "entity.h"

typedef struct s_Effect
{
	Uint8		_inuse;						/**<Memory management flag*/
	float		statusDamage;
	Uint32		statusStart;
	Uint32		currentTime;


	Uint8		fireEffect;
	Uint8		TTL_fire;

	Uint8		freezeEffect;
	float		TTL_freeze;

}Effects;


/*
* @brief free all entities in the manager
* @Param ignore do not clean up this status
*/
void effect_system_clear_all();

/*
* @brief free a previously created status
*/
void effect_free(Effects* self);

/*
* @brief creates a new effect that is usable to give DOT effect
*/
Effects* effect_new();

/*
* brief Closes the status system
*/
void effect_system_close();


/**
* @brief initialize the status sub status system_init
* @param maxEnts upper limit for how many entities can exist at once
*/
void effect_system_init(Uint32 maxEnts);


/*
* @param The status to give the status condition to
*/
void status_give_effect(Entity* self);// , Effects* status);



/*
* @Brief Sets the default values for a status condition
*/
Effects* new_status_assign(Entity* self);


#endif

