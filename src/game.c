#include <SDL.h>
#include "simple_logger.h"
#include "gfc_config_def.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "gfc_input.h"

#include "font.h"

#include "camera.h"
#include "entity.h"
#include "player.h"
#include "monster.h"
#include "fire_wizard.h"
#include "fast_wizard.h"
#include "ice_wizard.h"
#include "melee_wizard.h"
#include "end_boss.h"
#include "particles.h"
#include "gfc_vector.h"
#include "items.h"
#include "windows.h"
#include "button.h"
#include "pets.h"



#include "world.h"
#include "status.h"

#include "UI.h"
//#include "level.h"


Uint8 _DRAWBOUNDS_ = 0;
int process = 0;
int done = 0;
const Uint8* keys;



//extern World* world;

int main(int argc, char* argv[])
{
    /*variable declarations*/
    const Uint8* keys;
    //Sprite* sprite;
    //World* world;
    //Window* window;

    int mx, my;
    float mf = 0;
    Sprite* mouse;
    GFC_Color mouseGFC_Color = gfc_color8(255, 100, 255, 200);
    Entity* player;
    Entity* monster;
    Entity* fire_wizard;
    Entity* ice_wizard;
    Entity* fast_wizard;
    Entity* melee_wizard;
    Entity* boss_wizard;
    GFC_Vector2D position = gfc_vector2d(100.0f, 500.0f);
    GFC_Vector2D monsterposition = gfc_vector2d(500.0f, 500.0f);
    //GFC_Rect rectangle = gfc_rect(100, 100, 1000, 500);

    /*program initializtion*/
    init_logger("gf2d.log", 0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        gfc_vector4d(0, 0, 0, 255),
        0);
    gfc_input_init("config/input.cfg");
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    font_init(1024);
    particle_system_init(1000);
    entity_system_init(1024);
    effect_system_init(1024);
    window_system_init(1024);
    //items_initalize("def/item.def");
    SDL_ShowCursor(SDL_DISABLE);
    camera_set_size(gfc_vector2d(1200, 720));

    gfc_config_def_init();
    items_initalize("def/item.def");

    //gfc_config_def_load("def/spray_particle.def");

    /*demo setup*/
    //sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    //world = world_load("maps/testworld.txt");
    //if (!world) {
  //      slog("ERROR: Failed to load world!");
   // }
    //mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16, 0);
    /*window = main_menu();*/

    //particle_system_draw();

    //player = player_new_entity(position);
   // monster = monster_new_entity(monsterposition);
   // fire_wizard = fire_wizard_new_entity(gfc_vector2d(1000, 500));
    //ice_wizard = ice_wizard_new_entity(gfc_vector2d(2000, 500));
    //fast_wizard = fast_wizard_new_entity(gfc_vector2d(2500, 500));
    //melee_wizard = melee_wizard_new_entity(gfc_vector2d(700, 500));
   // boss_wizard = end_boss_new_entity(gfc_vector2d(2000, 500));

    //world = world_test_new();

    //world_save(world, "maps/testsave.map");
    slog("press [escape] to quit");
    /*main game loop*/
    while (!done)
    {
        gfc_input_update();
     //   SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        //font_cleanup();
        /*update things here*/

        //SDL_GetMouseState(&mx, &my);  // <-- add this
        //if (SDL_GetMouseState(&mx, &my))
        //{
        //    //particles_from_file("def/spray_particle.def", 100, gfc_vector2d(mx, my), gfc_vector2d(1, -1), gfc_vector2d(0, 0.1));


        //}

        //Mix_PlayChannel(1, blaster, 0)
        mf += 0.1;
        if (mf >= 16.0)mf = 0;
        //entity_system_think();
        //slog("Here");

        //entity_system_update();
        //entity_system_move();


        switch (process)
        {
        case 1:
              level_process();
              break;
        case 2:
            level_editor();
            break;
        default:
              level_main_menu();
              break;
        }

        /*
        int scene = 0;        
        switch(scene):
        1) level.process();
        */

        game_draw();
        
        //slog("Runnning");
        //gf2d_graphics_clear_screen();// clears drawing buffers
        //// all drawing should happen betweem clear_screen and next_frame
        //    //backgrounds drawn first
        //    //gf2d_sprite_draw_image(sprite,gfc_vector2d(0,0));
        //    
        //    //world_draw(world);
        //   // window_draw(window);
        //    button_system_draw();

        if (process == 2)
        {
            SDL_GetMouseState(&mx, &my);
            if (SDL_GetMouseState(&mx, &my))
            {
                //particles_from_file("def/spray_particle.def", 100, gfc_vector2d(mx, my), gfc_vector2d(1, -1), gfc_vector2d(0, 0.1));
                //world

            }
        }

        //    //gf2d_draw_rect(rectangle, GFC_COLOR_DARKYELLOW);
        //    //gf2d_draw_rect(player->bounds, GFC_COLOR_RED);
        //    entity_system_draw();
            //UI elements last
            //gf2d_sprite_draw(
            //    mouse,
            //    gfc_vector2d(mx,my),
            //    NULL,
            //    NULL,
            //    NULL,
            //    NULL,
            //    &mouseGFC_Color,
            //    (int)mf);

            //UI_health_bar(gfc_rect(15, 15, 300, 75));
            //slog("%d, %d, %d, %d", get_world_bounds().x, get_world_bounds().y, get_world_bounds().h, get_world_bounds().w);

       // gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])
        {
            switch (process) {
            case 1:
                level_free();
                break;
            case 2:
                level_editor_free();
                break;
            default:
                slog("Nothing assigned, this is default");
            }
            process = 0; // exit condition

        }

        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    level_free();
    //entity_free(player);
    //world_free(world);
    //Mix_HaldMusic
    //Mix_FreeMusic
    //Mix
    slog("---==== END ====---");
    return 0;
}

//void parse_args(int argc, char* args[]);

//void parse_args(int argc, char* args[])
//{
//    //if argcount < 2 return
//    // for i = 1; i < argcount i++
//    // gfc_stringlcmp = 0 in an if check some value (drawbounds)  argv sub i
//    // set global to 1
//}

/*eol@eof*/ 


/*
Channel means different things:
Output channels
Mixing channels: (Like background audio)
Input and ouput channels
Mix chunks
Full background music
LoadWAV - Load a WAV file and convert into a mixed chunk handle (Must free) [WAVE, AIFF, RIFF, OGG, VOC)
-1 means idc pick a new channel
*/

/*

Vector2D(x, y* .5% + z-1 *.5)

*/