#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "gfc_shape.h"
#include "gfc_vector.h"
#include "entity.h"
#include "world.h"

extern int process;
extern int mx, my;
extern float mf;
extern const Uint8* keys;



Sprite* sprite;
World* world;


Entity* player;
Entity* monster;
Entity* fire_wizard;
Entity* ice_wizard;
Entity* fast_wizard;
Entity* melee_wizard;
Entity* boss_wizard;
Entity* pinkMonster;
GFC_Vector2D position;
GFC_Vector2D monsterposition;
GFC_Rect rectangle;

/**
* @brief The process to change the scene for each level
*/
void level_process();

void level_setup();


void level_free();

void level_main_draw();



void level_editor();

void level_editor_setup();

void level_editor_free();
#endif