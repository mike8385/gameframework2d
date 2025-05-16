#include <SDL.h>
#include "simple_logger.h"

#include "monster.h"
#include "player.h"
#include "magic.h"
#include "status.h"



static SJson* _monsterJson = NULL;
static SJson* _monsterDefs = NULL;
#define MAX_MONSTERS 12

static Entity* placedPets[MAX_MONSTERS] = { 0 };





void monster_close();


void monster_initalize(const char* filename)
{
	if (!filename)
	{
		slog("No filename provided for monster initalization");
		return;
	}
	_monsterJson = sj_load(filename);
	if (!_monsterJson)
	{
		slog("Failed to load the json for monster definition");
	}
	_monsterDefs = sj_object_get_value(_monsterJson, "monster");
	if (!_monsterDefs)
	{
		slog("Pet definition file %s does not contain monster list");
		sj_free(_monsterJson);
		return;
	}
	atexit(monster_close);
}

void monster_close()
{
	if (_monsterJson)
	{
		sj_free(_monsterJson);
		_monsterJson = NULL;
		_monsterDefs = NULL;
	}
}



SJson* monster_get_def_by_name(const char* name)
{
	int i, c;
	SJson* monster;
	const char* monsterName = NULL;
	
	if (!name) return NULL;

	//This checks if theres any definitions for monster
	if (!_monsterDefs)
	{
		slog("No monster definitions loaded");
		return NULL;
	}

	c = sj_array_get_count(_monsterDefs);	//Gets the amount of monster in the JSON
	for (i = 0; i < c; i++)			//iterates through the monster
	{
		monster = sj_array_get_nth(_monsterDefs, i);
		if (!monster) continue;
		monsterName = sj_object_get_value_as_string(monster, "name");
		if (!monsterName) continue;
		if (gfc_strlcmp(name, monsterName) == 0)
		{
			//Found it
			return monster;
		}
	}
	slog("No monster found by name %s", name);
	return NULL;
}

Entity* monster_new(const char* name)
{
	Entity* monster;
	SJson* monsterDef;
	SJson* spriteObj;
	const char* collidedType;
	const char* spellCollidedType;
	const char* spriteFile = NULL;

	monsterDef = monster_get_def_by_name(name);
	if (!monsterDef)
	{
		slog("monster definition not found %s", name);
		return NULL;
	}
	monster = entity_new();
	if (!monster)
	{
		slog("Cannot make a monster entity with entity_new");
		return NULL;
	}
	gfc_line_cpy(monster->name, name);

	collidedType = sj_object_get_value_as_string(monsterDef, "collidedType");
	if (!collidedType)
	{
		slog("%s missing 'type'", name);
		return NULL;
	}
	monster->collidedType = string_to_collision_type(collidedType);

	spellCollidedType = sj_object_get_value_as_string(monsterDef, "spellCollidedType");
	if (!spellCollidedType)
	{
		slog("%s missing 'monsterType'", name);
		return NULL;
	}
	monster->magicType = string_to_collision_type(spellCollidedType);

	spriteObj = sj_object_get_value(monsterDef, "sprite");	//Gets the object or list of sprite stuff
	if (spriteObj)		//If it finds sprite stuff grab that info
	{
		monster->filename = sj_object_get_value_as_string(spriteObj, "filename");
		sj_object_get_value_as_int(spriteObj, "frame", &monster->frame);

		if (monster->filename)
		{
			monster->sprite = gf2d_sprite_load_all(monster->filename, 128, 128, 16, 0);
			if (!monster->sprite)
			{
				slog("Failed to load sprite for monster %s from %s", name, monster->filename);
			}
		}
		else
		{
			slog("Sprite filename missing for monster: %s", name);
		}
	}
	else
	{
		slog("Not sprite object found for item: %s", name);
	}
	monster->count = 1;

	return monster;

}



void monster_new_entity_placed(Entity* self, GFC_Vector2D position)
{
	if (!self)
	{
		slog("failed to spawn a monster entity");
		return NULL;
	}


	for (int i = 0; i < MAX_MONSTERS; i++)
	{
		if (!placedPets[i])
		{
			placedPets[i] = self;
			break;
		}
	}

	gfc_vector2d_copy(self->position, position);
	//slog("%s", self->sprite->filepath);
	self->think = monster_think;
	self->update = monster_update;
	self->collidedType = ETC_monster;
	self->magicCooldown = 0;
	self->lastAttackTime = 0;
	self->health = 100.0f;
	//self->isFollowing = 0;
	new_status_assign(self);
	//slog("Pet %s placed", self->name);
	//slog("Pet location %f %f", self->position.x, self->position.y);


}








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
	self->collidedType = ETC_monster;
	self->magicCooldown = 0;
	self->lastAttackTime = 0;
	self->health = 100.0f;
	new_status_assign(self);



}

void monster_move(Entity* self)
{
	if (!self) return;
	GFC_Rect playerBounds = get_player_bounds();

	gfc_vector2d_add(self->position, self->velocity, self->position);
	if ((monster_see_player(self)) && (self->statusEffects->freezeEffect == 0))
	{
		self->velocity.x = -1;

		if (self->position.x <= (playerBounds.x + playerBounds.w + (double)50))
		{
			self->velocity.x = 0;
		}
		if ((playerBounds.x + playerBounds.w + (double)50) >= self->position.x)
		{
			self->velocity.x = 1;
		}
	}
	//slog("Position is: %f", self->position);


}

void monster_think(Entity* self)
{
	if (!self) return;
	//GFC_Vector2D velocity;
	self->bounds = gfc_rect(self->position.x, self->position.y, 128, 128);
	monster_see_player(self);


}

void monster_update(Entity* self)
{
	if (!self) return;
	monster_damage(self);
	monster_move(self);
	self->frame += 0.05f;
	//slog("Frame is: %f", self->frame);
	if (self->frame >= 4) self->frame = 0;
	monster_attack(self);
	monster_status(self);
	self->worldTime = SDL_GetTicks();



}

void monster_damage(Entity* self)
{
	if (!self) return;
	
	if (self->health <= 0.0f)
	{
		gf2d_sprite_free(self->sprite);
		entity_free(self);
	}
}

Uint8 monster_see_player(Entity* self)
{
	if (!self) return;
	GFC_Vector2D playerPosition = get_player_position();
	GFC_Rect playerBounds = get_player_bounds();

	if (self->position.x <= (playerBounds.x + playerBounds.w + (double)400))
	{
		//slog("I can see you!!");
		return 1;
	}

	if (self->position.x <= (playerBounds.x - playerBounds.w - (double)400))
	{
		//slog("I can see you!!");
		return 1;
	}

	return 0;
}

void monster_track_player(Entity* self)
{
	if (!self) return;
	GFC_Vector2D playerPosition = get_player_position();
}

void monster_attack(Entity* self)
{
	if (!self)return;
	GFC_Rect playerBounds = get_player_bounds();
	if (monster_see_player(self))
	{
		if (self->statusEffects->freezeEffect == 0)
		{
			Uint32 currentTime = SDL_GetTicks(); // Get current time in milliseconds
			if (currentTime - self->lastAttackTime >= 1000) { // 3000 ms = 3 seconds
				self->magicCooldown = 0;
			}
			if (self->magicCooldown == 0)
			{
				Entity* spell = spell_new_entity(gfc_vector2d(self->position.x + 2, self->position.y));

				spell->collidedType = ETC_monster_spell;


				spell_move(spell);
				self->magicCooldown = 3;
				self->lastAttackTime = currentTime;
			}
		}

		

	}
}

void monster_status(Entity* self)
{
	if (!self) return;

	if (!self->statusEffects) return;




	//slog("Freeze Effect: %d", self->statusEffects->freezeEffect);

	if (self->statusEffects->fireEffect)
	{
		float currentTime = SDL_GetTicks();
		if (currentTime - self->lastDamageTime >= 200) // 1 second cooldown
		{
			slog("Burning");
			self->health -= self->statusEffects->statusDamage;
			self->lastDamageTime = currentTime;
			slog("HEalth: %f", self->health);
		}

		if (currentTime - self->statusEffects->statusStart >= self->statusEffects->TTL_fire)
		{
			slog("Unburn");
			self->statusEffects->fireEffect = 0;
			slog("HEalth: %f", self->health);

		}
	}


	if (self->statusEffects->freezeEffect) // If frozen
	{

		if (self->worldTime - self->statusEffects->statusStart >= self->statusEffects->TTL_freeze)
		{
			self->statusEffects->freezeEffect = 0;
			self->velocity.x = -1; // Stay frozen
			slog("Unfrozen");


		}
		else
		{
			self->velocity.x = 0; // Stay frozen
			slog("Frozen");

		}

	}

}

void monster_free(Entity* self)
{
	if ((!self) || (!self->data) || (!self->statusEffects)) return;
	Entity* data = self->data;
	//gf2d_sprite_free(data->)
	gf2d_sprite_free(data->sprite);
	free(self->data);
	free(data);
	self->data = NULL;
	slog("Freed");
	free(self);
}