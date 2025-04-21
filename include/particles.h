#ifndef __PARTICLES_H__
#define __PARTICLES_H__

#include "gfc_color.h"
#include "gfc_vector.h"
#include "simple_json.h"


typedef struct
{
	Uint8			_inuse;
	GFC_Vector2D	position;
	GFC_Vector2D	velocity;
	GFC_Vector2D	acceleration;
	GFC_Color		drawColor;
	GFC_Color		colorVector;
	GFC_Color		colorDestinination;
	Uint32			ttl;		//Time to live in frames
} Particle;

void particle_system_init(Uint32 maxParticles);

void particle_system_close();

void particleDraw(Particle* particle);

Particle* particle_new();

void particle_free(Particle* particle);

void particle_system_draw();

void* particles_from_file(const char* filename, Uint32 count, GFC_Vector2D position, GFC_Vector2D direction, GFC_Vector2D acceleration);

Particle* particle_from_json(SJson* json, GFC_Vector2D position, GFC_Vector2D direction, GFC_Vector2D acceleration);
#endif
#pragma once
