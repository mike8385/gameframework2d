#include "simple_json.h"

#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "gfc_color.h"
#include "SDL_mouse.h"
#include "windows.h"
#include "elements.h"
#include "button.h"
#include "font.h"

typedef struct {
	Uint32 window_max;
	Window* window_list;
}WindowSystem;

typedef struct
{
	Uint32 button_max;
	Button* button_list;
}ButtonSystem;

static WindowSystem window_system = { 0 }; /**<Initalize a LOCAL global window manager*/
//static ButtonSystem button_system = { 0 }; /**<Initalize a LOCAL global window manager*/


Window* window_new()
{
	int i = 0;
	for (i = 0; i < window_system.window_max; i++)
	{
		if (window_system.window_list[i]._inuse) continue;	//Skip any active entities
		memset(&window_system.window_list[i], 0, sizeof(Window));
		window_system.window_list[i]._inuse = 1;
		//Setting the default color
		//Setting the default scale
		window_system.window_list[i].buttons = gfc_list_new();

		return &window_system.window_list[i];
	}
	slog("No more available windows");
	return NULL;
}


void window_system_init(Uint32 maxEnts)
{
	button_system_init(1024);
	if (window_system.window_list)
	{
		slog("Cannot have two instances of window manager, one is already active");
		return;
	}
	if (!maxEnts)
	{
		slog("cannot intialize an Entity manager for Zero Windows!");
		return;
	}
	window_system.window_list = gfc_allocate_array(sizeof(Window), maxEnts);
	if (!window_system.window_list)
	{
		slog("Failed to allocate global window list");
		return;
	}
	window_system.window_max = maxEnts;
	atexit(window_system_close);
}



Window* main_menu()
{
	Window*		win;
	Button*		but;
	Button*		exit;
	Button*		edit;
	Button*		multiplayer;
	GFC_TextLine text;
	GFC_TextLine text2;


	win = window_new();

	win->color = GFC_COLOR_BLUE;
	win->size = gfc_rect(0, 0, 1200, 720);
	gf2d_draw_rect_filled(win->size, win->color);
	
	gfc_line_cpy(text, "New    Game");
	gfc_line_cpy(text2, "Exit");
	//Button* but = button_new_button(gfc_vector2d(400, 100), gfc_vector2d(200, 60), GFC_COLOR_RED, text);
	// Optionally: add `but` to `win->sprites` or another list to manage them
	but = button_new_button_text(gfc_vector2d(400, 100), gfc_vector2d(200, 60), GFC_COLOR_PURPLE, text);
	exit = button_new_button_text(gfc_vector2d(700, 100), gfc_vector2d(200, 60), GFC_COLOR_RED, text2);
	edit = button_new_button_text(gfc_vector2d(550, 300), gfc_vector2d(200, 60), GFC_COLOR_PINK, "Edit");
	multiplayer = button_new_button_text(gfc_vector2d(550, 500), gfc_vector2d(200, 60), GFC_COLOR_ORANGE, "Multiplayer");


	but->type = BT_NewGame;
	exit->type = BT_Exit;
	edit->type = BT_Edit;
	multiplayer->type = BT_Multi;
	gfc_list_append(win->buttons, but);
	gfc_list_append(win->buttons, exit);
	return win;
}



void window_system_close()
{
	window_system_clear_all();
	if (window_system.window_list != NULL)
	{
		free(window_system.window_list);
	}

	/*window_system.window_list = NULL;
	window_system.window_max = 0;
	slog("Entity system closed");*/
	memset(&window_system, 0, sizeof(WindowSystem));
}



void window_system_clear_all()
{
	int i;
	for (i = 0; i < window_system.window_max; i++)
	{
		if (!window_system.window_list[i]._inuse) continue;	//Skip this iteration of the loop
		window_free(&window_system.window_list[i]);

	}
}

void window_free(Window* self)
{
	if (!self) return;
	self->_inuse--;
}

void window_draw(Window* win)
{
	if (!win) return;
	gf2d_draw_rect_filled(win->size, win->color);
}

