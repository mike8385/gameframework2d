#include "simple_logger.h"
#include "entity.h"
#include "monster.h"

typedef struct
{
	const char* name;
	const char* deffile;
	Entity (*spawn)(GFC_Vector2D position, const char* defFile);
}SpawnPair;

static SpawnPair spawn_list[] =
{
	{
		"monster_bug",
		"defs/bug.def",
		monster_new_entity
	},
	{0}
};

/**
* @breif spawn an entity by its name
* @param name the name of the entity
*/
//Entity* spawn_entity(const char* name, GFC_Vector2D position)
//{
//	int i;
//	if (!name)
//	{
//		slog("spawn failed, no name provided");
//		return NULL;
//
//	}
//	for (i = 0;spawn_list[i].name != NULL; i++)
//	{
//		if (gfc_strlcmp(spawn_list[i].name, name) == 0)
//		{
//			if (spawn_list[i].spawn)
//			{
//				return spawn_list[i].spawn(positon, spawn_list[i].deffile);
//			}
//		}
//	}
//}