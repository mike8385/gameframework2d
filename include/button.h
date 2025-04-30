#ifndef __BUTTON_H__
#define __BUTTON_H__


#include <simple_json.h>
#include "simple_logger.h"
#include "gfc_list.h"
#include "gfc_color.h"
#include "gfc_shape.h"
#include "gf2d_sprite.h"
#include "gfc_text.h"




typedef enum
{
	BT_def,
	BT_NewGame,
	BT_Continue,
	BT_Exit,
	BT_Load,
	BT_Edit,
	BT_MAX
}ButtonType;

typedef struct
{
	Uint8					_inuse;						/**<Memory management flag*/
	GFC_TextLine			name;						/**<Name of the entity for debugging*/
	GFC_TextLine			text;
	GFC_Vector2D			position;
	GFC_Rect				size;
	Sprite*					sprite;
	GFC_List*				sprites;
	GFC_Color				color;
	Uint8					isButton;
	Uint8					isHover;
	ButtonType				type;

}Button;



Button* button_new();

//Button* main_menu_button(GFC_Rect size, GFC_Color color);

Button* button_new_button_text(GFC_Vector2D position, GFC_Vector2D size, GFC_Color color, GFC_TextLine text);

Button* button_new_button(GFC_Vector2D position, GFC_Vector2D size, GFC_Color color);

void button_system_close();

void button_system_init(Uint32 maxEnts);

void button_system_clear_all();

void button_free(Button* self);

Uint8 button_hover_check(Button* button);

//void button_draw_text(GFC_TextLine text);

void button_system_draw();

void button_system_update();

void button_update(Button* self);


void button_system_think();

void button_think(Button* self);



void button_click_actions(Button* self);


void button_think(Button* self);

#endif