#include <SDL.h>
#include "simple_logger.h"

#include "pets.h"
#include "player.h"
#include "magic.h"
#include "status.h"


static SJson* _petJson = NULL;
static SJson* _petsDefs = NULL;
#define MAX_PETS 12

static Entity* placedPets[MAX_PETS] = { 0 };


void pets_close();

void pets_initalize(const char* filename)
{
	if (!filename)
	{
		slog("No filename provided for pet initalization");
		return;
	}
	_petJson = sj_load(filename);
	if (!_petJson)
	{
		slog("Failed to load the json for pet definition");
	}
	_petsDefs = sj_object_get_value(_petJson, "pets");
	if (!_petsDefs)
	{
		slog("Pet definition file %s does not contain pets list");
		sj_free(_petJson);
		return;
	}
	atexit(pets_close);
}

void pets_close()
{
	if (_petJson)
	{
		sj_free(_petJson);
		_petJson = NULL;
		_petsDefs = NULL;
	}
}

SJson* pets_get_def_by_name(const char* name)
{
	int i, c;
	SJson* pet;
	const char* petName = NULL;
	if (!name) return NULL;

	//This checks if theres any definitions for pets
	if (!_petsDefs)
	{
		slog("No pet definitions loaded");
		return NULL;
	}

	c = sj_array_get_count(_petsDefs);	//Gets the amount of pets in the JSON
	for (i = 0; i < c; i++)			//iterates through the pets
	{
		pet = sj_array_get_nth(_petsDefs, i);
		if (!pet) continue;
		petName = sj_object_get_value_as_string(pet, "name");
		if (!petName) continue;
		if (gfc_strlcmp(name, petName) == 0)
		{
			//Found it
			return pet;
		}
	}
	slog("No Pet found by name %s", name);
	return NULL;
}

Entity* pets_new(const char* name)
{
	Entity* pet;
	SJson* petDef;
	SJson* spriteObj;
	const char* type;
	const char* petType;
	const char* spriteFile = NULL;

	petDef = pets_get_def_by_name(name);
	if (!petDef)
	{
		slog("Pet definition not found %s", name);
		return NULL;
	}
	pet = entity_new();
	if (!pet)
	{
		slog("Cannot make a pet entity with entity_new");
		return NULL;
	}
	gfc_line_cpy(pet->name, name);

	type = sj_object_get_value_as_string(petDef, "type");
	if (!type)
	{
		slog("%s missing 'type'", name);
		return NULL;
	}
	pet->collidedType = string_to_collision_type(type);

	petType = sj_object_get_value_as_string(petDef, "petType");
	if (!petType)
	{
		slog("%s missing 'petType'", name);
		return NULL;
	}
	pet->hasPet = string_to_pet_type(petType);

	spriteObj = sj_object_get_value(petDef, "sprite");	//Gets the object or list of sprite stuff
	if (spriteObj)		//If it finds sprite stuff grab that info
	{
		pet->filename = sj_object_get_value_as_string(spriteObj, "filename");
		sj_object_get_value_as_int(spriteObj, "frame", &pet->frame);

		if (pet->filename)
		{
			pet->sprite = gf2d_sprite_load_all(pet->filename, 32, 32, 4, 0);
			if (!pet->sprite)
			{
				slog("Failed to load sprite for pet %s from %s", name, pet->filename);
			}
		}
		else
		{
			slog("Sprite filename missing for pet: %s", name);
		}
	}
	else
	{
		slog("Not sprite object found for item: %s", name);
	}
	pet->count = 1;

	return pet;

}

void pets_new_entity_placed(Entity* self, GFC_Vector2D position)
{
	if (!self)
	{
		slog("failed to spawn a pets entity");
		return NULL;
	}


	for (int i = 0; i < MAX_PETS; i++)
	{
		if (!placedPets[i])
		{
			placedPets[i] = self;
			break;
		}
	}

	gfc_vector2d_copy(self->position, position);
	//slog("%s", self->sprite->filepath);
	self->think = pets_think;
	self->update = pets_update;
	self->isFollowing = 0;
	new_status_assign(self);
	//slog("Pet %s placed", self->name);
	//slog("Pet location %f %f", self->position.x, self->position.y);


}

Entity* pets_new_entity(GFC_Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a pets entity");
		return NULL;
	}
	gfc_vector2d_copy(self->position, position);
	self->think = pets_think;
	self->update = pets_update;
	self->sprite = gf2d_sprite_load_all(
		"images/pets/Pink_Monster/Pink_Monster_Idle_4.png",
		32,
		32,
		4,
		0);
	self->collidedType = ETC_pets;
	self->magicCooldown = 0;
	self->lastAttackTime = 0;
	self->health = 100.0f;
	self->isFollowing = 0;
	new_status_assign(self);
	self->count = 1;

	return self;



}

void pets_move(Entity* self)
{
	if (!self) return;
	GFC_Rect playerBounds = get_player_bounds();

	gfc_vector2d_add(self->position, self->velocity, self->position);

	self->velocity.x = 0;


	if ((self->isFollowing) && (pets_see_player(self)))
	{

		self->velocity.x = -3;
		self->velocity.y = -3;
		
		if (self->position.x <= (playerBounds.x + playerBounds.w))
		{
			self->velocity.x = 0;
		}
		if ((playerBounds.x >= self->position.x))
		{
			self->velocity.x = 3;
		}

		if (self->position.y <= (playerBounds.y + playerBounds.h))
		{
			self->velocity.y = 0;
		}
		if ((playerBounds.y >= self->position.y))
		{
			self->velocity.y = 3;
		}
		
	}
	
	//slog("Position is: %f", self->position);


}

void pets_think(Entity* self)
{
	if (!self) return;
	//GFC_Vector2D velocity;
	self->bounds = gfc_rect(self->position.x, self->position.y, 128, 128);
	pets_see_player(self);


}

void pets_update(Entity* self)
{
	if (!self) return;
	//pets_damage(self);
	pets_move(self);
	pets_collision(self);
	//self->frame += 0.05f;
	////slog("Frame is: %f", self->frame);
	//if (self->frame >= 4) self->frame = 0;
	pets_attack(self);
	//pets_status(self);
	//self->worldTime = SDL_GetTicks();



}

void pets_damage(Entity* self)
{
	if (!self) return;

	if (self->health <= 0.0f)
	{
		gf2d_sprite_free(self->sprite);
		entity_free(self);
	}
}

Uint8 pets_see_player(Entity* self)
{
	if (!self) return;
	GFC_Vector2D playerPosition = get_player_position();
	GFC_Rect playerBounds = get_player_bounds();

	if (self->position.x <= (playerBounds.x + playerBounds.w + 5))
	{
		//slog("I can see you!!");
		return 1;
	}

	if (self->position.x <= (playerBounds.x - playerBounds.w - 5))
	{
		//slog("I can see you 2!!");
		return 1;
	}

	return 0;
}

void pets_track_player(Entity* self)
{
	if (!self) return;
	GFC_Vector2D playerPosition = get_player_position();
}

void pets_attack(Entity* self)
{
	if (!self)return;
	GFC_Rect playerBounds = get_player_bounds();
	if (self->isFollowing&& self->hasPet == PT_Green)
	{
		self->damageDelt = 10;
		if (self->statusEffects->freezeEffect == 0)
		{
			Uint32 currentTime = SDL_GetTicks(); // Get current time in milliseconds
			if (currentTime - self->lastAttackTime >= 5000) { // 3000 ms = 3 seconds
				self->magicCooldown = 0;
			}
			if (self->magicCooldown == 0)
			{
				Entity* spell = spell_new_entity(gfc_vector2d(self->position.x + 2, self->position.y));

				spell->collidedType = ETC_pets_spell;
				spell->damageDelt = 10;

				spell_move(spell);
				self->magicCooldown = 3;
				self->lastAttackTime = currentTime;
			}
		}



	}
}

void pets_status(Entity* self)
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

void pets_collision(Entity* pet)
{
    if (!pet) return;

    GFC_List* list = entity_collide_all(pet);
    if (!gfc_list_count(list))
    {
        gfc_list_delete(list); 
        return;
    }

    for (int i = 0; i < gfc_list_count(list); i++)
    {
        Entity* other = (Entity*)gfc_list_nth(list, i);
        if (!other || other->collidedType != ETC_entity) continue;

        Entity* player = other;

        // If player has no pet, pick up this one
        if (player->hasPet == PT_None)
        {
            player->hasPet = pet->hasPet;
            pet->isFollowing = 1;
            player_update_pet(pet->hasPet);
            //slog("Player picked up pet: %d", pet->hasPet);
            break;
        }
        // If player has a different pet than this one
        else if (pet->hasPet != player->hasPet)
        {
            // First find and stop current following pet
            for (int j = 0; j < MAX_PETS; j++)
            {
                if (placedPets[j] && 
                    placedPets[j]->isFollowing && 
                    placedPets[j]->hasPet == player->hasPet)
                {
                    placedPets[j]->isFollowing = 0;
                    //slog("Stopped following pet: %d", placedPets[j]->hasPet);
                    break;
                }
            }
            
            // Then make this pet follow
            player->hasPet = pet->hasPet;
            pet->isFollowing = 1;
            player_update_pet(pet->hasPet);
            //slog("Player now following pet: %d", pet->hasPet);
            break;
        }
        // If player is colliding with their current pet that isn't following
        else if (!pet->isFollowing)
        {
            // Just resume following
            pet->isFollowing = 1;
            //slog("Resumed following pet: %d", pet->hasPet);
            break;
        }
    }
    
    gfc_list_delete(list);
}


/*

void pets_collision(Entity* pet)
{
	if (!pet) return;

	int i;
	GFC_List* list = entity_collide_all(pet);
	if (!gfc_list_count(list))
	{
		gfc_list_delete(list); return;
	}
	for (i = 0; i < gfc_list_count(list); ++i)
	{
		Entity* other = (Entity*)gfc_list_nth(list, i);
		if (!other)
		{
			slog("No other entity found for collision");
		}

		if (other->collidedType != ETC_entity) continue; //Its not the player

		Entity* player = other;

		//If the player collides with a pet
		if ((player->hasPet == PT_None))//And isFollowing is off
		{
		player->hasPet = pet->hasPet;		//Sets which pet the player has
		pet->isFollowing = 1;
		other->isFollowing = 1;			//Sets the following on for player
		//slog("Player Pet: %d", get_player_pet());
		if (!get_player_pet()) player_update_pet(pet->hasPet);
		break;
		}

		if ((pet->hasPet != player->hasPet))
		{
			pet->isFollowing = 0;
			other->isFollowing = 0;
			slog("Player: %d Pet: %d", player->isFollowing, pet->isFollowing);

			if (get_player_pet()) player_update_pet(0);
			slog("Player: %d Pet: %d", player->isFollowing, pet->isFollowing);


		}


			//slog("Player: %d Pet: %d", other->isFollowing, pet->isFollowing);

		}
		//if ((pet->collidedType == ETC_pets) && (other->collidedType == ETC_entity) && (pet->isFollowing == 0))//And isFollowing is off
		//{

		//	//pets_move(self);
		//}

	}

*/

void pets_free(Entity* self)
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