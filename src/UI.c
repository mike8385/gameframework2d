#include "simple_json.h"

#include "UI.h"
#include "player.h"
#include "gf2d_sprite.h"


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


