//Entity* pets_new(const char* name, GFC_Vector2D position)
//{
//	Entity* pet;
//	SJson* petDef;
//	SJson* spriteObj;
//	const char* type;
//	const char* spriteFile = NULL;
//
//	petDef = pets_get_def_by_name(name);
//	if (!petDef)
//	{
//		slog("Pet definition not found %s", name);
//		return NULL;
//	}
//	pet = entity_new();
//	if (!pet)
//	{
//		slog("Cannot make a pet entity with entity_new");
//		return NULL;
//	}
//	gfc_line_cpy(pet->name, name);
//
//	type = sj_object_get_value_as_string(petDef, "type");
//	if (!type)
//	{
//		slog("%s missing 'type'", name);
//		return NULL;
//	}
//	pet->collidedType = string_to_collision_type(type);
//
//	spriteObj = sj_object_get_value(petDef, "sprite");	//Gets the object or list of sprite stuff
//	if (spriteObj)		//If it finds sprite stuff grab that info
//	{
//		pet->filename = sj_object_get_value_as_string(spriteObj, "filename");
//		sj_object_get_value_as_int(spriteObj, "frame", &pet->frame);
//
//		if (pet->filename)
//		{
//			pet->sprite = gf2d_sprite_load_all(pet->filename, 32, 32, 4, 0);
//			if (!pet->sprite)
//			{
//				slog("Failed to load sprite for pet %s from %s", name, pet->filename);
//			}
//		}
//		else
//		{
//			slog("Sprite filename missing for pet: %s", name);
//		}
//	}
//	else
//	{
//		slog("Not sprite object found for item: %s", name);
//	}
//	pet->count = 1;
//	pet->pos
//
//	return pet;
//
//}
//
//void pets_new_entity_placed(Entity* self, GFC_Vector2D position)
//{
//	if (!self)
//	{
//		slog("failed to spawn a pets entity");
//		return NULL;
//	}
//
//
//	for (int i = 0; i < MAX_PETS; i++)
//	{
//		if (!placedPets[i])
//		{
//			placedPets[i] = self;
//			break;
//		}
//	}
//
//	gfc_vector2d_copy(self->position, position);
//	slog("%s", self->sprite->filepath);
//	self->think = pets_think;
//	self->update = pets_update;
//	self->isFollowing = 0;
//	new_status_assign(self);
//	slog("Pet %s placed", self->name);
//	slog("Pet location %f %f", self->position.x, self->position.y);
//
//
//}