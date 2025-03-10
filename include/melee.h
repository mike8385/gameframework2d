#ifndef __MELEE_H__
#define __MELEE_H__


#include "simple_json.h"

#include "world.h"
#include "entity.h"



Entity* melee_new_entity(GFC_Vector2D position);

void melee_move(Entity* self, Entity* other);

void melee_think(Entity* self);

void melee_update(Entity* self);

void melee_attack(Entity* self);

void melee_world_collision(Entity* self);

void melee_collision(Entity* self);
#pragma once
#endif


#pragma once
