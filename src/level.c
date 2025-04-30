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
#include "UI.h"
#include "items.h"
#include "world.h"

int setup = 0; //Initilizes if I need to set up the characters and game info.
Sprite* sprite = NULL;
Sprite* mouse = NULL;
World* world = NULL;
Window* window = NULL;
int mx = 0, my = 0;
float mf = 0;
GFC_Color mouseGFC_Color;
GFC_Vector2D camera;
Uint32 mb;
///Item* items;



void game_draw()
{
    mf += 0.1;
    if (mf >= 16.0)mf = 0;
    SDL_GetMouseState(&mx, &my);
    // Start frame
    gf2d_graphics_clear_screen();

    // Draw background
    gf2d_sprite_draw_image(sprite, gfc_vector2d(0, 0));

    // Draw world or window depending on scene
    switch (process)
    {
    case 1:
        world_draw(world);          // if you're in the game
        entity_system_draw();       // draw player, monsters, etc
        items_draw_all();  // draw items after the world but before mouse

        break;
    case 2:
        world_draw(world);          // if you're in the game
        entity_system_draw();       // draw player, monsters, etc
        items_draw_all();  // draw items after the world but before mouse

        break;
    default:
        window_draw(window);        // menu window
        button_system_draw();       // draw buttons
        break;
    }


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
    Item* items;
    //SJson* wjson;
   // const char* spriteImage;
    
    gf2d_sprite_free(mouse);
    mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16, 0); // <-- Reload mouse sprite here

    camera_set_size(gfc_vector2d(1200, 720));
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    world = world_load("maps/testworld.txt");

    
    //items = item_new("waffle");
    //items_place(items, gfc_vector2d(400, 700));

    //world = world_test_new();

    GFC_Vector2D position = gfc_vector2d(100.0f, 700.0f);
    GFC_Vector2D monsterposition = gfc_vector2d(700.0f, 700.0f);
    GFC_Rect rectangle = gfc_rect(100, 100, 1000, 500);

    player = player_new_entity(position);
    monster = monster_new_entity(monsterposition);
    fire_wizard = fire_wizard_new_entity(gfc_vector2d(1500, 700));
    ice_wizard = ice_wizard_new_entity(gfc_vector2d(2000, 700));
    fast_wizard = fast_wizard_new_entity(gfc_vector2d(2500, 700));
    melee_wizard = melee_wizard_new_entity(gfc_vector2d(1000, 700));
    boss_wizard = end_boss_new_entity(gfc_vector2d(3200, 700));
    pinkMonster = pets_new_entity(monsterposition);


   // slog("Here");

}

void level_free()
{
    entity_system_clear_all(); // Clear all entities like player, monster, etc.
    items_clear_all();         // Clear all items like waffle

    // Set all entity pointers to NULL after clearing
    player = NULL;
    monster = NULL;
    fire_wizard = NULL;
    ice_wizard = NULL;
    fast_wizard = NULL;
    melee_wizard = NULL;
    boss_wizard = NULL;

    if (world)
    {
        world_free(world);
        world = NULL;
    }

    setup = 0;
}


void level_main_menu()
{
    if (!setup)
    {
        mouseGFC_Color = gfc_color8(255, 100, 255, 200);
        if (mouse) gf2d_sprite_free(mouse); // Free old mouse sprite if it exists
        mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16, 0);

        if (window) window_free(window); // Free old window if it somehow exists
        window = main_menu();

        setup = 1;
    }

    button_system_think();
    button_system_update();
}


void level_editor()
{
    if (!setup)
    {

        level_editor_setup();
        setup = 1;
    }
    // Handle mouse-based camera manually (no player update)
    mb = SDL_GetMouseState(&mx, &my);
    camera_center_on_mouse();
    Item* items = item_new("waffle");

    // Allow placing tiles/items/buttons if you want
    button_system_think();
    button_system_update();

    gfc_input_update();

    if (mb)
    {
        camera = camera_get_position();
        //world_set_tile(world, gfc_vector2d(mx + camera.x, my + camera.y),(mb & SDL_BUTTON(1))?1:0);
        world_set_item(world, gfc_vector2d(mx + camera.x, my + camera.y), items);

    }
}


void level_editor_setup()
{
    //Item* items;
    //SJson* wjson;
   // const char* spriteImage;

    gf2d_sprite_free(mouse);
    mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16, 0); // <-- Reload mouse sprite here

    camera_set_size(gfc_vector2d(1200, 720));
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    world = world_load("maps/testworld.txt");


    //items_place(items, gfc_vector2d(400, 700));

    //world = world_test_new();

    GFC_Vector2D position = gfc_vector2d(100.0f, 700.0f);
    //GFC_Vector2D monsterposition = gfc_vector2d(700.0f, 700.0f);
    //GFC_Rect rectangle = gfc_rect(100, 100, 1000, 500);

    player = player_new_entity(position);
    //monster = monster_new_entity(monsterposition);
    //fire_wizard = fire_wizard_new_entity(gfc_vector2d(1500, 700));
    //ice_wizard = ice_wizard_new_entity(gfc_vector2d(2000, 700));
    //fast_wizard = fast_wizard_new_entity(gfc_vector2d(2500, 700));
    //melee_wizard = melee_wizard_new_entity(gfc_vector2d(1000, 700));
    //boss_wizard = end_boss_new_entity(gfc_vector2d(3200, 700));

    //world = world_test_new();

}

void level_editor_free()
{
    world_save(world, "maps/testsave.map");
    entity_system_clear_all(); // Clear all entities like player, monster, etc.
    items_clear_all();         // Clear all items like waffle

    // Set all entity pointers to NULL after clearing
    player = NULL;
    monster = NULL;
    fire_wizard = NULL;
    ice_wizard = NULL;
    fast_wizard = NULL;
    melee_wizard = NULL;
    boss_wizard = NULL;

    if (world)
    {
        world_free(world);
        world = NULL;
    }

    setup = 0;
}
