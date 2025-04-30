#ifndef __WINDOWS_H__
#define __WINDOWS_H__


#include <simple_json.h>
#include "simple_logger.h"
#include "gfc_list.h"
#include "gfc_color.h"
#include "gfc_shape.h"
#include "gf2d_sprite.h"
#include "gfc_text.h"

#include "player.h"


typedef struct
{
	Uint8					_inuse;						/**<Memory management flag*/
	GFC_TextLine			name;						/**<Name of the entity for debugging*/
	GFC_Rect				size;
	Sprite*					sprite;
	GFC_List*				sprites;
	GFC_List*				buttons;
	GFC_Color				color;
	GFC_TextBlock			text;

}Window;

typedef struct
{
	Uint8					_inuse;						/**<Memory management flag*/
	GFC_TextLine			name;						/**<Name of the entity for debugging*/
	GFC_Rect				size;
	Sprite* sprite;
	GFC_List*				sprites;
	GFC_List*				buttons;
	GFC_Color				color;
	GFC_TextBlock			text;


}MainMenuWindow;




Window* window_new();

Window* main_menu();

void window_system_close();

void window_system_init(Uint32 maxEnts);

void window_system_clear_all();

void window_free(Window* self);

void window_draw(Window* win);

//Button* button_new();
//
//Button* main_menu_button(GFC_Rect size, GFC_Color color);
//
//Button* button_new_button(GFC_Vector2D position, GFC_Vector2D size, GFC_Color color);
//
//void button_system_close();
//
//void button_system_init(Uint32 maxEnts);
//
//void button_system_clear_all();
//
//void button_free(Entity* self);
//
//Uint8 button_hover_check();


//
//typedef struct Element_S Element;
//
//typedef enum
//{
//	BE_TL,
//	BE_Top,
//	BE_TR,
//	BE_Left,
//	BE_Right,
//	BE_BL,
//	BE_Bottom,
//	BE_BR
//} BorderElements;
//
//typedef struct Window_S
//{
//	int					_inuse;
//	int					hidden;
//	GFC_TextLine		name;
//	int					no_draw_generic;
//	GFC_List*			elements;
//	GFC_List*			focus_elements;
//	Element*			focus;
//	Sprite*				background;
//	Sprite*				border;
//	GFC_Rect			dimensions;
//	GFC_Rect			canvas;
//	GFC_Color			color;
//	int					blocks_input;
//	struct Window_S*	parent;
//	struct Window_S*	child;
//	void (*close_child)(struct Window_S* win, struct Window_S* child);
//	int (*update)(struct Window_S* win);
//	int(*free_data)(struct Window_S* win);
//	void* data;
//}Windows;
//
///*
//Element Data:
//-Id number needs to be unique
//Name - should be unique
//Position - RElative to window
//Color, alpha
//
//LabelData
//	text, font style, alignment
//
//imagelabeldata
//	Sprite, frame, scaling, rotation
//
//ButtonData
//	State (None, highlight, pressed, released)
//	Label and Sprite
//
//
//Labels
//Sprites
//Buttons
//Text
//Sliders
//Element List
//*/
//
///*
//Managed Menu Systen
//List of windows....more of a stack
//Draw back to front
//Update front to back
//
//Menu
//-Poisition
//-List of elements
//Child Windows/Parent Windows
//
//
//WindowUpdates
//
//int main_menu_update(Window *self, List* updatedElements)
//{
//	iterated through the updatedElements and handle any updated things
//	also do any custom logic here
//	return true if i handled input, false if another window can do stuff
//
//}
//
//window system
//Updates:
//foreach window
//	ForEach Element
//		Call the element Update Function
//
//
//*/
//void window_system_close();
//
//Windows* window_init(Uint32 maxWin);
//
//void window_system_clear_all();
//
//void window_free(Entity* self);


#endif