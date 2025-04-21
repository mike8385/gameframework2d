#include <SDL.h>
#include "simple_logger.h"
#include "gfc_config_def.h"
#include "gf2d_graphics.h"

#include "font.h"
#include "gfc_color.h"
#include "camera.h"

#include "level.h"
#include "button.h"
#include "windows.h"
#include "gfc_color.h"

int setup = 0; //Initilizes if I need to set up the characters and game info.
Sprite* sprite = NULL;
Sprite* mouse = NULL;
World* world = NULL;
Window* window = NULL;
int mx = 0, my = 0;
float mf = 0;
GFC_Color mouseGFC_Color;

void game_draw()
{
    // Start frame
    gf2d_graphics_clear_screen();

    // Draw background
    gf2d_sprite_draw_image(sprite, gfc_vector2d(0, 0));

    // Draw world or window depending on scene
    switch (process)
    {
    case 1:
        world_draw(world);          // if you're in the game
        entity_system_draw();       // draw player, monsters, etc.
        break;
    default:
        window_draw(window);        // menu window
        button_system_draw();       // draw buttons
        break;
    }

    font_draw_text("Press ESC to quit\n isnt that neat", FS_large, GFC_COLOR_CYAN, gfc_vector2d(10, 10));

    // Always draw mouse on top
    gf2d_sprite_draw(
        mouse,
        gfc_vector2d(mx, my),
        NULL, NULL, NULL, NULL,
        &mouseGFC_Color,
        (int)mf);

    // End frame
    gf2d_graphics_next_frame();
}


void level_process()
{

    if (!setup)
    {

        level_setup();
        setup = 1;
    }


    //slog("Here");
    entity_system_think();
    //slog("Here");

    entity_system_update();
    entity_system_move();
    //slog("Here");
    //entity_bounds_update(player);


    gfc_input_update();



}


void level_setup()
{
    camera_set_size(gfc_vector2d(1200, 720));
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    //world = world_load("maps/testworld.txt");

    world = world_test_new();

    GFC_Vector2D position = gfc_vector2d(100.0f, 500.0f);
    GFC_Vector2D monsterposition = gfc_vector2d(500.0f, 500.0f);
    GFC_Rect rectangle = gfc_rect(100, 100, 1000, 500);

    player = player_new_entity(position);
    monster = monster_new_entity(monsterposition);
    fire_wizard = fire_wizard_new_entity(gfc_vector2d(1000, 500));
    ice_wizard = ice_wizard_new_entity(gfc_vector2d(2000, 500));
    fast_wizard = fast_wizard_new_entity(gfc_vector2d(2500, 500));
    melee_wizard = melee_wizard_new_entity(gfc_vector2d(700, 500));
    boss_wizard = end_boss_new_entity(gfc_vector2d(2000, 500));
   // slog("Here");

}

void level_free()
{
    entity_free(player);
    world_free(world);
}

void level_main_menu()
{
    if (!setup)
    {
        mouseGFC_Color = gfc_color8(255, 100, 255, 200);
        mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16, 0);
        window = main_menu();

        setup = 1;
    }
    mf += 0.1;
    if (mf >= 16.0)mf = 0;
    SDL_GetMouseState(&mx, &my);

    button_system_think();
    button_system_update();
}


