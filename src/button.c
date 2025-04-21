#include "simple_json.h"

#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "gfc_color.h"
#include "SDL_mouse.h"
#include "elements.h"
#include "button.h"



typedef struct
{
	Uint32 button_max;
	Button* button_list;
}ButtonSystem;

static ButtonSystem button_system = { 0 }; /**<Initalize a LOCAL global window manager*/


Button* button_new()
{
	int i = 0;
	for (i = 0; i < button_system.button_max; i++)
	{
		if (button_system.button_list[i]._inuse) continue;	//Skip any active entities
		memset(&button_system.button_list[i], 0, sizeof(Button));
		button_system.button_list[i]._inuse = 1;
		//Setting the default color
		//Setting the default scale
		return &button_system.button_list[i];
	}
	slog("No more available buttons");
	return NULL;
}




void button_system_init(Uint32 maxEnts)
{
	if (button_system.button_list)
	{
		slog("Cannot have two instances of button manager, one is already active");
		return;
	}
	if (!maxEnts)
	{
		slog("cannot intialize an Entity manager for Zero buttons!");
		return;
	}
	button_system.button_list = gfc_allocate_array(sizeof(Button), maxEnts);
	if (!button_system.button_list)
	{
		slog("Failed to allocate global button list");
		return;
	}
	button_system.button_max = maxEnts;
	atexit(button_system_close);
}

Button* button_new_button(GFC_Vector2D position, GFC_Vector2D size, GFC_Color color, GFC_TextLine text)
{
	Button* but;
	but = button_new();
	but->position = position;
	but->size = gfc_rect(position.x, position.y, size.x, size.y);  // Apply position to rectangle
	gfc_vector2d_copy(but->position, position);
	but->color = color;
	but->isHover = 0;
	but->isButton = 1;
	gf2d_draw_rect_filled(but->size, but->color);
	
	if (text)
	{
		strcpy(but->text, text);
	}
	button_draw_text(text);
	return but;
}

void button_system_close()
{
	button_system_clear_all();
	if (button_system.button_list != NULL)
	{
		free(button_system.button_list);
	}

	/*window_system.window_list = NULL;
	window_system.window_max = 0;
	slog("Entity system closed");*/
	memset(&button_system, 0, sizeof(ButtonSystem));
}

void button_system_clear_all()
{
	int i;
	for (i = 0; i < button_system.button_max; i++)
	{
		if (!button_system.button_list[i]._inuse) continue;	//Skip this iteration of the loop
		button_free(&button_system.button_list[i]);

	}
}

void button_free(Button* self)
{
	if (!self) return;
	self->_inuse--;
}


void button_system_think()
{
	int i;
	for (i = 0; i < button_system.button_max; i++)
	{
		if (!button_system.button_list[i]._inuse) continue;
		button_think(&button_system.button_list[i]);
	}
}

void button_system_update()
{
	int i;
	for (i = 0; i < button_system.button_max; i++)
	{
		if (!button_system.button_list[i]._inuse) continue;
		button_update(&button_system.button_list[i]);
	}
}

Uint8 button_hover_check(GFC_Rect button)
{
	int mx, my;

	SDL_GetMouseState(&mx, &my);

		if (gfc_point_in_rect(gfc_vector2d(mx, my), button))
		{
			return 1;
		}
		else
		{
			return 0;
		}
}

void button_system_draw()
{
	for (int i = 0; i < button_system.button_max; i++)
	{
		Button* b = &button_system.button_list[i];
		if (!b->_inuse) continue;
		gf2d_draw_rect_filled(b->size, b->color);
		button_draw_text(b->text);
	}
}


void button_draw_text(GFC_TextLine text)
{
	//text
}

void button_think(Button* self)
{
	if (!self) return;
	self->isHover = button_hover_check(self->size);
	//slog("%d", self->isHover);
}

void button_update(Button* self)
{
	if (!self) return;

	button_click_actions(self);

	//if (self->update)self->update(self);
	//entity_bounds_update(self);
}

void button_click_actions(Button* self)
{
	int mx, my;

	if (!self) return;
	if (self->isHover)
	{
		if (SDL_GetMouseState(&mx, &my))
		{
			slog("Button Clicked");
			slog ("%d", self->isHover);
		}
	}
}