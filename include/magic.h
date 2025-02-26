#ifndef __MAGIC_H__
#define __MAGIC_H__


#include "simple_json.h"

#include "world.h"

Entity* spell_new_entity(GFC_Vector2D position);

void spell_move(Entity* self);

void spell_think(Entity* self);

void spell_update(Entity* self);

void spell_attack(Entity* self);

void spell_world_collision(Entity* self);
#pragma once
#endif