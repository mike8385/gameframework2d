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
#include "particles.h"
#include "pets.h"
#include "player2.h"

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
Entity*             pets;
Entity*             playerTransition;
const char* current_item_name;
const char* filename;
Uint8 transition_flag;



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
        //button_system_update();
        button_system_draw();       // draw buttons

        break;
    case 2:
        world_draw(world);          // if you're in the game
        entity_system_draw();       // draw player, monsters, etc
        items_draw_all();  // draw items after the world but before mouse
        break;
    case 3:
        level_process();
        break;
    case 4:

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

    particle_system_draw();


    // End frame
    gf2d_graphics_next_frame();


}


void level_process()
{

    if (!setup)
    {
        currentLevel++;
        level_setup();
        setup = 1;
    }


    if (multiplayer)
    {
        camera_center_on_two_players(get_player_position(), get_player2_position());
    }
    else
    {
	    camera_center_on(player->position);
    }

    //slog("Here");
    entity_system_think();
    //slog("Here");

    entity_system_update();

    entity_system_move();
    button_system_think();
    button_system_update();
    items_collide_all();
    //UI_stats_bar_update();
    //slog("Here");
    //entity_bounds_update(player);


    gfc_input_update();



}


void level_setup()
{
    


    //Item* items;
    //SJson* wjson;
   // const char* spriteImage;
    
    gf2d_sprite_free(mouse);
    mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16, 0); // <-- Reload mouse sprite here

    camera_set_size(gfc_vector2d(1200, 720));
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    slog("filename: !!%s", filename);
    world = world_load(filename);
    //slog("Next World: %s", world_get_next_world()->background);
    slog("Initalize again");

    
    //items = item_new("waffle");
    //items_place(items, gfc_vector2d(400, 700));
    

    //world = world_test_new();

    position = gfc_vector2d(100.0f, 700.0f);
    GFC_Vector2D monsterposition = gfc_vector2d(700.0f, 700.0f);
    GFC_Rect rectangle = gfc_rect(100, 100, 1000, 500);

    if (!transition_flag) player = player_new_entity(position);
    if (multiplayer && !transition_flag)      player2 = player2_new_entity(gfc_vector2d(100.0f, 400.0f));


    

    //pet = pets_new("owl_monster");
    //pets_new_entity_placed(pet, gfc_vector2d(1500, 700));

    //pet = pets_new("ember_monster");
    //pets_new_entity_placed(pet, monsterposition);

    basic_wizard = basic_wizard_new_entity(monsterposition);
    fire_wizard = fire_wizard_new_entity(gfc_vector2d(1500, 400));
    ice_wizard = ice_wizard_new_entity(gfc_vector2d(2000, 500));
    fast_wizard = fast_wizard_new_entity(gfc_vector2d(2500, 800));
    melee_wizard = melee_wizard_new_entity(gfc_vector2d(1000, 600));
    boss_wizard = end_boss_new_entity(gfc_vector2d(3200, 700));
    /*pinkMonster = pets_new_entity(monsterposition);
    pinkMonster->hasPet = PT_Pink;
    slog("Pet count: %d", pinkMonster->count);*/
    //UI_stats_bar();



   // slog("Here");

}

void level_free()
{
    entity_system_clear_all(); // Clear all entities like player, monster, etc.
    items_clear_all();         // Clear all items like waffle
    button_system_clear_all();
    world_clear_tile_layer(world);
    //gf2d_sprite_clear_all();

    // Set all entity pointers to NULL after clearing
    //player = NULL;
    monster = NULL;
    fire_wizard = NULL;
    ice_wizard = NULL;
    fast_wizard = NULL;
    melee_wizard = NULL;
    boss_wizard = NULL;
    pet = NULL;
    sprite = NULL;

    if (world)
    {
        slog("Freeing old world");
        world_free(world);
        world = NULL;
    }

}

Uint8 level_transition_flag(Uint8 flag)
{
    transition_flag = flag;
    return transition_flag;
}

Uint8 get_level_transition_flag()
{
    return transition_flag;
}

void level_transition()
{
    //slog("Inside transition");

    player->_inuse = 0;
    if (multiplayer)player2->_inuse = 0;
    level_free();
    player->position = position;
    player->_inuse = 1;
    if (multiplayer)player2->_inuse = 1;
    //slog("Filename: %s", world_get_next_world());
    //strcpy(filename, world_get_next_world());
    //slog("Filename: %s", world_get_next_world());
    if (strcmp("None", world_get_next_world()) == 0)
    {
        level_free();
        setup = 0;
        process = 0;
        
    }
    else
    {
        filename = world_get_next_world();
        slog("Loading world %s", filename);
        level_setup();

    }



    transition_flag = 0;

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
        filename = "maps/testworld.txt";
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
    static int prev_mb = 0;  // Previous mouse button state
    gfc_input_update();


    mb = SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT);
    camera_center_on_mouse();



    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_1]) current_item_name = "waffle";
    if (keys[SDL_SCANCODE_2]) current_item_name = "steak";
    if (keys[SDL_SCANCODE_3]) current_item_name = "soup";
    if (keys[SDL_SCANCODE_4]) current_item_name = "tea";



    // Add more as needed

    // Place selected item when mouse button is clicked
    if ((mb & SDL_BUTTON(SDL_BUTTON_LEFT)) && !(prev_mb & SDL_BUTTON(SDL_BUTTON_LEFT)))
    {
        camera = camera_get_position();
        if (current_item_name)
        {
            GFC_Vector2D pos = gfc_vector2d(mx + camera.x, my + camera.y);
            Item* new_item = item_new(current_item_name);
            if (new_item)
            {
                world_set_item(world, pos, new_item);
                slog("Placed item: %s at (%.1f, %.1f)", current_item_name, pos.x, pos.y);
                slog("Filename: %s, Frame: %d", new_item->filename, new_item->frame);
            }
            else
            {
                slog("Failed to create item: %s", current_item_name);
            }
        }
    }

    
    // Allow placing tiles/items/buttons if you want
    button_system_think();
    button_system_update();

    prev_mb = mb; // Update previous state

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
    current_item_name = "";

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
