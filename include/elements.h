//#ifndef __ELEMENTS_H__
//#define __ELEMENTS_H__
//
//
//#include <simple_json.h>
//#include "gfc_list.h"
//#include "gfc_color.h"
//#include "gfc_shape.h"
//#include "gfc_text.h"
//#include "windows.h"
//
//
//
//typedef enum
//{
//	ES_idle,
//	ES_disable,
//	ES_highlight,
//	ES_hidden,
//	ES_active
//}ElementState;
//
//typedef enum
//{
//	EUR_None,
//	EUR_Handled
//}ElementUpdateReturn;
//
//typedef enum
//{
//	ET_List,
//	ET_Label,
//	ET_Actor,
//	ET_Button,
//	ET_Entry,
//	ET_Percent
//}ElementTypes;
//
//struct Element_S
//{
//	int				index;			/**<order of highlight in the menu, -e for does not recive highlight*/
//	GFC_TextLine	name;			/**<*/
//
//	Uint8			canHasFocus;	/**<If true, this element can be the focus for keyboard input*/
//	Uint8			hasFocus;		/**<If true, this element does have focus*/
//	GFC_Rect		bounds;			/**<Drawing bounds for the element*/
//	GFC_Vector2D	lastDrawPosition;
//	GFC_Color		color;
//
//	GFC_Color		backgroundColor;
//	int				backgroundDraw;
//
//	int state;
//	int type;
//	void(*draw);
//
//	// Element_S*		(*get_next)(struct Element_S* element, struct Element_S* from);
//	GFC_List* (*update)		(struct Element_S* element, GFC_Vector2D offset);
//	void(*free_data)		(struct Element_S* element);
//	struct Element_S*		(*get_by_name)(struct Element_S* element, const char* name);
//	Windows*				win;		/**<My parent Window*/
//	void*					data;	
//};
//
///**
//*@brief allocate and initalize a new element
//* @return NULL on error or a new element otherwise
//*/
//Element* gf2d_element_new();
//
///**
//* @brief allocate and set values for a new element
//* @param index the index of the element
//* @param name the name of the element
//* @param bounds the drawing bounds of the element
//* @param color the draw color of the element
//* @param state the initial state of the element
//* @param backgroundColor the color to draw for the background
//* @param backgroundDraw if true, draw a background for the element
//* @param win the window this element ultimately belongs to
//* @return NULL on error or a new element otherwise
//*/
//Element* gf2d_element_new_full(
//	Element*		parent,
//	int				index,
//	GFC_TextLine	name,
//	GFC_Rect		bounds,
//	GFC_Color		color,
//	int				state,
//	GFC_Color		backgroundColor,
//	int				backgroundDraw,
//	Windows*		win
//);
//
//#endif
