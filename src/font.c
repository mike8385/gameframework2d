#include <SDL_ttf.h>
#include "simple_logger.h"
#include "gfc_list.h"
#include "gfc_color.h"

#include "gf2d_graphics.h"
#include "font.h"

typedef struct
{
	char* text;
	FontStyles style;
	GFC_Color color;
	SDL_Texture* texture;
	GFC_Vector2D size;
	Uint32 timestamp;
}FontCache;

typedef struct
{
	GFC_List* fonts;
	//GFC_List* recents;
	//TTF_Font * TTF_OpenFont(const char* file, int ptsize);
}FontManager;

static FontManager font_manager = { 0 };


//void font_cache_free(FontCache* cache);


void font_close()
{
	//Other Cleanup First
	TTF_Font* font;
	FontCache* cache;
	int i, c;
	c = gfc_list_get_count(font_manager.fonts);
	for (i = 0; i < c; i++)
	{
		font = gfc_list_get_nth(font_manager.fonts, i);
		if (!font)continue;
		TTF_CloseFont(font);
	}
	gfc_list_delete(font_manager.fonts);

	//c = gfc_list_get_count(font_manager.recents);
	//for (i = 0; i < c; i++)
	//{
	//	cache = gfc_list_get_nth(font_manager.recents, i);
	//	if (!cache)continue;
	//	font_cache_free(cache);
	//}
	//gfc_list_delete(font_manager.recents);

	memset(&font_manager, 0, sizeof(FontManager));
	TTF_Quit();
}

void font_init()
{
	int i;
	TTF_Font* font;
	SDL_Surface* surface;
	SDL_Texture* texture;
	if (TTF_Init() != 0)
	{
		slog("Failed to initialize TTF system");
		return;
	}
	font_manager.fonts = gfc_list_new();
	//font_manager.recents = gfc_list_new();
	//Error check here
	for (i = 0; i < FS_MAX; i++)
	{
		font = TTF_OpenFont("fonts/Enchanted_Land.otf", 40 + (1 * 4));
		if (!font)
		{
			slog("Failed to open font donts/Enchanted_Land.otf");
			continue;
		}
		gfc_list_append(font_manager.fonts, font);
	}
	atexit(font_close);
}

//void font_cache_free(FontCache* cache)
//{
//	if (!cache)return;
//	if (cache->text)free(cache->text);
//	if (cache->texture)SDL_DestroyTexture(cache->texture);
//	free(cache);
//}

//void font_cleanup()
//{
//	Uint32 now;
//	int i, c;
//	//FontCache* cache;
//	now = SDL_GetTicks();
//	c = gfc_list_get_count(font_manager.recents);
//	for (i = c - 1; i >= 0; i--)
//	{
//		//cache = gfc_list_get_nth(font_manager.recents, i);
//		//if (!cache) continue;
//		if (now > cache->timestamp + 1000)
//		{
//			font_cache_free(cache);
//			gfc_list_delete_nth(font_manager.recents, i);
//		}
//	}
//}

//void font_add_recent(const char* text, FontStyles style, GFC_Color color, SDL_Texture* texture, GFC_Vector2D size)
//{
//	FontCache* cache;
//	size_t length;
//
//	if ((!text) || (!texture)) return;
//	cache = gfc_allocate_array(sizeof(FontCache), 1);
//	if (!cache)return;
//	cache->texture = texture;
//	gfc_vector2d_copy(cache->size, size);
//	cache->style = style;
//	length = strlen(text) + 1;
//	cache->text = gfc_allocate_array(sizeof(char), length);
//	strncpy(cache->text, text, length);
//	cache->timestamp = SDL_GetTicks();
//	gfc_color_copy(cache->color, color);
//	gfc_list_append(font_manager.recents, cache);
//}

//FontCache *font_get_recent(const char* text, FontStyles style, GFC_Color color)
//{
//	FontCache* cache;
//	int i, c;
//	if (!text) return NULL;
//	c = gfc_list_get_count(font_manager.recents);
//	for (i = 0; i < c; i++)
//	{
//		cache = gfc_list_get_nth(font_manager.recents, i);
//		if (!cache) continue;
//		if (gfc_strlcmp(text, cache->text) != 0)continue;
//		if (cache->style != style) continue;
//		if (!gfc_color_cmp(color, cache->color))continue;
//		return cache;
//	}
//}

void font_draw_text(const char* text, FontStyles style, GFC_Color color, GFC_Vector2D position)
{
	TTF_Font* font;
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect rect;
	SDL_Color fg;
	//FontCache* cache;


	/*cache = font_get_recent(text, style, color);
	if (cache)
	{
		rect.x = position.x;
		rect.y = position.y;
		rect.w = cache->size.x;
		rect.h = cache->size.y;
		SDL_RenderCopy(gf2d_graphics_get_renderer(),
			cache->texture,
			NULL,
			&rect);
		cache->timestamp = SDL_GetTicks();
		return;
	}*/

	font = gfc_list_get_nth(font_manager.fonts, style);
	if (!font)
	{
		slog("Failed to render text %s, missing font style %i", text, style);
		return;
	}
	fg = gfc_color_to_sdl(color);
	surface = TTF_RenderUTF8_Blended_Wrapped(font, text, fg, 0);
	if (!surface)
	{
		slog("Error");
		return;
	}
	surface = gf2d_graphics_screen_convert(&surface);
	if (!surface)
	{
		slog("Error");
		return;
	}
	texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surface);
	if (!texture)
	{
		SDL_FreeSurface(surface);
		slog("This error");
		return;
	}
	rect.x = position.x;
	rect.y = position.y;
	rect.w = surface->w;
	rect.h = surface->h;
	SDL_RenderCopy(gf2d_graphics_get_renderer(),
		texture,
		NULL,
		&rect);
	SDL_FreeSurface(surface);
	//font_add_recent(text, style, color, texture, gfc_vector2d(rect.w, rect.h));

 }

