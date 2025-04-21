#include "simple_json.h"

#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "gfc_color.h"
#include "SDL_mouse.h"
#include "windows.h"
#include "elements.h"
#include "button.h"

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
	window_system.window_list = gfc_allocate_array(sizeof(Entity), maxEnts);
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
	 
	win = window_new();
	
	win->color = GFC_COLOR_BLUE;
	win->size = gfc_rect(0, 0, 1200, 720);
	gf2d_draw_rect_filled(win->size, win->color);
	GFC_TextLine text = "Start";
	//Button* but = button_new_button(gfc_vector2d(400, 100), gfc_vector2d(200, 60), GFC_COLOR_RED, text);
	// Optionally: add `but` to `win->sprites` or another list to manage them
	but = button_new_button(gfc_vector2d(400, 100), gfc_vector2d(200,60), GFC_COLOR_RED);
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

//Button* button_new()
//{
//	int i = 0;
//	for (i = 0; i < button_system.button_max; i++)
//	{
//		if (button_system.button_list[i]._inuse) continue;	//Skip any active entities
//		memset(&button_system.button_list[i], 0, sizeof(Entity));
//		button_system.button_list[i]._inuse = 1;
//		//Setting the default color
//		//Setting the default scale
//		return &button_system.button_list[i];
//	}
//	slog("No more available buttons");
//	return NULL;
//}
//
//
//
//
//void button_system_init(Uint32 maxEnts)
//{
//	if (button_system.button_list)
//	{
//		slog("Cannot have two instances of button manager, one is already active");
//		return;
//	}
//	if (!maxEnts)
//	{
//		slog("cannot intialize an Entity manager for Zero buttons!");
//		return;
//	}
//	button_system.button_list = gfc_allocate_array(sizeof(Entity), maxEnts);
//	if (!button_system.button_list)
//	{
//		slog("Failed to allocate global button list");
//		return;
//	}
//	button_system.button_max = maxEnts;
//	atexit(button_system_close);
//}
//
//Button* button_new_button(GFC_Vector2D position, GFC_Vector2D size, GFC_Color color)
//{
//	Button* but;
//	but = button_new();
//	but->position = position;
//	but->size = gfc_rect(position.x, position.y, size.x, size.y);  // Apply position to rectangle
//	gfc_vector2d_copy(but->position, position);
//	but->color = color;
//	but->isHover = 0;
//	but->isButton = 1;
//	gf2d_draw_rect_filled(but->size, but->color);
//}
//
//void button_system_close()
//{
//	button_system_clear_all();
//	if (button_system.button_list != NULL)
//	{
//		free(button_system.button_list);
//	}
//
//	/*window_system.window_list = NULL;
//	window_system.window_max = 0;
//	slog("Entity system closed");*/
//	memset(&button_system, 0, sizeof(ButtonSystem));
//}
//
//void button_system_clear_all()
//{
//	int i;
//	for (i = 0; i < button_system.button_max; i++)
//	{
//		if (!button_system.button_list[i]._inuse) continue;	//Skip this iteration of the loop
//		button_free(&button_system.button_list[i]);
//
//	}
//}
//
//void button_free(Entity* self)
//{
//	if (!self) return;
//	self->_inuse--;
//}
//
//
//Uint8 button_hover_check()
//{
//
//}
//Button* button_new(GFC_Rect size, GFC_Color color)
//{
//
//}

//
////typedef struct
////{
////	int				selectedOption;
////	Sprite* background;
////	GFC_TextLine	
////	Windows* win;
////} MainMenuData;
//
//
//
//typedef enum
//{
//	WDS_Tilted,
//	WDS_Stretched,
//	WDS_Max
//} WindowDrawStyle;
//
//
////typedef struct {
////	Sprite* generic_border;
////	Sprite* generic_background;
////	WindowDrawStyle	style;
////	Windows* window_list;
////	int window_max;
////	GFC_List* window_deque;
////	int drawbounds;
////	HashMap* sounds;
////}WindowManager;
//
//typedef struct {
//	Sprite*				generic_border;
//	Sprite*				generic_background;
//	//WindowDrawStyle		style;
//	Windows*			window_list;
//	int					window_max;
//	GFC_List*			window_deque;
//	int					drawbounds;
//}WindowManager;
//
//static WindowManager window_manager = { 0 }; /**<Initalize a LOCAL global window manager*/
//
//Windows* window_init(Uint32 maxWin)
//{
//	GFC_TextLine	background, border;
//	if (window_manager.window_list)
//	{
//		slog("Cannot have two instances of window manager, one is already active");
//		return;
//	}
//	if (!maxWin)
//	{
//		slog("cannot intialize an window manager for Zero windows!");
//		return;
//	}
//	window_manager.window_list = (Windows*)malloc(sizeof(Windows) * maxWin);
//	if (window_manager.window_list == NULL)
//	{
//		slog("Failed to allocate memory for window system");
//		return;
//	}
//	memset(window_manager.window_list, 0, sizeof(Windows) * maxWin);
//	window_manager.window_max = maxWin;
//	window_manager.window_deque = gfc_list_new();
//	//defaults
//	//gfc_line_cpy(background, "images/ui/window_background.png");
//	//gfc_line_cpy(border, "images/ui/window_border.png");
//	atexit(window_system_close);
//}
//
//void window_system_close()
//{
//	window_system_clear_all();
//	if (window_manager.window_list != NULL)
//	{
//		free(window_manager.window_list);
//	}
//
//	/*window_system.window_list = NULL;
//	window_system.window_max = 0;
//	slog("Entity system closed");*/
//	memset(&window_manager, 0, sizeof(WindowManager));
//}
//
//void window_system_clear_all()
//{
//	int i;
//	for (i = 0; i < window_manager.window_max; i++)
//	{
//		if (!window_manager.window_list[i]._inuse) continue;	//Skip this iteration of the loop
//		window_free(&window_manager.window_list[i]);
//
//	}
//}
//
//
//void window_free(Entity* self)
//{
//	if (!self) return;
//	self->_inuse--;
//}
//
//
//Windows* window_new()
//{
//	int i = 0;
//	for (i = 0; i < window_manager.window_max; i++)
//	{
//		if (window_manager.window_list[i]._inuse) continue;	//Skip any active entities
//		memset(&window_manager.window_list[i], 0, sizeof(Windows));
//		window_manager.window_list[i]._inuse = 1;
//		//Setting the default color
//		//Setting the default scale
//		return &window_manager.window_list[i];
//	}
//	slog("No more available effects");
//	return NULL;
//}
//
//
//
////Windows* main_menu()
////{
////	Windows* win;
////	MainMenuData* data;
////	win = gf2d_window_load(json);
////	if (!win)
////	{
////		slog("Failed to load edior menu");
////		return NULL;
////	}
////	win->update = main_menu_update;
////	win->free_data = main_menu_free;
////	win->draw = main_menu_draw;
////	data = (MainMenuData*)gfc_allocate_array(sizeof(MainMenuData), 1);
////	data->background = gf2d_sprite_load_image("image");
////	gfc_line_cpy(data->filename, "saves/");
////	gf2d_window_set_focus_to(win, gf2d_window_get_element_by_name(win, "newgame"));
////	win->data = data;
////	data->Windows = win;
////	data->selectedOption = 1;
////	return win;
////}
//
//
////void windows_init(int max_windows, const char* config)
////{
////	GFC_TextLine background, border;
////	const char* str;
////	GFC_Vector2D borderSize = { 64, 64 };
////	int borderFPL = 8;
////	SJson* file, * window, * sounds;
////	if (max_windows <= 0)
////	{
////		slog("Cannot Initilize window system for 0 windows");
////		return;
////	}
////	window_manager.window_list = (Windows*)malloc(sizeof(Windows) * max_windows);
////	if (window_manager.window_list == NULL)
////	{
////		slog("Failed to allocate memory for window system");
////		return;
////	}
////	memset(window_manager.window_list, 0, sizeof(Windows) * max_windows);
////	window_manager.window_max = max_windows;
////	window_manager.window_deque = gfc_list_new();
////	//defaults
////	gfc_line_cpy(background, "images/ui/window_background.png");
////	gfc_line_cpy(border, "images/ui/window_border.png");
////
////	//config overrides
////	if (config) {
////		file = gfc_pak_load_json(config);
////		if (file)
////		{
////			window = sj_object_get_value(file, "window");
////			if (window)
////			{
////				str = sj_get_string_value(sj_object_get_value(window, "background"));
////				if (str) gfc_line_cpy(background, str);
////				str = sj_get_string_value(sj_object_get_value(window, "border"));
////				if (str) gfc_line_cpy(border, str);
////				sj_value_as_vector2d(sj_object_get_value(window, "borderSize"), &borderSize);
////				sj_get_integer_value(sj_object_get_value(window, "borderFPL"), &borderFPL);
////				str = sj_get_string_value(sj_object_get_value(window, "drawStyle"));
////				if (str)
////				{
////					if (strcmp(str, "tilted") == 0) window_manager.style = WDS_Tilted;
////					else if (strcmp(str, "stretched") == 0)window_manager.style = WDS_Stretched;
////				}
////			}
////			sounds = sj_object_get_value(file, "sounds");
////			if (sounds)
////			{
////				window_manager.sounds = gfc_sound_pack_parse(sounds);
////			}
////		}
////		sj_free(file);
////	}
////	window_manager.generic_background = gf2d_sprite_load_image(background);
////	window_manager.generic_border = gf2d_sprite_load_all(border, borderSize.x,borderSize.y,borderFPL,false);
////	window_manager.drawbounds = 0;
////	slog("Window system initilized");
////	atexit(windows_close);
////
////}