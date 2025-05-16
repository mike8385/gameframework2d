#include "simple_json.h"

#include "UI.h"
#include "player.h"
#include "gf2d_sprite.h"
#include "button.h"
#include "player.h"

void UI_health_bar(GFC_Rect box)
{

	Entity* player;
	float playerHealth = get_player_health();
	Sprite* sprite;
	sprite = gf2d_sprite_load_image("images/UI/fullhealth.png");
	gf2d_sprite_draw_image(sprite, gfc_vector2d(box.x, box.y));
	gf2d_draw_rect(box, GFC_COLOR_BLACK);
	//slog("Health Update: %f", playerHealth);
	if (playerHealth == 50.0f)
	{
		//slog("Health Update: %f", playerHealth);

		gf2d_sprite_free(sprite);
		sprite = gf2d_sprite_load_image("images/UI/5health.png");
		gf2d_sprite_draw_image(sprite, gfc_vector2d(box.x, box.y));
	}
	if (playerHealth == 40.0f)
	{
		//slog("Health Update: %f", playerHealth);
		//sprite = sprite
		gf2d_sprite_free(sprite);
		sprite = gf2d_sprite_load_image("images/UI/4health.png");
		gf2d_sprite_draw_image(sprite, gfc_vector2d(box.x, box.y));
	}
	if (playerHealth == 30.0f)
	{
		//slog("Health Update: %f", playerHealth);

		gf2d_sprite_free(sprite);
		sprite = gf2d_sprite_load_image("images/UI/halfhealth.png");
		gf2d_sprite_draw_image(sprite, gfc_vector2d(box.x, box.y));
	}
	if (playerHealth == 20.0f)
	{
		//slog("Health Update: %f", playerHealth);

		gf2d_sprite_free(sprite);
		sprite = gf2d_sprite_load_image("images/UI/2health.png");
		gf2d_sprite_draw_image(sprite, gfc_vector2d(box.x, box.y));
	}
	if (playerHealth == 10.0f)
	{
		//slog("Health Update: %f", playerHealth);

		gf2d_sprite_free(sprite);
		sprite = gf2d_sprite_load_image("images/UI/1health.png");
		gf2d_sprite_draw_image(sprite, gfc_vector2d(box.x, box.y));
	}
	if (playerHealth == 0)
	{
		//slog("Health Update: %f", playerHealth);

		gf2d_sprite_free(sprite);
		sprite = gf2d_sprite_load_image("images/UI/nohealth.png");
		gf2d_sprite_draw_image(sprite, gfc_vector2d(box.x, box.y));
	}

}

void UI_stats_bar()
{

	Button*	strengthButton;
	Button*	magicButton;
	Button*	defenseButton;
	Button*	speedButton;
	Button*	luckButton;
	Button*	EXPButton;
	char		text[25];
	GFC_Rect	magicUI;
	GFC_Rect	defenseUI;
	GFC_Rect	speedUI;
	GFC_Rect	luckUI;
	GFC_Rect	EXPUI;
	Stats* playerStats;

	playerStats = get_player_stats();

	//strengthUI = gfc_rect(350, 15, 70, 75);
	sprintf(text, "%d", get_player_stats()->strength);
	//slog("Button Created");
	strengthButton = button_new_button_text_named(gfc_vector2d(350, 15), gfc_vector2d(70, 75), GFC_COLOR_BROWN, text, "strengthButton");
	//slog("Button Created");
	sprintf(text, "%d", get_player_stats()->magic);
	magicButton = button_new_button_text_named(gfc_vector2d(350+70, 15), gfc_vector2d(70, 75), GFC_COLOR_LIGHTPURPLE, text, "magicButton");
	//slog("Button Created");
	//sprintf(text, "%d", get_player_stats()->strength);
	//strengthButton = button_new_button_text(gfc_vector2d(350, 15), gfc_vector2d(70, 75), GFC_COLOR_BROWN, text, strengthButton);


}


UI_stats_bar_update()
{
	Stats* stats = get_player_stats();
	Button* strength = button_get_by_name("strength");
	if (strength)
	{
		sprintf(strength->text, "%d", stats->strength);
	}

	Button* magic = button_get_by_name("magic");
	if (magic)
	{
		sprintf(magic->text, "%d", stats->magic);
	}

}